#include <pthread.h>

#define COMMAND_WRITE		0x01
#define COMMAND_STREAM		0x02
#define ADDR_SEND_BYTES		0x01
#define ADDR_SHOOT_FREQ		0x02
#define ADDR_SHOOT_DELAY	0x03
#define ADDR_SHOOT_DURATION_MIN 0x04
#define ADDR_SHOOT_DURATION_MAX 0x05
#define ADDR_SHOOT_MODE		0x06
#define ADDR_BURST_AMOUNT_MIN	0x07
#define ADDR_BURST_AMOUNT_MAX	0x08
#define ADDR_BURST_FREQ		0x09
#define PIN_TRIGGER		5
#define CHECK_INTERVAL_TRIGGER_IN_MS	50
#define CHECK_INTERVAL_BT_IN_MS		100
#define CHECK_INTERVAL_IR_IN_MS		100

typedef struct shoot {

	unsigned long	send_bytes,
			frequency,
			delay,
			duration_min,
			duration_max,
			mode,
			burst_amount_min,
			burst_amount_max,
			burst_frequency;
} shoot;

pthread_t	thread_check_trigger,
		thread_read_bt,
		thread_bt_stream,
		thread_ir_stream;

//pthread_mutex_t ;
