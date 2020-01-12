/*
 * time.c
 *
 * Created: October 31, 2019, 12:33:01 PM
 *  Author: K. C. Lee
 * Copyright (c) 2019 by K. C. Lee
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.

 If not, see http://www.gnu.org/licenses/gpl-3.0.en.html 
 */ 
#include "hardware.h"

volatile uint8_t time_flag,ticks;
volatile time_hms_t time;
volatile nv_setting_t Setting;
uint32_t DDS_PhaseInc;
volatile uint32_t DDS_Accum;

void Time_Init(void)
{	// prescaler
	TIM1->PSCRH = TIM1_PSCR_H;
	TIM1->PSCRL = TIM1_PSCR_L;
	
	// Preload, Counter enable
	
	// use DDS for clock fine adjustments
	DDS_PhaseInc = DDS_INC + DDS_Adj;
	
	// Autoreload
	TIM1->ARRH = (TIM1_RELOAD-1) >> 8;
	TIM1->ARRL = (TIM1_RELOAD-1) & 0xff;

	TIM1->CR1 = TIM1_CR1_ARPE|TIM1_CR1_CEN;
	// Update Interrupt
	TIM1->IER = TIM1_IER_UIE;
}

// RTC

@far @interrupt void TIM1_IRQ(void)
{
	uint8_t carry;
	
	// clear UIF
	TIM1->SR1 &= ~TIM1_SR1_UIF;
	
	// tick comes from Direct Digital Synthesis
	DDS_Accum += DDS_PhaseInc;
	
	if (DDS_Accum & DDS_CARRY)
	{
		if(ticks)
		{	
			// blink decimal point
			if(ticks==(TICKS_PER_SEC/2))
				time_flag |= TIME_HALF_SEC;
	
			ticks--;
		}
		else
		{
			ticks = TICKS_PER_SEC-1;
	
			time.sec++;
			
			if(time.sec > 59)
			{
				time.sec = 0;
				time.min++;
			
				if(time.min > 59)
				{
					time.min = 0;			
					time.hour++;
				}
				if(time.hour > TIME_HR_MAX)
					time.hour = TIME_HR_MIN;
			}
			
			time_flag |= TIME_SEC_FLAG|TIME_FULL_SEC;
		}	
	}
	
	DDS_Accum &= DDS_MASK;
	time_flag |= TIME_TICK;
}

void SetTime(uint8_t Hour, uint8_t Min, uint8_t Sec)
{
	sim();
	time.hour =	Hour;
	time.min =	Min;	
	time.sec =	Sec;
	ticks = TICKS_PER_SEC-1;
	rim();
}