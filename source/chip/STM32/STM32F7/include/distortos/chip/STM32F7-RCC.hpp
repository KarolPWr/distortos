/**
 * \file
 * \brief Header for RCC-related functions for STM32F7
 *
 * \author Copyright (C) 2017-2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_STM32F7_RCC_HPP_
#define SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_STM32F7_RCC_HPP_

#include "distortos/distortosConfiguration.h"

#include <cstdint>

namespace distortos
{

namespace chip
{

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/// system clock source
enum class SystemClockSource : uint8_t
{
	/// HSI oscillator selected as system clock
	hsi,
	/// HSE oscillator selected as system clock
	hse,
	/// main PLL selected as system clock
	pll,
};

/*---------------------------------------------------------------------------------------------------------------------+
| global constants
+---------------------------------------------------------------------------------------------------------------------*/

/// minimum allowed value for PLLM
constexpr uint8_t minPllm {2};

/// maximum allowed value for PLLM
constexpr uint8_t maxPllm {63};

/// minimum allowed value for PLLN
constexpr uint16_t minPlln {50};

/// maximum allowed value for PLLN
constexpr uint16_t maxPlln {432};

/// minimum allowed value for PLLQ
constexpr uint8_t minPllq {2};

/// maximum allowed value for PLLQ
constexpr uint8_t maxPllq {15};

/// minimum allowed value for PLLR
constexpr uint8_t minPllr {2};

/// maximum allowed value for PLLR
constexpr uint8_t maxPllr {7};

/// first allowed value for PLLP - 2
constexpr uint8_t pllpDiv2 {2};

/// second allowed value for PLLP - 4
constexpr uint8_t pllpDiv4 {4};

/// third allowed value for PLLP - 6
constexpr uint8_t pllpDiv6 {6};

/// fourth allowed value for PLLP - 8
constexpr uint8_t pllpDiv8 {8};

/// first allowed value for AHB divider - 1
constexpr uint16_t hpreDiv1 {1};

/// second allowed value for AHB divider - 2
constexpr uint16_t hpreDiv2 {2};

/// third allowed value for AHB divider - 4
constexpr uint16_t hpreDiv4 {4};

/// fourth allowed value for AHB divider - 8
constexpr uint16_t hpreDiv8 {8};

/// fifth allowed value for AHB divider - 16
constexpr uint16_t hpreDiv16 {16};

/// sixth allowed value for AHB divider - 64
constexpr uint16_t hpreDiv64 {64};

/// seventh allowed value for AHB divider - 128
constexpr uint16_t hpreDiv128 {128};

/// eighth allowed value for AHB divider - 256
constexpr uint16_t hpreDiv256 {256};

/// ninth allowed value for AHB divider - 512
constexpr uint16_t hpreDiv512 {512};

/// first allowed value for APB1 and APB2 dividers - 1
constexpr uint8_t ppreDiv1 {1};

/// second allowed value for APB1 and APB2 dividers - 2
constexpr uint8_t ppreDiv2 {2};

/// third allowed value for APB1 and APB2 dividers - 4
constexpr uint8_t ppreDiv4 {4};

/// fourth allowed value for APB1 and APB2 dividers - 8
constexpr uint8_t ppreDiv8 {8};

/// fifth allowed value for APB1 and APB2 dividers - 16
constexpr uint8_t ppreDiv16 {16};

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Configures divider of AHB clock (HPRE value).
 *
 * \param [in] hpre is the HPRE value, {1, 2, 4, 8, 16, 64, 128, 256, 512} or {hpreDiv1, hpreDiv2, hpreDiv4, hpreDiv8,
 * hpreDiv16, hpreDiv64, hpreDiv128, hpreDiv256, hpreDiv512}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a hpre value is invalid;
 */

int configureAhbClockDivider(uint16_t hpre);

/**
 * \brief Configures divider of APB1 or APB2 clock (PPRE1 or PPRE2 value).
 *
 * \param [in] ppre2 selects whether PPRE1 (false) or PPRE2 (true) is configured
 * \param [in] ppre is the PPRE value, {1, 2, 4, 8, 16} or {ppreDiv1, ppreDiv2, ppreDiv4, ppreDiv8, ppreDiv16}
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a ppre value is invalid;
 */

int configureApbClockDivider(bool ppre2, uint8_t ppre);

/**
 * \brief Configures source of PLL48 clock.
 *
 * \param [in] pllsaip selects whether PLLQ (false) or PLLSAIP (true) is used as source of PLL48 clock
 */

void configurePll48ClockSource(bool pllsaip);

/**
 * \brief Configures clock source of main and audio PLLs.
 *
 * \warning Before changing configuration of any PLL make sure that they are not used in any way (as core clock or as
 * source of peripheral clocks) and that they are disabled.
 *
 * \param [in] hse selects whether HSI (false) or HSE (true) is used as clock source of main and audio PLLs
 */

void configurePllClockSource(bool hse);

/**
 * \brief Configures divider of PLL input clock (PLLM value) for main and audio PLLs.
 *
 * \warning Before changing configuration of any PLL make sure that they are not used in any way (as core clock or as
 * source of peripheral clocks) and that they are disabled.
 *
 * \param [in] pllm is the PLLM value for main and audio PLLs, [2; 63] or [minPllm; maxPllm]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pllm value is invalid;
 */

int configurePllInputClockDivider(uint8_t pllm);

/**
 * \brief Disables HSE clock.
 *
 * \warning Before changing configuration of HSE clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of RTC clock).
 */

void disableHse();

/**
 * \brief Disables main PLL.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks).
 */

void disablePll();

/**
 * \brief Disables PLLI2S.
 *
 * \warning Before changing configuration of PLLI2S make sure that it is not used in any way (as source of peripheral
 * clocks).
 */

void disablePlli2s();

/**
 * \brief Disables PLLSAI.
 *
 * \warning Before changing configuration of PLLSAI make sure that it is not used in any way (as source of peripheral
 * clocks).
 */

void disablePllsai();

/**
 * \brief Enables HSE clock.
 *
 * Enables HSE clock using crystal/ceramic resonator (bypass disabled) or external user clock (bypass enabled). This
 * function waits until the HSE oscillator is stable after enabling the clock.
 *
 * \warning Before changing configuration of HSE clock make sure that it is not used in any way (as core clock, as
 * source for any PLL or as source of RTC clock).
 *
 * \param [in] bypass selects whether crystal/ceramic resonator (false) or external user clock (true) is used
 */

void enableHse(bool bypass);

#if defined(DISTORTOS_CHIP_STM32F76) || defined(DISTORTOS_CHIP_STM32F77)

/**
 * \brief Enables main PLL.
 *
 * Enables main PLL using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks) and that it is disabled.
 *
 * \param [in] plln is the PLLN value for main PLL, [50; 432] or [minPlln; maxPlln]
 * \param [in] pllp is the PLLP value for main PLL, {2, 4, 6, 8} or {pllpDiv2, pllpDiv4, pllpDiv6, pllpDiv8}
 * \param [in] pllq is the PLLQ value for main PLL, [2; 15] or [minPllq; maxPllq]
 * \param [in] pllr is the PLLR value for main PLL, [2; 7] or [minPllr; maxPllr]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a plln or \a pllp or \a pllq or \a pllr value is invalid;
 */

int enablePll(uint16_t plln, uint8_t pllp, uint8_t pllq, uint8_t pllr);

#else	// !defined(DISTORTOS_CHIP_STM32F76) && !defined(DISTORTOS_CHIP_STM32F77)

/**
 * \brief Enables main PLL.
 *
 * Enables main PLL using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of main PLL make sure that it is not used in any way (as core clock or as
 * source of peripheral clocks) and that it is disabled.
 *
 * \param [in] plln is the PLLN value for main PLL, [50; 432] or [minPlln; maxPlln]
 * \param [in] pllp is the PLLP value for main PLL, {2, 4, 6, 8} or {pllpDiv2, pllpDiv4, pllpDiv6, pllpDiv8}
 * \param [in] pllq is the PLLQ value for main PLL, [2; 15] or [minPllq; maxPllq]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a plln or \a pllp or \a pllq value is invalid;
 */

int enablePll(uint16_t plln, uint8_t pllp, uint8_t pllq);

#endif	// !defined(DISTORTOS_CHIP_STM32F76) && !defined(DISTORTOS_CHIP_STM32F77)

#if defined(DISTORTOS_CHIP_STM32F72) || defined(DISTORTOS_CHIP_STM32F73)

/**
 * \brief Enables PLLI2S.
 *
 * Enables PLLI2S using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of PLLI2S make sure that it is not used in any way (as source of peripheral
 * clocks) and that it is disabled.
 *
 * \param [in] plli2sn is the PLLI2SN value for PLLI2S, [50; 432] or [minPlln; maxPlln]
 * \param [in] plli2sq is the PLLI2SQ value for PLLI2S, [2; 15] or [minPllq; maxPllq]
 * \param [in] plli2sr is the PLLI2SR value for PLLI2S, [2; 7] or [minPllr; maxPllr]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a plli2sn or \a plli2sq or \a plli2sr value is invalid;
 */

int enablePlli2s(uint16_t plli2sn, uint8_t plli2sq, uint8_t plli2sr);

#else	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)

/**
 * \brief Enables PLLI2S.
 *
 * Enables PLLI2S using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of PLLI2S make sure that it is not used in any way (as source of peripheral
 * clocks) and that it is disabled.
 *
 * \param [in] plli2sn is the PLLI2SN value for PLLI2S, [50; 432] or [minPlln; maxPlln]
 * \param [in] plli2sp is the PLLI2SP value for PLLI2S, {2, 4, 6, 8} or {pllpDiv2, pllpDiv4, pllpDiv6, pllpDiv8}
 * \param [in] plli2sq is the PLLI2SQ value for PLLI2S, [2; 15] or [minPllq; maxPllq]
 * \param [in] plli2sr is the PLLI2SR value for PLLI2S, [2; 7] or [minPllr; maxPllr]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a plli2sn or \a plli2sp or \a plli2sq or \a plli2sr value is invalid;
 */

int enablePlli2s(uint16_t plli2sn, uint8_t plli2sp, uint8_t plli2sq, uint8_t plli2sr);

#endif	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)

#if defined(DISTORTOS_CHIP_STM32F72) || defined(DISTORTOS_CHIP_STM32F73)

/**
 * \brief Enables PLLSAI.
 *
 * Enables PLLSAI using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of PLLSAI make sure that it is not used in any way (as source of peripheral
 * clocks) and that it is disabled.
 *
 * \param [in] pllsain is the PLLSAIN value for PLLSAI, [50; 432] or [minPlln; maxPlln]
 * \param [in] pllsaip is the PLLSAIP value for PLLSAI, {2, 4, 6, 8} or {pllpDiv2, pllpDiv4, pllpDiv6, pllpDiv8}
 * \param [in] pllsaiq is the PLLSAIQ value for PLLSAI, [2; 15] or [minPllq; maxPllq]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pllsain or \a pllsaip or \a pllsaiq value is invalid;
 */

int enablePllsai(uint16_t pllsain, uint8_t pllsaip, uint8_t pllsaiq);

#else	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)

/**
 * \brief Enables PLLSAI.
 *
 * Enables PLLSAI using selected parameters and waits until it is stable.
 *
 * \warning Before changing configuration of PLLSAI make sure that it is not used in any way (as source of peripheral
 * clocks) and that it is disabled.
 *
 * \param [in] pllsain is the PLLSAIN value for PLLSAI, [50; 432] or [minPlln; maxPlln]
 * \param [in] pllsaip is the PLLSAIP value for PLLSAI, {2, 4, 6, 8} or {pllpDiv2, pllpDiv4, pllpDiv6, pllpDiv8}
 * \param [in] pllsaiq is the PLLSAIQ value for PLLSAI, [2; 15] or [minPllq; maxPllq]
 * \param [in] pllsair is the PLLSAIR value for PLLSAI, [2; 7] or [minPllr; maxPllr]
 *
 * \return 0 on success, error code otherwise:
 * - EINVAL - \a pllsain or \a pllsaip or \a pllsaiq or \a pllsair value is invalid;
 */

int enablePllsai(uint16_t pllsain, uint8_t pllsaip, uint8_t pllsaiq, uint8_t pllsair);

#endif	// !defined(DISTORTOS_CHIP_STM32F72) && !defined(DISTORTOS_CHIP_STM32F73)

/**
 * \brief Switches system clock.
 *
 * \param [in] source is the new source of system clock
 */

void switchSystemClock(SystemClockSource source);

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_STM32F7_INCLUDE_DISTORTOS_CHIP_STM32F7_RCC_HPP_
