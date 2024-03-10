/*
 * Copyright 2010 Michael Ossmann
 *
 * This file is part of Project Ubertooth.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __UBERTOOTH_TYPES_H
#define __UBERTOOTH_TYPES_H

#include <stdint.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

typedef struct BitField_s
{
  uint32_t bit0:1;
  uint32_t bit1:1;
  uint32_t bit2:1;
  uint32_t bit3:1;
  uint32_t bit4:1;
  uint32_t bit5:1;
  uint32_t bit6:1;
  uint32_t bit7:1;
  uint32_t bit8:1;
  uint32_t bit9:1;
  uint32_t bit10:1;
  uint32_t bit11:1;
  uint32_t bit12:1;
  uint32_t bit13:1;
  uint32_t bit14:1;
  uint32_t bit15:1;
  uint32_t bit16:1;
  uint32_t bit17:1;
  uint32_t bit18:1;
  uint32_t bit19:1;
  uint32_t bit20:1;
  uint32_t bit21:1;
  uint32_t bit22:1;
  uint32_t bit23:1;
  uint32_t bit24:1;
  uint32_t bit25:1;
  uint32_t bit26:1;
  uint32_t bit27:1;
  uint32_t bit28:1;
  uint32_t bit29:1;
  uint32_t bit30:1;
  uint32_t bit31:1;
} Bitfield_Typedef;

typedef union
{
  Bitfield_Typedef  ub;
  uint32_t          uw;
} bit32_t;

#endif
