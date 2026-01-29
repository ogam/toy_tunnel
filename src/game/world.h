#ifndef ENTITY_H
#define ENTITY_H

#define PICO_ECS_MAX_COMPONENTS 32
#define PICO_ECS_MAX_SYSTEMS 64

#pragma warning(push)
#pragma warning(disable: 4267)
#pragma warning(disable: 4244)

#define PICO_ECS_IMPLEMENTATION
#include "pico_ecs.h"

#pragma warning(pop)

#ifndef ECS_ENTITY_COUNT
#define ECS_ENTITY_COUNT 4096
#endif

#ifndef ECS_GET_COMP
#define ECS_GET_COMP(COMP) cf_hashtable_get(s_app->world.components, cf_sintern(#COMP))
#endif

#ifndef ECS_GET_SYSTEM
#define ECS_GET_SYSTEM(SYSTEM) cf_hashtable_get(s_app->world.systems, cf_sintern(#SYSTEM))
#endif

#ifndef ECS_REGISTER_COMP
#define ECS_REGISTER_COMP(COMP) \
{ \
ecs_comp_t id = ecs_define_component(s_app->world.ecs, sizeof(COMP), NULL, NULL); \
cf_hashtable_set(s_app->world.components, cf_sintern(#COMP), id); \
}
#endif

#ifndef ECS_REGISTER_COMP_CB
#define ECS_REGISTER_COMP_CB(COMP, CTOR, DTOR) \
{ \
ecs_comp_t id = ecs_define_component(s_app->world.ecs, sizeof(COMP), CTOR, DTOR); \
cf_hashtable_set(s_app->world.components, cf_sintern(#COMP), id); \
}
#endif

#ifndef ECS_REGISTER_SYSTEM
#define ECS_REGISTER_SYSTEM(SYSTEM) \
{ \
ecs_system_t id = ecs_define_system(s_app->world.ecs, (ecs_mask_t){0}, SYSTEM, NULL, NULL, NULL); \
cf_hashtable_set(s_app->world.systems, cf_sintern(#SYSTEM), id); \
}
#endif

#ifndef ECS_REQUIRE_COMP
#define ECS_REQUIRE_COMP(SYSTEM, COMP) ecs_require_component(s_app->world.ecs, ECS_GET_SYSTEM(SYSTEM), ECS_GET_COMP(COMP))
#endif

#ifndef ECS_EXCLUDE_COMP
#define ECS_EXCLUDE_COMP(SYSTEM, COMP) ecs_exclude_component(s_app->world.ecs, ECS_GET_SYSTEM(SYSTEM), ECS_GET_COMP(COMP))
#endif

#ifndef ECS_GET
#define ECS_GET(ENTITY, COMP) ecs_get(s_app->world.ecs, ENTITY, ECS_GET_COMP(COMP))
#endif

typedef struct Camera
{
    // controls how much sway and side translation is applied
    f32 angle;
    f32 angle_drift;
    f32 drift_angle_min;
    f32 drift_angle_max;
    
    f32 target_depth;
    f32 depth;
    // higher value more lower angle towards player
    f32 depth_focus_offset;
    f32 depth_min;
    f32 depth_max;
    
    CF_V2 focus_uv_min;
    CF_V2 focus_uv_max;
    
    f32 shake_duration;
    CF_V2 shake_offset;
} Camera;

typedef struct Tunnel
{
    f32 speed;
    f32 angle;
    f32 world_speed;
    f32 distance_traveled;
    
    // cache path terms at start of every frame
    struct 
    {
        f32 a_x;
        f32 b_x;
        f32 c_x;
        f32 d_x;
        
        f32 a_y;
        f32 b_y;
        f32 c_y;
        f32 d_y;
        
        f32 arena_x;
        f32 arena_y;
    } path;
} Tunnel;

typedef struct C_Transform
{
    Polar polar;
    CF_Transform transform;
    CF_Transform prev_transform;
    f32 depth;
    f32 prev_depth;
    
    f32 depth_scale;
    f32 prev_depth_scale;
    
    CF_V2 draw_position;
    f32 draw_radius;
} C_Transform;

typedef struct C_Movement
{
    Polar velocity;
    f32 depth;
} C_Movement;

typedef struct C_Collider
{
    f32 radius;
} C_Collider;

typedef u8 Weapon_State;
enum
{
    Weapon_State_Ready,
    Weapon_State_Firing,
    Weapon_State_Cooldown,
};

typedef u8 Bullet_Pattern;
enum
{
    Bullet_Pattern_Simple,
    Bullet_Pattern_Spread_Speed,
    Bullet_Pattern_Spread_2,
    Bullet_Pattern_Spread_3,
    Bullet_Pattern_Spread_Scatter_3,
    Bullet_Pattern_Spew,
};

typedef struct C_Weapon
{
    f32 fire_rate;
    f32 last_fire_time;
    
    Polar direction;
    f32 depth_direction;
    
    f32 forward_speed;
    f32 side_speed;
    
    f32 burst_delay;
    s32 burst_count;
    
    CF_Coroutine co;
    
    ecs_entity_t self;
    
    Weapon_State state;
    b8 fire_at_player;
    Bullet_Pattern pattern;
} C_Weapon;

typedef struct C_Health
{
    s32 prev_value;
    s32 value;
    s32 delta;
} C_Health;

typedef struct C_Invulnerable
{
    f32 timer;
} C_Invulnerable;

typedef u8 C_Player;
typedef u8 C_Enemy;
typedef struct C_Bullet
{
    s32 damage;
    b8 is_absorbed;
    b8 is_destroy_on_hit;
} C_Bullet;
typedef u8 C_Boss;
typedef u8 C_Obstacle;

typedef u8 Enemy_Type;
enum
{
    // normal
    Enemy_Type_Obstacle,
    Enemy_Type_Simple,
    Enemy_Type_Side_Dodger,
    Enemy_Type_Streamer,
    Enemy_Type_Spread_Simple,
    Enemy_Type_Spread_Side_Dodger,
    Enemy_Type_Spread_Streamer,
    Enemy_Type_Normal_Count,
    // bosses
    Enemy_Type_Boss,
};

typedef struct C_AI
{
    ecs_entity_t self;
    CF_Coroutine co;
    Enemy_Type enemy_type;
} C_AI;

typedef struct C_Particle
{
    f32 life_time;
    f32 start_radius;
    f32 end_radius;
} C_Particle;

typedef struct C_Eye
{
    // 0..1, 0 being in center, 1 being at the edge
    f32 look_strength;
    CF_V2 direction;
} C_Eye;

typedef u8 Bullet_Charger_State;
enum
{
    Bullet_Charger_State_Ready,
    Bullet_Charger_State_Release,
    Bullet_Charger_State_Cooldown,
};

typedef struct C_Bullet_Charger
{
    f32 cooldown_delay;
    f32 absorb_radius_multiplier;
    s32 absorb_amount;
    Bullet_Charger_State state;
    u8 try_fire;
} C_Bullet_Charger;

typedef struct C_Bullet_Eater
{
    f32 strength01;
} C_Bullet_Eater;

typedef struct C_Score
{
    s32 value;
} C_Score;

typedef s32 Stage_Difficulty;
enum
{
    Stage_Difficulty_Easy,
    Stage_Difficulty_Normal,
    Stage_Difficulty_Hard,
};

typedef struct Stage
{
    // seed is applied once on inital load, stage and following stages has this
    // time modifier carried over
    CF_Coroutine co;
    
    f32 seed_time_modifier;
    f32 difficulty_scaling;
    f32 distance_to_boss;
    s32 remaining_bosses;
    s32 index;
    
    Stage_Difficulty difficulty;
} Stage;

typedef u8 World_State;
enum
{
    World_State_None,
    World_State_Play,
    World_State_Pause,
    World_State_Game_Over,
};

typedef struct World
{
    ecs_t* ecs;
    cf_htbl ecs_system_t* systems;
    cf_htbl ecs_comp_t* components;
    
    Camera camera;
    Tunnel tunnel;
    Stage stage;
    CF_Rnd rnd;
    CF_Rnd particles_rnd;
    
    s32 player_count;
    s32 enemy_count;
    s32 boss_count;
    
    s32 player_lives;
    f32 distance_traveled;
    s32 score;
    
    f32 time;
    f32 dt;
    
    World_State state;
} World;

void init_world();
void update_world();
void draw_world();

void world_set_state(World_State state);

void world_load(Stage_Difficulty difficulty, s32 level);
void world_clear();

void world_advance_stage();

CF_V2 camera_get_translation_scaling_offset();
void push_camera_transform();
void pop_camera_transform();

typedef struct Bullet_Params
{
    Polar polar;
    f32 depth;
    
    Polar direction;
    f32 depth_direction;
    
    f32 forward_speed;
    f32 side_speed;
    
    ecs_comp_t side;
    
    const char* sfx_name;
} Bullet_Params;

f32 make_radial_speed(f32 radial_speed);
f32 angle_towards_tunnel_back(f32 angle);

f32 tunnel_get_speed_modifier();
void tunnel_ring_calculate_path_terms(f32 t);
CF_V2 tunnel_ring_position(f32 z);
CF_V2 tunnel_ring_position_mid(f32 z, f32 mix01);
CF_V2 tunnel_ring_position_end(f32 z, f32 mix01);
CF_V2 tunnel_path(f32 z);

ecs_entity_t make_player(Polar polar_position, f32 depth);
ecs_entity_t make_enemy(Polar polar_position, f32 depth, Enemy_Type type);
ecs_entity_t make_bullet(Bullet_Params params);
ecs_entity_t make_bullet_eater(Bullet_Params params, f32 strength01);
void make_particles(Polar polar_position, f32 depth, s32 count);
void make_thruster_particles(Polar polar_position, f32 depth, s32 count);

void comp_weapon_destructor(ecs_t* ecs, ecs_entity_t entity, void* comp_ptr);
void comp_ai_destructor(ecs_t* ecs, ecs_entity_t entity, void* comp_ptr);

ecs_ret_t system_update_invulnerable_timers(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_update_healths(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_update_players(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_update_enemies(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_update_bosses(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_update_weapons(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);

ecs_ret_t system_update_bullet_chargers(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_update_bullet_eaters(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_collision_test_bullets(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);

ecs_ret_t system_update_bullets(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_update_particles(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_update_movements(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);

ecs_ret_t system_update_polar_to_transform(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);

ecs_ret_t system_update_player_eyes(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_find_nearest_player_to_enemy(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);

ecs_ret_t system_update_enemy_eyes(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_find_nearest_enemy_to_player(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);

void find_nearest_eye_target_directions(fixed Polar* queries, fixed CF_V2* directions, ecs_entity_t* entities, size_t entity_count);
// collision begin

ecs_ret_t system_update_player_bullet_query(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_collision_test_player_hit_enemy(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);

ecs_ret_t system_update_enemy_bullet_query(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_update_enemy_collision_query(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_collision_test_enemy_hit_player(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);

fixed struct Query_Hit* collision_test_hit_entities(fixed Sphere_Sweep* query_spheres, ecs_entity_t* entities, size_t entity_count);
void collision_resolve_hits(fixed struct Query_Hit* hits, ecs_entity_t* entities, size_t entity_count);

// collision end

ecs_ret_t system_draw_background(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_draw_particles(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_draw_enemies(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_draw_enemy_obstacles(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_draw_bosses(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_draw_bullets(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_draw_bullet_eaters(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_draw_players(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_draw_bullet_chargers(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_draw_ship_colliders(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);
ecs_ret_t system_draw_eyes(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata);

b32 is_invulnerable_flicker(f32 timer);
void draw_player(CF_V2 position, CF_SinCos rotation, f32 radius, f32 collider_radius);
void draw_enemy(CF_V2 position, CF_SinCos rotation, f32 radius);
void draw_obstacle(CF_V2 position, CF_SinCos rotation, f32 radius);
void draw_boss(CF_V2 position, CF_SinCos rotation, f32 radius);
void draw_eyes(ecs_entity_t* entities, size_t count);
void draw_bullet_charger(CF_V2 position, CF_SinCos rotation, f32 radius, f32 strength01);
void draw_bullet_eater(CF_V2 position, CF_SinCos rotation, f32 radius, f32 strength01);

void stage_co(CF_Coroutine co);

void ai_enemy_stub_co(CF_Coroutine co);
void ai_enemy_simple_co(CF_Coroutine co);
void ai_enemy_side_dodger_co(CF_Coroutine co);
void ai_enemy_boss_co(CF_Coroutine co);

ecs_comp_t weapon_owner_side(ecs_entity_t entity);
void weapon_co(CF_Coroutine co);

void bullet_pattern_simple(Bullet_Params params);
void bullet_pattern_spread_speed(Bullet_Params params);
void bullet_pattern_spread_2(Bullet_Params params);
void bullet_pattern_spread_3(Bullet_Params params);

#endif //ENTITY_H
