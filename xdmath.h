#ifndef XDMATH_H
#define XDMATH_H

#include <math.h>

typedef struct {
  int x, y, z;
} D3i;

typedef struct {
  float x, y, z;
} D3;

D3 D3Add(D3 a, D3 b);
D3 D3Sub(D3 a, D3 b);
D3 D3Scl(D3 a, float s);
D3 D3Mul(D3 a, D3 b);
float D3Dot(D3 a, D3 b);
D3 D3Crs(D3 a, D3 b);
float D3Mag(D3 a);
D3 D3Nrm(D3 a);

#ifdef XDMATH_IMPLEMENTATION
inline D3 D3Add(D3 a, D3 b)
{
  return (D3){
    a.x + b.x,
    a.y + b.y,
    a.z + b.z
  };
}

inline D3 D3Sub(D3 a, D3 b)
{
  return (D3){
    a.x - b.x,
    a.y - b.y,
    a.z - b.z
  };
}

inline D3 D3Scl(D3 a, float s)
{
  return (D3){
    a.x * s,
    a.y * s,
    a.z * s
  };
}

inline D3 D3Mul(D3 a, D3 b)
{
  return (D3){
    a.x * b.x,
    a.y * b.y,
    a.z * b.z
  };
}

inline float D3Dot(D3 a, D3 b)
{
  return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

inline D3 D3Crs(D3 a, D3 b)
{
  return (D3) {
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  };
}

inline float D3Mag(D3 a)
{
  return sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

inline D3 D3Nrm(D3 a)
{
  float m = D3Mag(a);
  return (D3){a.x / m, a.y / m, a.z / m};
}
#endif // XDMATH_IMPLEMENTATION

typedef struct {
  float x, y;
} D2;

D2 D2Add(D2 a, D2 b);
D2 D2Sub(D2 a, D2 b);
D2 D2Scl(D2 a, float s);
D2 D2Mul(D2 a, D2 b);
float D2Dot(D2 a, D2 b);
float D2Mag(D2 a);
D2 D2Nrm(D2 a);

#ifdef XDMATH_IMPLEMENTATION
inline D2 D2Add(D2 a, D2 b)
{
  return (D2){
    a.x + b.x,
    a.y + b.y,
  };
}

inline D2 D2Sub(D2 a, D2 b)
{
  return (D2){
    a.x - b.x,
    a.y - b.y
  };
}

inline D2 D2Scl(D2 a, float s)
{
  return (D2){
    a.x * s,
    a.y * s
  };
}

inline D2 D2Mul(D2 a, D2 b)
{
  return (D2){
    a.x * b.x,
    a.y * b.y,
  };
}

inline float D2Dot(D2 a, D2 b)
{
  return ((a.x * b.x) + (a.y * b.y));
}

inline float D2Mag(D2 a)
{
  return sqrtf((a.x * a.x) + (a.y * a.y));
}

inline D2 D2Nrm(D2 a)
{
  float m = D2Mag(a);
  return (D2){a.x / m, a.y / m};
}
#endif // XDMATH_IMPLEMENTATION

// All are in column major
typedef struct {
  float m11, m21, m31, m41;
  float m12, m22, m32, m42;
  float m13, m23, m33, m43;
  float m14, m24, m34, m44;
} M4x4;

typedef struct {
  float x, y, z, w;
} D4;

M4x4 M4x4Translate(float x, float y, float z);
M4x4 M4x4RotateYAxis(float a);
M4x4 M4x4Fuck(M4x4 r, M4x4 t);
D4 M4x4MulD4(M4x4 m, D4 v);

#ifdef XDMATH_IMPLEMENTATION
inline M4x4 M4x4Translate(float x, float y, float z)
{
  return (M4x4) {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
       x,    y,    z, 1.0f
  };
}

inline M4x4 M4x4RotateYAxis(float a)
{
  float cosine = cosf(a);
  float sine = sinf(a);
  return (M4x4) {
    cosine, 0.0f, -sine, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    sine, 0.0f, cosine, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
}

inline M4x4 M4x4Fuck(M4x4 r, M4x4 t)
{
  return (M4x4) {
    r.m11, r.m21, r.m31, 0.0f,
    r.m12, r.m22, r.m32, 0.0f,
    r.m13, r.m23, r.m33, 0.0f,
    t.m14, t.m24, t.m34, 1.0f
  };
}

inline D4 M4x4MulD4(M4x4 m, D4 v)
{
  return (D4) {
    (m.m11*v.x)+(m.m12*v.y)+(m.m13*v.z)+(m.m14*v.w),
    (m.m21*v.x)+(m.m22*v.y)+(m.m23*v.z)+(m.m24*v.w),
    (m.m31*v.x)+(m.m32*v.y)+(m.m33*v.z)+(m.m34*v.w),
    (m.m41*v.x)+(m.m42*v.y)+(m.m43*v.z)+(m.m44*v.w)
  };
}
#endif // XDMATH_IMPLEMENTATION

#endif // XDMATH_H
