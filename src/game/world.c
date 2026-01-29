#include "game/world.h"

void init_ecs()
{
    World* world = &s_app->world;
    world->ecs = ecs_new(ECS_ENTITY_COUNT, NULL);
    
    // components
    {
        ECS_REGISTER_COMP(C_Transform);
        ECS_REGISTER_COMP(C_Movement);
        ECS_REGISTER_COMP(C_Collider);
        ECS_REGISTER_COMP(C_Player);
        ECS_REGISTER_COMP(C_Enemy);
        ECS_REGISTER_COMP(C_Bullet);
        ECS_REGISTER_COMP(C_Boss);
        ECS_REGISTER_COMP_CB(C_AI, NULL, comp_ai_destructor);
        ECS_REGISTER_COMP_CB(C_Weapon, NULL, comp_weapon_destructor);
        ECS_REGISTER_COMP(C_Health);
        ECS_REGISTER_COMP(C_Particle);
        ECS_REGISTER_COMP(C_Eye);
        ECS_REGISTER_COMP(C_Invulnerable);
        ECS_REGISTER_COMP(C_Obstacle);
        ECS_REGISTER_COMP(C_Bullet_Charger);
        ECS_REGISTER_COMP(C_Bullet_Eater);
        ECS_REGISTER_COMP(C_Score);
    }
    
    // systems
    {
        ECS_REGISTER_SYSTEM(system_update_invulnerable_timers);
        ECS_REQUIRE_COMP(system_update_invulnerable_timers, C_Invulnerable);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_healths);
        ECS_REQUIRE_COMP(system_update_healths, C_Health);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_players);
        ECS_REQUIRE_COMP(system_update_players, C_Transform);
        ECS_REQUIRE_COMP(system_update_players, C_Player);
        ECS_REQUIRE_COMP(system_update_players, C_Weapon);
        ECS_REQUIRE_COMP(system_update_players, C_Health);
        ECS_REQUIRE_COMP(system_update_players, C_Bullet_Charger);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_enemies);
        ECS_REQUIRE_COMP(system_update_enemies, C_Transform);
        ECS_REQUIRE_COMP(system_update_enemies, C_AI);
        ECS_REQUIRE_COMP(system_update_enemies, C_Enemy);
        ECS_REQUIRE_COMP(system_update_enemies, C_Health);
        ECS_REQUIRE_COMP(system_update_enemies, C_Score);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_bosses);
        ECS_REQUIRE_COMP(system_update_bosses, C_Boss);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_weapons);
        ECS_REQUIRE_COMP(system_update_weapons, C_Weapon);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_bullet_chargers);
        ECS_REQUIRE_COMP(system_update_bullet_chargers, C_Transform);
        ECS_REQUIRE_COMP(system_update_bullet_chargers, C_Collider);
        ECS_REQUIRE_COMP(system_update_bullet_chargers, C_Weapon);
        ECS_REQUIRE_COMP(system_update_bullet_chargers, C_Bullet_Charger);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_bullet_eaters);
        ECS_REQUIRE_COMP(system_update_bullet_eaters, C_Transform);
        ECS_REQUIRE_COMP(system_update_bullet_eaters, C_Collider);
        ECS_REQUIRE_COMP(system_update_bullet_eaters, C_Bullet_Eater);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_collision_test_bullets);
        ECS_REQUIRE_COMP(system_collision_test_bullets, C_Transform);
        ECS_REQUIRE_COMP(system_collision_test_bullets, C_Collider);
        ECS_REQUIRE_COMP(system_collision_test_bullets, C_Bullet);
        ECS_EXCLUDE_COMP(system_collision_test_bullets, C_Bullet_Eater);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_bullets);
        ECS_REQUIRE_COMP(system_update_bullets, C_Transform);
        ECS_REQUIRE_COMP(system_update_bullets, C_Bullet);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_particles);
        ECS_REQUIRE_COMP(system_update_particles, C_Transform);
        ECS_REQUIRE_COMP(system_update_particles, C_Particle);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_movements);
        ECS_REQUIRE_COMP(system_update_movements, C_Transform);
        ECS_REQUIRE_COMP(system_update_movements, C_Movement);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_polar_to_transform);
        ECS_REQUIRE_COMP(system_update_polar_to_transform, C_Transform);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_player_eyes);
        ECS_REQUIRE_COMP(system_update_player_eyes, C_Transform);
        ECS_REQUIRE_COMP(system_update_player_eyes, C_Eye);
        ECS_REQUIRE_COMP(system_update_player_eyes, C_Player);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_find_nearest_player_to_enemy);
        ECS_REQUIRE_COMP(system_find_nearest_player_to_enemy, C_Transform);
        ECS_EXCLUDE_COMP(system_find_nearest_player_to_enemy, C_Bullet);
        ECS_REQUIRE_COMP(system_find_nearest_player_to_enemy, C_Enemy);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_enemy_eyes);
        ECS_REQUIRE_COMP(system_update_enemy_eyes, C_Transform);
        ECS_REQUIRE_COMP(system_update_enemy_eyes, C_Eye);
        ECS_REQUIRE_COMP(system_update_enemy_eyes, C_Enemy);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_find_nearest_enemy_to_player);
        ECS_REQUIRE_COMP(system_find_nearest_enemy_to_player, C_Transform);
        ECS_EXCLUDE_COMP(system_find_nearest_enemy_to_player, C_Bullet);
        ECS_REQUIRE_COMP(system_find_nearest_enemy_to_player, C_Player);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_player_bullet_query);
        ECS_REQUIRE_COMP(system_update_player_bullet_query, C_Transform);
        ECS_REQUIRE_COMP(system_update_player_bullet_query, C_Collider);
        ECS_REQUIRE_COMP(system_update_player_bullet_query, C_Bullet);
        ECS_REQUIRE_COMP(system_update_player_bullet_query, C_Player);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_collision_test_player_hit_enemy);
        ECS_REQUIRE_COMP(system_collision_test_player_hit_enemy, C_Transform);
        ECS_REQUIRE_COMP(system_collision_test_player_hit_enemy, C_Collider);
        ECS_EXCLUDE_COMP(system_collision_test_player_hit_enemy, C_Bullet);
        ECS_REQUIRE_COMP(system_collision_test_player_hit_enemy, C_Enemy);
        ECS_REQUIRE_COMP(system_collision_test_player_hit_enemy, C_Invulnerable);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_enemy_bullet_query);
        ECS_REQUIRE_COMP(system_update_enemy_bullet_query, C_Transform);
        ECS_REQUIRE_COMP(system_update_enemy_bullet_query, C_Collider);
        ECS_REQUIRE_COMP(system_update_enemy_bullet_query, C_Bullet);
        ECS_REQUIRE_COMP(system_update_enemy_bullet_query, C_Enemy);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_update_enemy_collision_query);
        ECS_REQUIRE_COMP(system_update_enemy_collision_query, C_Transform);
        ECS_REQUIRE_COMP(system_update_enemy_collision_query, C_Collider);
        ECS_EXCLUDE_COMP(system_update_enemy_collision_query, C_Bullet);
        ECS_REQUIRE_COMP(system_update_enemy_collision_query, C_Enemy);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_collision_test_enemy_hit_player);
        ECS_REQUIRE_COMP(system_collision_test_enemy_hit_player, C_Transform);
        ECS_REQUIRE_COMP(system_collision_test_enemy_hit_player, C_Collider);
        ECS_EXCLUDE_COMP(system_collision_test_enemy_hit_player, C_Bullet);
        ECS_REQUIRE_COMP(system_collision_test_enemy_hit_player, C_Player);
        ECS_REQUIRE_COMP(system_collision_test_enemy_hit_player, C_Invulnerable);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_draw_background);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_draw_particles);
        ECS_REQUIRE_COMP(system_draw_particles, C_Transform);
        ECS_REQUIRE_COMP(system_draw_particles, C_Particle);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_draw_enemies);
        ECS_REQUIRE_COMP(system_draw_enemies, C_Transform);
        ECS_REQUIRE_COMP(system_draw_enemies, C_Enemy);
        ECS_EXCLUDE_COMP(system_draw_enemies, C_Bullet);
        ECS_EXCLUDE_COMP(system_draw_enemies, C_Boss);
        ECS_EXCLUDE_COMP(system_draw_enemies, C_Obstacle);
        ECS_REQUIRE_COMP(system_draw_enemies, C_Invulnerable);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_draw_enemy_obstacles);
        ECS_REQUIRE_COMP(system_draw_enemy_obstacles, C_Transform);
        ECS_REQUIRE_COMP(system_draw_enemy_obstacles, C_Enemy);
        ECS_EXCLUDE_COMP(system_draw_enemy_obstacles, C_Bullet);
        ECS_EXCLUDE_COMP(system_draw_enemy_obstacles, C_Boss);
        ECS_REQUIRE_COMP(system_draw_enemy_obstacles, C_Obstacle);
        ECS_REQUIRE_COMP(system_draw_enemy_obstacles, C_Invulnerable);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_draw_bosses);
        ECS_REQUIRE_COMP(system_draw_bosses, C_Transform);
        ECS_REQUIRE_COMP(system_draw_bosses, C_Enemy);
        ECS_EXCLUDE_COMP(system_draw_bosses, C_Bullet);
        ECS_REQUIRE_COMP(system_draw_bosses, C_Boss);
        ECS_EXCLUDE_COMP(system_draw_bosses, C_Obstacle);
        ECS_REQUIRE_COMP(system_draw_bosses, C_Invulnerable);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_draw_bullets);
        ECS_REQUIRE_COMP(system_draw_bullets, C_Transform);
        ECS_REQUIRE_COMP(system_draw_bullets, C_Bullet);
        ECS_EXCLUDE_COMP(system_draw_bullets, C_Bullet_Eater);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_draw_bullet_eaters);
        ECS_REQUIRE_COMP(system_draw_bullet_eaters, C_Transform);
        ECS_REQUIRE_COMP(system_draw_bullet_eaters, C_Bullet);
        ECS_REQUIRE_COMP(system_draw_bullet_eaters, C_Bullet_Eater);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_draw_players);
        ECS_REQUIRE_COMP(system_draw_players, C_Transform);
        ECS_REQUIRE_COMP(system_draw_players, C_Player);
        ECS_EXCLUDE_COMP(system_draw_players, C_Bullet);
        ECS_REQUIRE_COMP(system_draw_players, C_Invulnerable);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_draw_bullet_chargers);
        ECS_REQUIRE_COMP(system_draw_bullet_chargers, C_Transform);
        ECS_REQUIRE_COMP(system_draw_bullet_chargers, C_Collider);
        ECS_REQUIRE_COMP(system_draw_bullet_chargers, C_Bullet_Charger);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_draw_ship_colliders);
        ECS_REQUIRE_COMP(system_draw_ship_colliders, C_Transform);
        ECS_REQUIRE_COMP(system_draw_ship_colliders, C_Collider);
        ECS_EXCLUDE_COMP(system_draw_ship_colliders, C_Bullet);
    }
    
    {
        ECS_REGISTER_SYSTEM(system_draw_eyes);
        ECS_REQUIRE_COMP(system_draw_eyes, C_Transform);
        ECS_REQUIRE_COMP(system_draw_eyes, C_Eye);
        ECS_REQUIRE_COMP(system_draw_eyes, C_Invulnerable);
    }
}

void init_world()
{
    World* world = &s_app->world;
    
    init_ecs();
    
    world->rnd = cf_rnd_seed(cf_get_tick_frequency());
    world->particles_rnd = cf_rnd_seed(cf_get_tick_frequency());
    
    world_set_state(World_State_None);
}

void update_world()
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    Tunnel* tunnel = &world->tunnel;
    world->dt = CF_DELTA_TIME;
    
    if (world->state != World_State_Play && world->state != World_State_Game_Over)
    {
        world->dt = 0.0f;
    }
    
    if (world->state == World_State_Play)
    {
        world->distance_traveled = tunnel->distance_traveled;
    }
    
    if (world->stage.co.id)
    {
        if (cf_coroutine_state(world->stage.co) == CF_COROUTINE_STATE_DEAD)
        {
            world_advance_stage();
        }
        cf_coroutine_resume(world->stage.co);
    }
    
    tunnel->distance_traveled += tunnel->world_speed * world->dt;
    
    tunnel_ring_calculate_path_terms(world->time + world->stage.seed_time_modifier);
    
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_invulnerable_timers), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_healths), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_players), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_enemies), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_bosses), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_weapons), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_bullet_chargers), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_bullet_eaters), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_bullets), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_particles), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_movements), 0);
    
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_polar_to_transform), 0);
    
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_player_eyes), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_enemy_eyes), 0);
    
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_player_bullet_query), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_enemy_bullet_query), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_update_enemy_collision_query), 0);
    
    world->time += world->dt;
}

void draw_world()
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_draw_background), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_draw_particles), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_draw_enemies), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_draw_enemy_obstacles), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_draw_bosses), 0);
    
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_draw_bullets), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_draw_bullet_eaters), 0);
    
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_draw_players), 0);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_draw_bullet_chargers), 0);
    cf_render_to(s_app->scene_canvas, true);
    
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_draw_eyes), 0);
    
    //ecs_run_system(ecs, ECS_GET_SYSTEM(system_draw_ship_colliders), 0);
    
    {
        CF_V2 inverse_screen_scale = cf_v2(1.0f / s_app->screen_scale.x, 1.0f / s_app->screen_scale.y);
        CF_V2 draw_scale = cf_mul_v2(s_app->screen_size, inverse_screen_scale);
        cf_draw_canvas(cf_app_get_canvas(), cf_v2(0, 0), draw_scale);
        cf_render_to(s_app->scene_canvas, false);
        cf_draw_canvas(s_app->scene_canvas, cf_v2(0, 0), draw_scale);
        cf_render_to(cf_app_get_canvas(), true);
    }
}

void world_set_state(World_State state)
{
    s_app->world.state = state;
}

void world_load(Stage_Difficulty difficulty, s32 level)
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    world_clear();
    
    u64 seed = (u64)(difficulty * level);
    
    world->time = 180.0f * difficulty + 30.0f * level;
    world->rnd = cf_rnd_seed(seed);
    world->stage.index = level;
    world->stage.seed_time_modifier = cf_rnd_range_float(&world->rnd, 0.0f, 9274.0f);
    world->stage.difficulty = difficulty;
    
    world_advance_stage();
    
    world_set_state(World_State_Play);
    
    world->player_lives = 11;
}

void world_clear()
{
    World* world = &s_app->world;
    ecs_reset(world->ecs);
    world->time = 0.0f;
    world->tunnel = (Tunnel){ 0 };
    
    world->camera = (Camera){ 0 };
    world->camera.depth = PLAYER_MIN_DEPTH;
    
    if (world->stage.co.id)
    {
        cf_destroy_coroutine(world->stage.co);
    }
    
    world->stage.co = cf_make_coroutine(stage_co, 0, NULL);
    world->score = 0;
}

void world_advance_stage()
{
    World* world = &s_app->world;
    Stage* stage = &world->stage;
    
    f32 base_distance_multiplier = 100.0f;
    switch (stage->difficulty)
    {
        case Stage_Difficulty_Easy:
        {
            stage->difficulty_scaling = 0.5f;
            base_distance_multiplier = 50.0f;
            break;
        }
        case Stage_Difficulty_Hard:
        {
            stage->difficulty_scaling = 1.5f;
            base_distance_multiplier = 125.0f;
            break;
        }
        default:
        {
            stage->difficulty_scaling = 1.0f;
            base_distance_multiplier = 75.0f;
            break;
        }
    }
    
    stage->difficulty_scaling += stage->index / 20.0f;
    stage->distance_to_boss = TUNNEL_WORLD_MAX_SPEED * (base_distance_multiplier + 25.0f * stage->difficulty_scaling * stage->index);
    
    stage->remaining_bosses = cf_clamp(1 + stage->index / 10, 1, 3);
    stage->index++;
    
    cf_destroy_coroutine(world->stage.co);
    world->stage.co = cf_make_coroutine(stage_co, 0, NULL);
}

CF_V2 camera_get_translation_scaling_offset()
{
    Camera* camera = &s_app->world.camera;
    f32 depth01 = cf_remap(camera->depth + camera->depth_focus_offset, camera->depth_min, camera->depth_max, 0.0f, 1.0f);
    // reverse lerp here otherwise translation cuts off view
    f32 drift01 = cf_remap(camera->angle_drift, camera->drift_angle_min, camera->drift_angle_max, 1.0f, 0.0f);
    drift01 = cf_smoothstep(drift01);
    
    f32 translation_scaling_x = cf_lerp(camera->focus_uv_min.x, camera->focus_uv_max.x, drift01);
    f32 translation_scaling_y = cf_lerp(0.0f, camera->focus_uv_max.y - camera->focus_uv_min.y, depth01);
    
    return cf_v2(translation_scaling_x, translation_scaling_y);
}

void push_camera_transform()
{
    World* world = &s_app->world;
    Camera* camera = &world->camera;
    
    cf_draw_push();
    
    CF_V2 translation_scaling = camera_get_translation_scaling_offset();
    
    translation_scaling.x += camera->shake_offset.x;
    translation_scaling.y += camera->shake_offset.y;
    
    cf_draw_translate(s_app->screen_size.x * translation_scaling.x, s_app->screen_size.y * translation_scaling.y);
    cf_draw_rotate(camera->angle + (0.5f - camera->angle_drift) * CF_PI);
}

void pop_camera_transform()
{
    cf_draw_pop();
}

f32 make_radial_speed(f32 radial_speed)
{
    f32 speed = radial_speed / TUNNEL_MAX_DEPTH * TUNNEL_BACK_RADIUS;
    return speed;
}

f32 angle_towards_tunnel_back(f32 angle)
{
    Polar p = polar(1.0f, 0.0f);
    Tunnel* tunnel = &s_app->world.tunnel;
    
    f32 delta_angle = tunnel->angle - angle;
    p = polar_add_angle(p, delta_angle);
    p.angle = p.angle < CF_PI ? 1.0f : -1.0f;
    p.angle = cf_abs(delta_angle) / (CF_PI * tunnel->speed * p.angle);
    
    return p.angle;
}

inline f32 tunnel_get_speed_modifier()
{
    f32 tunnel_speed_modifier = cf_remap(s_app->world.tunnel.speed, TUNNEL_MIN_SPEED, TUNNEL_MAX_SPEED, 0.0f, 1.0f);
    return cf_lerp(TUNNEL_MAX_DEPTH_MODIFIER, TUNNEL_MIN_DEPTH_MODIFIER, tunnel_speed_modifier);
}

inline void tunnel_ring_calculate_path_terms(f32 t)
{
    Tunnel* tunnel = &s_app->world.tunnel;
    
    tunnel->path.a_x = cf_sin(t);
    tunnel->path.b_x = cf_sin(t * 0.1f);
    tunnel->path.c_x = cf_sin(cf_sin(t + 0.9f));
    tunnel->path.d_x = cf_sin(cf_sin(t + 1.9f));
    
    tunnel->path.a_y = cf_cos(t);
    tunnel->path.b_y = cf_cos(t * 0.2f);
    tunnel->path.c_y = cf_cos(cf_cos(t + 1.22f));
    tunnel->path.d_y = cf_cos(cf_cos(t + 2.22f));
    
    // @tunnel_depth
    // final sin and cos here is to ensure that tunnel clamps between -1 and 1
    // before being scaled by depth
    tunnel->path.arena_x = cf_sin(tunnel->path.a_x + tunnel->path.b_x);
    tunnel->path.arena_y = cf_cos(tunnel->path.a_y + tunnel->path.b_y);
}

// main tunnel path for gameplay arena
// t tunnel sway
// z depth of tunnel
inline CF_V2 tunnel_ring_position(f32 z)
{
    Tunnel* tunnel = &s_app->world.tunnel;
    
    // tunnel speed modifies how wide (slow) or narrow (fast) the tunnel looks
    // modifiers to curve needs to have input
    z = z / tunnel->speed * TUNNEL_SPEED_CURVE_MODIFIER;
    
    return cf_v2(tunnel->path.arena_x * z, tunnel->path.arena_y * z);
}

// background tunnel paths for aesthetics
// if adding more segments that has different turn shapes then the last term needs to be 
// treated as always there so the following change needs to be done
// A + B + mix01 * C => A + B + C + mix01 * D
// then each tunnel section begin and end will connect to the previous and next segment
// another example here is tunnel_ring_position_mid() and tunnel_ring_position_end()
inline CF_V2 tunnel_ring_position_mid(f32 z, f32 mix01)
{
    Tunnel* tunnel = &s_app->world.tunnel;
    
    z = z / tunnel->speed * TUNNEL_SPEED_CURVE_MODIFIER;
    
    mix01 = cf_smoothstep(mix01);
    
    f32 inner_x = tunnel->path.a_x + tunnel->path.b_x + mix01 * tunnel->path.c_x;
    f32 inner_y = tunnel->path.a_y + tunnel->path.b_y + mix01 * tunnel->path.c_y;
    // @tunnel_depth
    f32 x = cf_sin(inner_x);
    f32 y = cf_cos(inner_y);
    
    return cf_v2(x * z, y * z);
}

inline CF_V2 tunnel_ring_position_end(f32 z, f32 mix01)
{
    Tunnel* tunnel = &s_app->world.tunnel;
    
    z = z / tunnel->speed * TUNNEL_SPEED_CURVE_MODIFIER;
    
    mix01 = cf_smoothstep(mix01);
    
    f32 inner_x = tunnel->path.a_x + tunnel->path.b_x + tunnel->path.c_x + mix01 * tunnel->path.d_x;
    f32 inner_y = tunnel->path.a_y + tunnel->path.b_y + tunnel->path.c_y + mix01 * tunnel->path.d_y;
    // @tunnel_depth
    f32 x = cf_sin(inner_x);
    f32 y = cf_cos(inner_y);
    
    return cf_v2(x * z, y * z);
}

inline CF_V2 tunnel_path(f32 z)
{
    f32 dz = TUNNEL_MAX_DEPTH - z;
    CF_V2 p = cf_v2(0, 0);
    
    if (z > TUNNEL_MAX_DEPTH - 80)
    {
        f32 mix01 = 1.0f - dz / 80.0f;
        p = tunnel_ring_position_end(z, mix01);
    }
    else if (z > TUNNEL_MAX_DEPTH - 130)
    {
        f32 mix01 = 1.0f - (dz - 80.0f) / 50.0f;
        p = tunnel_ring_position_mid(z, mix01);
    }
    else
    {
        p = tunnel_ring_position(z);
    }
    return p;
}

inline void comp_set_transform(C_Transform* transform, Polar polar, f32 depth)
{
    transform->polar = polar;
    transform->depth = depth;
    transform->transform.p = polar_to_screen_v2(polar);
    transform->transform.r = cf_sincos(polar.angle + CF_PI * 0.5f);
    transform->prev_transform = transform->transform;
    transform->prev_depth = transform->depth;
}

ecs_entity_t make_player(Polar polar_position, f32 depth)
{
    ecs_t* ecs = s_app->world.ecs;
    
    ecs_entity_t entity = ecs_create(ecs);
    C_Transform* transform = ecs_add(ecs, entity, ECS_GET_COMP(C_Transform), NULL);
    C_Collider* collider = ecs_add(ecs, entity, ECS_GET_COMP(C_Collider), NULL);
    C_Player* player = ecs_add(ecs, entity, ECS_GET_COMP(C_Player), NULL);
    C_Weapon* weapon = ecs_add(ecs, entity, ECS_GET_COMP(C_Weapon), NULL);
    C_Health* health = ecs_add(ecs, entity, ECS_GET_COMP(C_Health), NULL);
    ecs_add(ecs, entity, ECS_GET_COMP(C_Eye), NULL);
    C_Invulnerable* invulnerable = ecs_add(ecs, entity, ECS_GET_COMP(C_Invulnerable), NULL);
    ecs_add(ecs, entity, ECS_GET_COMP(C_Bullet_Charger), NULL);
    
    transform->draw_radius = PLAYER_DRAW_RADIUS;
    comp_set_transform(transform, polar_position, depth);
    
    collider->radius = PLAYER_COLLIDER_RADIUS;
    
    invulnerable->timer = 2.0f;
    
    weapon->fire_rate = 0.02f;
    weapon->direction = polar(-1.0f, 0.0f);
    weapon->depth_direction = -1.0f;
    weapon->forward_speed = 80.0f;
    weapon->side_speed = 1.0f;
    
    weapon->self = entity;
    weapon->burst_count = 1;
    weapon->burst_delay = 0.1f;
    weapon->pattern = Bullet_Pattern_Spread_Speed;
    
    weapon->co = cf_make_coroutine(weapon_co, 0, weapon);
    
    health->value = 1;
    
    return entity;
}

ecs_entity_t make_enemy(Polar polar_position, f32 depth, Enemy_Type type)
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    ecs_entity_t entity = ecs_create(ecs);
    C_Transform* transform = ecs_add(ecs, entity, ECS_GET_COMP(C_Transform), NULL);
    ecs_add(ecs, entity, ECS_GET_COMP(C_Movement), NULL);
    C_Collider* collider = ecs_add(ecs, entity, ECS_GET_COMP(C_Collider), NULL);
    C_AI* ai = ecs_add(ecs, entity, ECS_GET_COMP(C_AI), NULL);
    ecs_add(ecs, entity, ECS_GET_COMP(C_Enemy), NULL);
    C_Weapon* weapon = ecs_add(ecs, entity, ECS_GET_COMP(C_Weapon), NULL);
    C_Health* health = ecs_add(ecs, entity, ECS_GET_COMP(C_Health), NULL);
    ecs_add(ecs, entity, ECS_GET_COMP(C_Invulnerable), NULL);
    C_Score* score = ecs_add(ecs, entity, ECS_GET_COMP(C_Score), NULL);
    
    comp_set_transform(transform, polar_position, depth);
    
    collider->radius = ENEMY_COLLIDER_RADIUS;
    
    ai->self = entity;
    
    weapon->fire_rate = 1.0f;
    weapon->direction = polar(1.0f, 0.0f);
    weapon->depth_direction = 1.0f;
    weapon->forward_speed = 40.0f;
    weapon->side_speed = 1.0f;
    weapon->self = entity;
    
    weapon->pattern = Bullet_Pattern_Simple;
    weapon->burst_count = 1;
    weapon->burst_delay = 0.1f;
    weapon->co = cf_make_coroutine(weapon_co, 0, weapon);
    
    health->value = 1;
    
    switch (type)
    {
        case Enemy_Type_Obstacle:
        {
            ai->co = cf_make_coroutine(ai_enemy_stub_co, 0, ai);
            collider->radius = OBSTACLE_COLLIDER_RADIUS;
            weapon->burst_count = 0;
            ecs_add(ecs, entity, ECS_GET_COMP(C_Obstacle), NULL);
            score->value = 10;
            break;
        }
        case Enemy_Type_Simple:
        {
            ai->co = cf_make_coroutine(ai_enemy_simple_co, 0, ai);
            weapon->pattern = Bullet_Pattern_Simple;
            score->value = 20;
            break;
        }
        case Enemy_Type_Side_Dodger:
        {
            ai->co = cf_make_coroutine(ai_enemy_side_dodger_co, 0, ai);
            weapon->pattern = Bullet_Pattern_Simple;
            score->value = 30;
            break;
        }
        case Enemy_Type_Streamer:
        {
            ai->co = cf_make_coroutine(ai_enemy_side_dodger_co, 0, ai);
            health->value = 4;
            weapon->fire_rate = 0.1f;
            weapon->pattern = Bullet_Pattern_Simple;
            score->value = 40;
            break;
        }
        case Enemy_Type_Spread_Simple:
        {
            ai->co = cf_make_coroutine(ai_enemy_simple_co, 0, ai);
            weapon->pattern = Bullet_Pattern_Spread_2;
            score->value = 25;
            break;
        }
        case Enemy_Type_Spread_Side_Dodger:
        {
            ai->co = cf_make_coroutine(ai_enemy_side_dodger_co, 0, ai);
            weapon->pattern = Bullet_Pattern_Spread_3;
            score->value = 35;
            break;
        }
        case Enemy_Type_Spread_Streamer:
        {
            ai->co = cf_make_coroutine(ai_enemy_simple_co, 0, ai);
            health->value = 4;
            weapon->fire_rate = 0.1f;
            weapon->pattern = Bullet_Pattern_Spread_2;
            score->value = 45;
            break;
        }
        case Enemy_Type_Boss:
        {
            ai->co = cf_make_coroutine(ai_enemy_boss_co, 0, ai);
            collider->radius = BOSS_COLLIDER_RADIUS;
            health->value = (s32)(200 * world->stage.difficulty_scaling);
            weapon->fire_at_player = true;
            ecs_add(ecs, entity, ECS_GET_COMP(C_Boss), NULL);
            ecs_add(ecs, entity, ECS_GET_COMP(C_Eye), NULL);
            score->value = 5000;
            break;
        }
        default:
        {
            ai->co = cf_make_coroutine(ai_enemy_stub_co, 0, ai);
            weapon->burst_count = 0;
            break;
        }
    }
    
    transform->draw_radius = collider->radius;
    
    return entity;
}

ecs_entity_t make_bullet(Bullet_Params params)
{
    ecs_t* ecs = s_app->world.ecs;
    ecs_entity_t entity = ecs_create(ecs);
    C_Transform* transform = ecs_add(ecs, entity, ECS_GET_COMP(C_Transform), NULL);
    C_Movement* movement = ecs_add(ecs, entity, ECS_GET_COMP(C_Movement), NULL);
    C_Collider* collider = ecs_add(ecs, entity, ECS_GET_COMP(C_Collider), NULL);
    C_Bullet* bullet = ecs_add(ecs, entity, ECS_GET_COMP(C_Bullet), NULL);
    C_Score* score = ecs_add(ecs, entity, ECS_GET_COMP(C_Score), NULL);
    ecs_add(ecs, entity, params.side, NULL);
    
    comp_set_transform(transform, params.polar, params.depth);
    transform->draw_radius = BULLET_COLLIDER_RADIUS;
    
    movement->velocity.angle = params.side_speed * params.direction.angle;
    // need to ensure TUNNEL_BACK_RADIUS is scaled in radius otherwise entitys won't be in the correct
    // 3d spatial space
    movement->velocity.radius = make_radial_speed(params.forward_speed * params.direction.radius);
    movement->depth = params.forward_speed * params.depth_direction;
    
    collider->radius = BULLET_COLLIDER_RADIUS;
    
    bullet->damage = 1;
    bullet->is_destroy_on_hit = true;
    
    score->value = 1;
    
    audio_play_sfx(params.sfx_name);
    
    return entity;
}

ecs_entity_t make_bullet_eater(Bullet_Params params, f32 strength01)
{
    ecs_t* ecs = s_app->world.ecs;
    
    ecs_entity_t entity = make_bullet(params);
    C_Bullet_Eater* bullet_eater = ecs_add(ecs, entity, ECS_GET_COMP(C_Bullet_Eater), NULL);
    C_Collider* collider = ecs_get(ecs, entity, ECS_GET_COMP(C_Collider));
    C_Transform* transform = ecs_get(ecs, entity, ECS_GET_COMP(C_Transform));
    C_Bullet* bullet = ecs_get(ecs, entity, ECS_GET_COMP(C_Bullet));
    
    bullet_eater->strength01 = strength01;
    
    collider->radius = BULLET_EATER_COLLIDER_RADIUS * (1.0f + strength01);
    transform->draw_radius = collider->radius;
    
    bullet->damage = (s32)(1000 * (1.0f * strength01));
    bullet->is_destroy_on_hit = false;
    
    audio_play_sfx("bullet_eater_fire");
    
    return entity;
}

void make_particles(Polar polar_position, f32 depth, s32 count)
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    CF_Rnd* rnd = &world->particles_rnd;
    
    Tunnel* tunnel = &world->tunnel;
    f32 tunnel_world_speed = tunnel->world_speed;
    
    for (s32 index = 0; index < count; ++index)
    {
        ecs_entity_t entity = ecs_create(ecs);
        C_Transform* transform = ecs_add(ecs, entity, ECS_GET_COMP(C_Transform), NULL);
        C_Movement* movement = ecs_add(ecs, entity, ECS_GET_COMP(C_Movement), NULL);
        C_Particle* particle = ecs_add(ecs, entity, ECS_GET_COMP(C_Particle), NULL);
        
        f32 speed = cf_rnd_range_float(rnd, -30.0f, tunnel_world_speed);
        movement->velocity.angle = cf_rnd_range_float(rnd, -CF_PI, CF_PI);
        movement->velocity.radius = make_radial_speed(speed);
        movement->depth = speed;
        
        particle->life_time = cf_rnd_range_float(rnd, 0.8f, 1.0f);
        particle->start_radius = cf_rnd_range_float(rnd, 2.0f, 5.0f);
        particle->end_radius = particle->start_radius;
        
        transform->polar = polar_position;
        transform->depth = depth;
        transform->draw_radius = particle->start_radius;
    }
}

void make_thruster_particles(Polar polar_position, f32 depth, s32 count)
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    CF_Rnd* rnd = &world->particles_rnd;
    
    Tunnel* tunnel = &world->tunnel;
    f32 inverse_speed_ratio = cf_remap(tunnel->speed, TUNNEL_MIN_SPEED, TUNNEL_MAX_SPEED, 1.0f, 0.1f);
    
    for (s32 index = 0; index < count; ++index)
    {
        ecs_entity_t entity = ecs_create(ecs);
        C_Transform* transform = ecs_add(ecs, entity, ECS_GET_COMP(C_Transform), NULL);
        C_Movement* movement = ecs_add(ecs, entity, ECS_GET_COMP(C_Movement), NULL);
        C_Particle* particle = ecs_add(ecs, entity, ECS_GET_COMP(C_Particle), NULL);
        
        f32 speed = tunnel->speed;
        f32 angle_min = -inverse_speed_ratio * 0.1f;
        f32 angle_max = inverse_speed_ratio * 0.1f;
        movement->velocity.angle = cf_rnd_range_float(rnd, angle_min, angle_max );
        movement->velocity.radius = make_radial_speed(speed + 10.0f * cf_sign(speed));
        movement->depth = speed;
        
        particle->life_time = cf_rnd_range_float(rnd, 0.8f, 1.0f);
        particle->start_radius = cf_rnd_range_float(rnd, 1.5f, 2.5f);
        particle->end_radius = cf_rnd_range_float(rnd, 7.0f, 10.0f);
        
        transform->polar = polar_position;
        transform->depth = depth;
        transform->draw_radius = particle->start_radius;
    }
}

void comp_weapon_destructor(ecs_t* ecs, ecs_entity_t entity, void* comp_ptr)
{
    C_Weapon* weapon = (C_Weapon*)comp_ptr;
    if (weapon)
    {
        cf_destroy_coroutine(weapon->co);
    }
}

void comp_ai_destructor(ecs_t* ecs, ecs_entity_t entity, void* comp_ptr)
{
    C_AI* ai = (C_AI*)comp_ptr;
    if (ai)
    {
        cf_destroy_coroutine(ai->co);
    }
}

ecs_ret_t system_update_invulnerable_timers(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    ecs_comp_t comp_invulnerable = ECS_GET_COMP(C_Invulnerable);
    f32 dt = s_app->world.dt;
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Invulnerable* invulnerable = ecs_get(ecs, entity, comp_invulnerable);
        
        invulnerable->timer -= dt;
    }
    
    return 0;
}

ecs_ret_t system_update_healths(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    ecs_comp_t comp_health = ECS_GET_COMP(C_Health);
    f32 dt = s_app->world.dt;
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Health* health = ecs_get(ecs, entity, comp_health);
        
        health->delta = health->value - health->prev_value;
        health->prev_value = health->value;
    }
    
    return 0;
}

ecs_ret_t system_update_players(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    Input* input = &s_app->input;
    CF_V2 direction = input->direction;
    u8 fire = input->fire;
    u8 secondary_fire = input->secondary_fire;
    
    World* world = &s_app->world;
    Tunnel* tunnel = &world->tunnel;
    Camera* camera = &world->camera;
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_player = ECS_GET_COMP(C_Player);
    ecs_comp_t comp_weapon = ECS_GET_COMP(C_Weapon);
    ecs_comp_t comp_health = ECS_GET_COMP(C_Health);
    ecs_comp_t comp_bullet_charger = ECS_GET_COMP(C_Bullet_Charger);
    
    f32 dt = s_app->world.dt;
    f32 rotation_rate = CF_PI * 0.25f;
    f32 angle_drift_rate = 0.1f;
    f32 depth_rate = 2.0f;
    
    tunnel->world_speed -= tunnel->world_speed * TUNNEL_FRICTION * dt;
    tunnel->world_speed = cf_max(tunnel->world_speed, 0.0f);
    
    camera->depth_min = 0;
    camera->depth_max = PLAYER_MAX_DEPTH;
    camera->focus_uv_min = cf_v2(-0.3f, 0.1f);
    camera->focus_uv_max = cf_v2( 0.3f, 0.5f);
    camera->drift_angle_min = -0.3f;
    camera->drift_angle_max = 0.3f;
    
    camera->focus_uv_min = cf_div(camera->focus_uv_min, s_app->screen_scale);
    camera->focus_uv_max = cf_div(camera->focus_uv_max, s_app->screen_scale);
    
    world->player_count = (s32)entity_count;
    
    if (dt <= 0.0f)
    {
        rotation_rate = 0.0f;
        angle_drift_rate = 0.0f;
        depth_rate = 0.0f;
        fire = false;
    }
    
    if (entity_count == 0)
    {
        tunnel->speed = cf_clamp(tunnel->speed - dt, TUNNEL_MIN_SPEED, TUNNEL_MAX_SPEED);
        camera->target_depth = cf_clamp(camera->depth + 0.4f * depth_rate, camera->depth_min, camera->depth_max);
    }
    
    camera->angle_drift *= cf_clamp01(1.0f - dt);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Weapon* weapon = ecs_get(ecs, entity, comp_weapon);
        C_Health* health = ecs_get(ecs, entity, comp_health);
        C_Player* player = ecs_get(ecs, entity, comp_player);
        C_Bullet_Charger* bullet_charger = ecs_get(ecs, entity, comp_bullet_charger);
        
        u8 weapon_fire = fire;
        if (secondary_fire && 
            bullet_charger->state == Bullet_Charger_State_Ready && 
            bullet_charger->absorb_amount >= BULLET_CHARGER_REQUIRED_TO_FIRE)
        {
            weapon_fire = false;
        }
        
        transform->polar = polar_add_angle(transform->polar, direction.x * rotation_rate * dt);
        transform->polar.radius = cf_clamp(transform->polar.radius - direction.y * 0.1f * dt, PLAYER_MIN_RADIUS, TUNNEL_BACK_RADIUS);
        transform->depth = cf_clamp(transform->depth - direction.y * depth_rate * dt, PLAYER_MIN_DEPTH, PLAYER_MAX_DEPTH);
        
        // accelerate when all the way forward
        // decellerate when all the way backwards
        f32 player_speed = cf_remap(transform->depth, PLAYER_MIN_DEPTH, PLAYER_MAX_DEPTH, 1.0f, -2.0f);
        tunnel->speed += player_speed * dt;
        tunnel->speed = cf_clamp(tunnel->speed, TUNNEL_MIN_SPEED, TUNNEL_MAX_SPEED);
        
        if (dt > 0.0f)
        {
            tunnel->world_speed += tunnel->speed * world->stage.difficulty_scaling;
        }
        
        tunnel->angle = transform->polar.angle;
        camera->target_depth = transform->depth;
        camera->angle = transform->polar.angle;
        
        camera->angle_drift += direction.x * angle_drift_rate * dt;
        camera->angle_drift = cf_clamp(camera->angle_drift, camera->drift_angle_min, camera->drift_angle_max);
        
        f32 speed_ratio = tunnel->world_speed / TUNNEL_WORLD_MAX_SPEED;
        
        if (dt > 0 && speed_ratio > 0.05f)
        {
            Polar left = transform->polar;
            Polar right = transform->polar;
            left.radius += 0.025f * TUNNEL_BACK_RADIUS;
            left.angle -= 0.028f;
            right.radius += 0.025f * TUNNEL_BACK_RADIUS;
            right.angle += 0.028f;
            
            if (speed_ratio > 0.6f)
            {
                if (cf_mod_f(world->dt, 0.25f) < 0.1f)
                {
                    make_thruster_particles(left, transform->depth, 1);
                    make_thruster_particles(right, transform->depth, 1);
                }
            }
            else
            {
                if (cf_mod_f(world->dt, 1.0f) < 0.1f)
                {
                    make_thruster_particles(left, transform->depth, 1);
                    make_thruster_particles(right, transform->depth, 1);
                }
            }
        }
        
        if (weapon_fire && weapon->state == Weapon_State_Ready)
        {
            weapon->state = Weapon_State_Firing;
        }
        
        if (secondary_fire && bullet_charger->state == Bullet_Charger_State_Ready)
        {
            bullet_charger->try_fire = true;
        }
        
        if (health->value <= 0)
        {
            ecs_destroy(ecs, entity);
            make_particles(transform->polar, transform->depth, 128);
            camera->shake_duration = 1.0f;
            
            audio_play_sfx("player_die");
        }
    }
    
    camera->depth = cf_lerp(camera->depth, camera->target_depth, dt * 3.0f);
    camera->depth_focus_offset = cf_remap(camera->depth, PLAYER_MIN_DEPTH, PLAYER_MAX_DEPTH, 0.0f, 30.0f);
    camera->shake_duration = cf_max(camera->shake_duration - dt, 0.0f);
    
    if (camera->shake_duration > 0)
    {
        CF_Rnd shake_rnd = cf_rnd_seed((u64)(camera->shake_duration * 100.0f));
        f32 shake_magnitude = 0.05f * camera->shake_duration;
        camera->shake_offset.x = cf_rnd_range_float(&shake_rnd, -shake_magnitude, shake_magnitude);
        camera->shake_offset.y = cf_rnd_range_float(&shake_rnd, -shake_magnitude, shake_magnitude);
    }
    else
    {
        camera->shake_offset.x = 0.0f;
        camera->shake_offset.y = 0.0f;
    }
    
    return 0;
}

ecs_ret_t system_update_enemies(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_enemy = ECS_GET_COMP(C_Enemy);
    ecs_comp_t comp_ai = ECS_GET_COMP(C_AI);
    ecs_comp_t comp_health = ECS_GET_COMP(C_Health);
    ecs_comp_t comp_score = ECS_GET_COMP(C_Score);
    UNUSED(comp_enemy);
    
    ecs_comp_t comp_boss = ECS_GET_COMP(C_Boss);
    
    world->enemy_count = (s32)entity_count;
    
    f32 dt = world->dt;
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_AI* ai = ecs_get(ecs, entity, comp_ai);
        C_Health* health = ecs_get(ecs, entity, comp_health);
        C_Score* score = ecs_get(ecs, entity, comp_score);
        
        cf_coroutine_resume(ai->co);
        
        if (transform->depth > TUNNEL_MAX_DEPTH)
        {
            ecs_destroy(ecs, entity);
        }
        
        if (health->value <= 0)
        {
            s32 particle_count = 16;
            if (ecs_is_ready(ecs, entity))
            {
                if (ecs_has(ecs, entity, comp_boss))
                {
                    particle_count = 128;
                    audio_play_sfx("boss_die");
                }
                else
                {
                    audio_play_sfx("enemy_die");
                }
            }
            world->score += score->value;
            
            make_particles(transform->polar, transform->depth, particle_count);
            ecs_destroy(ecs, entity);
        }
        else if (health->delta < 0)
        {
            audio_play_sfx("enemy_hit");
        }
    }
    
    return 0;
}

// essentially a stub function to measure boss count
ecs_ret_t system_update_bosses(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    ecs_comp_t comp_boss = ECS_GET_COMP(C_Boss);
    UNUSED(comp_boss);
    
    world->boss_count = (s32)entity_count;
    
    return 0;
}

ecs_ret_t system_update_weapons(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    ecs_comp_t comp_weapon = ECS_GET_COMP(C_Weapon);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Weapon* weapon = ecs_get(ecs, entity, comp_weapon);
        
        switch (weapon->state)
        {
            case Weapon_State_Firing:
            {
                cf_coroutine_resume(weapon->co);
                break;
            }
            case Weapon_State_Cooldown:
            {
                if (world->time > weapon->last_fire_time + weapon->fire_rate)
                {
                    weapon->state = Weapon_State_Ready;
                }
                break;
            }
            default:
            break;
        }
    }
    
    return 0;
}

typedef struct Bullet_Query_Info
{
    fixed Sphere* queries;
    fixed ecs_entity_t** bullet_sets;
} Bullet_Query_Info;

ecs_ret_t system_update_bullet_chargers(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_collider = ECS_GET_COMP(C_Collider);
    ecs_comp_t comp_weapon = ECS_GET_COMP(C_Weapon);
    ecs_comp_t comp_bullet_charger = ECS_GET_COMP(C_Bullet_Charger);
    
    ecs_comp_t comp_player = ECS_GET_COMP(C_Player);
    ecs_comp_t comp_enemy = ECS_GET_COMP(C_Enemy);
    ecs_comp_t comp_score = ECS_GET_COMP(C_Score);
    
    f32 dt = world->dt;
    
    // radius goes down the more you charge
    f32 charger_radius_multiplier_min = 3.5f;
    f32 charger_radius_multiplier_max = 2.0f;
    f32 delay = 0.5f;
    
    Bullet_Query_Info* query_info = scratch_alloc(sizeof(Bullet_Query_Info));
    MAKE_SCRATCH_ARRAY(query_info->queries, entity_count);
    MAKE_SCRATCH_ARRAY(query_info->bullet_sets, entity_count);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Collider* collider = ecs_get(ecs, entity, comp_collider);
        C_Weapon* weapon = ecs_get(ecs, entity, comp_weapon);
        C_Bullet_Charger* bullet_charger = ecs_get(ecs, entity, comp_bullet_charger);
        
        if (bullet_charger->try_fire)
        {
            if (bullet_charger->state == Bullet_Charger_State_Ready &&
                bullet_charger->absorb_amount >= BULLET_CHARGER_REQUIRED_TO_FIRE)
            {
                bullet_charger->state = Bullet_Charger_State_Release;
            }
        }
        bullet_charger->try_fire = false;
        
        if (bullet_charger->state == Bullet_Charger_State_Ready)
        {
            f32 t = cf_clamp01((f32)bullet_charger->absorb_amount / BULLET_CHARGER_MAX_ABSORB);
            bullet_charger->absorb_radius_multiplier = cf_lerp(charger_radius_multiplier_min, charger_radius_multiplier_max, t);
            
            f32 r = collider->radius * transform->depth_scale * bullet_charger->absorb_radius_multiplier;
            V3 p = v3(transform->transform.p.x, transform->transform.p.y, transform->depth);
            Sphere sphere = make_sphere(p,  r);
            
            cf_array_push(query_info->queries, sphere);
        }
        else if (bullet_charger->state == Bullet_Charger_State_Release)
        {
            bullet_charger->state = Bullet_Charger_State_Cooldown;
            bullet_charger->cooldown_delay = delay;
            
            if (bullet_charger->absorb_amount)
            {
                f32 t = cf_clamp01((f32)bullet_charger->absorb_amount / BULLET_CHARGER_MAX_ABSORB);
                
                Bullet_Params params = { 0 };
                
                params.polar = transform->polar;
                params.depth = transform->depth;
                params.forward_speed = weapon->forward_speed;
                params.side_speed = weapon->side_speed;
                params.direction = weapon->direction;
                params.depth_direction = weapon->depth_direction;
                
                ecs_entity_t bullet_eater = make_bullet_eater(params, t);
                if (ecs_has(ecs, entity, comp_player))
                {
                    ecs_add(ecs, bullet_eater, comp_player, NULL);
                }
                else
                {
                    ecs_add(ecs, bullet_eater, comp_enemy, NULL);
                }
            }
        }
        else if (bullet_charger->state == Bullet_Charger_State_Cooldown)
        {
            bullet_charger->absorb_amount = 0;
            bullet_charger->cooldown_delay -= dt;
            
            if (bullet_charger->cooldown_delay <= 0.0f)
            {
                bullet_charger->state = Bullet_Charger_State_Ready;
            }
        }
    }
    
    if (cf_array_count(query_info->queries))
    {
        ecs_set_system_udata(ecs, ECS_GET_SYSTEM(system_collision_test_bullets), query_info);
        ecs_run_system(ecs, ECS_GET_SYSTEM(system_collision_test_bullets), 0);
        
        ecs_comp_t comp_bullet = ECS_GET_COMP(C_Bullet);
        
        for (size_t index = 0; index < entity_count; ++index)
        {
            ecs_entity_t entity = entities[index];
            C_Bullet_Charger* bullet_charger = ecs_get(ecs, entity, comp_bullet_charger);
            
            b32 is_player = ecs_has(ecs, entity, comp_player);
            
            fixed ecs_entity_t* hit_bullets = query_info->bullet_sets[index];
            s32 absorb_amount = 0;
            
            for (s32 hit_bullet_index = 0; hit_bullet_index < cf_array_count(hit_bullets); ++hit_bullet_index)
            {
                ecs_entity_t bullet_entity = hit_bullets[hit_bullet_index];
                
                if (ecs_is_ready(ecs, bullet_entity))
                {
                    C_Transform* transform = ecs_get(ecs, bullet_entity, comp_transform);
                    C_Bullet* bullet = ecs_get(ecs, bullet_entity, comp_bullet);
                    C_Score* score = ecs_get(ecs, bullet_entity, comp_score);
                    b32 is_bullet_player_owned = ecs_has(ecs, bullet_entity, comp_player);
                    b32 can_absorb = is_player != is_bullet_player_owned;
                    
                    if (!bullet->is_absorbed && can_absorb)
                    {
                        bullet->is_absorbed = true;
                        absorb_amount++;
                        make_particles(transform->polar, transform->depth, 1);
                        
                        if (!is_bullet_player_owned)
                        {
                            world->score += score->value;
                        }
                        
                        audio_play_sfx("bullet_graze");
                    }
                }
            }
            
            bullet_charger->absorb_amount += absorb_amount;
            bullet_charger->absorb_amount = cf_clamp(bullet_charger->absorb_amount, 0, BULLET_CHARGER_MAX_ABSORB);
        }
    }
    
    return 0;
}

ecs_ret_t system_update_bullet_eaters(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_collider = ECS_GET_COMP(C_Collider);
    ecs_comp_t comp_bullet_eater = ECS_GET_COMP(C_Bullet_Eater);
    UNUSED(comp_bullet_eater);
    
    ecs_comp_t comp_score = ECS_GET_COMP(C_Score);
    ecs_comp_t comp_enemy = ECS_GET_COMP(C_Enemy);
    
    f32 dt = world->dt;
    
    Bullet_Query_Info* query_info = scratch_alloc(sizeof(Bullet_Query_Info));
    MAKE_SCRATCH_ARRAY(query_info->queries, entity_count);
    MAKE_SCRATCH_ARRAY(query_info->bullet_sets, entity_count);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Collider* collider = ecs_get(ecs, entity, comp_collider);
        
        f32 r = collider->radius * transform->depth_scale;
        V3 p = v3(transform->transform.p.x, transform->transform.p.y, transform->depth);
        Sphere sphere = make_sphere(p,  r);
        
        cf_array_push(query_info->queries, sphere);
    }
    
    if (cf_array_count(query_info->queries))
    {
        ecs_set_system_udata(ecs, ECS_GET_SYSTEM(system_collision_test_bullets), query_info);
        ecs_run_system(ecs, ECS_GET_SYSTEM(system_collision_test_bullets), 0);
        
        for (size_t index = 0; index < entity_count; ++index)
        {
            fixed ecs_entity_t* hit_bullets = query_info->bullet_sets[index];
            
            for (s32 hit_bullet_index = 0; hit_bullet_index < cf_array_count(hit_bullets); ++hit_bullet_index)
            {
                ecs_entity_t bullet_entity = hit_bullets[hit_bullet_index];
                
                if (ecs_is_ready(ecs, bullet_entity))
                {
                    C_Transform* transform = ecs_get(ecs, bullet_entity, comp_transform);
                    C_Score* score = ecs_get(ecs, bullet_entity, comp_score);
                    
                    if (ecs_has(ecs, bullet_entity, comp_enemy))
                    {
                        world->score += score->value * 2;
                    }
                    
                    make_particles(transform->polar, transform->depth, 2);
                    ecs_destroy(ecs, bullet_entity);
                    audio_play_sfx("bullet_eat");
                }
            }
        }
    }
    
    return 0;
}

ecs_ret_t system_collision_test_bullets(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_collider = ECS_GET_COMP(C_Collider);
    ecs_comp_t comp_bullet = ECS_GET_COMP(C_Bullet);
    
    Bullet_Query_Info* query_info = (Bullet_Query_Info*)udata;
    
    for (s32 query_index = 0; query_index < cf_array_count(query_info->queries); ++query_index)
    {
        Sphere query_sphere = query_info->queries[query_index];
        fixed ecs_entity_t* hit_bullets = NULL;
        MAKE_SCRATCH_ARRAY(hit_bullets, entity_count);
        cf_array_push(query_info->bullet_sets, hit_bullets);
        
        CF_ASSERT(cf_array_count(hit_bullets) == 0);
        
        for (size_t index = 0; index < entity_count; ++index)
        {
            ecs_entity_t entity = entities[index];
            C_Transform* transform = ecs_get(ecs, entity, comp_transform);
            C_Collider* collider = ecs_get(ecs, entity, comp_collider);
            C_Bullet* bullet = ecs_get(ecs, entity, comp_bullet);
            
            V3 p = v3(transform->transform.p.x, transform->transform.p.y, transform->depth);
            Sphere sphere = make_sphere(p, collider->radius * transform->depth_scale);
            
            if (sphere_to_sphere(query_sphere, sphere))
            {
                cf_array_push(hit_bullets, entity);
            }
        }
    }
    
    return 0;
}

ecs_ret_t system_update_particles(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_particle = ECS_GET_COMP(C_Particle);
    
    f32 dt = world->dt;
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Particle* particle = ecs_get(ecs, entity, comp_particle);
        
        particle->life_time -= dt;
        transform->draw_radius = cf_lerp(particle->start_radius, particle->end_radius, 1.0f - cf_clamp01(particle->life_time));
        
        if (particle->life_time < 0)
        {
            ecs_destroy(ecs, entity);
        }
    }
    
    return 0;
}

ecs_ret_t system_update_bullets(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_bullet = ECS_GET_COMP(C_Bullet);
    UNUSED(comp_bullet);
    
    f32 dt = s_app->world.dt;
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        
        if (transform->depth > BULLET_MAX_DEPTH || transform->depth < BULLET_MIN_DEPTH)
        {
            ecs_destroy(ecs, entity);
        }
    }
    
    return 0;
}

ecs_ret_t system_update_movements(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_movement = ECS_GET_COMP(C_Movement);
    
    Tunnel* tunnel = &world->tunnel;
    f32 tunnel_speed = tunnel->speed * world->stage.difficulty_scaling;
    
    f32 dt = s_app->world.dt;
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Movement* movement = ecs_get(ecs, entity, comp_movement);
        
        transform->polar = polar_add_angle(transform->polar, movement->velocity.angle * dt);
        transform->polar.radius = cf_clamp01(transform->polar.radius + movement->velocity.radius * dt * tunnel_speed);
        transform->depth += movement->depth * dt * tunnel_speed;
    }
    
    return 0;
}

ecs_ret_t system_update_polar_to_transform(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    f32 tunnel_speed_modifier = tunnel_get_speed_modifier();
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        
        transform->prev_transform = transform->transform;
        transform->prev_depth = transform->depth;
        transform->prev_depth_scale = transform->depth_scale;
        transform->transform.p = polar_to_screen_v2(transform->polar);
        transform->transform.r = cf_sincos(transform->polar.angle + CF_PI * 0.5f);
        
        f32 depth01 = cf_clamp01(transform->depth / TUNNEL_MAX_DEPTH);
        transform->depth_scale = CF_POWF(depth01, 8.0f / (depth01 + 1.0f));
        
        f32 depth = (TUNNEL_MAX_DEPTH - transform->depth) * TUNNEL_BACK_RADIUS / tunnel_speed_modifier;
        
        CF_V2 p = transform->transform.p;
        CF_V2 tp = tunnel_path(depth);
        p = cf_add(tp, p);
        
        transform->draw_position = p;
    }
    
    return 0;
}

typedef struct Eye_Query_Info
{
    fixed Polar* queries;
    fixed CF_V2* directions;
} Eye_Query_Info;

ecs_ret_t system_update_player_eyes(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_eye = ECS_GET_COMP(C_Eye);
    ecs_comp_t comp_player = ECS_GET_COMP(C_Player);
    
    UNUSED(comp_player);
    
    Eye_Query_Info* query_info = scratch_alloc(sizeof(Eye_Query_Info));
    CF_MEMSET(query_info, 0, sizeof(Eye_Query_Info));
    
    MAKE_SCRATCH_ARRAY(query_info->queries, (s32)entity_count);
    MAKE_SCRATCH_ARRAY(query_info->directions, (s32)entity_count);
    
    f32 dt = s_app->world.dt;
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Eye* eye = ecs_get(ecs, entity, comp_eye);
        
        eye->look_strength = cf_clamp01(eye->look_strength - dt);
        
        cf_array_push(query_info->queries, transform->polar);
    }
    
    ecs_set_system_udata(ecs, ECS_GET_SYSTEM(system_find_nearest_player_to_enemy), query_info);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_find_nearest_player_to_enemy), 0);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Eye* eye = ecs_get(ecs, entity, comp_eye);
        
        CF_V2 direction = query_info->directions[index];
        if (cf_len_sq(direction) > 0)
        {
            eye->direction = direction;
            eye->look_strength = 1.0f;
        }
    }
    
    return 0;
}

ecs_ret_t system_find_nearest_player_to_enemy(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_enemy = ECS_GET_COMP(C_Enemy);
    UNUSED(comp_transform);
    UNUSED(comp_enemy);
    
    Eye_Query_Info* query_info = (Eye_Query_Info*)udata;
    
    find_nearest_eye_target_directions(query_info->queries, query_info->directions, entities, entity_count);
    
    return 0;
}

ecs_ret_t system_update_enemy_eyes(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_eye = ECS_GET_COMP(C_Eye);
    ecs_comp_t comp_enemy = ECS_GET_COMP(C_Enemy);
    
    UNUSED(comp_enemy);
    
    Eye_Query_Info* query_info = scratch_alloc(sizeof(Eye_Query_Info));
    CF_MEMSET(query_info, 0, sizeof(Eye_Query_Info));
    
    MAKE_SCRATCH_ARRAY(query_info->queries, (s32)entity_count);
    MAKE_SCRATCH_ARRAY(query_info->directions, (s32)entity_count);
    
    f32 dt = s_app->world.dt;
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Eye* eye = ecs_get(ecs, entity, comp_eye);
        
        eye->look_strength = cf_clamp01(eye->look_strength - dt);
        
        cf_array_push(query_info->queries, transform->polar);
    }
    
    ecs_set_system_udata(ecs, ECS_GET_SYSTEM(system_find_nearest_enemy_to_player), query_info);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_find_nearest_enemy_to_player), 0);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Eye* eye = ecs_get(ecs, entity, comp_eye);
        
        CF_V2 direction = query_info->directions[index];
        if (cf_len_sq(direction) > 0)
        {
            eye->direction = direction;
            eye->look_strength = 1.0f;
        }
    }
    
    return 0;
}

ecs_ret_t system_find_nearest_enemy_to_player(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_player = ECS_GET_COMP(C_Player);
    UNUSED(comp_transform);
    UNUSED(comp_player);
    
    Eye_Query_Info* query_info = (Eye_Query_Info*)udata;
    
    find_nearest_eye_target_directions(query_info->queries, query_info->directions, entities, entity_count);
    
    return 0;
}

void find_nearest_eye_target_directions(fixed Polar* queries, fixed CF_V2* directions, ecs_entity_t* entities, size_t entity_count)
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    
    for (s32 query_index = 0; query_index < cf_array_count(queries); ++query_index)
    {
        Polar query = queries[query_index];
        f32 closest_distance = F32_MAX;
        CF_V2 direction = cf_v2(0, 0);
        f32 closest_depth = F32_MAX;
        
        for (size_t index = 0; index < entity_count; ++index)
        {
            ecs_entity_t entity = entities[index];
            C_Transform* transform = ecs_get(ecs, entity, comp_transform);
            
            f32 distance = cf_abs(query.radius - transform->polar.radius);
            
            if (distance < closest_distance)
            {
                closest_distance = distance;
                direction.y = query.radius - transform->polar.radius;
                
                f32 angle = transform->polar.angle;
                f32 query_angle = query.angle;
                f32 angle_diff = angle - query_angle;
                
                if (cf_abs(angle_diff) > CF_PI)
                {
                    if (query_angle > angle)
                    {
                        angle_diff += 2.0f * CF_PI;
                    }
                    else
                    {
                        angle_diff -= 2.0f * CF_PI;
                    }
                }
                
                direction.x = angle_diff / CF_PI * 0.5f;
            }
        }
        
        directions[query_index] = cf_safe_norm(direction);
    }
}

// collision begin

typedef struct Query_Hit
{
    s32 index;
    ecs_entity_t entity;
} Query_Hit;

typedef struct Query_Info
{
    fixed Sphere_Sweep* queries;
    fixed Query_Hit* hits;
} Query_Info;

ecs_ret_t system_update_player_bullet_query(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_collider = ECS_GET_COMP(C_Collider);
    ecs_comp_t comp_bullet = ECS_GET_COMP(C_Bullet);
    ecs_comp_t comp_player = ECS_GET_COMP(C_Player);
    
    UNUSED(comp_bullet);
    UNUSED(comp_player);
    
    Query_Info* query_info = scratch_alloc(sizeof(Query_Info));
    CF_MEMSET(query_info, 0, sizeof(Query_Info));
    MAKE_SCRATCH_ARRAY(query_info->queries, entity_count);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Collider* collider = ecs_get(ecs, entity, comp_collider);
        
        V3 p0 = v3(transform->prev_transform.p.x, transform->prev_transform.p.y, transform->prev_depth);
        Sphere s0 = make_sphere(p0,  collider->radius * transform->prev_depth_scale);
        
        V3 p1 = v3(transform->transform.p.x, transform->transform.p.y, transform->depth);
        Sphere s1 = make_sphere(p1, collider->radius * transform->depth_scale);
        
        Sphere_Sweep sweep = { s0, s1 };
        
        cf_array_push(query_info->queries, sweep);
    }
    
    ecs_set_system_udata(ecs, ECS_GET_SYSTEM(system_collision_test_player_hit_enemy), query_info);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_collision_test_player_hit_enemy), 0);
    
    for (s32 hit_index = 0; hit_index < cf_array_count(query_info->hits); ++hit_index)
    {
        Query_Hit hit = query_info->hits[hit_index];
        C_Transform* transform = ecs_get(ecs, entities[hit.index], comp_transform);
        make_particles(transform->polar, transform->depth, 4);
    }
    
    collision_resolve_hits(query_info->hits, entities, entity_count);
    
    return 0;
}

ecs_ret_t system_collision_test_player_hit_enemy(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_collider = ECS_GET_COMP(C_Collider);
    ecs_comp_t comp_enemy = ECS_GET_COMP(C_Enemy);
    ecs_comp_t comp_invulnerable = ECS_GET_COMP(C_Invulnerable);
    
    UNUSED(comp_transform);
    UNUSED(comp_collider);
    UNUSED(comp_enemy);
    
    fixed ecs_entity_t* hit_test_entities = NULL;
    MAKE_SCRATCH_ARRAY(hit_test_entities, entity_count);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Invulnerable* invulnerable = ecs_get(ecs, entity, comp_invulnerable);
        
        if (invulnerable->timer <= 0.0f)
        {
            cf_array_push(hit_test_entities, entity);
        }
    }
    
    Query_Info* query_info = (Query_Info*)udata;
    
    query_info->hits = collision_test_hit_entities(query_info->queries, hit_test_entities, cf_array_count(hit_test_entities));
    
    return 0;
}

ecs_ret_t system_update_enemy_bullet_query(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_collider = ECS_GET_COMP(C_Collider);
    ecs_comp_t comp_bullet = ECS_GET_COMP(C_Bullet);
    ecs_comp_t comp_enemy = ECS_GET_COMP(C_Enemy);
    
    UNUSED(comp_bullet);
    UNUSED(comp_enemy);
    
    Query_Info* query_info = scratch_alloc(sizeof(Query_Info));
    CF_MEMSET(query_info, 0, sizeof(Query_Info));
    MAKE_SCRATCH_ARRAY(query_info->queries, entity_count);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Collider* collider = ecs_get(ecs, entity, comp_collider);
        
        V3 p0 = v3(transform->prev_transform.p.x, transform->prev_transform.p.y, transform->prev_depth);
        Sphere s0 = make_sphere(p0,  collider->radius * transform->prev_depth_scale);
        
        V3 p1 = v3(transform->transform.p.x, transform->transform.p.y, transform->depth);
        Sphere s1 = make_sphere(p1, collider->radius * transform->depth_scale);
        
        Sphere_Sweep sweep = { s0, s1 };
        
        cf_array_push(query_info->queries, sweep);
    }
    
    ecs_set_system_udata(ecs, ECS_GET_SYSTEM(system_collision_test_enemy_hit_player), query_info);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_collision_test_enemy_hit_player), 0);
    
    for (s32 hit_index = 0; hit_index < cf_array_count(query_info->hits); ++hit_index)
    {
        Query_Hit hit = query_info->hits[hit_index];
        C_Transform* transform = ecs_get(ecs, entities[hit.index], comp_transform);
        make_particles(transform->polar, transform->depth, 4);
    }
    
    collision_resolve_hits(query_info->hits, entities, entity_count);
    
    return 0;
}

ecs_ret_t system_update_enemy_collision_query(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_collider = ECS_GET_COMP(C_Collider);
    ecs_comp_t comp_enemy = ECS_GET_COMP(C_Enemy);
    
    UNUSED(comp_enemy);
    
    Query_Info* query_info = scratch_alloc(sizeof(Query_Info));
    CF_MEMSET(query_info, 0, sizeof(Query_Info));
    MAKE_SCRATCH_ARRAY(query_info->queries, entity_count);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Collider* collider = ecs_get(ecs, entity, comp_collider);
        
        V3 p0 = v3(transform->prev_transform.p.x, transform->prev_transform.p.y, transform->prev_depth);
        Sphere s0 = make_sphere(p0,  collider->radius * transform->prev_depth_scale);
        
        V3 p1 = v3(transform->transform.p.x, transform->transform.p.y, transform->depth);
        Sphere s1 = make_sphere(p1, collider->radius * transform->depth_scale);
        
        Sphere_Sweep sweep = { s0, s1 };
        
        cf_array_push(query_info->queries, sweep);
    }
    
    ecs_set_system_udata(ecs, ECS_GET_SYSTEM(system_collision_test_enemy_hit_player), query_info);
    ecs_run_system(ecs, ECS_GET_SYSTEM(system_collision_test_enemy_hit_player), 0);
    
    collision_resolve_hits(query_info->hits, entities, entity_count);
    
    return 0;
}

ecs_ret_t system_collision_test_enemy_hit_player(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_collider = ECS_GET_COMP(C_Collider);
    ecs_comp_t comp_player = ECS_GET_COMP(C_Player);
    ecs_comp_t comp_invulnerable = ECS_GET_COMP(C_Invulnerable);
    
    UNUSED(comp_transform);
    UNUSED(comp_collider);
    UNUSED(comp_player);
    
    fixed ecs_entity_t* hit_test_entities = NULL;
    MAKE_SCRATCH_ARRAY(hit_test_entities, entity_count);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Invulnerable* invulnerable = ecs_get(ecs, entity, comp_invulnerable);
        
        if (invulnerable->timer <= 0.0f)
        {
            cf_array_push(hit_test_entities, entity);
        }
    }
    
    Query_Info* query_info = (Query_Info*)udata;
    
    query_info->hits = collision_test_hit_entities(query_info->queries, hit_test_entities, cf_array_count(hit_test_entities));
    
    return 0;
}

fixed Query_Hit* collision_test_hit_entities(fixed Sphere_Sweep* query_spheres, ecs_entity_t* entities, size_t entity_count)
{
    ecs_t* ecs = s_app->world.ecs;
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_collider = ECS_GET_COMP(C_Collider);
    
    fixed Query_Hit* hits = NULL;
    MAKE_SCRATCH_ARRAY(hits, cf_array_count(query_spheres));
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Collider* collider = ecs_get(ecs, entity, comp_collider);
        
        V3 p = v3(transform->transform.p.x, transform->transform.p.y, transform->depth);
        Sphere sphere = make_sphere(p, collider->radius * transform->depth_scale);
        
        for (s32 query_index = 0; query_index < cf_array_count(query_spheres); ++query_index)
        {
            Sphere_Sweep sweep = query_spheres[query_index];
            
            if (sphere_sweep(sweep.s0, sweep.s1, sphere))
            {
                Query_Hit hit;
                hit.index = query_index;
                hit.entity = entity;
                
                cf_array_push(hits, hit);
                
                break;
            }
        }
    }
    
    return hits;
}

void collision_resolve_hits(fixed Query_Hit* hits, ecs_entity_t* entities, size_t entity_count)
{
    ecs_t* ecs = s_app->world.ecs;
    
    ecs_comp_t comp_health = ECS_GET_COMP(C_Health);
    ecs_comp_t comp_bullet = ECS_GET_COMP(C_Bullet);
    
    for (s32 hit_index = 0; hit_index < cf_array_count(hits); ++hit_index)
    {
        Query_Hit hit = hits[hit_index];
        ecs_entity_t attacker = entities[hit.index];
        ecs_entity_t entity = hit.entity;
        s32 damage = 1;
        
        if (ecs_is_ready(ecs, attacker))
        {
            if (ecs_has(ecs, attacker, comp_bullet))
            {
                C_Bullet* bullet = ecs_get(ecs, attacker, comp_bullet);
                damage = bullet->damage;
                if (bullet->is_destroy_on_hit)
                {
                    ecs_destroy(ecs, attacker);
                }
            }
        }
        
        if (ecs_is_ready(ecs, entity))
        {
            C_Health* health = ecs_get(ecs, entity, comp_health);
            if (health)
            {
                health->value -= damage;
            }
        }
    }
}

// collision end

ecs_ret_t system_draw_background(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    push_camera_transform();
    
    CF_Color front_color = cf_color_grey();
    CF_Color back_color = cf_color_clear();
    
    // if near == far then rings will only be translated in screen space
    // if near != far then you'll get a tunnel curving perspective
    // so in order to get tunnel near to far you'll need to offset each
    // ring from 1..N by near
    f32 depth = TUNNEL_MAX_DEPTH;
    s32 count = (s32)TUNNEL_MAX_DEPTH;
    s32 segment_end = 0;
    s32 segment_begin = 80;
    s32 segment_count = 80;
    
    cf_draw_push_shader(s_app->tunnel_shader);
    cf_draw_set_uniform_int("u_disabled", TUNNEL_SHADER_DISABLED);
    cf_draw_set_uniform_int("u_use_color_band", TUNNEL_SHADER_USE_COLOR_BAND);
    cf_draw_set_uniform_int("u_color_type", 0);
    {
        // do back to front to avoid doing any extra sorting
        
        for (s32 index = count; index > 0; --index)
        {
            CF_V2 p = tunnel_path((f32)index);
            f32 r = depth / index * TUNNEL_RING_RADIUS;
            
            f32 t = index / (f32)count;
            t = cf_smoothstep(t);
            cf_draw_push_color(cf_color_lerp(front_color, back_color, t));
            cf_draw_push_vertex_attributes(p.x, p.y, 0, 0);
            cf_draw_circle2(p, r, 1.0f);
            cf_draw_pop_vertex_attributes();
            cf_draw_pop_color();
        }
    }
    cf_draw_pop_shader();
    
    // front text
    if (BIT_IS_SET(s_app->debug_mode, Debug_Mode_Depth_Ring))
    {
        dyna char* s = NULL;
        cf_string_fit(s, 32);
        
        if (s_app->debug_ring == -1)
        {
            for (s32 index = count - segment_end; index > 0; --index)
            {
                CF_V2 p = tunnel_path((f32)index);
                f32 r = depth / index * TUNNEL_RING_RADIUS;
                
                CF_V2 pt = cf_v2(p.x, p.y - r);
                cf_string_int(s, count - index);
                cf_draw_text(s, pt, -1);
            }
        }
        else
        {
            s32 index = s_app->debug_ring;
            CF_V2 p = tunnel_path((f32)index);
            f32 r = depth / index * TUNNEL_RING_RADIUS;
            
            CF_V2 pt = cf_v2(p.x, p.y - r);
            cf_string_int(s, count - index);
            cf_draw_text(s, pt, -1);
        }
        cf_string_free(s);
    }
    
    pop_camera_transform();
    return 0;
}

ecs_ret_t system_draw_particles(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    push_camera_transform();
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_particle = ECS_GET_COMP(C_Particle);
    
    CF_Color start_color = cf_color_white();
    CF_Color end_color = cf_color_clear();
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Particle* particle = ecs_get(ecs, entity, comp_particle);
        
        cf_draw_push_color(cf_color_lerp(start_color, end_color, 1.0f - cf_clamp01(particle->life_time)));
        cf_draw_circle_fill2(transform->draw_position, transform->draw_radius * transform->depth_scale);
        cf_draw_pop_color();
    }
    
    pop_camera_transform();
    
    return 0;
}

ecs_ret_t system_draw_enemies(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    push_camera_transform();
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_enemy = ECS_GET_COMP(C_Enemy);
    ecs_comp_t comp_invulnerable = ECS_GET_COMP(C_Invulnerable);
    UNUSED(comp_enemy);
    
    cf_draw_push_shader(s_app->tunnel_shader);
    cf_draw_set_uniform_int("u_disabled", TUNNEL_SHADER_DISABLED);
    cf_draw_set_uniform_int("u_use_color_band", TUNNEL_SHADER_USE_COLOR_BAND);
    cf_draw_set_uniform_int("u_color_type", 2);
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Invulnerable* invulnerable = ecs_get(ecs, entity, comp_invulnerable);
        
        if (is_invulnerable_flicker(invulnerable->timer))
        {
            continue;
        }
        
        cf_draw_push_vertex_attributes(transform->polar.angle, transform->depth_scale, 0, 0.0f);
        draw_enemy(transform->draw_position, transform->transform.r, transform->depth_scale * transform->draw_radius);
        cf_draw_pop_vertex_attributes();
    }
    cf_draw_pop_shader();
    
    pop_camera_transform();
    
    return 0;
}

ecs_ret_t system_draw_enemy_obstacles(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    push_camera_transform();
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_enemy = ECS_GET_COMP(C_Enemy);
    ecs_comp_t comp_invulnerable = ECS_GET_COMP(C_Invulnerable);
    UNUSED(comp_enemy);
    
    cf_draw_push_shader(s_app->tunnel_shader);
    cf_draw_set_uniform_int("u_disabled", TUNNEL_SHADER_DISABLED);
    cf_draw_set_uniform_int("u_use_color_band", TUNNEL_SHADER_USE_COLOR_BAND);
    cf_draw_set_uniform_int("u_color_type", 2);
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Invulnerable* invulnerable = ecs_get(ecs, entity, comp_invulnerable);
        
        if (is_invulnerable_flicker(invulnerable->timer))
        {
            continue;
        }
        
        cf_draw_push_vertex_attributes(transform->polar.angle, transform->depth_scale, 0, 0.0f);
        draw_obstacle(transform->draw_position, transform->transform.r, transform->depth_scale * transform->draw_radius);
        cf_draw_pop_vertex_attributes();
    }
    cf_draw_pop_shader();
    
    pop_camera_transform();
    
    return 0;
}

ecs_ret_t system_draw_bosses(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    push_camera_transform();
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_boss = ECS_GET_COMP(C_Boss);
    ecs_comp_t comp_invulnerable = ECS_GET_COMP(C_Invulnerable);
    UNUSED(comp_boss);
    
    cf_draw_push_shader(s_app->tunnel_shader);
    cf_draw_set_uniform_int("u_disabled", TUNNEL_SHADER_DISABLED);
    cf_draw_set_uniform_int("u_use_color_band", TUNNEL_SHADER_USE_COLOR_BAND);
    cf_draw_set_uniform_int("u_color_type", 2);
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Invulnerable* invulnerable = ecs_get(ecs, entity, comp_invulnerable);
        
        if (is_invulnerable_flicker(invulnerable->timer))
        {
            continue;
        }
        
        cf_draw_push_vertex_attributes(transform->polar.angle, transform->depth_scale, 0, 0.0f);
        draw_boss(transform->draw_position, transform->transform.r, transform->depth_scale * transform->draw_radius);
        cf_draw_pop_vertex_attributes();
    }
    cf_draw_pop_shader();
    
    pop_camera_transform();
    
    return 0;
}

ecs_ret_t system_draw_bullets(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    push_camera_transform();
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_bullet = ECS_GET_COMP(C_Bullet);
    
    CF_Color absorbed_bullet_color = cf_make_color_rgb_f(0.35f, 0.35f, 0.35f);
    CF_Color normal_bullet_color = cf_color_white();
    
    cf_draw_push_shader(s_app->tunnel_shader);
    cf_draw_set_uniform_int("u_disabled", TUNNEL_SHADER_DISABLED);
    cf_draw_set_uniform_int("u_use_color_band", TUNNEL_SHADER_USE_COLOR_BAND);
    cf_draw_set_uniform_int("u_color_type", 1);
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Bullet* bullet = ecs_get(ecs, entity, comp_bullet);
        
        f32 r = transform->depth_scale * transform->draw_radius;
        
        cf_draw_push_vertex_attributes(transform->draw_position.x, transform->draw_position.y, r, transform->polar.angle);
        
        if (bullet->is_absorbed)
        {
            cf_draw_push_color(absorbed_bullet_color);
        }
        else
        {
            cf_draw_push_color(normal_bullet_color);
        }
        cf_draw_circle_fill2(transform->draw_position, r);
        cf_draw_pop_color();
        cf_draw_pop_vertex_attributes();
    }
    cf_draw_pop_shader();
    
    pop_camera_transform();
    
    return 0;
}

ecs_ret_t system_draw_bullet_eaters(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    push_camera_transform();
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_bullet = ECS_GET_COMP(C_Bullet);
    ecs_comp_t comp_bullet_eater = ECS_GET_COMP(C_Bullet_Eater);
    
    UNUSED(comp_bullet);
    
    cf_draw_push_shader(s_app->tunnel_shader);
    cf_draw_set_uniform_int("u_disabled", TUNNEL_SHADER_DISABLED);
    cf_draw_set_uniform_int("u_use_color_band", TUNNEL_SHADER_USE_COLOR_BAND);
    cf_draw_set_uniform_int("u_color_type", 1);
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Bullet_Eater* bullet_eater = ecs_get(ecs, entity, comp_bullet_eater);
        
        f32 r = transform->depth_scale * transform->draw_radius;
        
        cf_draw_push_vertex_attributes(transform->draw_position.x, transform->draw_position.y, r, transform->polar.angle);
        
        draw_bullet_eater(transform->draw_position, transform->transform.r, r, bullet_eater->strength01);
        cf_draw_pop_vertex_attributes();
    }
    cf_draw_pop_shader();
    
    pop_camera_transform();
    
    return 0;
}

ecs_ret_t system_draw_players(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    push_camera_transform();
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_player = ECS_GET_COMP(C_Player);
    ecs_comp_t comp_invulnerable = ECS_GET_COMP(C_Invulnerable);
    
    UNUSED(comp_player);
    
    cf_draw_push_shader(s_app->tunnel_shader);
    cf_draw_set_uniform_int("u_disabled", TUNNEL_SHADER_DISABLED);
    cf_draw_set_uniform_int("u_use_color_band", TUNNEL_SHADER_USE_COLOR_BAND);
    cf_draw_set_uniform_int("u_color_type", 2);
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Invulnerable* invulnerable = ecs_get(ecs, entity, comp_invulnerable);
        
        if (is_invulnerable_flicker(invulnerable->timer))
        {
            continue;
        }
        
        cf_draw_push_vertex_attributes(transform->polar.angle, transform->depth_scale, 0, 0.0f);
        draw_player(transform->draw_position, transform->transform.r, transform->depth_scale * transform->draw_radius, transform->depth_scale * PLAYER_COLLIDER_RADIUS);
        cf_draw_pop_vertex_attributes();
    }
    cf_draw_pop_shader();
    
    pop_camera_transform();
    
    return 0;
}

ecs_ret_t system_draw_bullet_chargers(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    push_camera_transform();
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_collider = ECS_GET_COMP(C_Collider);
    ecs_comp_t comp_bullet_charger = ECS_GET_COMP(C_Bullet_Charger);
    
    CF_Color not_ready_color = cf_color_grey();
    CF_Color ready_color = cf_color_white();
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Collider* collider = ecs_get(ecs, entity, comp_collider);
        C_Bullet_Charger* bullet_charger = ecs_get(ecs, entity, comp_bullet_charger);
        
        if (bullet_charger->state == Bullet_Charger_State_Ready)
        {
            f32 radius = collider->radius * bullet_charger->absorb_radius_multiplier;
            f32 t = (f32)bullet_charger->absorb_amount / BULLET_CHARGER_MAX_ABSORB;
            
            if (bullet_charger->absorb_amount >= BULLET_CHARGER_REQUIRED_TO_FIRE)
            {
                cf_draw_push_color(ready_color);
            }
            else
            {
                cf_draw_push_color(not_ready_color);
            }
            draw_bullet_charger(transform->draw_position, transform->transform.r, radius, t);
            cf_draw_pop_color();
        }
    }
    
    pop_camera_transform();
    
    return 0;
}

ecs_ret_t system_draw_ship_colliders(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    World* world = &s_app->world;
    
    push_camera_transform();
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_collider = ECS_GET_COMP(C_Collider);
    
    dyna char* s = NULL;
    
    cf_draw_push_shader(s_app->tunnel_shader);
    cf_draw_set_uniform_int("u_disabled", TUNNEL_SHADER_DISABLED);
    cf_draw_set_uniform_int("u_use_color_band", TUNNEL_SHADER_USE_COLOR_BAND);
    cf_draw_set_uniform_int("u_color_type", 2);
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Collider* collider = ecs_get(ecs, entity, comp_collider);
        
        cf_draw_push_vertex_attributes(transform->polar.angle, transform->depth_scale, 0, 0.0f);
        cf_draw_circle2(transform->draw_position, collider->radius * transform->depth_scale, 1.0f);
        cf_draw_pop_vertex_attributes();
        
        cf_string_fmt(s, "%.2f", transform->polar.angle);
        cf_draw_text(s, transform->draw_position, -1);
    }
    cf_draw_pop_shader();
    
    pop_camera_transform();
    
    cf_string_free(s);
    
    return 0;
}

ecs_ret_t system_draw_eyes(ecs_t* ecs, ecs_entity_t* entities, size_t entity_count, void* udata)
{
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_eye = ECS_GET_COMP(C_Eye);
    ecs_comp_t comp_invulnerable = ECS_GET_COMP(C_Invulnerable);
    
    UNUSED(comp_transform);
    UNUSED(comp_eye);
    
    fixed ecs_entity_t* viewable_entities = NULL;
    MAKE_SCRATCH_ARRAY(viewable_entities, entity_count);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Invulnerable* invulnerable = ecs_get(ecs, entity, comp_invulnerable);
        
        if (!is_invulnerable_flicker(invulnerable->timer))
        {
            cf_array_push(viewable_entities, entity);
        }
    }
    
    draw_eyes(viewable_entities, cf_array_count(viewable_entities));
    
    return 0;
}

b32 is_invulnerable_flicker(f32 timer)
{
    b32 is_flickering = false;
    if (timer > 0)
    {
        if (timer > 1.0f)
        {
            if (cf_mod_f(timer, 0.25f) > 0.125f)
            {
                is_flickering = true;
            }
        }
        
        if (cf_mod_f(timer, 0.1f) > 0.05f)
        {
            is_flickering = true;
        }
    }
    return is_flickering;
}

void draw_player(CF_V2 position, CF_SinCos rotation, f32 radius, f32 collider_radius)
{
    f32 thruster_radius = radius * 0.25f;
    
    // body
    {
        CF_V2 top = cf_v2(0, radius);
        CF_V2 bottom_left = cf_v2(-radius, -radius);
        CF_V2 bottom_right = cf_v2(radius, -radius);
        
        top = cf_mul_sc_v2(rotation, top);
        bottom_left = cf_mul_sc_v2(rotation, bottom_left);
        bottom_right = cf_mul_sc_v2(rotation, bottom_right);
        
        top = cf_add(position, top);
        bottom_left = cf_add(position, bottom_left);
        bottom_right = cf_add(position, bottom_right);
        
        cf_draw_tri(top, bottom_left, bottom_right, 1.0f, 0.0f);
    }
    
    // thruster left
    {
        CF_V2 left = cf_v2(-radius * 0.5f, -radius);
        left= cf_mul_sc_v2(rotation, left);
        left= cf_add(left, position);
        cf_draw_circle2(left, thruster_radius, 1.0f);
    }
    
    // thruster right
    {
        CF_V2 right = cf_v2(radius * 0.5f, -radius);
        right = cf_mul_sc_v2(rotation, right);
        right = cf_add(right, position);
        cf_draw_circle2(right, thruster_radius, 1.0f);
    }
    
    // colider
    {
        cf_draw_push_color(cf_color_grey());
        cf_draw_circle2(position, collider_radius, 1.0f);
        cf_draw_pop_color();
    }
}

void draw_enemy(CF_V2 position, CF_SinCos rotation, f32 radius)
{
    f32 ornament_half_width = radius * 0.25f;
    
    // body
    {
        cf_draw_circle2(position, radius, 1.0f);
    }
    
    // bottom
    {
        CF_V2 p0 = cf_v2(-ornament_half_width, -radius - ornament_half_width);
        CF_V2 p1 = cf_v2(0, -radius);
        CF_V2 p2 = cf_v2( ornament_half_width, -radius - ornament_half_width);
        
        p0 = cf_mul_sc_v2(rotation, p0);
        p1 = cf_mul_sc_v2(rotation, p1);
        p2 = cf_mul_sc_v2(rotation, p2);
        
        p0 = cf_add(position, p0);
        p1 = cf_add(position, p1);
        p2 = cf_add(position, p2);
        
        cf_draw_bezier_line(p0, p1, p2, 8, 1.0f);
    }
}

void draw_obstacle(CF_V2 position, CF_SinCos rotation, f32 radius)
{
    f32 inner_radius = radius * 0.8f;
    // make spike look a tiny bit larger so it seems like you can get a near miss
    f32 spike_radius = radius * 1.25f;
    s32 spike_count = 8;
    f32 spike_side_angle_diff = 0.025f;
    
    // body
    {
        cf_draw_circle2(position, inner_radius, 1.0f);
    }
    
    // spikes
    for (s32 index = 0; index < spike_count; ++index)
    {
        f32 a1 = (f32)index / spike_count * CF_PI * 2.0f;
        f32 a0 = a1 - spike_side_angle_diff * CF_PI * 2.0f;
        f32 a2 = a1 + spike_side_angle_diff * CF_PI * 2.0f;
        
        CF_V2 p0 = cf_v2(inner_radius * cf_cos(a0), inner_radius * cf_sin(a0));
        CF_V2 p1 = cf_v2(spike_radius * cf_cos(a1), spike_radius * cf_sin(a1));
        CF_V2 p2 = cf_v2(inner_radius * cf_cos(a2), inner_radius * cf_sin(a2));
        
        p0 = cf_mul_sc_v2(rotation, p0);
        p1 = cf_mul_sc_v2(rotation, p1);
        p2 = cf_mul_sc_v2(rotation, p2);
        
        p0 = cf_add(position, p0);
        p1 = cf_add(position, p1);
        p2 = cf_add(position, p2);
        
        cf_draw_tri(p0, p1, p2, 1.0f, 0.0f);
    }
}

void draw_boss(CF_V2 position, CF_SinCos rotation, f32 radius)
{
    f32 ornament_half_width = radius * 0.25f;
    
    // body
    {
        CF_V2 top = cf_v2(0, radius);
        CF_V2 bottom_left = cf_v2(-radius, -radius);
        CF_V2 bottom_right = cf_v2(radius, -radius);
        
        top = cf_mul_sc_v2(rotation, top);
        bottom_left = cf_mul_sc_v2(rotation, bottom_left);
        bottom_right = cf_mul_sc_v2(rotation, bottom_right);
        
        top = cf_add(position, top);
        bottom_left = cf_add(position, bottom_left);
        bottom_right = cf_add(position, bottom_right);
        
        cf_draw_tri(top, bottom_left, bottom_right, 1.0f, 0.0f);
    }
    
    // top
    {
        CF_V2 p0 = cf_v2(-ornament_half_width, radius + ornament_half_width);
        CF_V2 p1 = cf_v2(0, radius);
        CF_V2 p2 = cf_v2( ornament_half_width, radius + ornament_half_width);
        
        p0 = cf_mul_sc_v2(rotation, p0);
        p1 = cf_mul_sc_v2(rotation, p1);
        p2 = cf_mul_sc_v2(rotation, p2);
        
        p0 = cf_add(position, p0);
        p1 = cf_add(position, p1);
        p2 = cf_add(position, p2);
        
        cf_draw_bezier_line(p0, p1, p2, 8, 1.0f);
    }
    
    // bottom
    {
        CF_V2 p0 = cf_v2(-ornament_half_width, -radius - ornament_half_width);
        CF_V2 p1 = cf_v2(0, -radius);
        CF_V2 p2 = cf_v2( ornament_half_width, -radius - ornament_half_width);
        
        p0 = cf_mul_sc_v2(rotation, p0);
        p1 = cf_mul_sc_v2(rotation, p1);
        p2 = cf_mul_sc_v2(rotation, p2);
        
        p0 = cf_add(position, p0);
        p1 = cf_add(position, p1);
        p2 = cf_add(position, p2);
        
        cf_draw_bezier_line(p0, p1, p2, 8, 1.0f);
    }
}

void draw_eyes(ecs_entity_t* entities, size_t entity_count)
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    ecs_comp_t comp_transform = ECS_GET_COMP(C_Transform);
    ecs_comp_t comp_eye = ECS_GET_COMP(C_Eye);
    
    f32 eye_radius_ratio = 0.45f;
    f32 eye_iris_ratio = 0.5f;
    f32 eye_pupil_ratio = 0.5f;
    
    push_camera_transform();
    
    CF_Canvas canvas = cf_app_get_canvas();
    
    CF_RenderState state = cf_render_state_defaults();
    state.stencil.enabled = true;
    cf_draw_push_antialias(false);
    cf_draw_push_alpha_discard(true);
    
    // draw eyes corneas
    state.stencil.back = state.stencil.front = (CF_StencilFunction) {
		.compare = CF_COMPARE_FUNCTION_ALWAYS,
		.pass_op = CF_STENCIL_OP_INCREMENT_CLAMP,
	};
	state.stencil.read_mask = 0x0;
	state.stencil.write_mask = 0xFF;
	state.stencil.reference = 1;
	cf_draw_push_render_state(state);
    
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Eye* eye = ecs_get(ecs, entity, comp_eye);
        
        f32 r = transform->draw_radius * transform->depth_scale;
        CF_SinCos rotation = transform->transform.r;
        
        f32 eye_radius = r * eye_radius_ratio;
        
        CF_V2 top = cf_v2(0, eye_radius);
        CF_V2 bottom_left = cf_v2(-eye_radius, -eye_radius);
        CF_V2 bottom_right = cf_v2(eye_radius, -eye_radius);
        
        top = cf_mul_sc_v2(rotation, top);
        bottom_left = cf_mul_sc_v2(rotation, bottom_left);
        bottom_right = cf_mul_sc_v2(rotation, bottom_right);
        
        top = cf_add(transform->draw_position, top);
        bottom_left = cf_add(transform->draw_position, bottom_left);
        bottom_right = cf_add(transform->draw_position, bottom_right);
        
        cf_draw_tri_fill(top, bottom_left, bottom_right, 0.0f);
    }
    cf_render_to(canvas, true);
    cf_draw_pop_render_state();
    
    // draw eyes iris and pupil
    state.stencil.back = state.stencil.front = (CF_StencilFunction) {
		.compare = CF_COMPARE_FUNCTION_ALWAYS,
		.pass_op = CF_STENCIL_OP_INCREMENT_CLAMP,
	};
	state.stencil.read_mask = 0x0;
	state.stencil.write_mask = 0xFF;
	state.stencil.reference = 1;
    
    cf_draw_push_antialias(true);
	cf_draw_push_render_state(state);
    cf_draw_push_color(cf_color_red());
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Eye* eye = ecs_get(ecs, entity, comp_eye);
        
        f32 r = transform->draw_radius * transform->depth_scale;
        CF_SinCos rotation = transform->transform.r;
        
        f32 eye_radius = r * eye_radius_ratio;
        f32 eye_iris_radius = eye_radius * eye_iris_ratio;
        
        CF_V2 direction = cf_mul(eye->direction, eye_radius * 0.5f * eye->look_strength);
        CF_V2 eye_center = cf_mul_sc_v2(rotation, direction);
        eye_center = cf_add(transform->draw_position, eye_center);
        
        cf_draw_circle_fill2(eye_center, eye_iris_radius);
    }
    cf_draw_pop_color();
    cf_render_to(canvas, false);
    
    cf_draw_pop_render_state();
    
    // clear everything outside of eyes
    cf_draw_push_alpha_discard(false);
	state.stencil.back = state.stencil.front = (CF_StencilFunction) {
		.compare = CF_COMPARE_FUNCTION_NOT_EQUAL,
		.pass_op = CF_STENCIL_OP_ZERO,
	};
	state.stencil.read_mask = 0xFF;
	state.stencil.write_mask = 0x0;
	state.stencil.reference = 2;
	cf_draw_push_render_state(state);
	{
		cf_draw_push_color(cf_color_invisible());
		cf_draw_box_fill(cf_make_aabb(cf_neg(s_app->screen_size), s_app->screen_size), 0);
        cf_draw_pop_color();
	}
    cf_render_to(canvas, false);
	cf_draw_pop_render_state();
    
    // draw eyes outline
	cf_draw_push_alpha_discard(true);
    cf_draw_push_color(cf_color_white());
    for (size_t index = 0; index < entity_count; ++index)
    {
        ecs_entity_t entity = entities[index];
        C_Transform* transform = ecs_get(ecs, entity, comp_transform);
        C_Eye* eye = ecs_get(ecs, entity, comp_eye);
        
        f32 r = transform->draw_radius * transform->depth_scale;
        CF_SinCos rotation = transform->transform.r;
        
        f32 eye_radius = r * eye_radius_ratio;
        
        CF_V2 top = cf_v2(0, eye_radius);
        CF_V2 bottom_left = cf_v2(-eye_radius, -eye_radius);
        CF_V2 bottom_right = cf_v2(eye_radius, -eye_radius);
        
        top = cf_mul_sc_v2(rotation, top);
        bottom_left = cf_mul_sc_v2(rotation, bottom_left);
        bottom_right = cf_mul_sc_v2(rotation, bottom_right);
        
        top = cf_add(transform->draw_position, top);
        bottom_left = cf_add(transform->draw_position, bottom_left);
        bottom_right = cf_add(transform->draw_position, bottom_right);
        
        cf_draw_tri(top, bottom_left, bottom_right, 1.0f, 0.0f);
    }
    cf_draw_pop_color();
    cf_render_to(canvas, false);
    pop_camera_transform();
}

void draw_bullet_charger(CF_V2 position, CF_SinCos rotation, f32 radius, f32 strength01)
{
    f32 world_time = s_app->world.time;
    
    f32 inner_radius = radius;
    // make spike look a tiny bit larger so it seems like you can get a near miss
    f32 spike_radius = radius * 1.1f;
    s32 spike_count = (s32)(10 * strength01);
    f32 spike_side_angle_diff = 0.025f;
    
    // body
    {
        cf_draw_circle2(position, inner_radius, 1.0f);
    }
    
    // spikes
    f32 angle_time_offset = cf_mod_f(world_time * 0.25f * strength01, CF_PI * 2.0f);
    for (s32 index = 0; index < spike_count; ++index)
    {
        f32 a1 = ((f32)index / spike_count + angle_time_offset) * CF_PI * 2.0f;
        f32 a0 = a1 - spike_side_angle_diff * CF_PI * 2.0f;
        f32 a2 = a1 + spike_side_angle_diff * CF_PI * 2.0f;
        
        CF_V2 p0 = cf_v2(inner_radius * cf_cos(a0), inner_radius * cf_sin(a0));
        CF_V2 p1 = cf_v2(spike_radius * cf_cos(a1), spike_radius * cf_sin(a1));
        CF_V2 p2 = cf_v2(inner_radius * cf_cos(a2), inner_radius * cf_sin(a2));
        
        p0 = cf_mul_sc_v2(rotation, p0);
        p1 = cf_mul_sc_v2(rotation, p1);
        p2 = cf_mul_sc_v2(rotation, p2);
        
        p0 = cf_add(position, p0);
        p1 = cf_add(position, p1);
        p2 = cf_add(position, p2);
        
        cf_draw_tri_fill(p0, p1, p2, 0.0f);
    }
}

void draw_bullet_eater(CF_V2 position, CF_SinCos rotation, f32 radius, f32 strength01)
{
    f32 world_time = s_app->world.time;
    
    f32 inner_radius = radius * 0.8f;
    // make spike look a tiny bit larger so it seems like you can get a near miss
    f32 spike_radius = radius * 1.25f;
    s32 spike_count = (s32)(8 * (1.0f + strength01));
    f32 spike_side_angle_diff = 0.025f;
    
    // body
    {
        cf_draw_circle_fill2(position, inner_radius);
    }
    
    // spikes
    for (s32 index = 0; index < spike_count; ++index)
    {
        f32 a1 = (f32)index / spike_count * CF_PI * 2.0f;
        f32 a0 = a1 - spike_side_angle_diff * CF_PI * 2.0f;
        f32 a2 = a1 + spike_side_angle_diff * CF_PI * 2.0f;
        
        CF_V2 p0 = cf_v2(inner_radius * cf_cos(a0), inner_radius * cf_sin(a0));
        CF_V2 p1 = cf_v2(spike_radius * cf_cos(a1), spike_radius * cf_sin(a1));
        CF_V2 p2 = cf_v2(inner_radius * cf_cos(a2), inner_radius * cf_sin(a2));
        
        p0 = cf_mul_sc_v2(rotation, p0);
        p1 = cf_mul_sc_v2(rotation, p1);
        p2 = cf_mul_sc_v2(rotation, p2);
        
        p0 = cf_add(position, p0);
        p1 = cf_add(position, p1);
        p2 = cf_add(position, p2);
        
        cf_draw_tri_fill(p0, p1, p2, 0.0f);
    }
    
    if (strength01 > 0.7f)
    {
        inner_radius *= 0.5f;
        spike_radius *= 0.5f;
        f32 offset = radius * cf_sin(world_time * 8.0f) * 1.5f;
        CF_V2 offsets[2] = 
        {
            cf_v2(-offset, 0.0f),
            cf_v2(offset, 0.0f),
        };
        
        for (s32 offset_index = 0; offset_index < CF_ARRAY_SIZE(offsets); ++offset_index)
        {
            CF_V2 offset = offsets[offset_index];
            offset = cf_mul_sc_v2(rotation, offset);
            offset = cf_add(position, offset);
            
            for (s32 index = 0; index < spike_count; ++index)
            {
                f32 a1 = (f32)index / spike_count * CF_PI * 2.0f;
                f32 a0 = a1 - spike_side_angle_diff * CF_PI * 2.0f;
                f32 a2 = a1 + spike_side_angle_diff * CF_PI * 2.0f;
                
                CF_V2 p0 = cf_v2(inner_radius * cf_cos(a0), inner_radius * cf_sin(a0));
                CF_V2 p1 = cf_v2(spike_radius * cf_cos(a1), spike_radius * cf_sin(a1));
                CF_V2 p2 = cf_v2(inner_radius * cf_cos(a2), inner_radius * cf_sin(a2));
                
                p0 = cf_mul_sc_v2(rotation, p0);
                p1 = cf_mul_sc_v2(rotation, p1);
                p2 = cf_mul_sc_v2(rotation, p2);
                
                p0 = cf_add(offset, p0);
                p1 = cf_add(offset, p1);
                p2 = cf_add(offset, p2);
                
                cf_draw_tri_fill(p0, p1, p2, 0.0f);
            }
        }
    }
}

void stage_co(CF_Coroutine co)
{
    World* world = &s_app->world;
    Stage* stage = &world->stage;
    Tunnel* tunnel = &world->tunnel;
    
    f32 player_spawn_delay = 2.0f;
    s32 player_spawn_count = 0;
    
    f32 stage_delay = 5.0f;
    
    while (world->stage.remaining_bosses > 0 || world->enemy_count > 0)
    {
        if (world->player_count < 1)
        {
            player_spawn_delay -= world->dt;
        }
        
        if (player_spawn_delay <= 0.0f && world->player_lives > 0)
        {
            make_player(polar(TUNNEL_BACK_RADIUS, world->tunnel.angle), TUNNEL_MAX_DEPTH);
            player_spawn_delay = 2.0f;
            world->player_lives--;
        }
        
        if (stage_delay > 0.0f)
        {
            stage_delay -= world->dt;
            cf_coroutine_yield(co);
            continue;
        }
        
        if (world->boss_count > 0)
        {
            world->stage.distance_to_boss = TUNNEL_WORLD_MAX_SPEED * (50.0f + 5.0f * stage->difficulty_scaling * stage->index);
            cf_coroutine_yield(co);
            continue;
        }
        
        stage->distance_to_boss -= tunnel->world_speed * world->dt;
        
        if (stage->distance_to_boss <= 0.0f)
        {
            if (stage->remaining_bosses > 0)
            {
                // wait til all small enemies are dead
                if (world->enemy_count < 1)
                {
                    f32 spawn_angle = tunnel->angle + cf_rnd_range_float(&world->rnd, -CF_PI * 0.25f, CF_PI * 0.25f);
                    make_enemy(polar(0.0f, spawn_angle), 0.0f, Enemy_Type_Boss);
                    stage->remaining_bosses--;
                    world->player_lives++;
                }
            }
            
            cf_coroutine_yield(co);
            continue;
        }
        
        if (world->enemy_count < 1)
        {
            s32 batch_min_count = 2 + stage->index / 2;
            s32 batch_max_count = 4 + stage->index;
            f32 angle_min_offset = -CF_PI * 0.25f;
            f32 angle_max_offset = CF_PI * 0.25f;
            
            switch (stage->difficulty)
            {
                case Stage_Difficulty_Normal:
                {
                    batch_min_count = 6 + stage->index / 2;
                    batch_max_count = 8 + stage->index;
                    break;
                }
                case Stage_Difficulty_Hard:
                {
                    batch_min_count = 10 + stage->index / 2;
                    batch_max_count = 14 + stage->index;
                    angle_min_offset = -CF_PI * 0.75f;
                    angle_max_offset = CF_PI * 0.75f;
                    break;
                }
            }
            
            
            s32 spawn_batch_count = cf_rnd_range_int(&world->rnd, batch_min_count, batch_max_count);
            // use a base angle and have the wave spawn nearby
            // having a full random from 0..2PI looks bad and doesn't seem to have a flow
            f32 spawn_angle = tunnel->angle + cf_rnd_range_float(&world->rnd, -CF_PI, CF_PI);
            for (s32 index = 0; index < spawn_batch_count; ++index)
            {
                spawn_angle += cf_rnd_range_float(&world->rnd, angle_min_offset, angle_max_offset);
                Enemy_Type enemy_type = cf_rnd_range_int(&world->rnd, Enemy_Type_Obstacle, Enemy_Type_Normal_Count - 1);
                make_enemy(polar(0.0f, spawn_angle), 0.0f, enemy_type);
            }
        }
        
        cf_coroutine_yield(co);
    }
}

void ai_enemy_stub_co(CF_Coroutine co)
{
    C_AI* ai = (C_AI*)cf_coroutine_get_udata(co);
    ecs_entity_t entity = ai->self;
    C_Movement* movement = ECS_GET(entity, C_Movement);
    
    f32 forward_speed = 15.0f;
    
    movement->velocity.angle = 0.0f;
    movement->velocity.radius = make_radial_speed(forward_speed);
    movement->depth = forward_speed;
    
    while (true)
    {
        cf_coroutine_yield(co);
    }
}

void ai_enemy_simple_co(CF_Coroutine co)
{
    ecs_t* ecs = s_app->world.ecs;
    
    C_AI* ai = (C_AI*)cf_coroutine_get_udata(co);
    ecs_entity_t entity = ai->self;
    C_Movement* movement = ECS_GET(entity, C_Movement);
    C_Weapon* weapon = ECS_GET(entity, C_Weapon);
    
    f32 forward_speed = cf_rnd_range_float(&s_app->world.rnd, 8.0f, 15.0f);
    
    movement->velocity.angle = 0.0f;
    movement->velocity.radius = make_radial_speed(forward_speed);
    movement->depth = forward_speed;
    
    while (true)
    {
        if (weapon->state == Weapon_State_Ready)
        {
            weapon->state = Weapon_State_Firing;
        }
        
        cf_coroutine_yield(co);
    }
}

void ai_enemy_side_dodger_co(CF_Coroutine co)
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    C_AI* ai = (C_AI*)cf_coroutine_get_udata(co);
    ecs_entity_t entity = ai->self;
    C_Movement* movement = ECS_GET(entity, C_Movement);
    C_Weapon* weapon = ECS_GET(entity, C_Weapon);
    
    f32 forward_speed = cf_rnd_range_float(&s_app->world.rnd, 5.0f, 12.0f);
    
    f32 next_turn_time = 1.0f;
    
    movement->velocity.angle = CF_PI * 0.1f;
    movement->velocity.radius = make_radial_speed(forward_speed);
    movement->depth = forward_speed;
    
    while (true)
    {
        next_turn_time -= world->dt;
        
        if (next_turn_time < 0)
        {
            movement->velocity.angle = -movement->velocity.angle;
            next_turn_time = 1.0f;
        }
        
        if (weapon->state == Weapon_State_Ready)
        {
            weapon->state = Weapon_State_Firing;
        }
        
        cf_coroutine_yield(co);
    }
}

void ai_enemy_boss_co(CF_Coroutine co)
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    C_AI* ai = (C_AI*)cf_coroutine_get_udata(co);
    ecs_entity_t entity = ai->self;
    C_Transform* transform = ECS_GET(entity, C_Transform);
    C_Movement* movement = ECS_GET(entity, C_Movement);
    C_Weapon* weapon = ECS_GET(entity, C_Weapon);
    
    f32 forward_speed = 10.0f;
    
    movement->velocity.angle = 0.0f;
    movement->velocity.radius = make_radial_speed(forward_speed);
    movement->depth = forward_speed;
    
    f32 arena_depth_min = 80.0f;
    f32 arena_depth_max = 150.0f;
    
    f32 state_change_rate = 30.0f - 5.0f * world->stage.difficulty_scaling;
    // hover in the back longer depending how many stages have passed
    f32 state_timer = 5.0f + world->stage.index * world->stage.difficulty_scaling;
    state_timer = cf_clamp(state_timer, 5.0f, 20.0f);
    
    f32 wiggle_rate = 1.0f;
    f32 wiggle_timer = 0.0f;
    
    weapon->burst_count = 2;
    weapon->pattern = Bullet_Pattern_Spread_3;
    
    enum
    {
        Boss_State_Charge,
        Boss_State_Bob_Front,
        Boss_State_Flee_Begin,
        Boss_State_Flee,
        Boss_State_Bob_Back,
    };
    b8 state = Boss_State_Bob_Back;
    
    while (true)
    {
        switch (state)
        {
            case Boss_State_Charge:
            {
                if (movement->depth > 0 && weapon->pattern == Bullet_Pattern_Spew)
                {
                    arena_depth_min = 80.0f;
                    arena_depth_max = 100.0f;
                    wiggle_rate = 0.75f;
                }
                
                if (weapon->state == Weapon_State_Cooldown || weapon->state == Weapon_State_Ready)
                {
                    weapon->direction.radius = -1.0f;
                    weapon->depth_direction = -1;
                    weapon->fire_rate = 0.0f;
                    weapon->burst_count = 20;
                    weapon->burst_delay = 0.02f;
                    weapon->pattern = Bullet_Pattern_Spew;
                    weapon->fire_at_player = false;
                    wiggle_rate = 2.0f;
                }
                
                state_timer -= world->dt;
                if (state_timer <= 0.0f)
                {
                    state = Boss_State_Bob_Front;
                    state_timer = state_change_rate;
                }
                
                break;
            }
            case Boss_State_Bob_Front:
            {
                arena_depth_min = 130.0f;
                arena_depth_max = 150.0f;
                wiggle_rate = 1.5f;
                
                if (weapon->state == Weapon_State_Cooldown || weapon->state == Weapon_State_Ready)
                {
                    weapon->direction.radius = 1;
                    weapon->depth_direction = 1;
                    weapon->fire_rate = 1.0f;
                    weapon->burst_count = 2;
                    weapon->burst_delay = 0.1f;
                    weapon->pattern = Bullet_Pattern_Spread_3;
                    weapon->fire_at_player = true;
                    wiggle_rate = 1.5f;
                }
                
                state_timer -= world->dt;
                if (state_timer <= 0.0f)
                {
                    state = Boss_State_Flee_Begin;
                    state_timer = 3.0f;
                }
                
                break;
            }
            case Boss_State_Flee_Begin:
            {
                arena_depth_min = 145.0f;
                arena_depth_max = 150.0f;
                wiggle_rate = 0.3f;
                state_timer -= world->dt;
                if (state_timer <= 0.0f)
                {
                    state = Boss_State_Flee;
                    state_timer = 0.5f;
                }
                
                weapon->burst_count = 0;
                
                break;
            }
            case Boss_State_Flee:
            {
                arena_depth_min = 130.0f;
                arena_depth_max = 150.0f;
                wiggle_rate = 1.5f;
                
                if (movement->depth < 0 && weapon->pattern == Bullet_Pattern_Spew)
                {
                    arena_depth_min = 80.0f;
                    arena_depth_max = 100.0f;
                    wiggle_rate = 0.75f;
                }
                
                if (weapon->state == Weapon_State_Cooldown || weapon->state == Weapon_State_Ready)
                {
                    weapon->direction.radius = 1;
                    weapon->depth_direction = 1;
                    weapon->fire_rate = 0.0f;
                    weapon->burst_count = 10;
                    weapon->burst_delay = 0.02f;
                    weapon->pattern = Bullet_Pattern_Spew;
                    weapon->fire_at_player = false;
                    wiggle_rate = 2.0f;
                }
                
                state_timer -= world->dt;
                if (state_timer <= 0.0f)
                {
                    state = Boss_State_Bob_Back;
                    state_timer = state_change_rate;
                }
                
                break;
            }
            case Boss_State_Bob_Back:
            {
                arena_depth_min = 80.0f;
                arena_depth_max = 100.0f;
                wiggle_rate = 0.75f;
                
                if (weapon->state == Weapon_State_Cooldown || weapon->state == Weapon_State_Ready)
                {
                    weapon->direction.radius = 1;
                    weapon->depth_direction = 1;
                    weapon->fire_rate = 1.0f;
                    weapon->burst_count = 1;
                    weapon->burst_delay = 0.1f;
                    weapon->pattern = Bullet_Pattern_Spread_Scatter_3;
                    weapon->fire_at_player = true;
                    wiggle_rate = 0.75f;
                }
                
                state_timer -= world->dt;
                if (state_timer <= 0.0f)
                {
                    state = Boss_State_Charge;
                    state_timer = 0.5f;
                }
                
                break;
            }
        }
        
        if (transform->depth < arena_depth_min)
        {
            movement->velocity.radius = make_radial_speed(30.0f);
            movement->depth = 30.0f;
        }
        else if (transform->depth > arena_depth_max)
        {
            if (state == Boss_State_Flee)
            {
                movement->velocity.radius = make_radial_speed(-40.0f);
                movement->depth = -40.0f;
            }
            else
            {
                movement->velocity.radius = make_radial_speed(-10.0f);
                movement->depth = -10.0f;
            }
        }
        
        wiggle_timer += world->dt;
        
        if (wiggle_timer > wiggle_rate)
        {
            f32 wiggle_factor = state == Boss_State_Flee_Begin ? 0.4f : 0.1f;
            movement->velocity.angle = cf_sin(cf_cos(world->time) + 0.1f);
            wiggle_timer = cf_mod_f(wiggle_timer, wiggle_rate);
        }
        
        if (weapon->state == Weapon_State_Ready)
        {
            weapon->state = Weapon_State_Firing;
        }
        
        cf_coroutine_yield(co);
    }
}

ecs_comp_t weapon_owner_side(ecs_entity_t entity)
{
    ecs_comp_t side = ECS_GET_COMP(C_Enemy);
    if (ecs_has(s_app->world.ecs, entity, ECS_GET_COMP(C_Player)))
    {
        side = ECS_GET_COMP(C_Player);
    }
    return side;
}

void weapon_co(CF_Coroutine co)
{
    C_Weapon* weapon = (C_Weapon*)cf_coroutine_get_udata(co);
    
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    C_Transform* transform = ecs_get(ecs, weapon->self, ECS_GET_COMP(C_Transform));
    ecs_comp_t side = weapon_owner_side(weapon->self);
    const char* sfx_name = "enemy_bullet_fire";
    if (side.id == ECS_GET_COMP(C_Player).id)
    {
        sfx_name = "player_bullet_fire";
    }
    
    
    while (true)
    {
        if (weapon->state == Weapon_State_Firing)
        {
            Polar weapon_direction = weapon->direction;
            if (weapon->fire_at_player)
            {
                weapon_direction.angle = angle_towards_tunnel_back(transform->polar.angle);
            }
            
            Bullet_Params params = { 0 };
            params.forward_speed = weapon->forward_speed;
            params.side_speed = weapon->side_speed;
            params.direction = weapon_direction;
            params.depth_direction = weapon->depth_direction;
            params.side = side;
            params.sfx_name = sfx_name;
            
            f32 burst_delay = 0.0f;
            s32 burst_counter = 0;
            while (burst_counter < weapon->burst_count)
            {
                burst_delay -= world->dt;
                if (burst_delay <= 0.0f)
                {
                    params.polar = transform->polar;
                    params.depth = transform->depth;
                    
                    switch (weapon->pattern)
                    {
                        case Bullet_Pattern_Simple:
                        {
                            bullet_pattern_simple(params);
                            break;
                        }
                        case Bullet_Pattern_Spread_Speed:
                        {
                            bullet_pattern_spread_speed(params);
                            break;
                        }
                        case Bullet_Pattern_Spread_2:
                        {
                            bullet_pattern_spread_2(params);
                            break;
                        }
                        case Bullet_Pattern_Spread_3:
                        {
                            bullet_pattern_spread_3(params);
                            break;
                        }
                        case Bullet_Pattern_Spread_Scatter_3:
                        {
                            for (s32 index = -1; index < 2; ++index)
                            {
                                Bullet_Params spew_params = params;
                                spew_params.polar = transform->polar;
                                spew_params.depth = transform->depth;
                                spew_params.direction.angle += index * CF_PI * 0.25f;
                                bullet_pattern_spread_3(spew_params);
                                
                                cf_coroutine_yield(co);
                            }
                            break;
                        }
                        case Bullet_Pattern_Spew:
                        {
                            for (s32 index = -1; index < 2; ++index)
                            {
                                Bullet_Params spew_params = params;
                                spew_params.polar = transform->polar;
                                spew_params.depth = transform->depth;
                                spew_params.direction.angle += index * 0.12f;
                                bullet_pattern_spread_2(spew_params);
                                
                                cf_coroutine_yield(co);
                            }
                            break;
                        }
                    }
                    
                    burst_counter++;
                    burst_delay = weapon->burst_delay;
                }
                cf_coroutine_yield(co);
            }
            
            weapon->last_fire_time = world->time;
            weapon->state = Weapon_State_Cooldown;
        }
        cf_coroutine_yield(co);
    }
}

void bullet_pattern_simple(Bullet_Params params)
{
    ecs_t* ecs = s_app->world.ecs;
    
    make_bullet(params);
}

void bullet_pattern_spread_speed(Bullet_Params params)
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    Tunnel* tunnel = &world->tunnel;
    
    f32 speed_ratio = tunnel->world_speed / TUNNEL_WORLD_MAX_SPEED;
    
    // normal shot
    if (speed_ratio < 0.5f)
    {
        make_bullet(params);
    }
    else
    {
        // twin shot
        {
            Bullet_Params twin_params = params;
            
            twin_params.polar = polar_add_angle(params.polar, -0.01f * CF_PI);
            make_bullet(twin_params);
            
            twin_params.polar = polar_add_angle(params.polar, 0.01f * CF_PI);
            make_bullet(twin_params);
        }
        if (speed_ratio >= 0.7f)
        {
            // side
            Bullet_Params side_params = params;
            
            // left side
            side_params.direction.angle = -CF_PI * 1.5f;
            make_bullet(side_params);
            
            // right side
            side_params.direction.angle = CF_PI * 1.5f;
            make_bullet(side_params);
        }
        if (speed_ratio >= 0.8f)
        {
            // wider side
            Bullet_Params wider_side_params = params;
            
            // left side
            wider_side_params.direction.angle = -CF_PI * 2.0f;
            make_bullet(wider_side_params);
            
            // right side
            wider_side_params.direction.angle = CF_PI * 2.0f;
            make_bullet(wider_side_params);
        }
    }
}

void bullet_pattern_spread_2(Bullet_Params params)
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    for (s32 index = -1; index < 2; index += 2)
    {
        Bullet_Params spread_params = params;
        spread_params.direction.angle += index * 0.1f;
        
        make_bullet(spread_params);
    }
}

void bullet_pattern_spread_3(Bullet_Params params)
{
    World* world = &s_app->world;
    ecs_t* ecs = world->ecs;
    
    for (s32 index = -1; index < 2; ++index)
    {
        Bullet_Params spread_params = params;
        spread_params.direction.angle += index * 0.1f;
        
        make_bullet(spread_params);
    }
}
