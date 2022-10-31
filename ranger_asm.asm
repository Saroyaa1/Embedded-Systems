; Include C header files
                	.cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","ranger.h"

		.text


TIMER3			.field TIMER3_BASE
RANGER_PORT		.field GPIO_PORTB_BASE


				.asmfunc

RangerDetect	        PUSH 	{lr}
					BL		SendStartPulse


loop			LDR		r0, TIMER3
				MOV		r1, #0
				BL		TimerIntStatus
				CMP		r0, #0
				BEQ		loop

				LDR		r0, TIMER3
				MOV		r1, #TIMER_B
				BL		TimerValueGet
				MOV		r2, r0

				LDR		r0, TIMER3
				MOV		r1, #TIMER_CAPB_EVENT
				BL		TimerIntClear


loop1			LDR		r0, TIMER3
				MOV		r1, #0
				BL		TimerIntStatus
				CMP		r0, #0
				BEQ		loop1

				LDR		r0, TIMER3
				MOV		r1, #TIMER_B
				BL		TimerValueGet
				MOV		r3, r0

				LDR		r0, TIMER3
				MOV		r1, #TIMER_B
				BL		TimerIntClear



				POP		{pc}

				.endasmfunc
