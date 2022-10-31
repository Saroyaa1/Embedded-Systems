
            	.cdecls "stopwatch.h"
            	.text



StopWatchIncrement  .asmfunc
                ; Update the first digit (right most) of the 7-segment display.
                ; It is digit[0] in Seg7Segment, and it represents 1/100 second in the stop watch.
                LDRB    r1, [r0, #0]        ; load c2 to r1, note c2's offset is zero which can be omitted
                ADD     r1, #1              ; increment c2 by one
                STRB    r1, [r0, #0]        ; store back c2
                CMP     r1, #10             ; compare c2 and 10
                BNE     return              ; if c2 != 10, jump to return
                MOV     r1, #0              ; reset c2 to zero
                STRB    r1, [r0, #0]        ; store back again

                ; ADD YOUR CODE BELOW: Update digit[1], digit[2], and digit[3], if needed
                LDRB    r1, [r0, #1]        ; load c2 to r1, note c2's offset is zero which can be omitted
                ADD     r1, #1              ; increment c2 by one
                STRB    r1, [r0, #1]        ; store back c2
                CMP     r1, #6             ; compare c2 and 10
                BNE     return              ; if c2 != 10, jump to return
                MOV     r1, #0              ; reset c2 to zero
                STRB    r1, [r0, #1]        ; store back again

                LDRB    r1, [r0, #2]        ; load c2 to r1, note c2's offset is zero which can be omitted
                ADD     r1, #1              ; increment c2 by one
                STRB    r1, [r0, #2]        ; store back c2
                CMP     r1, #10             ; compare c2 and 10
                BNE     return              ; if c2 != 10, jump to return
                MOV     r1, #0              ; reset c2 to zero
                STRB    r1, [r0, #2]        ; store back again

                LDRB    r1, [r0, #3]        ; load c2 to r1, note c2's offset is zero which can be omitted
                ADD     r1, #1              ; increment c2 by one
                STRB    r1, [r0, #3]        ; store back c2
                CMP     r1, #6             ; compare c2 and 10
                BNE     return              ; if c2 != 10, jump to return
                MOV     r1, #0              ; reset c2 to zero
                STRB    r1, [r0, #3]        ; store back again




return          BX      LR                  ; return
                .endasmfunc

                .end
