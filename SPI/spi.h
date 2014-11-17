// Title: spi.h
// Descr: my SPI driver
//
// Geoffrey Brown Intro to STM32 Exercise 6.1
//

enum spiSpeed { SPI_SLOW, SPI_MEDIUM, SPI_FAST };

void spiInit(SPI_TypeDef* SPIx);
int spiReadWrite(SPI_TypeDef* SPIx, uint8_t *rbuf , const uint8_t *tbuf, int cnt, enum spiSpeed speed);
int spiReadWrite16(SPI_TypeDef* SPIx, uint16_t *rbuf, const uint16_t *tbuf, int cnt, enum spiSpeed speed);
