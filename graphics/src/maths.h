#pragma once

#include "common.h"

#include <stdint.h>

#define pie 3.14159265358f

f64 todeg(f64 rad);
f64 torad(f64 deg);

typedef struct {
	u32 x;
	u32 y;
	u32 z;
	u32 w;
} v4u;

u32 v4u_magnitude(v4u v);
v4u make_v4u(u32 x, u32 y, u32 z, u32 w);
v4u v4u_mul(v4u a, v4u b);
v4u v4u_normalised(v4u v);
u32 v4u_dot(v4u a, v4u b);
v4u v4u_add(v4u a, v4u b);
v4u v4u_scale(v4u v, u32 s);
v4u v4u_div(v4u a, v4u b);
v4u v4u_sub(v4u a, v4u b);

typedef struct {
	f32 x;
	f32 y;
	f32 z;
} v3f;

f32 v3f_magnitude(v3f v);
v3f make_v3f(f32 x, f32 y, f32 z);
v3f v3f_mul(v3f a, v3f b);
v3f v3f_cross(v3f a, v3f b);
v3f v3f_normalised(v3f v);
f32 v3f_dot(v3f a, v3f b);
v3f v3f_add(v3f a, v3f b);
v3f v3f_scale(v3f v, f32 s);
v3f v3f_div(v3f a, v3f b);
v3f v3f_sub(v3f a, v3f b);

typedef struct {
	f64 x;
	f64 y;
	f64 z;
	f64 w;
} v4d;

f64 v4d_magnitude(v4d v);
v4d make_v4d(f64 x, f64 y, f64 z, f64 w);
v4d v4d_mul(v4d a, v4d b);
v4d v4d_normalised(v4d v);
f64 v4d_dot(v4d a, v4d b);
v4d v4d_add(v4d a, v4d b);
v4d v4d_scale(v4d v, f64 s);
v4d v4d_div(v4d a, v4d b);
v4d v4d_sub(v4d a, v4d b);

typedef struct {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} v4f;

f32 v4f_magnitude(v4f v);
v4f make_v4f(f32 x, f32 y, f32 z, f32 w);
v4f v4f_mul(v4f a, v4f b);
v4f v4f_normalised(v4f v);
f32 v4f_dot(v4f a, v4f b);
v4f v4f_add(v4f a, v4f b);
v4f v4f_scale(v4f v, f32 s);
v4f v4f_div(v4f a, v4f b);
v4f v4f_sub(v4f a, v4f b);

typedef struct {
	i32 x;
	i32 y;
} v2i;

i32 v2i_magnitude(v2i v);
v2i make_v2i(i32 x, i32 y);
v2i v2i_mul(v2i a, v2i b);
v2i v2i_normalised(v2i v);
i32 v2i_dot(v2i a, v2i b);
v2i v2i_add(v2i a, v2i b);
v2i v2i_scale(v2i v, i32 s);
v2i v2i_div(v2i a, v2i b);
v2i v2i_sub(v2i a, v2i b);

typedef struct {
	f32 x;
	f32 y;
} v2f;

f32 v2f_magnitude(v2f v);
v2f make_v2f(f32 x, f32 y);
v2f v2f_mul(v2f a, v2f b);
v2f v2f_normalised(v2f v);
f32 v2f_dot(v2f a, v2f b);
v2f v2f_add(v2f a, v2f b);
v2f v2f_scale(v2f v, f32 s);
v2f v2f_div(v2f a, v2f b);
v2f v2f_sub(v2f a, v2f b);

typedef struct {
	f64 x;
	f64 y;
	f64 z;
} v3d;

f64 v3d_magnitude(v3d v);
v3d make_v3d(f64 x, f64 y, f64 z);
v3d v3d_mul(v3d a, v3d b);
v3d v3d_cross(v3d a, v3d b);
v3d v3d_normalised(v3d v);
f64 v3d_dot(v3d a, v3d b);
v3d v3d_add(v3d a, v3d b);
v3d v3d_scale(v3d v, f64 s);
v3d v3d_div(v3d a, v3d b);
v3d v3d_sub(v3d a, v3d b);

typedef struct {
	f64 x;
	f64 y;
} v2d;

f64 v2d_magnitude(v2d v);
v2d make_v2d(f64 x, f64 y);
v2d v2d_mul(v2d a, v2d b);
v2d v2d_normalised(v2d v);
f64 v2d_dot(v2d a, v2d b);
v2d v2d_add(v2d a, v2d b);
v2d v2d_scale(v2d v, f64 s);
v2d v2d_div(v2d a, v2d b);
v2d v2d_sub(v2d a, v2d b);

typedef struct {
	u32 x;
	u32 y;
	u32 z;
} v3u;

u32 v3u_magnitude(v3u v);
v3u make_v3u(u32 x, u32 y, u32 z);
v3u v3u_mul(v3u a, v3u b);
v3u v3u_cross(v3u a, v3u b);
v3u v3u_normalised(v3u v);
u32 v3u_dot(v3u a, v3u b);
v3u v3u_add(v3u a, v3u b);
v3u v3u_scale(v3u v, u32 s);
v3u v3u_div(v3u a, v3u b);
v3u v3u_sub(v3u a, v3u b);

typedef struct {
	u32 x;
	u32 y;
} v2u;

u32 v2u_magnitude(v2u v);
v2u make_v2u(u32 x, u32 y);
v2u v2u_mul(v2u a, v2u b);
v2u v2u_normalised(v2u v);
u32 v2u_dot(v2u a, v2u b);
v2u v2u_add(v2u a, v2u b);
v2u v2u_scale(v2u v, u32 s);
v2u v2u_div(v2u a, v2u b);
v2u v2u_sub(v2u a, v2u b);

typedef struct {
	i32 x;
	i32 y;
	i32 z;
} v3i;

i32 v3i_magnitude(v3i v);
v3i make_v3i(i32 x, i32 y, i32 z);
v3i v3i_mul(v3i a, v3i b);
v3i v3i_cross(v3i a, v3i b);
v3i v3i_normalised(v3i v);
i32 v3i_dot(v3i a, v3i b);
v3i v3i_add(v3i a, v3i b);
v3i v3i_scale(v3i v, i32 s);
v3i v3i_div(v3i a, v3i b);
v3i v3i_sub(v3i a, v3i b);

typedef struct {
	i32 x;
	i32 y;
	i32 z;
	i32 w;
} v4i;

i32 v4i_magnitude(v4i v);
v4i make_v4i(i32 x, i32 y, i32 z, i32 w);
v4i v4i_mul(v4i a, v4i b);
v4i v4i_normalised(v4i v);
i32 v4i_dot(v4i a, v4i b);
v4i v4i_add(v4i a, v4i b);
v4i v4i_scale(v4i v, i32 s);
v4i v4i_div(v4i a, v4i b);
v4i v4i_sub(v4i a, v4i b);

typedef struct {
	i32 m[2][2];
} m2i;

m2i m2i_identity();
m2i make_m2i(i32 d);
m2i m2i_mul(m2i a, m2i b);

typedef struct {
	u32 m[2][2];
} m2u;

m2u m2u_identity();
m2u make_m2u(u32 d);
m2u m2u_mul(m2u a, m2u b);

typedef struct {
	i32 m[3][3];
} m3i;

m3i m3i_scale(m3i m, v3i v);
m3i m3i_identity();
m3i make_m3i(i32 d);
m3i m3i_rotate(m3i m, i32 a, v3i v);
m3i m3i_mul(m3i a, m3i b);

typedef struct {
	f64 m[4][4];
} m4d;

m4d m4d_translate(m4d m, v3d v);
m4d m4d_scale(m4d m, v3d v);
m4d m4d_identity();
m4d make_m4d(f64 d);
m4d m4d_rotate(m4d m, f64 a, v3d v);
v4d m4d_transform(m4d m, v4d v);
m4d m4d_mul(m4d a, m4d b);
m4d m4d_lookat(v3d c, v3d o, v3d u);
m4d m4d_pers(f64 fov, f64 asp, f64 n, f64 f);
m4d m4d_orth(f64 l, f64 r, f64 b, f64 t, f64 n, f64 f);

typedef struct {
	i32 m[4][4];
} m4i;

m4i m4i_translate(m4i m, v3i v);
m4i m4i_scale(m4i m, v3i v);
m4i m4i_identity();
m4i make_m4i(i32 d);
m4i m4i_rotate(m4i m, i32 a, v3i v);
v4i m4i_transform(m4i m, v4i v);
m4i m4i_mul(m4i a, m4i b);
m4i m4i_lookat(v3i c, v3i o, v3i u);
m4i m4i_pers(i32 fov, i32 asp, i32 n, i32 f);
m4i m4i_orth(i32 l, i32 r, i32 b, i32 t, i32 n, i32 f);

typedef struct {
	f32 m[4][4];
} m4f;

m4f m4f_translate(m4f m, v3f v);
m4f m4f_scale(m4f m, v3f v);
m4f m4f_identity();
m4f make_m4f(f32 d);
m4f m4f_rotate(m4f m, f32 a, v3f v);
v4f m4f_transform(m4f m, v4f v);
m4f m4f_mul(m4f a, m4f b);
m4f m4f_lookat(v3f c, v3f o, v3f u);
m4f m4f_pers(f32 fov, f32 asp, f32 n, f32 f);
m4f m4f_orth(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);
m4f m4f_inverse(m4f m);

typedef struct {
	u32 m[3][3];
} m3u;

m3u m3u_scale(m3u m, v3u v);
m3u m3u_identity();
m3u make_m3u(u32 d);
m3u m3u_rotate(m3u m, u32 a, v3u v);
m3u m3u_mul(m3u a, m3u b);

typedef struct {
	f64 m[2][2];
} m2d;

m2d m2d_identity();
m2d make_m2d(f64 d);
m2d m2d_mul(m2d a, m2d b);

typedef struct {
	f64 m[3][3];
} m3d;

m3d m3d_scale(m3d m, v3d v);
m3d m3d_identity();
m3d make_m3d(f64 d);
m3d m3d_rotate(m3d m, f64 a, v3d v);
m3d m3d_mul(m3d a, m3d b);

typedef struct {
	f32 m[2][2];
} m2f;

m2f m2f_identity();
m2f make_m2f(f32 d);
m2f m2f_mul(m2f a, m2f b);

typedef struct {
	f32 m[3][3];
} m3f;

m3f m3f_scale(m3f m, v3f v);
m3f m3f_identity();
m3f make_m3f(f32 d);
m3f m3f_rotate(m3f m, f32 a, v3f v);
m3f m3f_mul(m3f a, m3f b);

/* Implementation */
#ifdef SML_IMPL

#endif /* SML_IMPL */

