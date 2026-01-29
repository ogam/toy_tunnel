#ifndef MACROS_H
#define MACROS_H

#ifndef UNUSED
#define UNUSED(VAR) (void)VAR
#endif

#ifndef fixed
#define fixed
#endif

#ifndef KB
#define KB(X) (1024 * X)
#endif

#ifndef MB
#define MB(X) (KB(1024) * X)
#endif

#ifndef BIT_IS_SET_EX
#define BIT_IS_SET_EX(VALUE, MASK) ((VALUE & (MASK)) == (MASK))
#endif

#ifndef BIT_IS_SET
#define BIT_IS_SET(VALUE, INDEX) BIT_IS_SET_EX(VALUE, (1 << INDEX))
#endif

#ifndef BIT_SET
#define BIT_SET(VALUE, INDEX) (VALUE |= (1 << INDEX))
#endif

#ifndef BIT_UNSET
#define BIT_UNSET(VALUE, INDEX) (VALUE &= ~(1 << INDEX))
#endif

#ifndef BIT_TOGGLE
#define BIT_TOGGLE(VALUE, INDEX) (VALUE ^= (1 << INDEX))
#endif

#ifndef MAKE_SCRATCH_ARRAY
#define MAKE_SCRATCH_ARRAY(ARR, CAPACITY) \
{ \
size_t __buffer_size = sizeof(CF_Ahdr) + sizeof(*ARR) * CAPACITY; \
void* __buffer = scratch_alloc(__buffer_size); \
cf_array_static(ARR, __buffer, (s32)__buffer_size); \
}
#endif

#define TARGET_FRAMERATE 60
#define GAME_WIDTH 1280
#define GAME_HEIGHT 800

#define PLAYER_MIN_RADIUS 0.60f
#define PLAYER_MIN_DEPTH 196.0f
#define PLAYER_MAX_DEPTH 197.0f

#define TUNNEL_MAX_DEPTH 200.0f
#define TUNNEL_MIN_SPEED 1.0f
#define TUNNEL_MAX_SPEED 3.0f
#define TUNNEL_WORLD_MAX_SPEED (TARGET_FRAMERATE * TUNNEL_MAX_SPEED)
#define TUNNEL_BACK_RADIUS 0.65f
#define TUNNEL_FRICTION 1.0f
#define TUNNEL_RING_RADIUS 10.0f

#define TUNNEL_SPEED_CURVE_MODIFIER 3.0f
#define TUNNEL_MAX_DEPTH_MODIFIER cf_sqrt(TUNNEL_SPEED_CURVE_MODIFIER)
#define TUNNEL_MIN_DEPTH_MODIFIER (cf_sqrt(TUNNEL_SPEED_CURVE_MODIFIER) * TUNNEL_BACK_RADIUS)

#define BULLET_COLLIDER_RADIUS 8.0f
#define PLAYER_COLLIDER_RADIUS 20.0f
#define PLAYER_DRAW_RADIUS 40.0f
#define OBSTACLE_COLLIDER_RADIUS 100.0f
#define ENEMY_COLLIDER_RADIUS 80.0f
#define BOSS_COLLIDER_RADIUS 200.0f
#define BULLET_EATER_COLLIDER_RADIUS 120.0f

#define BULLET_CHARGER_REQUIRED_TO_FIRE 5
#define BULLET_CHARGER_MAX_ABSORB 20

#define TUNNEL_SHADER_DISABLED 0
#define TUNNEL_SHADER_USE_COLOR_BAND 1

#define BULLET_MIN_DEPTH -80.0f
#define BULLET_MAX_DEPTH (TUNNEL_MAX_DEPTH + 50.0f)

#endif //MACROS_H
