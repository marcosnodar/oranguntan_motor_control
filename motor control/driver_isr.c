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
int angular_speed1_rad_per_s = 0;
int timer_freq = 20000000/1024;

int wrap=0;
float P;
float I;
float D;
uint32_t setpoint=0;
float target_angular_speed1_rad_per_s = 1 * 2*3.14;
int error=0, error_old=0;
int  error_sum=0, error_d=0;
int  control_i=0, control_d=0, control_p=0;
uint32_t timer_diff = 0;
uint32_t target_timer_diff=0; 
uint8_t str[256];
				ISR(PCINT1_vect)
				{
					
						P=0.10/256;
						I=1./256;
						D=0;
						PD1_set_level(PC0_get_level());
					//1 timer tick = 51.2us
					target_timer_diff = (int) 15332./target_angular_speed1_rad_per_s;
					sprintf(str, "target diff = %d\n\r", target_timer_diff);
					USART0_put_string(str);
					timer_new =  TCNT1 & 0xFFL;
					timer_new |= (TCNT1H<<8) & 0xFF00;
					
					timer_diff = timer_new - timer_old;
					
				
					timer_old = timer_new;

					/* Insert your pin change 1 interrupt handling code here */

					error = target_timer_diff - timer_diff;
					error_sum = error_sum + I* error;
	
					sprintf(str, "diff = %d\n\r", timer_diff);
					USART0_put_string(str);
					sprintf(str, "error = %d\n\r", error);
					USART0_put_string(str);	
					if(error > 255)
						setpoint = setpoint - error/256;
					else 
						setpoint =setpoint - error;
					sprintf(str, "setpoint = %d\n\r", setpoint);
					USART0_put_string(str);
					TIMER_0_set_comp_a(setpoint);
					TIMER_0_set_comp_b(0);

				}
				ISR(TIMER1_OVF_vect)
				{
					// adjust all timer values	
					wrap ++;					
					sprintf(str, "***************************wrap = %d\n\r", wrap);
					USART0_put_string(str);
				//	PD1_set_level(PC0_get_level());
				//PD1_set_level(wrap%2);
				}
