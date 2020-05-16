#ifndef VARIABLETASK_HPP
#define VARIABLETASK_HPP 

//For OsWrapper::Thread
#include "thread.hpp" //For definition of class Thread
#include <array> //For std::array
#include "mailbox.hpp" //For definition of class MailBox
#include "event.hpp" //For definition of class event
#include "stm32f411xe.h" //Registers definition
#include "susudefs.hpp" //For tU32
#include "Adc.hpp" //For definition of class Adc
#include "Vlaznost.hpp" //For definition of class Variale
#include "Filter.hpp"//For definition of class Filter

using namespace::OsWrapper ;

//pointer to MailBox in spase OsWrapper
using tMailBox = OsWrapper::MailBox<tF32, 2> ;


//Psevdlonim for task VarialeTask 
using tVarialeTaskStack = std::array<OsWrapper::tStack, static_cast<tU16>(OsWrapper::StackDepth::minimal)> ;

class VarialeTask : public OsWrapper::Thread
{
private:
  
  Variale hum;
  tMailBox& box; //ccilka na ob"ekt classa MailBox
  Event& event; //ccilka na ob"ekt classa Event
  tU32 Count = tU32(0U); //chetcik poluchennih rezultatov vlaznosti
  static constexpr tU32 TimeToMeasure = tU32(100U); //vremia cherz kotoroe coverchaetca izmerenie
  static constexpr tU32 TimeToSendData = tU32(1000U); //vremia cherz kotoroe coverchaetca peredatcha
  
public:
 
  tVarialeTaskStack Stack;
  VarialeTask(tMailBox& mail,OsWrapper::Event& ev, Adc& Acp, Filter& Filt): box(mail), event(ev), hum(Acp, Filt) {}; constructor VarialeTask
  virtual void Execute() override
  {
    using OsWrapper::operator""ms ; 
 
   for(;;) 
   {
    tF32 result = Value(); //prisvaivaem result znachenie Value iz vlaznosti
    Count++; //yvelichivaet znachenie count on 1
    if (Count==(TimeToSendData/TimeToMeasure)) //sikl dla zapisi v mailbox
    {
      box.Put(result); //vivesti rezultat v mailbox
      Count = tU32(0U); //vivesti rezultat
    }
    event.Signal(); //zapis signala v event
    Sleep(100); //zaderzka 
   }
  }
    
};

#endif 