/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * MODIFIED 2023-02-13 AK Sommerville.
 * Removing most of fceu's particulars, making a raw 6502 emulator for educational purposes.
 * (but a 6502 with all the NES's CPU quirks, hopefully).
 * Removing the fceu debugger stuff.
 * Removing cycle counters, not interesting to us.
 */

#ifndef X6502_H
#define X6502_H

#include <stdint.h>
#define FASTAPASS(n)
#define FP_FASTAPASS(n)
typedef uint8_t (*ARead_fn)(unsigned int p);
typedef void (*BWrite_fn)(unsigned int p,uint8_t v);
extern ARead_fn ARead[0x10000];
extern BWrite_fn BWrite[0x10000];
extern uint8_t RAM[0x800];

typedef struct __X6502 {
  uint16_t PC;    /* I'll change this to uint32_t later... */
                  /* I'll need to AND PC after increments to 0xFFFF */
                  /* when I do, though.  Perhaps an IPC() macro? */
  uint8_t A,X,Y,S,P,mooPI;
  uint8_t jammed;

  uint32_t IRQlow; /* Simulated IRQ pin held low (or is it high?).
                      And other junk hooked on for speed reasons.*/
  uint8_t DB;      /* Data bus "cache" for reads from certain areas */
} X6502;

void X6502_Run();

extern X6502 X;

#define N_FLAG  0x80
#define V_FLAG  0x40
#define U_FLAG  0x20
#define B_FLAG  0x10
#define D_FLAG  0x08
#define I_FLAG  0x04
#define Z_FLAG  0x02
#define C_FLAG  0x01

#define NTSC_CPU 1789772.7272727272727272
#define PAL_CPU  1662607.125

#define FCEU_IQEXT      0x001
#define FCEU_IQEXT2     0x002
/* ... */
#define FCEU_IQRESET    0x020
#define FCEU_IQNMI2  0x040  // Delayed NMI, gets converted to *_IQNMI
#define FCEU_IQNMI  0x080
#define FCEU_IQDPCM     0x100
#define FCEU_IQFCOUNT   0x200
#define FCEU_IQTEMP     0x800

void X6502_Init(void);
void X6502_Reset(void);
void X6502_Power(void);

void TriggerNMI(void);
void TriggerNMI2(void);

uint8_t FASTAPASS(1) X6502_DMR(uint32_t A);
void FASTAPASS(2) X6502_DMW(uint32_t A, uint8_t V);

void FASTAPASS(1) X6502_IRQBegin(int w);
void FASTAPASS(1) X6502_IRQEnd(int w);

#endif
