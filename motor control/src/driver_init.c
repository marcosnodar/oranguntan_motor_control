/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <system.h>

void exint_0_init(void)
{

// Set pin direction to input
PC0_set_dir(PORT_DIR_IN);

PC0_set_pull_mode(
        // <y> Pull configuration
        // <id> pad_pull_config
        // <PORT_PULL_OFF"> Off
        // <PORT_PULL_UP"> Pull-up
        PORT_PULL_OFF);

// Set pin direction to input
PC1_set_dir(PORT_DIR_IN);

PC1_set_pull_mode(
        // <y> Pull configuration
        // <id> pad_pull_config
        // <PORT_PULL_OFF"> Off
        // <PORT_PULL_UP"> Pull-up
        PORT_PULL_OFF);

// Set pin direction to input
PC2_set_dir(PORT_DIR_IN);

PC2_set_pull_mode(
        // <y> Pull configuration
        // <id> pad_pull_config
        // <PORT_PULL_OFF"> Off
        // <PORT_PULL_UP"> Pull-up
        PORT_PULL_OFF);

// Set pin direction to input
PC3_set_dir(PORT_DIR_IN);

PC3_set_pull_mode(
        // <y> Pull configuration
        // <id> pad_pull_config
        // <PORT_PULL_OFF"> Off
        // <PORT_PULL_UP"> Pull-up
        PORT_PULL_OFF);

	EXTERNAL_IRQ_0_init();
}

void tc8_0_init(void)
{

// Set pin direction to output
PD6_set_dir(PORT_DIR_OUT);

PD6_set_level(
        // <y> Initial level
        // <id> pad_initial_level
        // <false"> Low
        // <true"> High
        false);

// Set pin direction to output
PD5_set_dir(PORT_DIR_OUT);

PD5_set_level(
        // <y> Initial level
        // <id> pad_initial_level
        // <false"> Low
        // <true"> High
        false);

	TIMER_0_init();
}

void tc8_2_init(void)
{

// Set pin direction to output
PB3_set_dir(PORT_DIR_OUT);

PB3_set_level(
        // <y> Initial level
        // <id> pad_initial_level
        // <false"> Low
        // <true"> High
        false);

// Set pin direction to output
PD3_set_dir(PORT_DIR_OUT);

PD3_set_level(
        // <y> Initial level
        // <id> pad_initial_level
        // <false"> Low
        // <true"> High
        false);

	TIMER_2_init();
}

void system_init()
{
	mcu_init();

sysctrl_init();

exint_0_init();

//I2C_0_init();

tc8_0_init();

tc8_2_init();

//USART_0_init();

}

