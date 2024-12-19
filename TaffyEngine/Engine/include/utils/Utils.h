#pragma once

#pragma warning (disable : 6269)
#pragma warning (disable : 4244)

#include <windows.h>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned u32;
typedef long s64;
typedef unsigned long u64;

#define internal static
#define global_var static
#define NULL 0

#include "Camera.h"
#include "CropInfo.h"
#include "Image.h"
#include "Math.h"
#include "RGBA.h"
#include "Shader.h"