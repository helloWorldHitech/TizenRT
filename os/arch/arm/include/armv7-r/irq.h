/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/include/armv7-r/irq.h
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/* This file should never be included directed but, rather, only indirectly
 * through include/tinyara/irq.h
 */

#ifndef __ARCH_ARM_INCLUDE_ARMV7_R_IRQ_H
#define __ARCH_ARM_INCLUDE_ARMV7_R_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/irq.h>

#ifndef __ASSEMBLY__
#include <stdint.h>
#include <arch/arch.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/*
  Region definitions for our platform
 * TODO: This definition should be moved to platform specific header
 *
 */
#define MPU_REG_ENTIRE_MAP    (0)
#define MPU_REG_USER_RAM    (1)
#define MPU_REG_USER_CONFIG1  (2)
#define MPU_REG_USER_CONFIG0  (3)
#define MPU_REG_TASK_STACK    (4)
#define MPU_REG_KERN_FLASH    (5)
#define MPU_REG_KERN_DATA   (6)
#define MPU_REG_KERN_VEC    (7)

/* Total number of regions for user, including the user task region */
#define MPU_TOTAL_USER_REG          (MPU_REG_KERN_FLASH - MPU_REG_USER_CONFIG1)

/* IRQ Stack Frame Format:
 *
 * Context is always saved/restored in the same way:
 *
 *   (1) stmia rx, {r0-r14}
 *   (2) then the PC and CPSR
 *
 * This results in the following set of indices that can be used to access
 * individual registers in the xcp.regs array:
 */

#define REG_R0              (0)
#define REG_R1              (1)
#define REG_R2              (2)
#define REG_R3              (3)
#define REG_R4              (4)
#define REG_R5              (5)
#define REG_R6              (6)
#define REG_R7              (7)
#define REG_R8              (8)
#define REG_R9              (9)
#define REG_R10             (10)
#define REG_R11             (11)
#define REG_R12             (12)
#define REG_R13             (13)
#define REG_R14             (14)
#define REG_R15             (15)
#define REG_CPSR            (16)

#define ARM_CONTEXT_REGS    (17)

/* If the MCU supports a floating point unit, then it will be necessary
 * to save the state of the FPU status register and data registers on
 * each context switch.  These registers are not saved during interrupt
 * level processing, however. So, as a consequence, floating point
 * operations may NOT be performed in interrupt handlers.
 *
 * The FPU provides an extension register file containing 32 single-
 * precision registers. These can be viewed as:
 *
 * - Sixteen 64-bit double word registers, D0-D15
 * - Thirty-two 32-bit single-word registers, S0-S31
 *   S<2n> maps to the least significant half of D<n>
 *   S<2n+1> maps to the most significant half of D<n>.
 */

#ifdef CONFIG_ARCH_FPU
#define REG_D0            (ARM_CONTEXT_REGS+0)	/* D0 */
#define REG_S0            (ARM_CONTEXT_REGS+0)	/* S0 */
#define REG_S1            (ARM_CONTEXT_REGS+1)	/* S1 */
#define REG_D1            (ARM_CONTEXT_REGS+2)	/* D1 */
#define REG_S2            (ARM_CONTEXT_REGS+2)	/* S2 */
#define REG_S3            (ARM_CONTEXT_REGS+3)	/* S3 */
#define REG_D2            (ARM_CONTEXT_REGS+4)	/* D2 */
#define REG_S4            (ARM_CONTEXT_REGS+4)	/* S4 */
#define REG_S5            (ARM_CONTEXT_REGS+5)	/* S5 */
#define REG_D3            (ARM_CONTEXT_REGS+6)	/* D3 */
#define REG_S6            (ARM_CONTEXT_REGS+6)	/* S6 */
#define REG_S7            (ARM_CONTEXT_REGS+7)	/* S7 */
#define REG_D4            (ARM_CONTEXT_REGS+8)	/* D4 */
#define REG_S8            (ARM_CONTEXT_REGS+8)	/* S8 */
#define REG_S9            (ARM_CONTEXT_REGS+9)	/* S9 */
#define REG_D5            (ARM_CONTEXT_REGS+10)	/* D5 */
#define REG_S10           (ARM_CONTEXT_REGS+10)	/* S10 */
#define REG_S11           (ARM_CONTEXT_REGS+11)	/* S11 */
#define REG_D6            (ARM_CONTEXT_REGS+12)	/* D6 */
#define REG_S12           (ARM_CONTEXT_REGS+12)	/* S12 */
#define REG_S13           (ARM_CONTEXT_REGS+13)	/* S13 */
#define REG_D7            (ARM_CONTEXT_REGS+14)	/* D7 */
#define REG_S14           (ARM_CONTEXT_REGS+14)	/* S14 */
#define REG_S15           (ARM_CONTEXT_REGS+15)	/* S15 */
#define REG_D8            (ARM_CONTEXT_REGS+16)	/* D8 */
#define REG_S16           (ARM_CONTEXT_REGS+16)	/* S16 */
#define REG_S17           (ARM_CONTEXT_REGS+17)	/* S17 */
#define REG_D9            (ARM_CONTEXT_REGS+18)	/* D9 */
#define REG_S18           (ARM_CONTEXT_REGS+18)	/* S18 */
#define REG_S19           (ARM_CONTEXT_REGS+19)	/* S19 */
#define REG_D10           (ARM_CONTEXT_REGS+20)	/* D10 */
#define REG_S20           (ARM_CONTEXT_REGS+20)	/* S20 */
#define REG_S21           (ARM_CONTEXT_REGS+21)	/* S21 */
#define REG_D11           (ARM_CONTEXT_REGS+22)	/* D11 */
#define REG_S22           (ARM_CONTEXT_REGS+22)	/* S22 */
#define REG_S23           (ARM_CONTEXT_REGS+23)	/* S23 */
#define REG_D12           (ARM_CONTEXT_REGS+24)	/* D12 */
#define REG_S24           (ARM_CONTEXT_REGS+24)	/* S24 */
#define REG_S25           (ARM_CONTEXT_REGS+25)	/* S25 */
#define REG_D13           (ARM_CONTEXT_REGS+26)	/* D13 */
#define REG_S26           (ARM_CONTEXT_REGS+26)	/* S26 */
#define REG_S27           (ARM_CONTEXT_REGS+27)	/* S27 */
#define REG_D14           (ARM_CONTEXT_REGS+28)	/* D14 */
#define REG_S28           (ARM_CONTEXT_REGS+28)	/* S28 */
#define REG_S29           (ARM_CONTEXT_REGS+29)	/* S29 */
#define REG_D15           (ARM_CONTEXT_REGS+30)	/* D15 */
#define REG_S30           (ARM_CONTEXT_REGS+30)	/* S30 */
#define REG_S31           (ARM_CONTEXT_REGS+31)	/* S31 */
#define REG_FPSCR         (ARM_CONTEXT_REGS+32)	/* Floating point status and control */
#define FPU_CONTEXT_REGS  (33)
#else
#define FPU_CONTEXT_REGS  (0)
#endif

#if defined(CONFIG_BUILD_PROTECTED)
#define REG_USR_CFG1    (ARM_CONTEXT_REGS+FPU_CONTEXT_REGS)
#define REG_RNUM      (REG_USR_CFG1 + 0)
#define REG_RBASE     (REG_USR_CFG1 + 1)
#define REG_RSIZE     (REG_USR_CFG1 + 2)
#define REG_RATTR     (REG_USR_CFG1 + 3)

#define REG_USR_CFG0    (REG_RATTR + 1)
#define REG_RNUM_0      (REG_USR_CFG0 + 0)
#define REG_RBASE_0     (REG_USR_CFG0 + 1)
#define REG_RSIZE_0     (REG_USR_CFG0 + 2)
#define REG_RATTR_0     (REG_USR_CFG0 + 3)

#define REG_USR_STK     (REG_RATTR_0 + 1)
#define REG_RNUM_STK    (REG_USR_STK + 0)
#define REG_RBASE_STK   (REG_USR_STK + 1)
#define REG_RSIZE_STK   (REG_USR_STK + 2)
#define REG_RATTR_STK   (REG_USR_STK + 3)

#define MPU_CONTEXT_REGS    (4 * MPU_TOTAL_USER_REG)	/* user configurable registers are available */
#define MPU_CONTEXT_SIZE    (4 * MPU_CONTEXT_REGS)	/* 3 regions, each region has 4 info, each info is 4 byte */
#else
#define MPU_CONTEXT_REGS  (0)
#endif

/* The total number of registers saved by software */

#define XCPTCONTEXT_REGS    (ARM_CONTEXT_REGS + FPU_CONTEXT_REGS + MPU_CONTEXT_REGS)
#define XCPTCONTEXT_SIZE    (4 * XCPTCONTEXT_REGS)

/* Friendly register names */

#define REG_A1              REG_R0
#define REG_A2              REG_R1
#define REG_A3              REG_R2
#define REG_A4              REG_R3
#define REG_V1              REG_R4
#define REG_V2              REG_R5
#define REG_V3              REG_R6
#define REG_V4              REG_R7
#define REG_V5              REG_R8
#define REG_V6              REG_R9
#define REG_V7              REG_R10
#define REG_SB              REG_R9
#define REG_SL              REG_R10
#define REG_FP              REG_R11
#define REG_IP              REG_R12
#define REG_SP              REG_R13
#define REG_LR              REG_R14
#define REG_PC              REG_R15

/* The PIC register is usually R10. It can be R9 is stack checking is enabled
 * or if the user changes it with -mpic-register on the GCC command line.
 */

#define REG_PIC             REG_R10

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef __ASSEMBLY__

/* This structure represents the return state from a system call */

#ifdef CONFIG_LIB_SYSCALL
struct xcpt_syscall_s {
#if defined(CONFIG_BUILD_KERNEL) || defined(CONFIG_BUILD_PROTECTED)
	uint32_t cpsr;				/* The CPSR value */
#endif
	uint32_t sysreturn;			/* The return PC */
};
#endif

/* This struct defines the way the registers are stored.  We need to save:
 *
 *  1  CPSR
 *  7  Static registers, v1-v7 (aka r4-r10)
 *  1  Frame pointer, fp (aka r11)
 *  1  Stack pointer, sp (aka r13)
 *  1  Return address, lr (aka r14)
 * ---
 * 11  (XCPTCONTEXT_USER_REG)
 *
 * On interrupts, we also need to save:
 *  4  Volatile registers, a1-a4 (aka r0-r3)
 *  1  Scratch Register, ip (aka r12)
 *---
 *  5  (XCPTCONTEXT_IRQ_REGS)
 *
 * For a total of 17 (XCPTCONTEXT_REGS)
 */

#ifndef __ASSEMBLY__

struct xcptcontext {
	/* The following function pointer is non-zero if there are pending signals
	 * to be processed.
	 */

#ifndef CONFIG_DISABLE_SIGNALS
	void *sigdeliver;			/* Actual type is sig_deliver_t */

	/* These are saved copies of LR and CPSR used during signal processing. */

	uint32_t saved_pc;
	uint32_t saved_cpsr;

#if defined(CONFIG_BUILD_PROTECTED) || defined(CONFIG_BUILD_KERNEL)
	/* This is the saved address to use when returning from a user-space
	 * signal handler.
	 */

	uint32_t sigreturn;

#endif
#endif

	/* Register save area
	 * save the MPU configurations of the user task (included in the CPU regs)
	 * MPU_REGS - Number of user configurable regions + user task region
	 * During the context switch, the prev task's MPU information is saved
	 * into this data structure which is part of the task's TCB. And MPU
	 * registers are overwritten by the new tasks's MPU info.
	 */
	uint32_t regs[XCPTCONTEXT_REGS];
	/* Extra fault address register saved for common paging logic.  In the
	 * case of the pre-fetch abort, this value is the same as regs[REG_R15];
	 * For the case of the data abort, this value is the value of the fault
	 * address register (FAR) at the time of data abort exception.
	 */

#ifdef CONFIG_PAGING
	uintptr_t far;
#endif

#ifdef CONFIG_LIB_SYSCALL
	/* The following array holds the return address and the exc_return value
	 * needed to return from each nested system call.
	 */

	uint8_t nsyscalls;
	struct xcpt_syscall_s syscall[CONFIG_SYS_NNEST];
#endif

#ifdef CONFIG_ARCH_ADDRENV
#ifdef CONFIG_ARCH_STACK_DYNAMIC
	/* This array holds the physical address of the level 2 page table used
	 * to map the thread's stack memory.  This array will be initially of
	 * zeroed and would be back-up up with pages during page fault exception
	 * handling to support dynamically sized stacks for each thread.
	 */

	FAR uintptr_t *ustack[ARCH_STACK_NSECTS];
#endif

#ifdef CONFIG_ARCH_KERNEL_STACK
	/* In this configuration, all syscalls execute from an internal kernel
	 * stack.  Why?  Because when we instantiate and initialize the address
	 * environment of the new user process, we will temporarily lose the
	 * address environment of the old user process, including its stack
	 * contents.  The kernel C logic will crash immediately with no valid
	 * stack in place.
	 */

	FAR uint32_t *ustkptr;		/* Saved user stack pointer */
	FAR uint32_t *kstack;		/* Allocate base of the (aligned) kernel stack */
#ifndef CONFIG_DISABLE_SIGNALS
	FAR uint32_t *kstkptr;		/* Saved kernel stack pointer */
#endif
#endif
#endif
};
#endif

#endif							/* __ASSEMBLY__ */

/****************************************************************************
 * Inline functions
 ****************************************************************************/

#ifndef __ASSEMBLY__

/* Return the current IRQ state */

static inline irqstate_t irqstate(void)
{
	unsigned int cpsr;

	__asm__ __volatile__
	(
		"\tmrs    %0, cpsr\n"
		: "=r"(cpsr)
		:
		: "memory"
	);

	return cpsr;
}

/* Disable IRQs and return the previous IRQ state */

static inline irqstate_t irqsave(void)
{
	unsigned int cpsr;

	__asm__ __volatile__
	(
		"\tmrs    %0, cpsr\n"
		"\tcpsid  i\n"
#if defined(CONFIG_ARMV7R_DECODEFIQ)
		"\tcpsid  f\n"
#endif
		: "=r"(cpsr)
		:
		: "memory"
	);

	return cpsr;
}

/* Enable IRQs and return the previous IRQ state */

static inline irqstate_t irqenable(void)
{
	unsigned int cpsr;

	__asm__ __volatile__
	(
		"\tmrs    %0, cpsr\n"
		"\tcpsie  i\n"
#if defined(CONFIG_ARMV7R_DECODEFIQ)
		"\tcpsie  f\n"
#endif
		: "=r"(cpsr)
		:
		: "memory"
	);

	return cpsr;
}

/* Restore saved IRQ & FIQ state */

static inline void irqrestore(irqstate_t flags)
{
	__asm__ __volatile__
	(
		"msr    cpsr_c, %0"
		:
		: "r"(flags)
		: "memory"
	);
}

#endif							/* __ASSEMBLY__ */

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif							/* __ARCH_ARM_INCLUDE_ARMV7_R_IRQ_H */
