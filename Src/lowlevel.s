/*
 * lowlevel.s
 *
 *  Created on: Jun 9, 2026
 *      Author: aishm
 */

.section .text

.global loop_3cycles
.type loop_3cycles, %function

.balign 4
loop_3cycles:
	SUB R0,#1
	BNE loop_3cycles
	BX LR
