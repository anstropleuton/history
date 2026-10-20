#ifndef PARTS_HPP
#define PARTS_HPP

#include <vector>

class vector3_t
{
public:
  float x, y, z;
};

class quaternion_t
{
public:
  float w, x, y, z;
};

class transform_t
{
public:
  vector3_t local_position;
  quaternion_t local_rotation;
  vector3_t local_scale;
};

class sphere_collider_t
{
public:
  bool value_set;
  vector3_t center;
  float radius;
};

class capsule_collider_t
{
public:
  bool value_set;
  vector3_t center;
  float radius;
  float hight;
  int direction;
};

class box_collider_t
{
public:
  bool value_set;
  vector3_t center;
  vector3_t size;
};

class physic_material_t
{
public:
  bool value_set;
  enum class physic_material_combine_t
  {
    average = 0,
    minimum = 1,
    multiply = 2,
    maximum = 3
  };
  float bounciness;
  float dynamic_friction;
  float static_friction;
  int friction_combine;
  int bounce_combine;
};

class rigidbody_t
{
public:
  bool value_set;
  enum class rigidbody_constraints_t
  {
    none = 0,
    freeze_position_x = 2,
    freeze_position_y = 4,
    freeze_position_z = 8,
    freeze_rotation_x = 0x10,
    freeze_rotation_y = 0x20,
    freeze_rotation_z = 0x40,
    freeze_position = 14,
    freeze_rotation = 112,
    freeze_all = 126
  };
  enum class collision_detection_mode_t
  {
    discrete,
    continuous,
    continuous_dynamic,
    continuous_speculative
  };
  enum class rigidbody_interpolation_t
  {
    none,
    interpolate,
    extrapolate
  };
  vector3_t velocity;
  vector3_t angular_velocity;
  float drag;
  float angular_drag;
  float mass;
  bool use_gravity;
  float max_depenetration_velocity;
  bool is_kinetic;
  bool freeze_rotation;
  int constraints;
  int collision_detection_mode;
  vector3_t center_of_mass;
  vector3_t world_center_of_mass;
  quaternion_t inertia_tensor_rotation;
  vector3_t inertia_tensor;
  bool detect_collisions;
  vector3_t position;
  quaternion_t rotation;
  int interpolation;
  int solver_iteration;
  float sleep_threshold;
  float max_angular_velocity;
  int solver_velocity_iteration;
};

class sprite_t
{
public:
  bool value_set;
  const char *id;
  float scale_x;
  float scale_y;
  int pivot_x;
  int pivot_y;
  bool update_collider;
  float default_camera_size;
  float default_camera_height;
  float default_screen_height;
  float default_atlas_size;
};

class serialized_sprite_t
{
public:
  bool value_set;
  const char *name;
};

class game_object_t
{
public:
  int layer;
  bool active_self;
  bool is_static;
  unsigned long long scene_culling_mask;
  transform_t transform;
  sphere_collider_t sphere_collider;
  capsule_collider_t capsule_collider;
  box_collider_t box_collider;
  physic_material_t physic_material;
  rigidbody_t rigidbody;
  sprite_t sprite;
  serialized_sprite_t serialized_sprite;
  std::vector<game_object_t> children;
};

class base_part_t
{
public:
  enum class joint_type_t
  {
    fixed_joint = 0,
    hinge_joint = 1
  };

  enum class part_tier_t
  {
    regular = 0,
    common = 1,
    rare = 2,
    epic = 3,
    legendary = 4
  };

  enum class part_type_t
  {
    unknown = 0,
    balloon = 1,
    balloons2 = 2,
    balloons3 = 3,
    // ... (rest of the enum values)
    max = 50
  };

  enum class auto_align_type_t
  {
    none = 0,
    rotate = 1,
    flip_vertically = 2
  };

  enum class direction_t
  {
    right = 0,
    up = 1,
    left = 2,
    down = 3,
    up_right = 4,
    up_left = 5,
    down_left = 6,
    down_right = 7
  };

  enum class grid_rotation_t
  {
    deg_0 = 0,
    deg_90 = 1,
    deg_180 = 2,
    deg_270 = 3,
    deg_45 = 4,
    deg_135 = 5,
    deg_225 = 6,
    deg_315 = 7,
    deg_max = 8
  };

  enum class joint_connection_type_t
  {
    none = 0,
    source = 1,
    target = 2
  };

  enum class joint_connection_direction_t
  {
    any = 0,
    right = 1,
    up = 2,
    left = 3,
    down = 4,
    left_and_right = 5,
    up_and_down = 6,
    none = 7
  };

  enum class joint_connection_strength_t
  {
    weak = 0,
    normal = 1,
    high = 2,
    extreme = 3,
    highly_extreme = 4
  };

  bool m_eight_way;
  int m_coord_x;
  int m_coord_y;
  float m_mass = 1.0f;
  float m_interactive_radius = 0.5f;
  float m_break_velocity;
  float m_power_consumption;
  float m_engine_power;
  float m_z_offset;
  int custom_part_index;
  bool craftable = true;
  bool loot_crate_reward = true;
  int m_joint_type;
  int m_part_tier;
  int m_part_type;
  int m_auto_align;
  bool m_flipped;
  int m_grid_rotation;
  int m_grid_x_min;
  int m_grid_x_max;
  int m_grid_y_min;
  int m_grid_y_max;
  bool m_static;
  int m_joint_connection_strength;
  int m_joint_connection_type = (int)joint_connection_type_t::target;
  int m_joint_connection_direction;
  int m_custom_joint_connection_direction = (int)joint_connection_direction_t::none;
  sprite_t sprite;
  bool visible_on_part_list_before_unlocking;
  bool joint_preprocessing;
  int connected_component;
  vector3_t wind_velocity;
  bool valid;
  int strict_connected_component;
  int general_connected_component;
  int generator_ref_count;
  int generation_level;
  int generation_index;
  float temperature;
  game_object_t game_object;
};

#endif // PARTS_HPP
