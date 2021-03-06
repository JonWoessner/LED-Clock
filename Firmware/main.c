/*
 * main.c
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

void main(void)
{	
	// load Setting
	//	Load_Prefs(&Setting);
	
	time.DST_Enable = 1;
	
	Init_Hardware();

	while(1)
	{
		if(!debugger)
			wfi();				// save some extra power - doesn't work with debugger
			
		if((time.Tick)&& power)
		{
			time.Tick=0;
			ClockApp();
		}
	}
}
