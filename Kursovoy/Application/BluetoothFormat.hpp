//To avoid redefinition
#ifndef BLUETOOTHFORMAT_HPP
#define BLUETOOTHFORMAT_HPP 

#include "stm32f411xe.h" //Registers definition
#include "susudefs.hpp" //For tU32 and tF32
#include <cstdio> //For sprintf

class BluetoothFormat
{
private:
  static char Stroka[48];
public:
  static char* TransformToString(tU32 Humi);
};

#endif 