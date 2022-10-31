#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include "launchpad.h"
#include "ranger.h"

#define TIMER3          TIMER3_BASE
#define RANGER_PERIPH   SYSCTL_PERIPH_GPIOB
#define RANGER_PORT     GPIO_PORTB_BASE
#define RANGER_PIN      GPIO_PIN_3

void RangerInit() {

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    SysCtlPeripheralEnable(RANGER_PERIPH);


    TimerConfigure(TIMER3, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME_UP));

    TimerControlEvent(TIMER3, TIMER_B, TIMER_EVENT_BOTH_EDGES);

    TimerEnable(TIMER3, TIMER_B);
}

void SendStartPulse() {
    GPIOPinTypeGPIOOutput(RANGER_PORT, RANGER_PIN);
    GPIOPinWrite(RANGER_PORT, RANGER_PIN, 0);

    WaitUs(2);

    GPIOPinWrite(RANGER_PORT, RANGER_PIN, RANGER_PIN);

    WaitUs(5);

    GPIOPinWrite(RANGER_PORT, RANGER_PIN, 0);
    GPIOPinTypeTimer(RANGER_PORT, RANGER_PIN);
    GPIOPinConfigure(GPIO_PB3_T3CCP1);

    TimerIntStatus(TIMER3, false);

    TimerIntClear(TIMER3, TIMER_CAPB_EVENT);
}
