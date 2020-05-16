#ifndef FILTER_HPP
#define FILTER_HPP

#include "stm32f411xe.h" //Registers definition
#include "susudefs.hpp" //For tU32
#include <cmath> //For exp

class Filter 
{
private:
  static constexpr tF32 RC = tF32(1000.0F); //postoiannaia vremeni
  static constexpr tF32 dt = tF32(100.0F); //chag discretizasii
  const tF32 tau = tF32(1.0F) - tF32(exp(-dt/RC)); //peremennaia, zavisachaa ot RC i dt
  tF32 OldFilterValue = tF32(0.0F); //staroe otfiltrovanoe znachenie
  

  
public:
  tF32 Update(tF32 value) //filtracia vlaznosti
  {
   tF32 FilterValue = OldFilterValue+(value-OldFilterValue)*tau; //vicheclenie poluchennogo znachenia
   OldFilterValue = FilterValue; //zapisivaet tecuchee znachenie dla dalneichei raboti
   return (FilterValue); //konets
   
  }
};
#endif