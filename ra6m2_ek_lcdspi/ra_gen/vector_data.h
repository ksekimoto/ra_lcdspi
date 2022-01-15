/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (9)
#endif
/* ISR prototypes */
void spi_rxi_isr(void);
void spi_txi_isr(void);
void spi_tei_isr(void);
void spi_eri_isr(void);
void rtc_carry_isr(void);
void sci_uart_rxi_isr(void);
void sci_uart_txi_isr(void);
void sci_uart_tei_isr(void);
void sci_uart_eri_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_SPI0_RXI ((IRQn_Type) 0) /* SPI0 RXI (Receive buffer full) */
#define SPI0_RXI_IRQn          ((IRQn_Type) 0) /* SPI0 RXI (Receive buffer full) */
#define VECTOR_NUMBER_SPI0_TXI ((IRQn_Type) 1) /* SPI0 TXI (Transmit buffer empty) */
#define SPI0_TXI_IRQn          ((IRQn_Type) 1) /* SPI0 TXI (Transmit buffer empty) */
#define VECTOR_NUMBER_SPI0_TEI ((IRQn_Type) 2) /* SPI0 TEI (Transmission complete event) */
#define SPI0_TEI_IRQn          ((IRQn_Type) 2) /* SPI0 TEI (Transmission complete event) */
#define VECTOR_NUMBER_SPI0_ERI ((IRQn_Type) 3) /* SPI0 ERI (Error) */
#define SPI0_ERI_IRQn          ((IRQn_Type) 3) /* SPI0 ERI (Error) */
#define VECTOR_NUMBER_RTC_CARRY ((IRQn_Type) 4) /* RTC CARRY (Carry interrupt) */
#define RTC_CARRY_IRQn          ((IRQn_Type) 4) /* RTC CARRY (Carry interrupt) */
#define VECTOR_NUMBER_SCI0_RXI ((IRQn_Type) 5) /* SCI0 RXI (Receive data full) */
#define SCI0_RXI_IRQn          ((IRQn_Type) 5) /* SCI0 RXI (Receive data full) */
#define VECTOR_NUMBER_SCI0_TXI ((IRQn_Type) 6) /* SCI0 TXI (Transmit data empty) */
#define SCI0_TXI_IRQn          ((IRQn_Type) 6) /* SCI0 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI0_TEI ((IRQn_Type) 7) /* SCI0 TEI (Transmit end) */
#define SCI0_TEI_IRQn          ((IRQn_Type) 7) /* SCI0 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI0_ERI ((IRQn_Type) 8) /* SCI0 ERI (Receive error) */
#define SCI0_ERI_IRQn          ((IRQn_Type) 8) /* SCI0 ERI (Receive error) */
#endif /* VECTOR_DATA_H */
