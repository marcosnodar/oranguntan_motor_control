#include <atmel_start.h>
#define F_CPU 20000000    // AVR clock frequency in Hz, used by util/delay.h
#include <avr/io.h>
#include <util/delay.h>
#include <usart.h>
#include <motor_control.h>
int motor1_steps;
#define RPM  60 * 8 * 10;
int16_t P;
int16_t I;
int16_t d;
int32_t K;
extern 	uint32_t timer_value[4];
extern uint32_t timer_diff[4];
extern uint32_t target_timer_diff[4];
extern volatile uint8_t update_pwm[4];
extern uint32_t it;
#define DBG

void init_pid(int32_t k, int16_t prop, int16_t integ, int16_t diff)
{	
		K = k;
		P = prop;
		I = integ;
		d = diff;
	
		return;
	
}

extern uint32_t timer_diff[4];
extern uint32_t timer_old[4];
extern uint8_t str[256];
extern uint32_t old_timer_diff[4];
extern uint32_t error_sum[4];
extern uint32_t error_old[4];
extern uint8_t int_stat;

void set_pmw_output(uint8_t pwm, uint8_t index)
{

	switch(index){
		case 0:
			TIMER_2_set_comp_b(pwm);
			break;
		
		case 1:
			TIMER_0_set_comp_a(pwm);
			break;
		
		case 2:
			TIMER_0_set_comp_b(pwm);
			break;
		
		case 3:
			TIMER_2_set_comp_a(pwm);
			break;
			
		default:
			DEBUG_PRINT("no associated PWM %d", index);
			break;
		
		}

}

void set_direction(uint8_t side, uint8_t dir)
{
	if(side == LEFT)
	{
		switch(dir){
			case FORWARD:
				PB0_set_level(true);
				PB1_set_level(false);
				break;
			case REVERSE:
				PB0_set_level(false);
				PB1_set_level(true);
				break;
			default:
				PB0_set_level(false);
				PB1_set_level(false);
				break;
		}
		
	}
	if(side == RIGHT)
	{
			switch(dir){
				case REVERSE:
					PD4_set_level(true);
					PD7_set_level(false);
					break;	
				case FORWARD:
					PD4_set_level(false);
					PD7_set_level(true);
					break;
				default:
					PD4_set_level(false);
					PD7_set_level(false);
					break;
				}
		
	}
	
	
}


int main(void)
{
	motor1_steps = 0;
	
		atmel_start_init();
	
		/* 
	
												---------------------
				Interrupt PC0				[]	|					|	[]	Interrupt PC3
				Bridge Motor A				[]	|					|	[]	Bridge motor B
				Timer 0 output A			[]	|					|	[]	Timer 0 output B
												|					|
												|					|
												|					|
												|					|
												|					|
				Interrupt		PC1			[]	|					|	[]	Interrupt PC2
				BB orangutan Motor 1		[]	|		  X 		|	[]	BB orangutan Motor 2
				Timer2 output A				[]	|		 XXX		|	[]	Timer2 output B
												|		  X			|
												VVVVVVVVVVVVVVVVVVVVV 	
													
		*/
		
		uint16_t j;
		float target_angular_speed1_rad_per_s = 1 * 2*3.14;
		/* Initializes MCU, drivers and middleware */

		uint8_t pwm;
		int i;

	/* set L/R direction controls */
	
	_delay_ms(3000);
	
	
		PB0_set_dir(PORT_DIR_OUT);
		PB0_set_level(true);
		PB1_set_dir(PORT_DIR_OUT);
		PB1_set_level(false);
		PD4_set_dir(PORT_DIR_OUT);
		PD4_set_dir(true);
		PD7_set_dir(PORT_DIR_OUT);
		PD7_set_level(false);
		

		
		
	/* set sensor interrupt pins */
		PC0_set_dir(PORT_DIR_IN);
		PC0_set_pull_mode(PORT_PULL_UP);
		PC1_set_dir(PORT_DIR_IN);
		PC1_set_pull_mode(PORT_PULL_UP);
		PC2_set_dir(PORT_DIR_IN);
		PC2_set_pull_mode(PORT_PULL_UP);
		PC3_set_dir(PORT_DIR_IN);
		PC3_set_pull_mode(PORT_PULL_UP);
		
		PCMSK1 &= ~0xF;
		PCMSK1 |= 1 << PCINT8;
		PCMSK1 |= 1 << PCINT9;
		PCMSK1 |= 1 << PCINT10;
		PCMSK1 |= 1 << PCINT11;
		
		PB3_set_dir(PORT_DIR_OUT);
		PB3_set_level(false);
		PD3_set_dir(PORT_DIR_OUT);
		PD3_set_level(false);
		PD5_set_dir(PORT_DIR_OUT);
		PD5_set_level(false);
		PD6_set_dir(PORT_DIR_OUT);
		PD6_set_level(false);

		set_direction(RIGHT, FORWARD);
		set_direction(LEFT, FORWARD);
		
		init_pid(65535, 100 ,50,20);
	
		/* Replace with your application code */
		
	
		// 8 steps =  1 rotation
		//1 timer tick = 51.2us
		

		_delay_ms(3000);
	
		for(i=0; i<4; i++){

			timer_old[i] = 0;
			target_timer_diff[i] =  (uint32_t) (15330./target_angular_speed1_rad_per_s);
			old_timer_diff[i] = 0;
			error_sum[i] = 0;
			error_old[i] = 0;
			update_pwm[i] = 1;
			timer_value[i] = get_timer();
			//set_pmw_output(100, i);
			
		}

		DEBUG_PRINT("target_timer_diff0 %lu, K = %ld, P = %d, I = %d, D = %d,  \t \n\r", target_timer_diff[0], K, P, I, d);

		int_stat = PINC & 0xF;
		cpu_irq_enable();
		j=0;
	
			//TIMER_2_set_comp_a(100);
			//TIMER_0_set_comp_a(100);
			//TIMER_0_set_comp_b(100);
			//TIMER_2_set_comp_b(100);
			
		
		while(1) {
			//_delay_ms(1);
			j++;
			for(i=0; i<4; i++){	

				if(update_pwm[i] != 0){
					update_pwm[i]=0;
					pwm = compute_new_pwm(timer_value[i], i);
					set_pmw_output(pwm, i);

					if(j%100==0){
						DEBUG_PRINT("pwm = %u\t it = %lu\n\r", pwm, it);
					}
				}

			}
		
	
			
		}
	
}

		
		
		