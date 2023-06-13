/*
 * Copyright 2012 Advanced Micro Devices, Inc.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef SI_PM4_H
#define SI_PM4_H

#include "winsys/radeon_winsys.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward definitions */
struct si_screen;
struct si_context;

/* State atoms are callbacks which write a sequence of packets into a GPU
 * command buffer (AKA indirect buffer, AKA IB, AKA command stream, AKA CS).
 */
struct si_atom {
   void (*emit)(struct si_context *ctx);
};

struct si_pm4_state {
   /* PKT3_SET_*_REG handling */
   uint16_t last_reg;   /* register offset in dwords */
   uint16_t last_pm4;
   uint16_t ndw;        /* number of dwords in pm4 */
   uint8_t last_opcode;
   uint8_t last_idx;

   /* For shader states only */
   bool is_shader;
   struct si_atom atom;

   /* commands for the DE */
   uint16_t max_dw;

   /* Used by SQTT to override the shader address */
   uint16_t reg_va_low_idx;

   /* This must be the last field because the array can continue after the structure. */
   uint32_t pm4[64];
};

void si_pm4_cmd_add(struct si_pm4_state *state, uint32_t dw);
void si_pm4_set_reg(struct si_pm4_state *state, unsigned reg, uint32_t val);
void si_pm4_set_reg_va(struct si_pm4_state *state, unsigned reg, uint32_t val);
void si_pm4_set_reg_idx3(struct si_screen *sscreen, struct si_pm4_state *state,
                         unsigned reg, uint32_t val);

void si_pm4_clear_state(struct si_pm4_state *state);
void si_pm4_free_state(struct si_context *sctx, struct si_pm4_state *state, unsigned idx);

void si_pm4_emit(struct si_context *sctx, struct si_pm4_state *state);
void si_pm4_reset_emitted(struct si_context *sctx);
struct si_pm4_state *si_pm4_create_sized(unsigned max_dw);
struct si_pm4_state *si_pm4_clone(struct si_pm4_state *orig);

#ifdef __cplusplus
}
#endif

#endif
