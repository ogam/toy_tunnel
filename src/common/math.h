#ifndef MATH_H
#define MATH_H

typedef struct Polar
{
    f32 radius;
    f32 angle;
} Polar;

typedef struct V3
{
    f32 x;
    f32 y;
    f32 z;
} V3;

typedef struct Sphere
{
    V3 p;
    f32 r;
} Sphere;

typedef struct Sphere_Sweep
{
    Sphere s0;
    Sphere s1;
} Sphere_Sweep;

inline Polar polar(f32 radius, f32 angle)
{
    return (Polar){.radius = radius, .angle = angle};
}

CF_V2 polar_to_v2(Polar p);
CF_V2 polar_to_screen_v2(Polar p);
f32 polar_distance_sq(Polar a, Polar b);

// handles wrapping to keep angle from 0-2PI
Polar polar_add_angle(Polar polar, f32 angle);

inline V3 v3(f32 x, f32 y, f32 z)
{
    return (V3){ x, y, z };
}

inline Sphere make_sphere(V3 v, f32 radius)
{
    return (Sphere){ v, radius };
}

b8 sphere_to_sphere(Sphere a, Sphere b);
// due to radius is dynamically scaled over depth, the radius from s0 to s1 varies
// so you cannot do a capsule to sphere test
b8 sphere_sweep(Sphere s0, Sphere s1, Sphere q);

#endif //MATH_H
