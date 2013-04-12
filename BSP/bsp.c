/*
 * bsp.c
 *
 *  Created on: 2013-4-12
 *      Author: Stone
 */

#include "includes.h"

INT32U SysTickOut;

void Board_Init(void)
{
    // 配置系统主时钟, 使用外部晶振16M
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ |
                   SYSCTL_OSC_MAIN);

    // 使能端口PB，然后配置PB.4为输出
    SysCtlPeripheralEnable(GPIO_LED_SYSCTL_PERIPH);
    GPIOPinTypeGPIOOutput(GPIO_LED_PORT_BASE, GPIO_LED_PIN);

    // 置PB.4为高电平
    GPIOPinWrite(GPIO_LED_PORT_BASE, GPIO_LED_PIN, GPIO_LED_PIN);

    // 延时500ms
    SysCtlDelay(5000 * SysCtlClockGet()/3000);

    // 置PB.4为低电平
    GPIOPinWrite(GPIO_LED_PORT_BASE, GPIO_LED_PIN, 0);

    // 延时500ms
    SysCtlDelay(5000 * SysCtlClockGet()/3000);

}

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
	//Enable SYS_TICK_OUTPUT to Test
	SysCtlPeripheralEnable(GPIO_PORTD_BASE);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_SYS_TICK_OUTPUT);
}


