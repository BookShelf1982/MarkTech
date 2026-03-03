#ifndef XDMATH_H
#define XDMATH_H

#include <math.h>

typedef struct {
  float x, y, z;
} D3;

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

typedef struct {
  float x, y;
} D2;

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

#endif // XDMATH_H
