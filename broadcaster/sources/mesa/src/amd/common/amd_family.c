/*
 * Copyright © 2017 Advanced Micro Devices, Inc.
 *
 * SPDX-License-Identifier: MIT
 */

#include "amd_family.h"

#include "util/macros.h"

const char *ac_get_family_name(enum radeon_family family)
{
   switch (family) {
   case CHIP_TAHITI:
      return "TAHITI";
   case CHIP_PITCAIRN:
      return "PITCAIRN";
   case CHIP_VERDE:
      return "VERDE";
   case CHIP_OLAND:
      return "OLAND";
   case CHIP_HAINAN:
      return "HAINAN";
   case CHIP_BONAIRE:
      return "BONAIRE";
   case CHIP_KABINI:
      return "KABINI";
   case CHIP_KAVERI:
      return "KAVERI";
   case CHIP_HAWAII:
      return "HAWAII";
   case CHIP_TONGA:
      return "TONGA";
   case CHIP_ICELAND:
      return "ICELAND";
   case CHIP_CARRIZO:
      return "CARRIZO";
   case CHIP_FIJI:
      return "FIJI";
   case CHIP_STONEY:
      return "STONEY";
   case CHIP_POLARIS10:
      return "POLARIS10";
   case CHIP_POLARIS11:
      return "POLARIS11";
   case CHIP_POLARIS12:
      return "POLARIS12";
   case CHIP_VEGAM:
      return "VEGAM";
   case CHIP_VEGA10:
      return "VEGA10";
   case CHIP_RAVEN:
      return "RAVEN";
   case CHIP_VEGA12:
      return "VEGA12";
   case CHIP_VEGA20:
      return "VEGA20";
   case CHIP_RAVEN2:
      return "RAVEN2";
   case CHIP_RENOIR:
      return "RENOIR";
   case CHIP_MI100:
      return "MI100";
   case CHIP_MI200:
      return "MI200";
   case CHIP_GFX940:
      return "GFX940";
   case CHIP_NAVI10:
      return "NAVI10";
   case CHIP_NAVI12:
      return "NAVI12";
   case CHIP_NAVI14:
      return "NAVI14";
   case CHIP_NAVI21:
      return "NAVI21";
   case CHIP_NAVI22:
      return "NAVI22";
   case CHIP_NAVI23:
      return "NAVI23";
   case CHIP_VANGOGH:
      return "VANGOGH";
   case CHIP_NAVI24:
      return "NAVI24";
   case CHIP_REMBRANDT:
      return "REMBRANDT";
   case CHIP_RAPHAEL_MENDOCINO:
      return "RAPHAEL_MENDOCINO";
   case CHIP_GFX1100:
      return "GFX1100";
   case CHIP_GFX1101:
      return "GFX1101";
   case CHIP_GFX1102:
      return "GFX1102";
   case CHIP_GFX1103_R1:
      return "GFX1103_R1";
   case CHIP_GFX1103_R2:
      return "GFX1103_R2";
   default:
      unreachable("Unknown GPU family");
   }
}
