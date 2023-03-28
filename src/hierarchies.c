#include "tail.h"

#include "ecs.h"

#include "hierarchies.h"

void add_parents_transforms(
  Transform *const total_transform,
  Entity const *const child,
  ECS const *const ecs
) {

  static Entity const *parent;
  parent = &ecs->entities[child->hierarchy.parent];

  total_transform->position = space__ccw_quat_rotate(
    parent->transform.rotation,
    scalar_x_vec3(
      parent->transform.scale,
      total_transform->position
    )
  );
  total_transform->position = vec3_plus_vec3(
    total_transform->position,
    parent->transform.position
  );
  total_transform->scale *= parent->transform.scale;
  total_transform->rotation = quaternion__multiply(
    total_transform->rotation,
    parent->transform.rotation
  );

  if (has_component(c_HAS_PARENT, parent->config))
    add_parents_transforms(total_transform, parent, ecs);
}