/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include <atmel_start.h>
#include <compiler.h>
extern int motor1_steps;
int timer_old=0;
int timer_new = 0;
int angular_speed_rad_per_s = 0;

				ISR(PCINT1_vect)
				{
					PD1_set_level(PC0_get_level());
					motor1_steps ++;
					timer_old = timer;
					timer=
					
					
					/* Insert your pin change 1 interrupt handling code here */

				}

