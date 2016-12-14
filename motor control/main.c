#include <atmel_start.h>
#define F_CPU 20000000    // AVR clock frequency in Hz, used by util/delay.h
#include <avr/io.h>
#include <util/delay.h>
#include <usart.h>
int motor1_steps;
#define RPM  60 * 8 * 10;


void ms_delay(int ms)
{
	int tmp;
	tmp = F_CPU / 1000 * ms;
	while(tmp--);
	
	
}

int main(void)
{
	motor1_steps = 0;
	
		atmel_start_init();
		
		int target_rps = 1;
		int control = 0;
		int target_steps_per_period = 8;
		int period = 125; // period in ms

		int motor_steps_old;
	extern int target_timer_diff ;
	extern int target_angular_speed1_rad_per_s;
		/* Initializes MCU, drivers and middleware */

		PC0_set_dir(PORT_DIR_IN);
		PC0_set_pull_mode(PORT_PULL_UP);

		PB3_set_dir(PORT_DIR_OUT);
		PD3_set_dir(PORT_DIR_OUT);
		PD1_set_dir(PORT_DIR_OUT);
		PB3_set_dir(PORT_DIR_OUT);
		PD3_set_dir(PORT_DIR_OUT);
		PD3_set_level(false);
		cpu_irq_enable();
		/* Replace with your application code */
		
	
// find best period / target steps per period depending on target rpm

		target_rps = 1;
		period = 1000;
		// 8 steps =  1 rotation
		//while(1){
			//PD1_set_level(true);
			//_delay_ms(1000);
					//PD1_set_level(false);
					//_delay_ms(1000);	
		//}
		target_timer_diff = (int) 153300./target_angular_speed1_rad_per_s;
		//target_steps_per_period = 2;
		while(1) {
			//error = control_p;
			//control_p = target_angular_speed1_rad_per_s - angular_speed1_rad_per_s; // need to handle overflows + how to converge more quickly
			//control_i = control_i + control_p;
			//control_d = control_p - error;
			//TIMER_0_set_comp_a(I*control_i + P*control_p + D*control_d);
			////TIMER_0_set_comp_a(0xFF);
			_delay_ms(1000);
			//USART0_put_string("test\n\r");
			//// try to have steps per period between 3 and 5
			//target_steps_per_period = target_rps *8 * period / 1000;
			//while(target_steps_per_period < 3)
			//{
				//period = period + 125;
				//target_steps_per_period = target_rps *8 * period / 1000;
			//}
			//
			//while(target_steps_per_period > 5 )
			//{
				//period = period - 125;
				//target_steps_per_period = target_rps *8 * period / 1000;
			//}
	//
			//error = target_steps_per_period - (motor1_steps - motor_steps_old); // need to handle overflows + how to converge more quickly
			//control_i = control_i + error;
//
			//TIMER_0_set_comp_a(I*control_i + P*error );
			//motor_steps_old = motor1_steps;
			//ms_delay(period);
		}
	
}
		//PD5_set_level(true);
		//PD6_set_level(false);
		//TIMER_0_set_comp_a(0x80+speed++);
		//PD1_set_level(true);
		//_delay_ms(2000);

		//PD5_set_level(false);
		//PD6_set_level(true);
		
		//PD1_set_level(false);
		//_delay_ms(2000);
		
		
		