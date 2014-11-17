#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include "spi.h"

void Delay(uint32_t nTime);
void csInit();

uint8_t txbuf[4], rxbuf[4];
uint16_t txbuf16[4], rxbuf16[4];

int main(void){
    int i, j;
    csInit();   // Initialize chip select PC03
    spiInit(SPI2);

    // 8 bit read/write (?)
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 4; j++)
            txbuf[j] = i*4 + j;

        GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
        spiReadWrite(SPI2 , rxbuf , txbuf , 4, SPI_SLOW );
        GPIO_WriteBit (GPIOC , GPIO_Pin_3 , 1);

        for(j = 0; j < 4; j++)
            if(rxbuf[j] != txbuf [j])
                assert_failed (__FILE__ , __LINE__ );
    }

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 4; j++)
            txbuf16 [j] = i*4 + j + (i << 8);
        GPIO_WriteBit (GPIOC , GPIO_Pin_3 , 0);
        spiReadWrite16 (SPI2 , rxbuf16 , txbuf16 , 4, SPI_SLOW );
        GPIO_WriteBit (GPIOC , GPIO_Pin_3 , 1);
        for (j = 0; j < 4; j++)
            if ( rxbuf16 [j] != txbuf16 [j])
                assert_failed (__FILE__ , __LINE__ );
    }
}

void csInit(){
// Initialize CS on PC03
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable peripheral Clocks
    // Enable clocks for GPIO Port C
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // Configure Pins
    // Pin PC9 must be configured as an output
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}

// Timer code
static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime){
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

void SysTick_Handler(void){
    if (TimingDelay != 0x00)
        TimingDelay--;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line){
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while(1);
}
#endif
