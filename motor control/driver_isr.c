/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include <atmel_start.h>
#include <compiler.h>
#include <motor_control.h>
#include <util/delay.h>


#define MAX_I 255
#define MIN_I -255
#define MAX_OUT 255
#define MIN_OUT 0
#define MAX_ERR 255
#define MIN_ERR -255

int32_t angular_speed1_rad_per_s = 0;
int32_t timer_freq = 20000000/1024;

uint32_t wrap[4];
volatile uint8_t update_pwm[4];

extern int16_t P;
extern int16_t I;
extern int16_t d;
extern int32_t K;
uint32_t timer_value[4];
uint32_t timer_old[4];

int32_t  error_old[4];
int32_t  error_sum[4], error_d[4];

uint32_t target_timer_diff[4];
uint32_t old_timer_diff[4];
uint8_t str[256];

uint32_t it = 0;
uint32_t get_timer()
{
		uint32_t timer;
		timer =  TCNT1 & 0xFFL;
		timer |= (TCNT1H << 8) & 0xFF00;
		return timer;
}

uint32_t timer_diff[4];

uint8_t compute_new_pwm(uint32_t timer_new, uint8_t index)
{
	
	int32_t coef_P = 0, coef_I = 0, coef_D = 0;
	int32_t error;
					int32_t output = 0;
					
					cpu_irq_disable();
					timer_diff[index] = timer_new + wrap[index] * 65536 - timer_old[index];
					if(wrap[index]){
						wrap[index] = 0;
					}
					cpu_irq_enable();


					error =  timer_diff[index] - target_timer_diff[index];
					
					if(P)
					{
						coef_P = P * error / K;
						if(coef_P > MAX_ERR){
							coef_P = MAX_ERR;
							DEBUG_PRINT("MAX P %ld\r\n", coef_P);
						}
						else if(coef_P < MIN_ERR){
							coef_P = MIN_ERR;
							DEBUG_PRINT("MIN P %ld\r\n", coef_P);
						}
					}

					if (I)
					{
						error_sum[index] = error_sum[index] +  error ;
						//DEBUG_PRINT("error = %ld\n\r", error);
					/*	if(error_sum > INT16_MAX)
							error_sum = INT16_MAX;
						else if(error_sum < INT16_MIN)
							error_sum = INT16_MIN;
						*/	
						coef_I = I * error_sum[index] / K;
						
						if(coef_I > MAX_I)
								coef_I = MAX_I;
						else if(coef_I < MIN_I)
								coef_I = MIN_I;
								
					}

					
					if(d)
					{
						error_d[index] =   (timer_diff[index] - old_timer_diff[index]);
						coef_D = d * error_d[index] / K;
						if(coef_D > MAX_I)
								coef_D = MAX_I;
						else if(coef_D < MIN_I)
								coef_D = MIN_I;
						
					}

					output =  coef_P + coef_I + coef_D;
					
					if(output > MAX_OUT){
						output = MAX_OUT;

					}
					else if(output < MIN_OUT){	
						output = MIN_OUT;
					
					}
					DEBUG_PRINT(" Motor %d P %ld I %ld , D %ld Out %u\n\r", index, coef_P, coef_I, coef_D, (uint8_t) output);
					old_timer_diff[index] = timer_diff[index];
					timer_old[index] = timer_new;		

				return (uint8_t) (output);
	
}


uint8_t int_stat;


				ISR(PCINT1_vect)
				{
					uint8_t index = 0;
					uint8_t sensor_stat;
					uint8_t pin_change;
					cpu_irq_disable();
					sensor_stat = PINC & 0xF; 
					pin_change = sensor_stat ^ int_stat;
					for(index=0; index<4; index++){
						if((pin_change & (1<<index))==(1<<index)){
							//DEBUG_PRINT("stat %x Index %d\n\r", sensor_stat, index);
							timer_value[index] = get_timer();
							update_pwm[index] = 1;
							//int_stat ^= 1<<index;
						}
					}
					int_stat = sensor_stat; 
					cpu_irq_enable();
					it++;
					
					
				}
				
				ISR(TIMER1_OVF_vect)
				{	
					cpu_irq_disable();
					uint8_t index = 0;
					for(index = 0; index < 4; index++)
					{
						wrap[index]++;
					}
					cpu_irq_enable();					
				}
