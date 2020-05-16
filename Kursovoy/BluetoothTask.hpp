#ifndef BLUETOOOTHTASK_HPP
#define BLUETOOOTHTASK_HPP

//For OsWrapper::Thread
#include "thread.hpp" //For definition of class Thread
#include "mailbox.hpp" //For definition of class MailBox
#include "stm32f411xe.h" //Registers definition
#include "susudefs.hpp" //For tU32, tF32
#include "VariableTask.hpp" //For definition of class VariableTask
#include "BluetoothFormat.hpp" //For definition of class BluetoothFormat
#include "Uart.hpp" //For definition of class Uart
#include <array> //For std::array
#include <cstring> //For strlen

//pointer to MailBox in spase OsWrapper
using tMailBox = OsWrapper::MailBox<tF32, 2> ;

//Psevdlonim for task BluetoothTask 
using tBluetoothTaskStack = std::array<OsWrapper::tStack, static_cast<tU16>(OsWrapper::StackDepth::minimal)> ; // stek dly zadachi BluetoothTask

class BluetoothTask : public OsWrapper::Thread
{
private:
  tMailBox& box; //silka na klass mailbox
  Uart& uart; //silka na klass uart
  tU32 TimeToTransmeet = tU32(1000U); //vremy cherez kotoroe sover peredacha
  tF32 Data; // hranit znachenie poluchenoe iz mailbox
public:
  tBluetoothTaskStack Stack;
  
  BluetoothTask(tMailBox& mail, Uart& usart): box(mail), uart(usart) {}; //konstruktor - sozdaet ob'ekt klassa i prisvaivat emu ssilki na mailbox i uart
  
  virtual void Execute() override
  {
   for(;;)
   {
     if (box.Get(Data, TimeToTransmeet))
     {
       tU32 data = tU32(round(Data)); 
       char* rezult = BluetoothFormat::TransformToString(data); //vizivaet funkciy BluetoothFormat i poluchaet iz nee stroku
       uart.SendData(rezult, strlen(rezult)); //otdaet znachenie v uart
     }
   }
  }
} ;
#endif 