/*
 * bsp.h
 *
 *  Created on: 2013-4-12
 *      Author: Stone
 */

/* LED PIN Definition */
#define GPIO_LED_SYSCTL_PERIPH  SYSCTL_PERIPH_GPIOB
#define GPIO_LED_PORT_BASE      GPIO_PORTB_BASE

#define GPIO_LED_PIN1            (GPIO_PIN_4)
#define GPIO_LED_PIN2            (GPIO_PIN_5)
#define GPIO_LED_PIN3            (GPIO_PIN_6)
#define GPIO_LED_PIN4            (GPIO_PIN_7)

/* Test Port Definition */
#define GPIO_TEST_OUT_BASE  	(GPIO_PORTD_BASE)
#define GPIO_SYS_TICK_OUTPUT    (GPIO_PIN_0)
#define GPIO_TEST1_OUTPUT    (GPIO_PIN_1)
#define GPIO_TEST2_OUTPUT    (GPIO_PIN_2)
#define GPIO_TEST3_OUTPUT    (GPIO_PIN_3)
#define GPIO_TEST4_OUTPUT    (GPIO_PIN_4)
#define GPIO_TEST5_OUTPUT    (GPIO_PIN_5)

/* BSP Relate Functions */
void Board_Init();
void BSP_SysTickInit();
void BSP_PENDSVSetPrio(unsigned char ucPriority);


