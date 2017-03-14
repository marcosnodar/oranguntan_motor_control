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
			//DEBUG_PRINT("stat %x Index %d\n", sensor_stat, index);
			timer_value[index] = get_timer();
			update_pwm[index] = 1;
		}
	}
	int_stat = sensor_stat; 
	cpu_irq_enable();
					
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
				
uint32_t i2c_data_ready;
uint8_t reg_addr;
uint8_t is_addr=0;

ISR(TWI_vect)
{				
	cpu_irq_disable();
	switch(TWSR & 0xFC){
		case START_WRITE: 
			TWCR |= 1<< TWINT;
			TWCR |= 1<< TWEA;
			is_addr = 1;
			break;
			
		case WRITE:
			if(is_addr)
			{
				reg_addr = TWDR;
				if(reg_addr >= sizeof(register_map))
				{
					reg_addr = sizeof(register_map) -1;
				}
				DEBUG_PRINT("Set address %x\n", reg_addr);
				is_addr = 0;
			}
			else
			{
				*(uint8_t*)(register_map + reg_addr) = TWDR;
				DEBUG_PRINT("write register  %x with %x\n", reg_addr,*(uint8_t*)(register_map + reg_addr) );
				if(reg_addr++ >= sizeof(register_map))
				{
					reg_addr = sizeof(register_map) - 1;
				}
				i2c_data_ready |= 1 << reg_addr;
			}
			TWCR |= 1<<TWINT;
			TWCR |= 1<< TWEA;
			break;
			
		case STOP_WRITE:
			TWCR |= 1<< TWINT;
			break;
			
		case START_READ:
		case CONTINUE_READ:		
			TWDR = *(uint8_t*)(register_map + reg_addr);
			//DEBUG_PRINT("Read register %x = %x\n", reg_addr,*(uint8_t*)(register_map + reg_addr) )
			TWCR |= 1 << TWINT;
			TWCR |= 1<< TWEA;
			if(reg_addr++ >= sizeof(register_map))
			{
				reg_addr = sizeof(register_map) -1;	
			}
			break;
				
		case STOP_READ:
			TWCR |= 1 << TWINT;
			TWCR |= 1 << TWEA;
			break;
		}
					
	cpu_irq_enable();
}
