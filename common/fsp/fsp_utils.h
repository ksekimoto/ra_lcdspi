/*
 * fsp_utils.h
 *
 *  Created on: 2021/01/23
 *      Author: ksgadget
 */

#ifndef FSP_UTILS_H_
#define FSP_UTILS_H_

__attribute__((always_inline)) static inline void ra_enable_irq(__attribute__((unused)) uint32_t state) {
    __asm__ volatile("cpsie f"
                     :
                     :
                     : "memory");
}

__attribute__((always_inline)) static inline uint32_t ra_disable_irq(void) {
    uint32_t state;
    __asm__ volatile("cpsid f"
                     : "=r"(state)
                     :
                     : "memory");
    return state;
}

#endif /* FSP_UTILS_H_ */
