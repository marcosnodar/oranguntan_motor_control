/**
 * \file
 *
 * \brief TC8 related functionality implementation.
*
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include <tc8.h>
#include <utils.h>

/**
 * \brief Initialize TIMER_0 interface
 */
int8_t TIMER_0_init()
{
	/* Enable TC0 */
	PRR &= ~(1 << PRTIM0);
    TCCR0A = (1 << COM0A1) | (0 << COM0A0) | // Clear OCA on Compare Match, set OCA on BOTTOM (non-inverting mode)
              (1 << COM0B1) | (0 << COM0B0) | // Clear OCA on Compare Match, set OCA on BOTTOM (non-inverting mode)
              (1 << WGM01) | (1 << WGM00); // Mode 3 Fast PWM

    TCCR0B = ( 0 << WGM02 ) | // Mode 3 Fast PWM
				(0 << CS02) | (0 << CS01) | (1 << CS00);
             // (0 << CS02) | (1 << CS01) | (0 << CS00); // IO clock divided by 8

    TIMSK0 = (0 << OCIE0B) | // Disable output compare match B interrupt
              (0 << OCIE0A) | // Disable output compare match A interrupt
              (0 << TOIE0);   // Disable overflow interrupt

    return 0;
}

/**
 * \brief Initialize TIMER_2 interface
 */
int8_t TIMER_2_init()
{
	/* Enable TC2 */
	PRR &= ~(1 << PRTIM2);
    TCCR2A = (1 << COM2A1) | (0 << COM2A0) | // Clear OCA on Compare Match, set OCA on BOTTOM (non-inverting mode)
              (1 << COM2B1) | (0 << COM2B0) | // Clear OCA on Compare Match, set OCA on BOTTOM (non-inverting mode)
              (1 << WGM21) | (1 << WGM20); // Mode 3 Fast PWM

    TCCR2B = ( 0 << WGM22 ) | // Mode 3 Fast PWM
              (0 << CS22) | (1 << CS21) | (0 << CS20); // IO clock divided by 8

    TIMSK2 = (0 << OCIE2B) | // Disable output compare match B interrupt
              (0 << OCIE2A) | // Disable output compare match A interrupt
              (0 << TOIE2);   // Disable overflow interrupt

    return 0;
}

