#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "maths.h"

double todeg(double rad) {
	return rad * (180.0 / pie);
}

double torad(double deg) {
	return deg * (pie / 180.0);
}

v4f v4f_mul(v4f a, v4f b) {
	return (v4f) { a.x* b.x, a.y* b.y, a.z* b.z, a.w* b.w };
}

v4f v4f_sub(v4f a, v4f b) {
	return (v4f) { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

float v4f_dot(v4f a, v4f b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));
}

v4f v4f_scale(v4f v, float s) {
	return (v4f) { v.x* s, v.y* s, v.z* s, v.w* s };
}

v4f make_v4f(float x, float y, float z, float w) {
	return (v4f) { x, y, z, w };
}

float v4f_magnitude(v4f v) {
	return (float)sqrt(((float)v.x * (float)v.x) + ((float)v.y * (float)v.y) + ((float)v.z * (float)v.z) + ((float)v.w * (float)v.w));
}

v4f v4f_div(v4f a, v4f b) {
	return (v4f) { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

v4f v4f_normalised(v4f v) {
	const float l = v4f_magnitude(v);
	return l == (float)0 ? (v4f) { (float)0, (float)0, (float)0, (float)0 } : (v4f) { (float)v.x / l, (float)v.y / l, (float)v.z / l, (float)v.w / l };
}

v4f v4f_add(v4f a, v4f b) {
	return (v4f) { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

v3f v3f_mul(v3f a, v3f b) {
	return (v3f) { a.x* b.x, a.y* b.y, a.z* b.z };
}

v3f v3f_sub(v3f a, v3f b) {
	return (v3f) { a.x - b.x, a.y - b.y, a.z - b.z };
}

float v3f_dot(v3f a, v3f b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

v3f v3f_scale(v3f v, float s) {
	return (v3f) { v.x* s, v.y* s, v.z* s };
}

v3f make_v3f(float x, float y, float z) {
	return (v3f) { x, y, z };
}

float v3f_magnitude(v3f v) {
	return (float)sqrt(((float)v.x * (float)v.x) + ((float)v.y * (float)v.y) + ((float)v.z * (float)v.z));
}

v3f v3f_cross(v3f a, v3f b) {
	return make_v3f(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

v3f v3f_div(v3f a, v3f b) {
	return (v3f) { a.x / b.x, a.y / b.y, a.z / b.z };
}

v3f v3f_normalised(v3f v) {
	const float l = v3f_magnitude(v);
	return l == (float)0 ? (v3f) { (float)0, (float)0, (float)0 } : (v3f) { (float)v.x / l, (float)v.y / l, (float)v.z / l };
}

v3f v3f_add(v3f a, v3f b) {
	return (v3f) { a.x + b.x, a.y + b.y, a.z + b.z };
}

v4d v4d_mul(v4d a, v4d b) {
	return (v4d) { a.x* b.x, a.y* b.y, a.z* b.z, a.w* b.w };
}

v4d v4d_sub(v4d a, v4d b) {
	return (v4d) { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

double v4d_dot(v4d a, v4d b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));
}

v4d v4d_scale(v4d v, double s) {
	return (v4d) { v.x* s, v.y* s, v.z* s, v.w* s };
}

v4d make_v4d(double x, double y, double z, double w) {
	return (v4d) { x, y, z, w };
}

double v4d_magnitude(v4d v) {
	return (double)sqrt(((double)v.x * (double)v.x) + ((double)v.y * (double)v.y) + ((double)v.z * (double)v.z) + ((double)v.w * (double)v.w));
}

v4d v4d_div(v4d a, v4d b) {
	return (v4d) { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

v4d v4d_normalised(v4d v) {
	const double l = v4d_magnitude(v);
	return l == (double)0 ? (v4d) { (double)0, (double)0, (double)0, (double)0 } : (v4d) { (double)v.x / l, (double)v.y / l, (double)v.z / l, (double)v.w / l };
}

v4d v4d_add(v4d a, v4d b) {
	return (v4d) { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

v3d v3d_mul(v3d a, v3d b) {
	return (v3d) { a.x* b.x, a.y* b.y, a.z* b.z };
}

v3d v3d_sub(v3d a, v3d b) {
	return (v3d) { a.x - b.x, a.y - b.y, a.z - b.z };
}

double v3d_dot(v3d a, v3d b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

v3d v3d_scale(v3d v, double s) {
	return (v3d) { v.x* s, v.y* s, v.z* s };
}

v3d make_v3d(double x, double y, double z) {
	return (v3d) { x, y, z };
}

double v3d_magnitude(v3d v) {
	return (double)sqrt(((double)v.x * (double)v.x) + ((double)v.y * (double)v.y) + ((double)v.z * (double)v.z));
}

v3d v3d_cross(v3d a, v3d b) {
	return make_v3d(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

v3d v3d_div(v3d a, v3d b) {
	return (v3d) { a.x / b.x, a.y / b.y, a.z / b.z };
}

v3d v3d_normalised(v3d v) {
	const double l = v3d_magnitude(v);
	return l == (double)0 ? (v3d) { (double)0, (double)0, (double)0 } : (v3d) { (double)v.x / l, (double)v.y / l, (double)v.z / l };
}

v3d v3d_add(v3d a, v3d b) {
	return (v3d) { a.x + b.x, a.y + b.y, a.z + b.z };
}

v2f v2f_mul(v2f a, v2f b) {
	return (v2f) { a.x* b.x, a.y* b.y };
}

v2f v2f_sub(v2f a, v2f b) {
	return (v2f) { a.x - b.x, a.y - b.y };
}

float v2f_dot(v2f a, v2f b) {
	return ((a.x * b.x) + (a.y * b.y));
}

v2f v2f_scale(v2f v, float s) {
	return (v2f) { v.x* s, v.y* s };
}

v2f make_v2f(float x, float y) {
	return (v2f) { x, y };
}

float v2f_magnitude(v2f v) {
	return (float)sqrt(((float)v.x * (float)v.x) + ((float)v.y * (float)v.y));
}

v2f v2f_div(v2f a, v2f b) {
	return (v2f) { a.x / b.x, a.y / b.y };
}

v2f v2f_normalised(v2f v) {
	const float l = v2f_magnitude(v);
	return l == (float)0 ? (v2f) { (float)0, (float)0 } : (v2f) { (float)v.x / l, (float)v.y / l };
}

v2f v2f_add(v2f a, v2f b) {
	return (v2f) { a.x + b.x, a.y + b.y };
}

v2d v2d_mul(v2d a, v2d b) {
	return (v2d) { a.x* b.x, a.y* b.y };
}

v2d v2d_sub(v2d a, v2d b) {
	return (v2d) { a.x - b.x, a.y - b.y };
}

double v2d_dot(v2d a, v2d b) {
	return ((a.x * b.x) + (a.y * b.y));
}

v2d v2d_scale(v2d v, double s) {
	return (v2d) { v.x* s, v.y* s };
}

v2d make_v2d(double x, double y) {
	return (v2d) { x, y };
}

double v2d_magnitude(v2d v) {
	return (double)sqrt(((double)v.x * (double)v.x) + ((double)v.y * (double)v.y));
}

v2d v2d_div(v2d a, v2d b) {
	return (v2d) { a.x / b.x, a.y / b.y };
}

v2d v2d_normalised(v2d v) {
	const double l = v2d_magnitude(v);
	return l == (double)0 ? (v2d) { (double)0, (double)0 } : (v2d) { (double)v.x / l, (double)v.y / l };
}

v2d v2d_add(v2d a, v2d b) {
	return (v2d) { a.x + b.x, a.y + b.y };
}

v3i v3i_mul(v3i a, v3i b) {
	return (v3i) { a.x* b.x, a.y* b.y, a.z* b.z };
}

v3i v3i_sub(v3i a, v3i b) {
	return (v3i) { a.x - b.x, a.y - b.y, a.z - b.z };
}

i32 v3i_dot(v3i a, v3i b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

v3i v3i_scale(v3i v, i32 s) {
	return (v3i) { v.x* s, v.y* s, v.z* s };
}

v3i make_v3i(i32 x, i32 y, i32 z) {
	return (v3i) { x, y, z };
}

i32 v3i_magnitude(v3i v) {
	return (i32)sqrt(((i32)v.x * (i32)v.x) + ((i32)v.y * (i32)v.y) + ((i32)v.z * (i32)v.z));
}

v3i v3i_cross(v3i a, v3i b) {
	return make_v3i(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

v3i v3i_div(v3i a, v3i b) {
	return (v3i) { a.x / b.x, a.y / b.y, a.z / b.z };
}

v3i v3i_normalised(v3i v) {
	const i32 l = v3i_magnitude(v);
	return l == (i32)0 ? (v3i) { (i32)0, (i32)0, (i32)0 } : (v3i) { (i32)v.x / l, (i32)v.y / l, (i32)v.z / l };
}

v3i v3i_add(v3i a, v3i b) {
	return (v3i) { a.x + b.x, a.y + b.y, a.z + b.z };
}

v2i v2i_mul(v2i a, v2i b) {
	return (v2i) { a.x* b.x, a.y* b.y };
}

v2i v2i_sub(v2i a, v2i b) {
	return (v2i) { a.x - b.x, a.y - b.y };
}

i32 v2i_dot(v2i a, v2i b) {
	return ((a.x * b.x) + (a.y * b.y));
}

v2i v2i_scale(v2i v, i32 s) {
	return (v2i) { v.x* s, v.y* s };
}

v2i make_v2i(i32 x, i32 y) {
	return (v2i) { x, y };
}

i32 v2i_magnitude(v2i v) {
	return (i32)sqrt(((i32)v.x * (i32)v.x) + ((i32)v.y * (i32)v.y));
}

v2i v2i_div(v2i a, v2i b) {
	return (v2i) { a.x / b.x, a.y / b.y };
}

v2i v2i_normalised(v2i v) {
	const i32 l = v2i_magnitude(v);
	return l == (i32)0 ? (v2i) { (i32)0, (i32)0 } : (v2i) { (i32)v.x / l, (i32)v.y / l };
}

v2i v2i_add(v2i a, v2i b) {
	return (v2i) { a.x + b.x, a.y + b.y };
}

v4i v4i_mul(v4i a, v4i b) {
	return (v4i) { a.x* b.x, a.y* b.y, a.z* b.z, a.w* b.w };
}

v4i v4i_sub(v4i a, v4i b) {
	return (v4i) { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

i32 v4i_dot(v4i a, v4i b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));
}

v4i v4i_scale(v4i v, i32 s) {
	return (v4i) { v.x* s, v.y* s, v.z* s, v.w* s };
}

v4i make_v4i(i32 x, i32 y, i32 z, i32 w) {
	return (v4i) { x, y, z, w };
}

i32 v4i_magnitude(v4i v) {
	return (i32)sqrt(((i32)v.x * (i32)v.x) + ((i32)v.y * (i32)v.y) + ((i32)v.z * (i32)v.z) + ((i32)v.w * (i32)v.w));
}

v4i v4i_div(v4i a, v4i b) {
	return (v4i) { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

v4i v4i_normalised(v4i v) {
	const i32 l = v4i_magnitude(v);
	return l == (i32)0 ? (v4i) { (i32)0, (i32)0, (i32)0, (i32)0 } : (v4i) { (i32)v.x / l, (i32)v.y / l, (i32)v.z / l, (i32)v.w / l };
}

v4i v4i_add(v4i a, v4i b) {
	return (v4i) { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

v4u v4u_mul(v4u a, v4u b) {
	return (v4u) { a.x* b.x, a.y* b.y, a.z* b.z, a.w* b.w };
}

v4u v4u_sub(v4u a, v4u b) {
	return (v4u) { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

u32 v4u_dot(v4u a, v4u b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));
}

v4u v4u_scale(v4u v, u32 s) {
	return (v4u) { v.x* s, v.y* s, v.z* s, v.w* s };
}

v4u make_v4u(u32 x, u32 y, u32 z, u32 w) {
	return (v4u) { x, y, z, w };
}

u32 v4u_magnitude(v4u v) {
	return (u32)sqrt(((u32)v.x * (u32)v.x) + ((u32)v.y * (u32)v.y) + ((u32)v.z * (u32)v.z) + ((u32)v.w * (u32)v.w));
}

v4u v4u_div(v4u a, v4u b) {
	return (v4u) { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

v4u v4u_normalised(v4u v) {
	const u32 l = v4u_magnitude(v);
	return l == (u32)0 ? (v4u) { (u32)0, (u32)0, (u32)0, (u32)0 } : (v4u) { (u32)v.x / l, (u32)v.y / l, (u32)v.z / l, (u32)v.w / l };
}

v4u v4u_add(v4u a, v4u b) {
	return (v4u) { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

v3u v3u_mul(v3u a, v3u b) {
	return (v3u) { a.x* b.x, a.y* b.y, a.z* b.z };
}

v3u v3u_sub(v3u a, v3u b) {
	return (v3u) { a.x - b.x, a.y - b.y, a.z - b.z };
}

u32 v3u_dot(v3u a, v3u b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

v3u v3u_scale(v3u v, u32 s) {
	return (v3u) { v.x* s, v.y* s, v.z* s };
}

v3u make_v3u(u32 x, u32 y, u32 z) {
	return (v3u) { x, y, z };
}

u32 v3u_magnitude(v3u v) {
	return (u32)sqrt(((u32)v.x * (u32)v.x) + ((u32)v.y * (u32)v.y) + ((u32)v.z * (u32)v.z));
}

v3u v3u_cross(v3u a, v3u b) {
	return make_v3u(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

v3u v3u_div(v3u a, v3u b) {
	return (v3u) { a.x / b.x, a.y / b.y, a.z / b.z };
}

v3u v3u_normalised(v3u v) {
	const u32 l = v3u_magnitude(v);
	return l == (u32)0 ? (v3u) { (u32)0, (u32)0, (u32)0 } : (v3u) { (u32)v.x / l, (u32)v.y / l, (u32)v.z / l };
}

v3u v3u_add(v3u a, v3u b) {
	return (v3u) { a.x + b.x, a.y + b.y, a.z + b.z };
}

v2u v2u_mul(v2u a, v2u b) {
	return (v2u) { a.x* b.x, a.y* b.y };
}

v2u v2u_sub(v2u a, v2u b) {
	return (v2u) { a.x - b.x, a.y - b.y };
}

u32 v2u_dot(v2u a, v2u b) {
	return ((a.x * b.x) + (a.y * b.y));
}

v2u v2u_scale(v2u v, u32 s) {
	return (v2u) { v.x* s, v.y* s };
}

v2u make_v2u(u32 x, u32 y) {
	return (v2u) { x, y };
}

u32 v2u_magnitude(v2u v) {
	return (u32)sqrt(((u32)v.x * (u32)v.x) + ((u32)v.y * (u32)v.y));
}

v2u v2u_div(v2u a, v2u b) {
	return (v2u) { a.x / b.x, a.y / b.y };
}

v2u v2u_normalised(v2u v) {
	const u32 l = v2u_magnitude(v);
	return l == (u32)0 ? (v2u) { (u32)0, (u32)0 } : (v2u) { (u32)v.x / l, (u32)v.y / l };
}

v2u v2u_add(v2u a, v2u b) {
	return (v2u) { a.x + b.x, a.y + b.y };
}

m3f m3f_rotate(m3f m, float a, v3f v) {
	m3f r = make_m3f((float)1);

	const float c = (float)cos((double)a);
	const float s = (float)sin((double)a);

	const float omc = (float)1 - c;

	const float x = v.x;
	const float y = v.y;
	const float z = v.z;

	r.m[0][0] = x * x * omc + c;
	r.m[0][1] = y * x * omc + z * s;
	r.m[0][2] = x * z * omc - y * s;
	r.m[1][0] = x * y * omc - z * s;
	r.m[1][1] = y * y * omc + c;
	r.m[1][2] = y * z * omc + x * s;
	r.m[2][0] = x * z * omc + y * s;
	r.m[2][1] = y * z * omc - x * s;
	r.m[2][2] = z * z * omc + c;

	return m3f_mul(m, r);
}

m3f m3f_identity() {
	return make_m3f((float)1);
}

m3f m3f_mul(m3f a, m3f b) {
	m3f r = make_m3f((float)1);

	r.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1] + a.m[2][0] * b.m[0][2];
	r.m[1][0] = a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1] + a.m[2][0] * b.m[1][2];
	r.m[2][0] = a.m[0][0] * b.m[2][0] + a.m[1][0] * b.m[2][1] + a.m[2][0] * b.m[2][2];
	r.m[0][1] = a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1] + a.m[2][1] * b.m[0][2];
	r.m[1][1] = a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1] + a.m[2][1] * b.m[1][2];
	r.m[2][1] = a.m[0][1] * b.m[2][0] + a.m[1][1] * b.m[2][1] + a.m[2][1] * b.m[2][2];
	r.m[0][2] = a.m[0][2] * b.m[0][0] + a.m[1][2] * b.m[0][1] + a.m[2][2] * b.m[0][2];
	r.m[1][2] = a.m[0][2] * b.m[1][0] + a.m[1][2] * b.m[1][1] + a.m[2][2] * b.m[1][2];
	r.m[2][2] = a.m[0][2] * b.m[2][0] + a.m[1][2] * b.m[2][1] + a.m[2][2] * b.m[2][2];

	return r;
}

m3f make_m3f(float d) {
	m3f r;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			r.m[x][y] = (float)0;
		}
	}

	r.m[0][0] = d;
	r.m[1][1] = d;
	r.m[2][2] = d;

	return r;
}

m3f m3f_scale(m3f m, v3f v) {
	m3f r = make_m3f((float)1);

	r.m[0][0] = v.x;
	r.m[1][1] = v.y;
	r.m[2][2] = v.z;

	return m3f_mul(m, r);
}

m2d m2d_identity() {
	return make_m2d((double)1);
}

m2d m2d_mul(m2d a, m2d b) {
	m2d r = make_m2d((double)1);

	r.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1];
	r.m[1][0] = a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1];
	r.m[0][1] = a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1];
	r.m[1][1] = a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1];

	return r;
}

m2d make_m2d(double d) {
	m2d r;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			r.m[x][y] = (double)0;
		}
	}

	r.m[0][0] = d;
	r.m[1][1] = d;

	return r;
}

m4f m4f_rotate(m4f m, float a, v3f v) {
	m4f r = make_m4f((float)1);

	const float c = (float)cos((double)a);
	const float s = (float)sin((double)a);

	const float omc = (float)1 - c;

	const float x = v.x;
	const float y = v.y;
	const float z = v.z;

	r.m[0][0] = x * x * omc + c;
	r.m[0][1] = y * x * omc + z * s;
	r.m[0][2] = x * z * omc - y * s;
	r.m[1][0] = x * y * omc - z * s;
	r.m[1][1] = y * y * omc + c;
	r.m[1][2] = y * z * omc + x * s;
	r.m[2][0] = x * z * omc + y * s;
	r.m[2][1] = y * z * omc - x * s;
	r.m[2][2] = z * z * omc + c;

	return m4f_mul(m, r);
}

m4f m4f_orth(float l, float r, float b, float t, float n, float f) {
	m4f res = make_m4f((float)1);

	res.m[0][0] = (float)2 / (r - l);
	res.m[1][1] = (float)2 / (t - b);
	res.m[2][2] = (float)2 / (n - f);

	res.m[3][0] = (l + r) / (l - r);
	res.m[3][1] = (b + t) / (b - t);
	res.m[3][2] = (f + n) / (f - n);

	return res;
}

m4f m4f_inverse(m4f matrix) {
	const f32* m = (f32*)matrix.m;

	f32 t0 = m[10] * m[15];
	f32 t1 = m[14] * m[11];
	f32 t2 = m[6] * m[15];
	f32 t3 = m[14] * m[7];
	f32 t4 = m[6] * m[11];
	f32 t5 = m[10] * m[7];
	f32 t6 = m[2] * m[15];
	f32 t7 = m[14] * m[3];
	f32 t8 = m[2] * m[11];
	f32 t9 = m[10] * m[3];
	f32 t10 = m[2] * m[7];
	f32 t11 = m[6] * m[3];
	f32 t12 = m[8] * m[13];
	f32 t13 = m[12] * m[9];
	f32 t14 = m[4] * m[13];
	f32 t15 = m[12] * m[5];
	f32 t16 = m[4] * m[9];
	f32 t17 = m[8] * m[5];
	f32 t18 = m[0] * m[13];
	f32 t19 = m[12] * m[1];
	f32 t20 = m[0] * m[9];
	f32 t21 = m[8] * m[1];
	f32 t22 = m[0] * m[5];
	f32 t23 = m[4] * m[1];

	m4f out_matrix;
	f32* o = (f32*)out_matrix.m;

	o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
	o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
	o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
	o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

	f32 d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

	o[0] = d * o[0];
	o[1] = d * o[1];
	o[2] = d * o[2];
	o[3] = d * o[3];
	o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
	o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
	o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
	o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
	o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
	o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
	o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
	o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
	o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
	o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
	o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
	o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));

	return out_matrix;
}

m4f m4f_identity() {
	return make_m4f((float)1);
}

m4f m4f_mul(m4f a, m4f b) {
	m4f r = make_m4f((float)1);

	r.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1] + a.m[2][0] * b.m[0][2] + a.m[3][0] * b.m[0][3];
	r.m[1][0] = a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1] + a.m[2][0] * b.m[1][2] + a.m[3][0] * b.m[1][3];
	r.m[2][0] = a.m[0][0] * b.m[2][0] + a.m[1][0] * b.m[2][1] + a.m[2][0] * b.m[2][2] + a.m[3][0] * b.m[2][3];
	r.m[3][0] = a.m[0][0] * b.m[3][0] + a.m[1][0] * b.m[3][1] + a.m[2][0] * b.m[3][2] + a.m[3][0] * b.m[3][3];
	r.m[0][1] = a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1] + a.m[2][1] * b.m[0][2] + a.m[3][1] * b.m[0][3];
	r.m[1][1] = a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1] + a.m[2][1] * b.m[1][2] + a.m[3][1] * b.m[1][3];
	r.m[2][1] = a.m[0][1] * b.m[2][0] + a.m[1][1] * b.m[2][1] + a.m[2][1] * b.m[2][2] + a.m[3][1] * b.m[2][3];
	r.m[3][1] = a.m[0][1] * b.m[3][0] + a.m[1][1] * b.m[3][1] + a.m[2][1] * b.m[3][2] + a.m[3][1] * b.m[3][3];
	r.m[0][2] = a.m[0][2] * b.m[0][0] + a.m[1][2] * b.m[0][1] + a.m[2][2] * b.m[0][2] + a.m[3][2] * b.m[0][3];
	r.m[1][2] = a.m[0][2] * b.m[1][0] + a.m[1][2] * b.m[1][1] + a.m[2][2] * b.m[1][2] + a.m[3][2] * b.m[1][3];
	r.m[2][2] = a.m[0][2] * b.m[2][0] + a.m[1][2] * b.m[2][1] + a.m[2][2] * b.m[2][2] + a.m[3][2] * b.m[2][3];
	r.m[3][2] = a.m[0][2] * b.m[3][0] + a.m[1][2] * b.m[3][1] + a.m[2][2] * b.m[3][2] + a.m[3][2] * b.m[3][3];
	r.m[0][3] = a.m[0][3] * b.m[0][0] + a.m[1][3] * b.m[0][1] + a.m[2][3] * b.m[0][2] + a.m[3][3] * b.m[0][3];
	r.m[1][3] = a.m[0][3] * b.m[1][0] + a.m[1][3] * b.m[1][1] + a.m[2][3] * b.m[1][2] + a.m[3][3] * b.m[1][3];
	r.m[2][3] = a.m[0][3] * b.m[2][0] + a.m[1][3] * b.m[2][1] + a.m[2][3] * b.m[2][2] + a.m[3][3] * b.m[2][3];
	r.m[3][3] = a.m[0][3] * b.m[3][0] + a.m[1][3] * b.m[3][1] + a.m[2][3] * b.m[3][2] + a.m[3][3] * b.m[3][3];

	return r;
}

m4f m4f_translate(m4f m, v3f v) {
	m4f r = make_m4f((float)1);

	r.m[3][0] = v.x;
	r.m[3][1] = v.y;
	r.m[3][2] = v.z;

	return m4f_mul(m, r);
}

m4f m4f_lookat(v3f c, v3f o, v3f u) {
	m4f r = make_m4f((float)1);

	const v3f f = v3f_normalised(v3f_sub(o, c));
	u = v3f_normalised(u);
	const v3f s = v3f_normalised(v3f_cross(f, u));
	u = v3f_cross(s, f);

	r.m[0][0] = s.x;
	r.m[1][0] = s.y;
	r.m[2][0] = s.z;
	r.m[0][1] = u.x;
	r.m[1][1] = u.y;
	r.m[2][1] = u.z;
	r.m[0][2] = -f.x;
	r.m[1][2] = -f.y;
	r.m[2][2] = -f.z;
	r.m[3][0] = -v3f_dot(s, c);
	r.m[3][1] = -v3f_dot(u, c);
	r.m[3][2] = v3f_dot(f, c);

	return r;
}

m4f make_m4f(float d) {
	m4f r;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			r.m[x][y] = (float)0;
		}
	}

	r.m[0][0] = d;
	r.m[1][1] = d;
	r.m[2][2] = d;
	r.m[3][3] = d;

	return r;
}

m4f m4f_scale(m4f m, v3f v) {
	m4f r = make_m4f((float)1);

	r.m[0][0] = v.x;
	r.m[1][1] = v.y;
	r.m[2][2] = v.z;

	return m4f_mul(m, r);
}

m4f m4f_pers(float fov, float asp, float n, float f) {
	m4f r = make_m4f((float)1);

	const float q = (float)1 / (float)tan(torad((double)fov) / 2.0);
	const float a = q / asp;
	const float b = (n + f) / (n - f);
	const float c = ((float)2 * n * f) / (n - f);

	r.m[0][0] = a;
	r.m[1][1] = q;
	r.m[2][2] = b;
	r.m[2][3] = (float)-1;
	r.m[3][2] = c;

	return r;
}

v4f m4f_transform(m4f m, v4f v) {
	return make_v4f(
		m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] + v.w,
		m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] + v.w,
		m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] + v.w,
		m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] + v.w);
}

m2f m2f_identity() {
	return make_m2f((float)1);
}

m2f m2f_mul(m2f a, m2f b) {
	m2f r = make_m2f((float)1);

	r.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1];
	r.m[1][0] = a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1];
	r.m[0][1] = a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1];
	r.m[1][1] = a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1];

	return r;
}

m2f make_m2f(float d) {
	m2f r;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			r.m[x][y] = (float)0;
		}
	}

	r.m[0][0] = d;
	r.m[1][1] = d;

	return r;
}

m4d m4d_rotate(m4d m, double a, v3d v) {
	m4d r = make_m4d((double)1);

	const double c = (double)cos((double)a);
	const double s = (double)sin((double)a);

	const double omc = (double)1 - c;

	const double x = v.x;
	const double y = v.y;
	const double z = v.z;

	r.m[0][0] = x * x * omc + c;
	r.m[0][1] = y * x * omc + z * s;
	r.m[0][2] = x * z * omc - y * s;
	r.m[1][0] = x * y * omc - z * s;
	r.m[1][1] = y * y * omc + c;
	r.m[1][2] = y * z * omc + x * s;
	r.m[2][0] = x * z * omc + y * s;
	r.m[2][1] = y * z * omc - x * s;
	r.m[2][2] = z * z * omc + c;

	return m4d_mul(m, r);
}

m4d m4d_orth(double l, double r, double b, double t, double n, double f) {
	m4d res = make_m4d((double)1);

	res.m[0][0] = (double)2 / (r - l);
	res.m[1][1] = (double)2 / (t - b);
	res.m[2][2] = (double)2 / (n - f);

	res.m[3][0] = (l + r) / (l - r);
	res.m[3][1] = (b + t) / (b - t);
	res.m[3][2] = (f + n) / (f - n);

	return res;
}

m4d m4d_identity() {
	return make_m4d((double)1);
}

m4d m4d_mul(m4d a, m4d b) {
	m4d r = make_m4d((double)1);

	r.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1] + a.m[2][0] * b.m[0][2] + a.m[3][0] * b.m[0][3];
	r.m[1][0] = a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1] + a.m[2][0] * b.m[1][2] + a.m[3][0] * b.m[1][3];
	r.m[2][0] = a.m[0][0] * b.m[2][0] + a.m[1][0] * b.m[2][1] + a.m[2][0] * b.m[2][2] + a.m[3][0] * b.m[2][3];
	r.m[3][0] = a.m[0][0] * b.m[3][0] + a.m[1][0] * b.m[3][1] + a.m[2][0] * b.m[3][2] + a.m[3][0] * b.m[3][3];
	r.m[0][1] = a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1] + a.m[2][1] * b.m[0][2] + a.m[3][1] * b.m[0][3];
	r.m[1][1] = a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1] + a.m[2][1] * b.m[1][2] + a.m[3][1] * b.m[1][3];
	r.m[2][1] = a.m[0][1] * b.m[2][0] + a.m[1][1] * b.m[2][1] + a.m[2][1] * b.m[2][2] + a.m[3][1] * b.m[2][3];
	r.m[3][1] = a.m[0][1] * b.m[3][0] + a.m[1][1] * b.m[3][1] + a.m[2][1] * b.m[3][2] + a.m[3][1] * b.m[3][3];
	r.m[0][2] = a.m[0][2] * b.m[0][0] + a.m[1][2] * b.m[0][1] + a.m[2][2] * b.m[0][2] + a.m[3][2] * b.m[0][3];
	r.m[1][2] = a.m[0][2] * b.m[1][0] + a.m[1][2] * b.m[1][1] + a.m[2][2] * b.m[1][2] + a.m[3][2] * b.m[1][3];
	r.m[2][2] = a.m[0][2] * b.m[2][0] + a.m[1][2] * b.m[2][1] + a.m[2][2] * b.m[2][2] + a.m[3][2] * b.m[2][3];
	r.m[3][2] = a.m[0][2] * b.m[3][0] + a.m[1][2] * b.m[3][1] + a.m[2][2] * b.m[3][2] + a.m[3][2] * b.m[3][3];
	r.m[0][3] = a.m[0][3] * b.m[0][0] + a.m[1][3] * b.m[0][1] + a.m[2][3] * b.m[0][2] + a.m[3][3] * b.m[0][3];
	r.m[1][3] = a.m[0][3] * b.m[1][0] + a.m[1][3] * b.m[1][1] + a.m[2][3] * b.m[1][2] + a.m[3][3] * b.m[1][3];
	r.m[2][3] = a.m[0][3] * b.m[2][0] + a.m[1][3] * b.m[2][1] + a.m[2][3] * b.m[2][2] + a.m[3][3] * b.m[2][3];
	r.m[3][3] = a.m[0][3] * b.m[3][0] + a.m[1][3] * b.m[3][1] + a.m[2][3] * b.m[3][2] + a.m[3][3] * b.m[3][3];

	return r;
}

m4d m4d_translate(m4d m, v3d v) {
	m4d r = make_m4d((double)1);

	r.m[3][0] = v.x;
	r.m[3][1] = v.y;
	r.m[3][2] = v.z;

	return m4d_mul(m, r);
}

m4d m4d_lookat(v3d c, v3d o, v3d u) {
	m4d r = make_m4d((double)1);

	const v3d f = v3d_normalised(v3d_sub(o, c));
	u = v3d_normalised(u);
	const v3d s = v3d_normalised(v3d_cross(f, u));
	u = v3d_cross(s, f);

	r.m[0][0] = s.x;
	r.m[1][0] = s.y;
	r.m[2][0] = s.z;
	r.m[0][1] = u.x;
	r.m[1][1] = u.y;
	r.m[2][1] = u.z;
	r.m[0][2] = -f.x;
	r.m[1][2] = -f.y;
	r.m[2][2] = -f.z;
	r.m[3][0] = -v3d_dot(s, c);
	r.m[3][1] = -v3d_dot(u, c);
	r.m[3][2] = v3d_dot(f, c);

	return r;
}

m4d make_m4d(double d) {
	m4d r;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			r.m[x][y] = (double)0;
		}
	}

	r.m[0][0] = d;
	r.m[1][1] = d;
	r.m[2][2] = d;
	r.m[3][3] = d;

	return r;
}

m4d m4d_scale(m4d m, v3d v) {
	m4d r = make_m4d((double)1);

	r.m[0][0] = v.x;
	r.m[1][1] = v.y;
	r.m[2][2] = v.z;

	return m4d_mul(m, r);
}

m4d m4d_pers(double fov, double asp, double n, double f) {
	m4d r = make_m4d((double)1);

	const double q = (double)1 / (double)tan(torad((double)fov) / 2.0);
	const double a = q / asp;
	const double b = (n + f) / (n - f);
	const double c = ((double)2 * n * f) / (n - f);

	r.m[0][0] = a;
	r.m[1][1] = q;
	r.m[2][2] = b;
	r.m[2][3] = (double)-1;
	r.m[3][2] = c;

	return r;
}

v4d m4d_transform(m4d m, v4d v) {
	return make_v4d(
		m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] + v.w,
		m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] + v.w,
		m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] + v.w,
		m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] + v.w);
}

m4i m4i_rotate(m4i m, i32 a, v3i v) {
	m4i r = make_m4i((i32)1);

	const i32 c = (i32)cos((double)a);
	const i32 s = (i32)sin((double)a);

	const i32 omc = (i32)1 - c;

	const i32 x = v.x;
	const i32 y = v.y;
	const i32 z = v.z;

	r.m[0][0] = x * x * omc + c;
	r.m[0][1] = y * x * omc + z * s;
	r.m[0][2] = x * z * omc - y * s;
	r.m[1][0] = x * y * omc - z * s;
	r.m[1][1] = y * y * omc + c;
	r.m[1][2] = y * z * omc + x * s;
	r.m[2][0] = x * z * omc + y * s;
	r.m[2][1] = y * z * omc - x * s;
	r.m[2][2] = z * z * omc + c;

	return m4i_mul(m, r);
}

m4i m4i_orth(i32 l, i32 r, i32 b, i32 t, i32 n, i32 f) {
	m4i res = make_m4i((i32)1);

	res.m[0][0] = (i32)2 / (r - l);
	res.m[1][1] = (i32)2 / (t - b);
	res.m[2][2] = (i32)2 / (n - f);

	res.m[3][0] = (l + r) / (l - r);
	res.m[3][1] = (b + t) / (b - t);
	res.m[3][2] = (f + n) / (f - n);

	return res;
}

m4i m4i_identity() {
	return make_m4i((i32)1);
}

m4i m4i_mul(m4i a, m4i b) {
	m4i r = make_m4i((i32)1);

	r.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1] + a.m[2][0] * b.m[0][2] + a.m[3][0] * b.m[0][3];
	r.m[1][0] = a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1] + a.m[2][0] * b.m[1][2] + a.m[3][0] * b.m[1][3];
	r.m[2][0] = a.m[0][0] * b.m[2][0] + a.m[1][0] * b.m[2][1] + a.m[2][0] * b.m[2][2] + a.m[3][0] * b.m[2][3];
	r.m[3][0] = a.m[0][0] * b.m[3][0] + a.m[1][0] * b.m[3][1] + a.m[2][0] * b.m[3][2] + a.m[3][0] * b.m[3][3];
	r.m[0][1] = a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1] + a.m[2][1] * b.m[0][2] + a.m[3][1] * b.m[0][3];
	r.m[1][1] = a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1] + a.m[2][1] * b.m[1][2] + a.m[3][1] * b.m[1][3];
	r.m[2][1] = a.m[0][1] * b.m[2][0] + a.m[1][1] * b.m[2][1] + a.m[2][1] * b.m[2][2] + a.m[3][1] * b.m[2][3];
	r.m[3][1] = a.m[0][1] * b.m[3][0] + a.m[1][1] * b.m[3][1] + a.m[2][1] * b.m[3][2] + a.m[3][1] * b.m[3][3];
	r.m[0][2] = a.m[0][2] * b.m[0][0] + a.m[1][2] * b.m[0][1] + a.m[2][2] * b.m[0][2] + a.m[3][2] * b.m[0][3];
	r.m[1][2] = a.m[0][2] * b.m[1][0] + a.m[1][2] * b.m[1][1] + a.m[2][2] * b.m[1][2] + a.m[3][2] * b.m[1][3];
	r.m[2][2] = a.m[0][2] * b.m[2][0] + a.m[1][2] * b.m[2][1] + a.m[2][2] * b.m[2][2] + a.m[3][2] * b.m[2][3];
	r.m[3][2] = a.m[0][2] * b.m[3][0] + a.m[1][2] * b.m[3][1] + a.m[2][2] * b.m[3][2] + a.m[3][2] * b.m[3][3];
	r.m[0][3] = a.m[0][3] * b.m[0][0] + a.m[1][3] * b.m[0][1] + a.m[2][3] * b.m[0][2] + a.m[3][3] * b.m[0][3];
	r.m[1][3] = a.m[0][3] * b.m[1][0] + a.m[1][3] * b.m[1][1] + a.m[2][3] * b.m[1][2] + a.m[3][3] * b.m[1][3];
	r.m[2][3] = a.m[0][3] * b.m[2][0] + a.m[1][3] * b.m[2][1] + a.m[2][3] * b.m[2][2] + a.m[3][3] * b.m[2][3];
	r.m[3][3] = a.m[0][3] * b.m[3][0] + a.m[1][3] * b.m[3][1] + a.m[2][3] * b.m[3][2] + a.m[3][3] * b.m[3][3];

	return r;
}

m4i m4i_translate(m4i m, v3i v) {
	m4i r = make_m4i((i32)1);

	r.m[3][0] = v.x;
	r.m[3][1] = v.y;
	r.m[3][2] = v.z;

	return m4i_mul(m, r);
}

m4i m4i_lookat(v3i c, v3i o, v3i u) {
	m4i r = make_m4i((i32)1);

	const v3i f = v3i_normalised(v3i_sub(o, c));
	u = v3i_normalised(u);
	const v3i s = v3i_normalised(v3i_cross(f, u));
	u = v3i_cross(s, f);

	r.m[0][0] = s.x;
	r.m[1][0] = s.y;
	r.m[2][0] = s.z;
	r.m[0][1] = u.x;
	r.m[1][1] = u.y;
	r.m[2][1] = u.z;
	r.m[0][2] = -f.x;
	r.m[1][2] = -f.y;
	r.m[2][2] = -f.z;
	r.m[3][0] = -v3i_dot(s, c);
	r.m[3][1] = -v3i_dot(u, c);
	r.m[3][2] = v3i_dot(f, c);

	return r;
}

m4i make_m4i(i32 d) {
	m4i r;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			r.m[x][y] = (i32)0;
		}
	}

	r.m[0][0] = d;
	r.m[1][1] = d;
	r.m[2][2] = d;
	r.m[3][3] = d;

	return r;
}

m4i m4i_scale(m4i m, v3i v) {
	m4i r = make_m4i((i32)1);

	r.m[0][0] = v.x;
	r.m[1][1] = v.y;
	r.m[2][2] = v.z;

	return m4i_mul(m, r);
}

m4i m4i_pers(i32 fov, i32 asp, i32 n, i32 f) {
	m4i r = make_m4i((i32)1);

	const i32 q = (i32)1 / (i32)tan(torad((double)fov) / 2.0);
	const i32 a = q / asp;
	const i32 b = (n + f) / (n - f);
	const i32 c = ((i32)2 * n * f) / (n - f);

	r.m[0][0] = a;
	r.m[1][1] = q;
	r.m[2][2] = b;
	r.m[2][3] = (i32)-1;
	r.m[3][2] = c;

	return r;
}

v4i m4i_transform(m4i m, v4i v) {
	return make_v4i(
		m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] + v.w,
		m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] + v.w,
		m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] + v.w,
		m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] + v.w);
}

m3d m3d_rotate(m3d m, double a, v3d v) {
	m3d r = make_m3d((double)1);

	const double c = (double)cos((double)a);
	const double s = (double)sin((double)a);

	const double omc = (double)1 - c;

	const double x = v.x;
	const double y = v.y;
	const double z = v.z;

	r.m[0][0] = x * x * omc + c;
	r.m[0][1] = y * x * omc + z * s;
	r.m[0][2] = x * z * omc - y * s;
	r.m[1][0] = x * y * omc - z * s;
	r.m[1][1] = y * y * omc + c;
	r.m[1][2] = y * z * omc + x * s;
	r.m[2][0] = x * z * omc + y * s;
	r.m[2][1] = y * z * omc - x * s;
	r.m[2][2] = z * z * omc + c;

	return m3d_mul(m, r);
}

m3d m3d_identity() {
	return make_m3d((double)1);
}

m3d m3d_mul(m3d a, m3d b) {
	m3d r = make_m3d((double)1);

	r.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1] + a.m[2][0] * b.m[0][2];
	r.m[1][0] = a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1] + a.m[2][0] * b.m[1][2];
	r.m[2][0] = a.m[0][0] * b.m[2][0] + a.m[1][0] * b.m[2][1] + a.m[2][0] * b.m[2][2];
	r.m[0][1] = a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1] + a.m[2][1] * b.m[0][2];
	r.m[1][1] = a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1] + a.m[2][1] * b.m[1][2];
	r.m[2][1] = a.m[0][1] * b.m[2][0] + a.m[1][1] * b.m[2][1] + a.m[2][1] * b.m[2][2];
	r.m[0][2] = a.m[0][2] * b.m[0][0] + a.m[1][2] * b.m[0][1] + a.m[2][2] * b.m[0][2];
	r.m[1][2] = a.m[0][2] * b.m[1][0] + a.m[1][2] * b.m[1][1] + a.m[2][2] * b.m[1][2];
	r.m[2][2] = a.m[0][2] * b.m[2][0] + a.m[1][2] * b.m[2][1] + a.m[2][2] * b.m[2][2];

	return r;
}

m3d make_m3d(double d) {
	m3d r;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			r.m[x][y] = (double)0;
		}
	}

	r.m[0][0] = d;
	r.m[1][1] = d;
	r.m[2][2] = d;

	return r;
}

m3d m3d_scale(m3d m, v3d v) {
	m3d r = make_m3d((double)1);

	r.m[0][0] = v.x;
	r.m[1][1] = v.y;
	r.m[2][2] = v.z;

	return m3d_mul(m, r);
}

m2i m2i_identity() {
	return make_m2i((i32)1);
}

m2i m2i_mul(m2i a, m2i b) {
	m2i r = make_m2i((i32)1);

	r.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1];
	r.m[1][0] = a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1];
	r.m[0][1] = a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1];
	r.m[1][1] = a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1];

	return r;
}

m2i make_m2i(i32 d) {
	m2i r;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			r.m[x][y] = (i32)0;
		}
	}

	r.m[0][0] = d;
	r.m[1][1] = d;

	return r;
}

m3i m3i_rotate(m3i m, i32 a, v3i v) {
	m3i r = make_m3i((i32)1);

	const i32 c = (i32)cos((double)a);
	const i32 s = (i32)sin((double)a);

	const i32 omc = (i32)1 - c;

	const i32 x = v.x;
	const i32 y = v.y;
	const i32 z = v.z;

	r.m[0][0] = x * x * omc + c;
	r.m[0][1] = y * x * omc + z * s;
	r.m[0][2] = x * z * omc - y * s;
	r.m[1][0] = x * y * omc - z * s;
	r.m[1][1] = y * y * omc + c;
	r.m[1][2] = y * z * omc + x * s;
	r.m[2][0] = x * z * omc + y * s;
	r.m[2][1] = y * z * omc - x * s;
	r.m[2][2] = z * z * omc + c;

	return m3i_mul(m, r);
}

m3i m3i_identity() {
	return make_m3i((i32)1);
}

m3i m3i_mul(m3i a, m3i b) {
	m3i r = make_m3i((i32)1);

	r.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[1][0] * b.m[0][1] + a.m[2][0] * b.m[0][2];
	r.m[1][0] = a.m[0][0] * b.m[1][0] + a.m[1][0] * b.m[1][1] + a.m[2][0] * b.m[1][2];
	r.m[2][0] = a.m[0][0] * b.m[2][0] + a.m[1][0] * b.m[2][1] + a.m[2][0] * b.m[2][2];
	r.m[0][1] = a.m[0][1] * b.m[0][0] + a.m[1][1] * b.m[0][1] + a.m[2][1] * b.m[0][2];
	r.m[1][1] = a.m[0][1] * b.m[1][0] + a.m[1][1] * b.m[1][1] + a.m[2][1] * b.m[1][2];
	r.m[2][1] = a.m[0][1] * b.m[2][0] + a.m[1][1] * b.m[2][1] + a.m[2][1] * b.m[2][2];
	r.m[0][2] = a.m[0][2] * b.m[0][0] + a.m[1][2] * b.m[0][1] + a.m[2][2] * b.m[0][2];
	r.m[1][2] = a.m[0][2] * b.m[1][0] + a.m[1][2] * b.m[1][1] + a.m[2][2] * b.m[1][2];
	r.m[2][2] = a.m[0][2] * b.m[2][0] + a.m[1][2] * b.m[2][1] + a.m[2][2] * b.m[2][2];

	return r;
}

m3i make_m3i(i32 d) {
	m3i r;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			r.m[x][y] = (i32)0;
		}
	}

	r.m[0][0] = d;
	r.m[1][1] = d;
	r.m[2][2] = d;

	return r;
}

m3i m3i_scale(m3i m, v3i v) {
	m3i r = make_m3i((i32)1);

	r.m[0][0] = v.x;
	r.m[1][1] = v.y;
	r.m[2][2] = v.z;

	return m3i_mul(m, r);
}
