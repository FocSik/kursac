//#include "stm32f411xe.h" //Registers definition
#include "BluetoothFormat.hpp" 

char BluetoothFormat::Stroka[48]; 
 
char* BluetoothFormat::TransformToString(tU32 Humi) 
{ 
sprintf(Stroka, "Humidity is %d percents \n", Humi); 
return(Stroka); 
}