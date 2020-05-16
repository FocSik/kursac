#ifndef LED_HPP
#define LED_HPP

//For OsWrapper::Thread
#include "thread.hpp" //For definition of class Thread
#include "event.hpp" //For definition of class Event
#include "stm32f411xe.h" //Registers definition
#include "susudefs.hpp" //For tU32, tF32
#include <array> //For std::array
#include <cstring> //For strlen

using namespace::OsWrapper ;

//Psevdlonim for task tLedTaskStack 
using tLedStack = std::array<OsWrapper::tStack, static_cast<tU16>(OsWrapper::StackDepth::minimal)> ;

class Led : public OsWrapper::Thread
{
private:
  static constexpr tU32 TimeToChageState = tU32(100U); //vremy izmememiy sostoyniy
  Event& event;
  
public:
  tLedStack Stack;
  Led(Event& ev): event(ev) {}; //konstruktor - sozdaet ob'ekt klassa i prisvaivat emu ssilki na klass event
  virtual void Execute() override // pri poluchenii sobitiy menyet sostoynie svetodioda
  {
   for(;;)
   {
     if (event.Wait()) // vkluchenie portov svetodiodov
     {
       GPIOA->ODR ^= GPIO_ODR_OD5;
       GPIOC->ODR ^= GPIO_ODR_OD5;
       GPIOC->ODR ^= GPIO_ODR_OD8;
       GPIOC->ODR ^= GPIO_ODR_OD9;
     }
    }
  }
};
#endif 