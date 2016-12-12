#include <atmel_start.h>
#define F_CPU 20000000    // AVR clock frequency in Hz, used by util/delay.h
#include <avr/io.h>
#include <util/delay.h>

int motor1_steps;
#define RPM  60 * 8 * 10;

void set_speed(int rpm)
{
	
		TIMER_0_set_comp_a(rpm);
	
}

int main(void)
{
	motor1_steps = 0;
	atmel_start_init();
	int target_rpm = 60;
	int control = 0;
	int target_steps_per_100ms = 0;
	int P=0;
	int I=0;
	int D=0;
	int error=0;
	target_steps_per_100ms = target_rpm / (60*8*0.1);
	/* Initializes MCU, drivers and middleware */
	PC0_set_dir(PORT_DIR_IN);
	PC0_set_pull_mode(PORT_PULL_UP);

	PB3_set_dir(PORT_DIR_OUT);
	PD3_set_dir(PORT_DIR_OUT);
	PD1_set_dir(PORT_DIR_OUT);
	PB3_set_dir(PORT_DIR_OUT);
	PD3_set_dir(PORT_DIR_OUT);
	cpu_irq_enable();
	/* Replace with your application code */
	P=1;
	//steps per 100ms = 8 at 1rpm
	while(1) {
		error = target_steps_per_100ms - motor1_steps;
		control = control + P*error;
		TIMER_0_set_comp_a(control);
		motor1_steps = 0;
		_delay_ms(100);
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