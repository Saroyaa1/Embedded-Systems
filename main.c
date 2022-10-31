#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include "launchpad.h"
#include "ranger.h"
#include "seg7.h"

 enum { Run, Pause
 } sysState = Pause;


 Seg7Display seg7 = { { 0, 0, 0, 0}, true };

 enum { Inch, MM
 } sysUnit = MM;


 void Measure(uint32_t time)
 {
     uint32_t Capture_time = RangerDetect();
     uint32_t distance = (Capture_time * 340 / 2) / 10000;

     int delay = 100;
     uprintf("%s%d\n\r", "Measure distance",distance);

     if(sysState == Run) {
         uprintf("%s\n\r", "Measure distance");

         if (sysUnit == MM) {

             if(distance > 9999) {
                 seg7.digit[0] = 9;
                 seg7.digit[1] = 9;
                 seg7.digit[2] = 9;
                 seg7.digit[3] = 9;
                 seg7.colon_on = true;
             }

             else {

                 if (distance / 1000 == 0) {
                     seg7.digit[3] = 11;
                 }
                 else {
                     seg7.digit[3] = distance / 1000;
                 }

                 if ( ( (distance % 1000) / 100 == 0) && (seg7.digit[3] == 11) ) {
                     seg7.digit[2] = 11;
                 }
                 else {
                     seg7.digit[2] = (distance % 1000) / 100;
                 }

                 if ( ( (distance % 100) / 10 == 0) && (seg7.digit[2] == 11)) {
                     seg7.digit[1] = 11;
                 }
                 else {
                     seg7.digit[1] = (distance % 100) / 10;
                 }

                 seg7.digit[0] = distance % 10;
                 seg7.colon_on = false;

             }
         }

         else {
             int inch = distance / 25.4;
             int feet = inch % 12;


             if ( (inch / 12) / 10 == 0) {
                 seg7.digit[3] = 11;
             }
             else {
                 seg7.digit[3] = (inch / 12) / 10;
             }
             seg7.digit[2] = (inch / 12) % 10;
             if (feet / 10 == 0) {
                 seg7.digit[1] = 11;
             }
             else {
                 seg7.digit[1] = feet / 10;
             }

             seg7.digit[0] = feet % 10;

             seg7.colon_on = true;



         }
     }
     uprintf("Measurement Distance: %d%d %d%d \n\r", seg7.digit[3], seg7.digit[2], seg7.digit[1], seg7.digit[0]);
     Seg7Update(&seg7);

     ScheduleCallback(Measure, time + delay);

 }

 void PushButtonISR()
 {

     GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);


     static uint32_t last_time = 0;
     uint32_t current_time = SysTimeGet();
     if (current_time < last_time + 150)
         return;
     last_time = current_time;


     switch (PbRead())
     {

     case 1:
         uprintf("%s\n\r", "System state Updated to : ");
         if (sysState == Pause) {
             sysState = Run;
             uprintf("%s\n\r", "Run");

         }
         else if (sysState == Run){
             sysState = Pause;
             uprintf("%s\n\r", "Pause");
         }


         break;

     case 2:
         uprintf("%s\n\r", "Measurement type Update to : ");
         if(sysUnit == Inch) {
             sysUnit = MM;
             uprintf("%s\n\r", "Mm");
             }
         else if( sysUnit == MM) {
             sysUnit = Inch;
             uprintf("%s\n\r", "Inch");

         }

         break;
     }
 }

 void InterruptInit()
 {
     // Set interrupt on Port F, pin 0 (SW1) and pin 4 (SW2)
     GPIOIntRegister(GPIO_PORTF_BASE, PushButtonISR); // register the interrupt handler
     GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, // interrupt on falling edge, note that SW1 and SW2 are active low
                    GPIO_FALLING_EDGE);
     IntPrioritySet(INT_GPIOF, 0); // set interrupt level to 0 (0 is the highest for programmable interrupts)
     GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // enable interrupts on SW1 and SW2 input

 }

 int main(void){
     LpInit();
     Seg7Init();
     RangerInit();
     InterruptInit();
     ScheduleCallback(Measure, 1000);
     uprintf("%s\n\r", "lab 8");

     Seg7Update(&seg7);




     while (true)
     {
         ScheduleExecute();

         asm(" wfi");
     }

 }

