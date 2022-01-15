/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = rtc_carry_isr, /* RTC CARRY (Carry interrupt) */
            [1] = sci_uart_rxi_isr, /* SCI9 RXI (Received data full) */
            [2] = sci_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [3] = sci_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [4] = sci_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [5] = spi_rxi_isr, /* SPI1 RXI (Receive buffer full) */
            [6] = spi_txi_isr, /* SPI1 TXI (Transmit buffer empty) */
            [7] = spi_tei_isr, /* SPI1 TEI (Transmission complete event) */
            [8] = spi_eri_isr, /* SPI1 ERI (Error) */
            [9] = spi_rxi_isr, /* SPI0 RXI (Receive buffer full) */
            [10] = spi_txi_isr, /* SPI0 TXI (Transmit buffer empty) */
            [11] = spi_tei_isr, /* SPI0 TEI (Transmission complete event) */
            [12] = spi_eri_isr, /* SPI0 ERI (Error) */
        };
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_IELS_ENUM(EVENT_RTC_CARRY), /* RTC CARRY (Carry interrupt) */
            [1] = BSP_PRV_IELS_ENUM(EVENT_SCI9_RXI), /* SCI9 RXI (Received data full) */
            [2] = BSP_PRV_IELS_ENUM(EVENT_SCI9_TXI), /* SCI9 TXI (Transmit data empty) */
            [3] = BSP_PRV_IELS_ENUM(EVENT_SCI9_TEI), /* SCI9 TEI (Transmit end) */
            [4] = BSP_PRV_IELS_ENUM(EVENT_SCI9_ERI), /* SCI9 ERI (Receive error) */
            [5] = BSP_PRV_IELS_ENUM(EVENT_SPI1_RXI), /* SPI1 RXI (Receive buffer full) */
            [6] = BSP_PRV_IELS_ENUM(EVENT_SPI1_TXI), /* SPI1 TXI (Transmit buffer empty) */
            [7] = BSP_PRV_IELS_ENUM(EVENT_SPI1_TEI), /* SPI1 TEI (Transmission complete event) */
            [8] = BSP_PRV_IELS_ENUM(EVENT_SPI1_ERI), /* SPI1 ERI (Error) */
            [9] = BSP_PRV_IELS_ENUM(EVENT_SPI0_RXI), /* SPI0 RXI (Receive buffer full) */
            [10] = BSP_PRV_IELS_ENUM(EVENT_SPI0_TXI), /* SPI0 TXI (Transmit buffer empty) */
            [11] = BSP_PRV_IELS_ENUM(EVENT_SPI0_TEI), /* SPI0 TEI (Transmission complete event) */
            [12] = BSP_PRV_IELS_ENUM(EVENT_SPI0_ERI), /* SPI0 ERI (Error) */
        };
        #endif
