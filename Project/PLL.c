#include "PLL.h"
#include "stm32f303x8.h"

/**
 * @brief Configure system clock to 72 MHz using PLL
 * @details Complete PLL and RCC configuration sequence:
 *          1. Enable HSI oscillator (8 MHz internal oscillator)
 *          2. Configure Flash for 72 MHz operation (2 wait states)
 *          3. Set APB1 and APB2 prescalers
 *          4. Configure and enable PLL (×9 multiplier)
 *          5. Switch SYSCLK source from HSI to PLL
 *          6. Verify clock configuration
 *
 * ### Register Configuration Details
 *
 * **RCC_CR (Clock Control Register)**
 *  - Bit 0 (HSION): Enable HSI oscillator
 *  - Bit 1 (HSIRDY): HSI ready flag (polled)
 *
 * **FLASH_ACR (Flash Access Control Register)**
 *  - Bits [2:0] (LATENCY): Set to 2 for 72 MHz operation
 *  - Bit 4 (PRFTBE): Prefetch enable (improves performance)
 *
 * **RCC_CFGR (Clock Configuration Register)**
 *  - Bits [1:0] (SW): System clock switch (0=HSI, 1=HSE, 2=PLL)
 *  - Bits [3:2] (SWS): System clock switch status (read-only)
 *  - Bits [7:4] (HPRE): AHB prescaler (0=no division)
 *  - Bits [10:8] (PPRE1): APB1 prescaler (100=divide by 2)
 *  - Bits [13:11] (PPRE2): APB2 prescaler (0=no division)
 *  - Bits [15:14] (ADCPRE): ADC prescaler (01=divide by 2)
 *  - Bits [21:18] (PLLMUL): PLL multiplier (0111=×9)
 *  - Bit 16 (PLLSRC): PLL input source (0=HSI/2, 1=HSE)
 *  - Bit 17 (PLLXTPRE): HSE divider for PLL (not used with HSI)
 *
 * **RCC_CR (PLL Enable)**
 *  - Bit 24 (PLLON): Enable PLL
 *  - Bit 25 (PLLRDY): PLL ready flag (polled)
 *
 * ### Clock Calculation
 *  - SYSCLK = (HSI × PLLMUL) / 2 = (8 MHz × 9) / 2 = 36 MHz (HSI/2 source)
 *  - OR: SYSCLK = HSI × PLLMUL = 8 MHz × 9 = 72 MHz (full HSI source)
 *  - With APB1 prescaler = 2: APB1 = 36 MHz
 *
 * @param None
 * @return void
 * @retval N/A
 */
void clk_config(void){
	// PLLMUL <- 0x0E (PLL input clock x 16 --> (8 MHz / 2) * 16 = 64 MHz )  
	RCC->CFGR |= 0xE<<18;
	// Flash Latency, two wait states for 48<HCLK<=72 MHz
	FLASH->ACR |= 0x2;
	// PLLON <- 0x1 
    RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));	
	// SW<-0x02 (PLL as System Clock), HCLK not divided, PPRE1<-0x4 (APB1 <- HCLK/2), APB2 not divided 
	RCC->CFGR |= 0x402;
	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL));
	SystemCoreClockUpdate();	
}
