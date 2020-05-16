#ifndef USART_HPP
#define USART_HPP

#include "stm32f411xe.h" //Registers definition
#include "susudefs.hpp" //For tU8

class Usart
{
public:
  Usart()
  {
    USART2->CR1 |= USART_CR1_UE; // konstruktor - vkluchaet modul' usart
  }
 
  bool IsBusy()
  {
    return (((USART2->SR)&(USART_SR_TXE))==0) ; // proveryet zapisano li cto to v registr peredaci dannih
  }
 
  void SendData(char* String, tU8 Size)
  {
       //Enable Transmit  
    USART2->CR1 |= USART_CR1_TE; 
    for (tU8 i = tU8(0U); i < Size; i++)
    {
      while(IsBusy())
      {
      }  
     // USART2->SR &=~ USART_SR_TC; 
      USART2->DR = String[i];      
    }
    //Disable Transmit
    USART2->CR1 &=~ USART_CR1_TE;
  }
};