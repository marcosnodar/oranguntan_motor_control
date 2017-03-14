#include <atmel_start.h>
#define F_CPU 20000000    // AVR clock frequency in Hz, used by util/delay.h
#include <avr/io.h>
#include <util/delay.h>
#include <usart.h>
#include <motor_control.h>

#define DBG

extern uint32_t i2c_data_ready;
extern uint8_t i2c_reg;
extern uint8_t i2c_data;

float target_angular_speed1_rad_per_s = 0 * 2*3.14;

int main(void)
{

		i2c_data_ready = 0;
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

		uint8_t pwm;
		int i;

		init_pins();
		init_interrupt_mask();

		set_direction(RIGHT, FORWARD);
		set_direction(LEFT, FORWARD);
		
		init_pid(65535, 100 ,50,20);
	
		//1 timer tick = 51.2us
	
		init_regmap();
	
		DEBUG_PRINT("target_timer_diff0 %lu, K = %ld, P = %d, I = %d, D = %d,  \t \n\r", target_timer_diff[0], K, P, I, d);
		init_counters();
		int_stat = PINC & 0xF;
		cpu_irq_enable();
	
		//TIMER_2_set_comp_a(100);
		//TIMER_0_set_comp_a(100);
		//TIMER_0_set_comp_b(100);
		//TIMER_2_set_comp_b(100);
			
		DEBUG_PRINT("Main loop\n", NULL);

		while(1) {
			//_delay_ms(1);

			if(i2c_data_ready != 0)
			{
				for(i=0; i<4; i++)
				{
					if(register_map[i+1] == 0)
						target_timer_diff[i] = 0;
					else
						target_timer_diff[i] = (uint32_t) (2*3.14*15330./register_map[i+1]);
					i2c_data_ready &= ~(1<<(i+1));	
					DEBUG_PRINT("Updated register %d with %ld\n", i, target_timer_diff[i])
				}
			}
			for(i=0; i<4; i++){	

				if(update_pwm[i] != 0){
					update_pwm[i]=0;
					pwm = compute_new_pwm(timer_value[i], i);
					set_pmw_output(pwm, i);

				}

		}
	}
	
}