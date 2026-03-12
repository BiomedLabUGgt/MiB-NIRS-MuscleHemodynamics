/**
 * @file PLL.h
 * @brief PLL and System Clock Configuration Driver (STM32F303K8)
 * @details Phase-Locked Loop (PLL) configuration for system clock setup.
 *
 * ### Hardware Configuration
 *  - **MCU**: STM32F303K8 (Cortex-M4)
 *  - **Target System Clock**: 64 MHz (using PLL)
 *  - **Clock Source**: Internal HSI (8 MHz)
 *  - **PLL Multiplier**: 16x via direct calculation (8 MHz / 2) × 16 = 64 MHz
 *  - **AHB Prescaler**: 1 (no division)
 *  - **APB1 Prescaler**: 2 (32 MHz for I2C and most peripherals)
 *  - **APB2 Prescaler**: 1 (64 MHz)
 *
 * ### Flash Configuration
 *  - **Flash Latency**: 2 wait states (required for 64 MHz operation)
 *
 * ### System Clock Tree
 *  - HSI (8 MHz) → PLL (÷2 → ×16) → SYSCLK (64 MHz)
 *  - SYSCLK → AHB (64 MHz) → APB1 (32 MHz), APB2 (64 MHz)
 *
 * @author Julio Fajardo
 * @date 2024-06-01
 * @version 1.0
 * @note Configures for 64 MHz, allowing I2C1 to operate at 400 kHz with APB1 @ 32 MHz
 * @note Call clk_config() before initializing any peripheral that depends on system clock
 */

#ifndef PLL_H_
#define PLL_H_

#include <stdint.h>

/**
 * @brief Configure system clock to 64 MHz using PLL
 * @details Complete PLL and RCC configuration sequence:
 *          1. Set PLL multiplier to 16 (HSI input, internal ÷2)
 *          2. Configure Flash for 64 MHz operation (2 wait states)
 *          3. Enable PLL oscillator
 *          4. Set APB1 prescaler to 2 (32 MHz)
 *          5. Switch SYSCLK source from HSI to PLL
 *          6. Update SystemCoreClock variable
 *
 * ### Register Operations
 *  - RCC_CFGR[21:18] = 0x0E (PLLMUL = 16)
 *  - FLASH_ACR[2:0] = 0x2 (Latency = 2 cycles)
 *  - RCC_CR[24] = 1 (Enable PLL)
 *  - RCC_CFGR[1:0] = 0x2 (SW = PLL as system clock)
 *  - RCC_CFGR[10:8] = 0x4 (PPRE1 = APB1 divide by 2)
 *
 * ### Timing
 *  - PLL lock time: <100 µs (typical)
 *  - Configuration time: <1 ms
 *  - Non-blocking operation
 *
 * @param None
 * @return void
 * @retval N/A
 */
void clk_config(void);

#endif /* PLL_H_ */

