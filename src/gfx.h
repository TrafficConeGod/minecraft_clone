#pragma once
#include "util.h"
#include "int_types.h"

typedef struct PACKED {
    u8 x;
    u8 y;
    u8 z;
    u8 : 8;
} pos_attr;

typedef struct PACKED {
    u8 u;
    u8 v;
    u16 : 16;
} uv_attr;

typedef struct PACKED {
    u8 u_min;
    u8 u_max;
    u8 v_min;
    u8 v_max;
} uv_bounds_attr;

#define ATTR_SIZE 4

_Static_assert(sizeof(pos_attr) == ATTR_SIZE, "pos_attr is not 4 bytes");
_Static_assert(sizeof(uv_attr) == ATTR_SIZE, "uv_attr is not 4 bytes");
_Static_assert(sizeof(uv_bounds_attr) == ATTR_SIZE, "uv_bounds_attr is not 4 bytes");