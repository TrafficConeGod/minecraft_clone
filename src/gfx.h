#pragma once
#include "util.h"
#include "int_types.h"
#include <cglm/cglm.h>
#include <cglm/struct.h>

typedef struct PACKED {
    u8 x;
    u8 y;
    u8 z;
    u8 : 8;
} pos_attr;

typedef struct {
    vec2 uv;
    ivec4 uv_bounds;
} uv_block;

_Static_assert(sizeof(pos_attr) == sizeof(u32), "pos_attr is not 4 bytes");