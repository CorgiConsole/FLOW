#ifndef SHIFT_H
#define SHIFT_H

#endif // SHIFT_H

#include <wiringPi.h>
#include <wiringShift.h>
#include <sr595.h>

unsigned char SOL[16];

//Initialize Shift Registers
void InitShiftRegister(void)
{
  //Configure Shift Register Output
  wiringPiSetup();
  sr595Setup(256,16,0,1,2);
}

//Update Shift Register array
unsigned char * UpdateShiftRegister(unsigned char *Solenoid)
{
  for(int i=0;i<16;i++)
  {
    digitalWrite(2,0);
    if(Solenoid[i] == 1)
    {
      digitalWrite(0,HIGH);
    }
    else
    {
      digitalWrite(0,LOW);
    }
    digitalWrite(1,0);
    digitalWrite(1,1);
    digitalWrite(2,1);
  }
  return Solenoid;
}

