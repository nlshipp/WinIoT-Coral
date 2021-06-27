/* main.c
 *
 * Copyright (C) 2006-2017 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */

#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#include "wolfssl/wolfcrypt/settings.h"

#include "cmsis_os.h"                 /* CMSIS RTOS definitions             */
#include "rl_net.h"                      /* Network definitions                */
#include <time.h>

#if defined(STM32F7xx)
#include "stm32f7xx_hal.h"
#elif defined(STM32F4xx)
#include "stm32f4xx_hal.h"
#elif defined(STM32F2xx)
#include "stm32f2xx_hal.h"
#endif

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

//   <h>Remote Address
//   ====================
//
//     <s.15>IP Address
//     <i>Static IPv4 Address
//     <i>Default: "192.168.1.1"
#define REMOTE_IP "192.168.10.4"

//   <s.6>Port
//   <i> Default: "11111"
#define REMOTE_PORT "11111"
//   </h>

//   <h>Protocol
//   ====================

//   <o>SSL/TLS Version<0=> SSL3 <1=> TLS1.0 <2=> TLS1.1 <3=> TLS1.2 <4=> TLS1.3
#define TLS_VER 3

//   <s.2>Other option
#define OTHER_OPTIONS ""
//   </h>

//   <h>RTC: for validate certificate date
//    <o>Year <1970-2099>
#define RTC_YEAR 2018
//    <o>Month <1=>Jan<2=>Feb<3=>Mar<4=>Apr<5=>May<6=>Jun<7=>Jul<8=>Aut<9=>Sep<10=>Oct<11=>Nov<12=>Dec
#define RTC_MONTH 1
//    <o>Day <1-31>
#define RTC_DAY 1
//    </h>

//------------- <<< end of configuration section >>> -----------------------
#warning "write MPU specific Set ups\n"
static void SystemClock_Config (void) {

}

static void MPU_Config (void) {

}

static void CPU_CACHE_Enable (void) {

}


/*-----------------------------------------------------------------------------
 *        Initialize a Flash Memory Card
 *----------------------------------------------------------------------------*/
#if !defined(NO_FILESYSTEM)
#include "rl_fs.h"                      /* FileSystem definitions             */

static void init_filesystem (void) {
  int32_t retv;

  retv = finit ("M0:");
  if (retv == fsOK) {
    retv = fmount ("M0:");
    if (retv == fsOK) {
      printf ("Drive M0 ready!\n");
    }
    else {
      printf ("Drive M0 mount failed(%d)!\n", retv);
    }
  }
  else {
    printf ("Drive M0 initialization failed!\n");
  }
}
#endif


void net_loop(void const *arg)
{
    while(1) {
        net_main ();
        osThreadYield ();
    }
}

osThreadDef(net_loop, osPriorityLow, 2, 0);

#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;
static  time_t epochTime;

uint32_t HAL_GetTick(void) {
    return os_time;
}

time_t time(time_t *t){
     return epochTime ;
}

void setTime(time_t t){
    epochTime = t;
}
#endif

#ifdef WOLFSSL_CURRTIME_OSTICK

#include <stdint.h>
extern uint32_t os_time;

double current_time(int reset)
{
    if(reset) os_time = 0 ;
    return (double)os_time /1000.0;
}

#else

#include <stdint.h>
#define DWT                 ((DWT_Type       *)     (0xE0001000UL)     )
typedef struct
{
  uint32_t CTRL;       /*!< Offset: 0x000 (R/W)  Control Register           */
  uint32_t CYCCNT;     /*!< Offset: 0x004 (R/W)  Cycle Count Register       */
} DWT_Type;

extern uint32_t SystemCoreClock ;

double current_time(int reset)
{
    if(reset) DWT->CYCCNT = 0 ;
    return ((double)DWT->CYCCNT/SystemCoreClock) ;
}
#endif

/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
 *---------------------------------------------------------------------------*/
#include <stdio.h>
typedef struct func_args {
    int    argc;
    char** argv;
} func_args;

extern void client_test(func_args * args) ;

int myoptind = 0;
char* myoptarg = NULL;

int main (void) {
    static char *argv[] =
        {   "client",   "-h", REMOTE_IP, "-p", REMOTE_PORT,
                                   "-v",  " ",  OTHER_OPTIONS } ;
    static   func_args args  =
        {  sizeof(argv)/sizeof(*argv[0]), argv } ;

    char *verStr[] = { "SSL3", "TLS1.0", "TLS1.1", "TLS1.2", "TLS1.3"};
    #define VERSIZE 2
    char ver[VERSIZE];

    MPU_Config();                             /* Configure the MPU              */
    CPU_CACHE_Enable();                       /* Enable the CPU Cache           */
    HAL_Init();                               /* Initialize the HAL Library     */
    SystemClock_Config();                     /* Configure the System Clock     */

    #if !defined(NO_FILESYSTEM)
    init_filesystem ();
    #endif
    net_initialize ();

    #if defined(DEBUG_WOLFSSL)
        printf("Turning ON Debug message\n") ;
        wolfSSL_Debugging_ON() ;
    #endif

    snprintf(ver, VERSIZE, "%d", TLS_VER);
    argv[6] = ver;

    printf("SSL/TLS Client(%d)\n ", sizeof(argv)/sizeof(argv[0])) ;
    printf("    Remote IP: %s, Port: %s\n    Version: %s\n", argv[2], argv[4],  verStr[TLS_VER]) ;
    printf("    Other options: %s\n", OTHER_OPTIONS);
    setTime((time_t)((RTC_YEAR-1970)*365*24*60*60) + RTC_MONTH*30*24*60*60 + RTC_DAY*24*60*60);

    osThreadCreate (osThread(net_loop), NULL);

    client_test(&args) ;

    while(1)
        osDelay(1000);

}
