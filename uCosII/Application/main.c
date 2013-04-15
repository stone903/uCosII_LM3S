#include "includes.h"

#define   NVIC_PENDSV_PRI (0xFF)	/* Lowest */
#define          TASK_STK_SIZE     512                /* Size of each task's stacks (# of WORDs)       */

OS_STK FatherTaskStack[TASK_STK_SIZE];
OS_STK My_Task1Stack[TASK_STK_SIZE];
OS_STK My_Task2Stack[TASK_STK_SIZE];

INT32U SysTest1;
INT32U SysTest2;
INT32U SysTest3;

void* My_Task1()
{
	static INT32U mytask1 = GPIO_LED_PIN2;
	SysTest1 = 1;

	printf("My_Task1 Starting...9...\n");
	printf("My_Task1 Starting...8...\n");
	printf("My_Task1 Starting...7...\n");
	printf("My_Task1 Starting...6...\n");
	printf("My_Task1 Starting...5...\n");
	printf("My_Task1 Starting...4...\n");
	printf("My_Task1 Starting...3...\n");
	printf("My_Task1 Starting...2...\n");
	printf("My_Task1 Starting...1...\n");

	OSTimeDly(3);
	while(1)
	{
		SysTest1++;
		mytask1 = ~mytask1;
		GPIOPinWrite(GPIO_TEST_OUT_BASE, GPIO_TEST1_OUTPUT,SysTest1<<1);	
		GPIOPinWrite(GPIO_LED_PORT_BASE, GPIO_LED_PIN2,mytask1);
		printf("My_Task1 Running...%d,%#x...\n",&mytask1,mytask1);
		OSTimeDly(OS_TICKS_PER_SEC);
	}
}

void* My_Task2()
{
	static INT32U mytask2 = GPIO_LED_PIN3;
	SysTest2 = 1;

	printf("My_Task2 Starting...9...\n");
	printf("My_Task2 Starting...8...\n");
	printf("My_Task2 Starting...7...\n");
	printf("My_Task2 Starting...6...\n");
	printf("My_Task2 Starting...5...\n");
	printf("My_Task2 Starting...4...\n");
	printf("My_Task2 Starting...3...\n");
	printf("My_Task2 Starting...2...\n");
	printf("My_Task2 Starting...1...\n");

	OSTimeDly(5);
	while(1)
	{
		SysTest2++;
		mytask2 = ~mytask2;
		GPIOPinWrite(GPIO_TEST_OUT_BASE, GPIO_TEST2_OUTPUT,SysTest2<<2);
		GPIOPinWrite(GPIO_LED_PORT_BASE, GPIO_LED_PIN3,mytask2);		
		printf("My_Task2 Running...%d,%#x...\n",&mytask2,mytask2);
		OSTimeDlyHMSM(0,0,2,0);
	}
}

void* FatherTask()
{
	BSP_SysTickInit();
	INT16S test = 0;
	SysTest3 = 1;
	printf("FatherTask Starting...\n");
	OSTaskCreate(My_Task1,
				 0,
				 &My_Task1Stack[TASK_STK_SIZE - 1],
				 1);
	OSTaskCreate(My_Task2,
				 0,
				 &My_Task2Stack[TASK_STK_SIZE - 1],
				 2);
	OSTimeDly(7);
	while(1)
	{
		SysTest3++;
		GPIOPinWrite(GPIO_TEST_OUT_BASE, GPIO_TEST3_OUTPUT,SysTest3<<3);
		printf("FatherTask Running...\n");
		OSTimeDlyHMSM(0,0,0,500);
	}
}

void main()
{
	Board_Init();
	BSP_PENDSVSetPrio(NVIC_PENDSV_PRI);
	BSP_TestOutPutInit();
	
	OSInit();
	printf("OS Initialize Done\n");
	
	OSTaskCreate(FatherTask,
				 0,
				 &FatherTaskStack[TASK_STK_SIZE - 1],
				 3);
	OSStart();

}

