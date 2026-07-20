/* camera.h
 *
 * */


extern uint8_t frame[90][640];
extern volatile uint8_t frame_ready;

#define FIFO_RCK_PORT GPIOA
#define FIFO_RCK_PIN 0

#define FIFO_OE_PORT GPIOA
#define FIFO_OE_PIN 1

#define FIFO_WR_PORT GPIOA
#define FIFO_WR_PIN 4

#define FIFO_RRST_PORT GPIOB
#define FIFO_RRST_PIN 0

#define FIFO_WRST_PORT GPIOC
#define FIFO_WRST_PIN 1

#define RESET_PORT GPIOC
#define RESET_PIN 0


#define DRIVE_HIGH(port, pin) do { \
	(port)->BSRR = 0x1UL << (pin); \
} while(0)

#define DRIVE_LOW(port, pin) do { \
	(port)->BSRR = 0x1UL << (16 + (pin)); \
} while(0)


void setup_camera(void);

void read_enable(void);

void read_frame(void);

uint8_t read_byte(void);
