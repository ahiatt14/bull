#ifndef __TAIL_DEBUGGER__
#define __TAIL_DEBUGGER__

void debugger__clear_and_print();

void debugger__add_float(const char *label, float value);
void debugger__add_vec3(const char *label, float *values);

#endif