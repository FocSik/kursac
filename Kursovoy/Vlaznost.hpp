#ifndef VLAZNOST_HPP
#define VLAZNOST_HPP

#include "stm32f411xe.h" //Registers definition
#include "susudefs.hpp" //For tF32
#include "Adc.hpp" //For definition of class Adc
#include "Filter.hpp"//For definition of class Filter
#include <cmath> //For pow

class Vlaznost
{

private:
  
  static constexpr tF32 c0 = +1.0f;
  static constexpr tF32 c1 = +1.0f;  
  static constexpr tF32 c2 = +1.0f;
  static constexpr tF32 c3 = +1.0f;


  Adc& adc; //ccilka na class adc
  Filter& filter; //ccilka na class filter
  
  tF32 Measure() const //polushenie znachenia calculate
  {
   adc.StartADC(); //vizivaiet start ADC
   while (!adc.IsReady()) //start ADC
   {
   }
   return adc.GetCode(); //vozvrachaet cod ADC
   }
  
  tF32 Calculate(tF32 AdcCode) const //poluchaem cod c ADC
  {
  return (c0*pow(AdcCode, 0)+c1*pow(AdcCode, 1)+c2*pow(AdcCode, 2)+c3*pow(AdcCode, 3)); //preobrazuet cod ADC v znachenie vlaznosti
  }
  
public:
  Vlaznost(Adc& Acp, Filter& Filt): adc(Acp), filter(Filt) {}; //Consctructor - cozdaiot class ADC i Filter
  
  tF32 GetValue() const //poluchenie preobrazovannogo znachenia
  {
  tF32 code = static_cast<tF32>(Measure()); //vizivaet Maesure
  tF32 Value = Calculate(code); //pricvaivaet znachenie Calculate k Value
  tF32 Result = filter.Update(Value); //pricvaivaet znachenie filter.Update k Value
  return Result; //zapusk filtra
  }
};

#endif