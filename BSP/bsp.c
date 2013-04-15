/*
 * bsp.c
 *
 *  Created on: 2013-4-12
 *      Author: Stone
 */

#include "includes.h"

/* SysTick output for test */
INT32U SysTickOut;
INT32U SysTickLedOut;

/* This Function is to initialize and test the system clock and GPIOs. */
void Board_Init(void)
{
    // 配置系统主时钟, 使用外部晶振16M
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ |
                   SYSCTL_OSC_MAIN);

    // 使能端口PB，然后配置PB.456为输出
    SysCtlPeripheralEnable(GPIO_LED_SYSCTL_PERIPH);
    GPIOPinTypeGPIOOutput(GPIO_LED_PORT_BASE, GPIO_LED_PIN1);
	GPIOPinTypeGPIOOutput(GPIO_LED_PORT_BASE, GPIO_LED_PIN2);
	GPIOPinTypeGPIOOutput(GPIO_LED_PORT_BASE, GPIO_LED_PIN3);

    // 置PB.4为高电平
    GPIOPinWrite(GPIO_LED_PORT_BASE, GPIO_LED_PIN1, GPIO_LED_PIN1);

    // 延时500ms
    SysCtlDelay(5000 * SysCtlClockGet()/3000);

    // 置PB.4为低电平
    GPIOPinWrite(GPIO_LED_PORT_BASE, GPIO_LED_PIN1, ~GPIO_LED_PIN1);

    // 延时500ms
    SysCtlDelay(5000 * SysCtlClockGet()/3000);

	// 置PB.4为高电平
    GPIOPinWrite(GPIO_LED_PORT_BASE, GPIO_LED_PIN2, GPIO_LED_PIN2);

    // 延时500ms
    SysCtlDelay(5000 * SysCtlClockGet()/3000);

    // 置PB.4为低电平
    GPIOPinWrite(GPIO_LED_PORT_BASE, GPIO_LED_PIN2, ~GPIO_LED_PIN2);

    // 延时500ms
    SysCtlDelay(5000 * SysCtlClockGet()/3000);

	// 置PB.4为高电平
    GPIOPinWrite(GPIO_LED_PORT_BASE, GPIO_LED_PIN3, GPIO_LED_PIN3);

    // 延时500ms
    SysCtlDelay(5000 * SysCtlClockGet()/3000);

    // 置PB.4为低电平
    GPIOPinWrite(GPIO_LED_PORT_BASE, GPIO_LED_PIN3, ~GPIO_LED_PIN3);

    // 延时500ms
    SysCtlDelay(5000 * SysCtlClockGet()/3000);
}

/* This function is to initialize the SysTick relate configurations */
void BSP_SysTickInit()
{
	OS_ENTER_CRITICAL();
	SysTickIntDisable();
	SysTickIntRegister(OSTimeTickIsr);
	SysTickPeriodSet(80000);
	SysTickIntEnable();
	SysTickEnable();
	OS_EXIT_CRITICAL();

	SysTickOut = 0x01;
	SysTickLedOut = GPIO_LED_PIN1;
}

/* This function is initialize the priority fo PendSV for context switch */
void BSP_PENDSVSetPrio(unsigned char ucPriority)
{
	IntPrioritySet(FAULT_PENDSV,ucPriority);
}

/* This function is to initialize the Test GPIOs to Output */
void BSP_TestOutPutInit()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOOutput(GPIO_TEST_OUT_BASE,GPIO_SYS_TICK_OUTPUT);
	GPIOPinTypeGPIOOutput(GPIO_TEST_OUT_BASE,GPIO_TEST1_OUTPUT);
	GPIOPinTypeGPIOOutput(GPIO_TEST_OUT_BASE,GPIO_TEST2_OUTPUT);
	GPIOPinTypeGPIOOutput(GPIO_TEST_OUT_BASE,GPIO_TEST3_OUTPUT);
	GPIOPinTypeGPIOOutput(GPIO_TEST_OUT_BASE,GPIO_TEST4_OUTPUT);
	GPIOPinTypeGPIOOutput(GPIO_TEST_OUT_BASE,GPIO_TEST5_OUTPUT);
}

