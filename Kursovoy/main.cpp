#include "rtos.hpp" //For real time operating system
#include "Adc.hpp" //For definition of class Adc
#include "DMA.hpp" //For definition of class DMA
#include "mailbox.hpp" //For definition of class MailBox
#include "Vlaznost".hpp" //For definition of class Humidity
#include "VariableTask.hpp" //For definition of class HumidityTask
#include "BluetoothTask.hpp" //For definition of class BluetoothTask
#include "Led.hpp" //For definition of class LedTask
#include "Filter.hpp"//For definition of class Filter
#include "thread.hpp" //For definition of class Thread
#include "USART.hpp" //For definition of class Uart


uint32_t SystemCoreClock = 16'000'000U;


extern "C" 
{
  int __low_level_init(void) {
  //Switch on external 16 MHz oscillator
  RCC->CR |= RCC_CR_HSION;
 while ((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY) 
 {
 }
  //Switch system clock on external oscillator
  RCC->CFGR |= RCC_CFGR_SW_HSI;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) 
  {
 }
  //Switch on clock on PortA and PortC
  RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN);
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN ;
  
  //LED1 on PortA.5, set PortA.5 as output
  GPIOA->MODER |= GPIO_MODER_MODE5_0;
  
  //LED2 on PortC.9, LED3 on PortC.8, LED4 on PortC.5 so set PortC.5,8,9 as output
  GPIOC->MODER |= (GPIO_MODER_MODE9_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE8_0); 
  
  //PortC.4 analog input
  GPIOC->MODER |= (GPIO_MODER_MODE4_0 | GPIO_MODER_MODE4_1);
  
  //Switch on clock on ADC
  RCC->APB2ENR|= RCC_APB2ENR_ADC1EN;
  
  //Switch on ADC
  ADC1->CR2|= ADC_CR2_ADON;
  
  //Switch on clock on USART
  RCC->APB1ENR|= RCC_APB1ENR_USART2EN;

  //System clock 
  constexpr unsigned long Frequence = 16000000;

  //Speed of transmet 
  constexpr unsigned long BaudRate = 9600;

  // Calculation of Mantissa and Fraction to register BRR
  constexpr unsigned long DaudRate9600Mantissa = (Frequence/(BaudRate*8*(2-0))) << 4 ;
  constexpr unsigned long DaudRate9600Fraction = 16*100*(Frequence % (BaudRate*8*(2-0))) / Frequence;
  constexpr unsigned long DaudRate9600 = DaudRate9600Mantissa + DaudRate9600Fraction ;
 
  //UART
  //PortA.2 and PortA.3 -- alternative function
  GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);
  
  GPIOA->MODER &= ~GPIO_MODER_MODER2_0; 
  GPIOA->MODER &= ~GPIO_MODER_MODER3_0; 

  // push pull
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT2; 
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;
  
  // alternative function pull-up
  GPIOA->PUPDR |= GPIO_PUPDR_PUPDR2_0;
  GPIOA->PUPDR |= GPIO_PUPDR_PUPDR3_0;
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2_1;
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD3_1;
 
  // stop bit, start bit, 8 data bits
  USART2->CR1 &=~USART_CR1_M; 
 
  // frequency of sampling
  USART2->CR1 &=~USART_CR1_OVER8; 
    
  //Alternate function choise 
  GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2_2) | (GPIO_AFRL_AFSEL2_1) | (GPIO_AFRL_AFSEL2_0);
  GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL3_2) | (GPIO_AFRL_AFSEL3_1) | (GPIO_AFRL_AFSEL3_0);
  GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_3;
  GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3_3;
  
  //Set speed of transmet
  USART2->BRR |= DaudRate9600;
  
  //razreshenie na peredachu
  //USART2->CR1 |= USART_CR1_TE;


  return 1;
}
}
  Adc adc;
  tMailBox box;
  Filter filter;
  Uart uart;
  using OsWrapper::operator""ms ;
  OsWrapper::Event event{1000ms, 1};
  HumidityTask humidityTask(box, event, adc, filter);
  BluetoothTask bluetoothTask(box, uart);
  LedTask ledTask(event);
  
  

int main()
{

  using namespace OsWrapper ;
  Rtos::CreateThread(humidityTask, humidityTask.Stack.data(), "humidityTask", ThreadPriority::lowest, humidityTask.Stack.size()) ;
  Rtos::CreateThread(bluetoothTask, bluetoothTask.Stack.data(), "bluetoothTask", ThreadPriority::lowest, bluetoothTask.Stack.size()) ;
  Rtos::CreateThread(ledTask, ledTask.Stack.data(), "ledTask", ThreadPriority::lowest, ledTask.Stack.size()) ;
  Rtos::Start() ;
  
  return 0;
}
