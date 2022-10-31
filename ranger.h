#ifndef RANGER_H_
#define RANGER_H_

#include <stdint.h>


void RangerInit();

void SendStartPulse();

uint32_t RangerDetect();


#endif /* RANGER_H_ */
