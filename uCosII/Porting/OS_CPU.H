/*
*********************************************************************************************************
*                                               uC/OS-II
*                                        The Real-Time Kernel
*
*
*   Reference Doc:  Doc0:Compiler SPNU151H
*   				Doc1:Datasheet SPMS211L
*   				Doc2:Instruction Set
*   				Doc3:Driver code
*
*                                       LM3S5956 Specific code
*
*
*                                          Borland C/C++ V4.51
*
* File         : OS_CPU.H
* By           : Stone903
*********************************************************************************************************
*/

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*                             we can get this info from the compiler document
*                             section 6: run-time Environment P117
*
*********************************************************************************************************
*/

typedef unsigned char    BOOLEAN;
typedef unsigned char    INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char    INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int     INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int  	 INT32S;                   /* Signed   32 bit quantity                           */
typedef float          	 FP32;                     /* Single precision floating point                    */
typedef double         	 FP64;                     /* Double precision floating point                    */

typedef unsigned int   OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef unsigned short OS_CPU_SR;                /* Define size of CPU status register (PSR = 32 bits) */

#define BYTE           INT8S                     /* Define data types for backward compatibility ...   */
#define UBYTE          INT8U                     /* ... to uC/OS V1.xx.  Not actually needed for ...   */
#define WORD           INT16S                    /* ... uC/OS-II.                                      */
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U

/* 
*********************************************************************************************************
*                              LM3S5956
*
* Method #1:  Disable/Enable interrupts using simple instructions.  After critical section, interrupts
*             will be enabled even if they were disabled before entering the critical section.
*
* Method #2:  Disable/Enable interrupts by preserving the state of interrupts.  In other words, if 
*             interrupts were disabled before entering the critical section, they will be disabled when
*             leaving the critical section.
*
* Method #3:  Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*             would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*             disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to 
*             disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*             into the CPU's status register.
*********************************************************************************************************
*/
#define  OS_CRITICAL_METHOD    1

#if      OS_CRITICAL_METHOD == 1								  /* Doc2:P210 								 */
#define  OS_ENTER_CRITICAL()  __asm("	CPSID i")                    /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   __asm("	CPSIE i")                    /* Enable  interrupts                        */
#endif

#if      OS_CRITICAL_METHOD == 2
#define  OS_ENTER_CRITICAL()  __asm(" MRS R0,PRIMASK\n PUSH {R0}\n CPSID i\n BX LR")           /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   __asm(" POP {R0}\n MSR PRIMASK,R0\n CPSIE i\n BX LR")                    /* Enable  interrupts                        */
#endif

#if      OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  (cpu_sr = CPUcpsid())    /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   (OS_CPUcpsie(cpu_sr))    /* Enable  interrupts                        */
#endif

/*
*********************************************************************************************************
*                           				LM3S5956 Miscellaneous
*********************************************************************************************************
*/

#define  OS_STK_GROWTH        1                       /* Stack grows from HIGH to LOW memory on 80x86  */

#define  OS_TASK_SW()         OSCtxSw()
/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/

OS_CPU_EXT  INT8U  OSTickDOSCtr;       /* Counter used to invoke DOS's tick handler every 'n' ticks    */

/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/

#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
void       OSCPURestoreSR(OS_CPU_SR cpu_sr);
#endif

void *OSTimeTickIsr(void);


