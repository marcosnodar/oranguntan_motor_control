#include <motor_control.h>

#define MAX_I 255
#define MIN_I -255
#define MAX_OUT 255
#define MIN_OUT 0
#define MAX_ERR 255
#define MIN_ERR -255

uint8_t str[256];
uint8_t int_stat;
volatile uint8_t update_pwm[4];

void init_pins(void){
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
	
	/* initialize pull ups on I2C */
	PC4_set_pull_mode(PORT_PULL_UP);
	PC5_set_pull_mode(PORT_PULL_UP);
	PC4_set_dir(PORT_DIR_IN);
	PC5_set_dir(PORT_DIR_IN);
	
	/* initialize pull ups on UART */
	PD0_set_pull_mode(PORT_PULL_UP);
	PD1_set_pull_mode(PORT_PULL_UP);
	
	PB3_set_dir(PORT_DIR_OUT);
	PB3_set_level(false);
	PD3_set_dir(PORT_DIR_OUT);
	PD3_set_level(false);
	PD5_set_dir(PORT_DIR_OUT);
	PD5_set_level(false);
	PD6_set_dir(PORT_DIR_OUT);
	PD6_set_level(false);
}

void init_interrupt_mask(void)
{
	PCMSK1 &= ~0xF;
	PCMSK1 |= 1 << PCINT8;
	PCMSK1 |= 1 << PCINT9;
	PCMSK1 |= 1 << PCINT10;
	PCMSK1 |= 1 << PCINT11;
}

void set_pmw_output(uint8_t pwm, uint8_t index)
{
	if(pwm==0)
	{
		DEBUG_PRINT("PWM of motor %d is off\n", index);
	}
	
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

int16_t P;
int16_t I;
int16_t d;
int32_t K;

void init_pid(int32_t k, int16_t prop, int16_t integ, int16_t diff)
{
	K = k;
	P = prop;
	I = integ;
	d = diff;
	
	return;
	
}

uint32_t get_timer()
{
	uint32_t timer;
	timer =  TCNT1 & 0xFFL;
	timer |= (TCNT1H << 8) & 0xFF00;
	return timer;
}


uint32_t timer_value[4];
uint32_t timer_old[4];

int32_t  error_old[4];
int32_t  error_sum[4], error_d[4];

uint32_t target_timer_diff[4];
uint32_t old_timer_diff[4];
uint32_t wrap[4];
uint32_t timer_diff[4];

uint8_t compute_new_pwm(uint32_t timer_new, uint8_t index)
{
	
	int32_t coef_P = 0, coef_I = 0, coef_D = 0;
	int32_t error;
	int32_t output = 0;
					
	if(target_timer_diff[index] == 0){
		DEBUG_PRINT("Compute PWM of motor %d = 0\n", index);
		return 0;
	}

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
		if(coef_P > MAX_ERR)
		{
			coef_P = MAX_ERR;
			DEBUG_PRINT("MAX P %ld\n", coef_P);
		}
		else if(coef_P < MIN_ERR)
		{
			coef_P = MIN_ERR;
			DEBUG_PRINT("MIN P %ld\n", coef_P);
		}
	}
	
	if (I)
	{
		error_sum[index] = error_sum[index] +  error ;
		//DEBUG_PRINT("error = %ld\n", error);
		/*
		if(error_sum > INT16_MAX)
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
					
		if(output > MAX_OUT)
		{
			output = MAX_OUT;
		}
		else if(output < MIN_OUT)
		{	
			output = MIN_OUT;
		}
		DEBUG_PRINT(" Motor %d P %ld I %ld , D %ld Out %u\n", index, coef_P, coef_I, coef_D, (uint8_t) output);
		old_timer_diff[index] = timer_diff[index];
		timer_old[index] = timer_new;		

		return (uint8_t) (output);
	
}
void init_counters(void){
	int i;
	
	for(i=0; i<4; i++){
		timer_old[i] = 0;
		//target_timer_diff[i] =  (uint32_t) (15330./target_angular_speed1_rad_per_s);
		target_timer_diff[i] = 0;
		old_timer_diff[i] = 0;
		error_sum[i] = 0;
		error_old[i] = 0;
		update_pwm[i] = 1;
		timer_value[i] = get_timer();

	}
}

void init_regmap(void)
{
	int i;
	
	for (i=0; i< 32; i++)
	{
		register_map[i] = 0;
	}
	
	register_map[ID] = 0x01;	

}