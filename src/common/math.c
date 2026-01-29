#include "common/math.h"

CF_V2 polar_to_v2(Polar p)
{
    return cf_v2(p.radius * cf_cos(p.angle), p.radius * cf_sin(p.angle));
}

CF_V2 polar_to_screen_v2(Polar p)
{
    f32 r = p.radius;
    if (r > 0.0f)
    {
        r = CF_POWF(r, 1.0f / r);
    }
    
    r = r * cf_max(GAME_WIDTH, GAME_HEIGHT);
    return cf_v2(r * cf_cos(p.angle), r * cf_sin(p.angle));
}

f32 polar_distance_sq(Polar a, Polar b)
{
    return a.radius * a.radius + b.radius * b.radius - 2.0f * a.radius * b.radius * cf_cos(b.angle - a.angle);
}

Polar polar_add_angle(Polar polar, f32 angle)
{
    polar.angle += angle;
    if (polar.angle < 0)
    {
        polar.angle += CF_PI * 2.0f;
    }
    polar.angle = cf_mod_f(polar.angle, CF_PI * 2.0f);
    return polar;
}

b8 sphere_to_sphere(Sphere a, Sphere b)
{
    f32 r = a.r + b.r;
    V3 dp = v3(a.p.x - b.p.x, a.p.y - b.p.y, a.p.z - b.p.z);
    f32 d = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z;
    return d < r * r;
}

b8 sphere_sweep(Sphere s0, Sphere s1, Sphere q)
{
    V3 p0 = s0.p;
    V3 p1 = s1.p;
    
    V3 dp = v3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
    f32 sweep_length_sq = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z;
    
    if (sweep_length_sq == 0.0f)
    {
        return sphere_to_sphere(s0, q);
    }
    
    V3 pq = v3(q.p.x - p0.x, q.p.y - p0.y, q.p.z - p0.z);
    f32 dot = pq.x * dp.x + pq.y * dp.y + pq.z * dp.z;
    f32 t = dot / sweep_length_sq;
    
    f32 epsilon = 1e-7f;
    
    if (t >= -epsilon && t <= 1.0f + epsilon)
    {
        V3 p = v3(p0.x + dp.x * t, p0.y + dp.y * t, p0.z + dp.z * t);
        f32 r = cf_lerp(s0.r, s1.r, cf_clamp01(t));
        Sphere s = make_sphere(p, r);
        
        return sphere_to_sphere(s, q);
    }
    
    return false;
}