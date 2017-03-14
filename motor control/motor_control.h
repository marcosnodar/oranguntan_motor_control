#include <stdio.h>
#include <usart.h>

extern uint8_t str[256];
#define DBG
#ifdef DBG
#define DEBUG_PRINT(fmt, args...) 		{ \
	sprintf((char*)str, fmt, args); \
	USART0_put_string(str); \
	}
#else
#define DEBUG_PRINT(fmt, args...)
#endif

enum direction{
	FORWARD,
	REVERSE,
	STOP
};
enum side{
	LEFT,
	RIGHT
};

uint8_t compute_new_pwm(uint32_t timer_new, uint8_t index);
void set_pmw_output(uint8_t pwm, uint8_t index);
uint32_t get_timer();
void set_direction(uint8_t side, uint8_t dir);

uint8_t register_map[32];

enum registers{
	ID,
	GSR,
	RLSPDSET,
	RLSPDGET,
	RLSTS,
	RRSPDSET,
	RRSPDGET,
	RRSTS,
	FLSPDSET,
	FLSPDGET,
	FLSTS,	
	FRSPDSET,
	FRSPDGET,
	FRSTS	
	};
	
#define START_WRITE 0x60
#define START_READ 0xA8
#define CONTINUE_READ 0xB8
#define STOP_WRITE 0xA0
#define WRITE 0x80
#define STOP_READ 0xC0

extern int16_t P;
extern int16_t I;
extern int16_t d;
extern int32_t K;
void init_pid(int32_t k, int16_t prop, int16_t integ, int16_t diff);

extern uint32_t timer_value[4];
extern uint32_t timer_old[4];

extern int32_t  error_old[4];
extern int32_t  error_sum[4], error_d[4];

extern uint32_t target_timer_diff[4];
extern uint32_t old_timer_diff[4];
extern uint32_t wrap[4];
extern uint8_t int_stat;
extern volatile uint8_t update_pwm[4];
void init_interrupt_mask(void);
void init_pins(void);