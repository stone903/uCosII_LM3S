/*
 * bsp.h
 *
 *  Created on: 2013-4-12
 *      Author: Stone
 */

#define GPIO_LED_SYSCTL_PERIPH  SYSCTL_PERIPH_GPIOB
#define GPIO_LED_PORT_BASE      GPIO_PORTB_BASE
#define GPIO_LED_PIN            (GPIO_PIN_4)
#define GPIO_TEST_OUT_BASE  	(GPIO_PORTD_BASE)
#define GPIO_SYS_TICK_OUTPUT    (GPIO_PIN_0)

void Board_Init();
void BSP_SysTickInit();


