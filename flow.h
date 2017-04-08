#ifndef FLOW_H
#define FLOW_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "pixy.h"
#include <wiringPi.h>
#include <wiringShift.h>
#include <sr595.h>

// Capture methods
int ConnectPixy(void);
void CaptureImage( unsigned char frame[200][320] );

//Shift Register methods
void InitShiftRegister(void);
unsigned char * UpdateShiftRegister(unsigned char *Solenoid);

//Hysteresis thresholding method
void Hysteresis_Thresholding(unsigned char low_Thresh, unsigned char high_Thresh, unsigned char diff[320][200]);

#endif // FLOW_H

