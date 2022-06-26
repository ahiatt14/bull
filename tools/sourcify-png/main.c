#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void filename_from_path(
  char *filename_out,
  char *filepath,
  size_t max_length
);

int main(int argc, char *argv[]) {

  if (argc != 5) {
    printf("Sourcify-png requires 4 argument.\n");
    return 1;
  }

  char *end_char = NULL;
  long int desired_channels = strtol(argv[2], &end_char, 10);

  if (desired_channels == 0 || desired_channels > 4) {
    printf(
      "Second argument to sourcify-png must be 1 thru 4 for\n"
      "number of channels\n"
    );
    return 1;
  }

  int width = 0;
  int height = 0;
  int channel_count = 0;

  unsigned char *data = stbi_load(
    argv[1],
    &width,
    &height,
    &channel_count,
    desired_channels
  );

  if (data == NULL) {
    printf("Sourcify-png could not load %s\n", argv[1]);
    return 1;
  }

  char png_filename[100] = {0};
  filename_from_path(
    png_filename,
    argv[1],
    200 // TODO: edge case of very long path here, should validate
  );

  // HEADER FILE
  FILE *header_file = fopen(argv[3], "w");
  if (header_file == NULL) {
    printf(
      "Sourcify-png could not open header file %s for writing.\n",
      argv[3]
    );
    return 1;
  }
  fprintf(header_file, "#ifndef __TAIL_%s_TEXTURE__\n", png_filename);
  fprintf(header_file, "#define __TAIL_%s_TEXTURE__\n", png_filename);
  fprintf(header_file, "#include \"tail.h\"\n");
  fprintf(
    header_file,
    "extern struct texture %s_texture;\n",
    png_filename
  );
  fprintf(header_file, "#endif\n");

  // SRC FILE
  int buffer_length = width * height * desired_channels;
  FILE *src_file = fopen(argv[4], "w");
  if (src_file == NULL) {
    printf("Sourcify-png could not open src file %s for writing.\n", argv[4]);
    return 1;
  }
  fprintf(src_file, "#include \"tail.h\"\n");
  fprintf(src_file, "#include \"%s_texture.h\"\n", png_filename);
  fprintf(src_file, "struct texture %s_texture = {\n", png_filename);
  fprintf(src_file, ".width = %i,\n", width);
  fprintf(src_file, ".height = %i,\n", height);
  fprintf(src_file, ".channels_count = %i,\n", desired_channels);
  fprintf(src_file, ".data = (unsigned char[]){\n");
  for (int i = 0; i < buffer_length; i++) {
    fprintf(src_file, "%i,", data[i]);
    if (i % 16 == 0) fprintf(src_file, "\n");
  }
  fprintf(src_file, "}};");

  fclose(header_file);
  fclose(src_file);
  stbi_image_free(data);
  return 0;
}

void filename_from_path(
  char *filename_out,
  char *filepath,
  size_t max_length
) {
  char *last_slash = strrchr(filepath, '/');
  if (last_slash == NULL) last_slash = filepath - 1;
  char *last_dot = strrchr(filepath, '.');
  strncpy(filename_out, last_slash, last_dot - ++last_slash);
}