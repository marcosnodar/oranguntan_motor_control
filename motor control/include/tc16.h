/* asf_license_stop
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _TC16_H_INCLUDED
#define _TC16_H_INCLUDED

#include <compiler.h>

/**
 * \addtogroup tc16 driver
 *
 * \section tc16_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialize TIMER_0 interface
 *
 * \return Initialization status.
 */
int8_t TIMER_3_init();

/**
 * \brief Set compare channel A value
 *
 * \param[in] value The new compare value to set 
 */
static inline void TIMER_3_set_comp_a(uint16_t value)
{
	OCR1A = value;

}

/**
 * \brief Set compare channel B value
 *
 * \param[in] value The new compare value to set 
 */
static inline void TIMER_3_set_comp_b(uint16_t value)
{
	OCR1B = value;

}

#ifdef __cplusplus
}
#endif

#endif /* _TC16_H_INCLUDED */