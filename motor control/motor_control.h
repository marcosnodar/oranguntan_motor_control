#include <stdio.h>
extern uint8_t str[256];
#define DBG
#ifdef DBG
#define DEBUG_PRINT(fmt, args...) 		{ \
	sprintf(str, fmt, args); \
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
	
#define DETECT_START_WRITE 0x60
#define DETECT_START_READ 0xA8
#define DETECT_CONTINUE_READ 0xB8
#define DETECT_STOP 0xA0
#define WRITE_BYTE 0x80
#define DETECT_STOP_READ 0xC0