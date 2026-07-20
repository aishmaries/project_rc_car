
#include "stm32f411xe.h"
#include "camera.h"

volatile uint8_t frame_ready = 0;
uint8_t frame[90][640];

void setup_camera(void){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

//FIFO_RCK -> ADC/GPIO :PA0
	GPIOA->MODER |= GPIO_MODER_MODER0_0;

//FIFO_OE -> ADC/GPIO :PA1
	GPIOA->MODER |= GPIO_MODER_MODER1_0;

//FIFO_WR -> ADC/GPIO :PA4
	GPIOA->MODER |= GPIO_MODER_MODER4_0;

//FIFO_RRST -> ADC/GPIO :PB0
	GPIOB->MODER |= GPIO_MODER_MODER0_0;

//SIO_C -> I2C_SCL :PB8
	GPIOB->MODER |= GPIO_MODER_MODER8_1;
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL8_2;
//SIO_D -> I2C_SDA :PB9
	GPIOB->MODER |= GPIO_MODER_MODER9_1;
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL9_2;

//FIFIO_WRST -> GPIO :PC1
	GPIOC->MODER |= GPIO_MODER_MODER1_0;


//VSYNC -> EXTERNAL INTERRUPT :PC3
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PC;

	EXTI->IMR |= EXTI_IMR_MR3_Msk;
	EXTI->RTSR |= EXTI_RTSR_TR3_Msk;
	NVIC_EnableIRQ(EXTI1_IRQn);
//RESET -> GPIO :PC0
	GPIOC->MODER |= GPIO_MODER_MODER0_0;


//Set D0-D7 to GPIO Input :D0-D7
	GPIOA->MODER &= ~GPIO_MODER_MODER8; //PA8
	GPIOB->MODER &= ~GPIO_MODER_MODER10; //PB10
	GPIOB->MODER &= ~GPIO_MODER_MODER4; //PB4
	GPIOB->MODER &= ~GPIO_MODER_MODER5; //PB5
	GPIOB->MODER &= ~GPIO_MODER_MODER3; //PB3
	GPIOA->MODER &= ~GPIO_MODER_MODER10; //PA10
	GPIOA->MODER &= ~GPIO_MODER_MODER2; //PA2
	GPIOA->MODER &= ~GPIO_MODER_MODER3; //PA3

//reset write pointer and enable it
	DRIVE_LOW(FIFO_WRST_PORT, FIFO_WRST_PIN);
	DRIVE_LOW(FIFO_WR_PORT, FIFO_WR_PIN);
}

void read_enable(void){
	DRIVE_LOW(FIFO_OE_PORT, FIFO_OE_PIN);
}

uint8_t read_byte(){
	uint8_t data = 0;
	uint8_t D0 = (GPIOA->IDR &  GPIO_IDR_ID3) ? 1 : 0;
	uint8_t D1 = (GPIOA->IDR &  GPIO_IDR_ID2) ? 1 : 0;
	uint8_t D2 = (GPIOA->IDR &  GPIO_IDR_ID10) ? 1 : 0;
	uint8_t D3 = (GPIOB->IDR &  GPIO_IDR_ID3) ? 1 : 0;
	uint8_t D4 = (GPIOB->IDR &  GPIO_IDR_ID5) ? 1 : 0;
	uint8_t D5 = (GPIOB->IDR &  GPIO_IDR_ID4) ? 1 : 0;
	uint8_t D6 = (GPIOB->IDR &  GPIO_IDR_ID10) ? 1 : 0;
	uint8_t D7 = (GPIOA->IDR &  GPIO_IDR_ID8) ? 1 : 0;

	data |= (D0 << 0);
	data |= (D1 << 1);
	data |= (D2 << 2);
	data |= (D3 << 3);
	data |= (D4 << 4);
	data |= (D5 << 5);
	data |= (D6 << 6);
	data |= (D7 << 7);

	return data;
}

void read_frame(void) {
	int i = 0;
	int j = 0;
	for(int row = 0; row < 480; row++){
		for(int col = 0; col < 1280; col++){
			//pulse rising edge of RCK
			DRIVE_LOW(FIFO_RCK_PORT, FIFO_RCK_PIN);
			DRIVE_HIGH(FIFO_RCK_PORT, FIFO_RCK_PIN);

			if(((0 <= row) && (row <= 29)) || ((215 <= row) && (row <= 244)) || ((430 <= row) && (row <= 459))){
				if(col % 2 == 0){
					frame[i][j] = read_byte();
					j++;
					if(j >= 640) {
						j = 0;
						i++;
					}
				}
			}
		}
	}
}

void EXTI3_IRQHandler(void) {
	EXTI->PR |= EXTI_PR_PR3; //set the pending bit to 1 to clear it
	//reset read pointer of FIFO and set flag
	DRIVE_LOW(FIFO_RRST_PORT, FIFO_RRST_PIN);
	DRIVE_HIGH(FIFO_RRST_PORT, FIFO_RRST_PIN);
	frame_ready = 1;
}
