/*
 *
 *
 *  Created on: Sep 28, 2016
 *      Author: Zhentao Xie
 */
/*
#ifndef MODULE
#define MOUDLE
#endif

#ifndef __KERNEL__
#define __KERNEL__
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <rtai.h>
#include <rtai_sched.h> //scheduler
#include <rtai_sem.h>
#include "ece4220lab3.h"

MODULE_LICENSE("GPL");
*/


#ifndef MODULE
#define MODULE
#endif
#ifndef __KERNEL__
#define __KERNEL__
#endif
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <rtai_sched.h>/home/zxdhf/workspace/Lab3P2/Release
#include <rtai_fifos.h>
#include <linux/time.h>
#include <rtai.h>
#include "ece4220lab3.h"
#include <rtai_sem.h>

MODULE_LICENSE("GPL");

SEM light_sem;
static RT_TASK task1, task2, task3;
RTIME period;

/*
unsigned long *PBDR,*PBDDR;  //create pointer to port B DR/DDR
unsigned long *ptr;
ptr = (unsigned long*)__ioremap(0x80840000,4096,0);
PBDR = ptr+1;
PBDDR = ptr+5;
*PBDDR = 0xE0;  // give LEDs output, buttons input. pbddr 11100000
*/

static void rt_process1(int t)
{
	unsigned long *PBDR,*PBDDR;  //create pointer to port B DR/DDR
	unsigned long *ptr;
    ptr = (unsigned long*)__ioremap(0x80840000,4096,0);
	PBDR = ptr+1;
	PBDDR = ptr+5;
	*PBDDR = 0xE0;  // give LEDs output, buttons input. pbddr 11100000
  while(1)
  {
	rt_sem_wait(&light_sem);
    *PBDR |= 0x20; //00100000 Turn on red PortB5
    rt_sleep(2000*period); //RTIME delay
    *PBDR &= 0xDF; //11011111 Turn off red PortB5
    rt_sem_signal(&light_sem);
  }

}

static void rt_process2(int t)
{
	unsigned long *PBDR,*PBDDR;  //create pointer to port B DR/DDR
	unsigned long *ptr;
	ptr = (unsigned long*)__ioremap(0x80840000,4096,0);
	PBDR = ptr+1;
	PBDDR = ptr+5;
	*PBDDR = 0xE0;  // give LEDs output, buttons input. pbddr 11100000
  while(1)
  {
	rt_sem_wait(&light_sem);
    *PBDR |= 0x40; //01000000 Turn on yellow PortB6
    rt_sleep(2000*period); //RTIME delay
    *PBDR &= 0xBF; //10111111 Trun off yellow PortB6
    rt_sem_signal(&light_sem);
  }
}

static void rt_process3(int t)
{
	unsigned long *PBDR,*PBDDR;  //create pointer to port B DR/DDR
	unsigned long *ptr;
	ptr = (unsigned long*)__ioremap(0x80840000,4096,0);
	PBDR = ptr+1;
	PBDDR = ptr+5;
	*PBDDR = 0xE0;  // give LEDs output, buttons input. pbddr 11100000
  while(1)
 {
	rt_sem_wait(&light_sem);
    if(check_button())
    {
      *PBDR |= 0x80; //10000000 Turn on green PortB7
      rt_sleep(3000*period); //RTIME delay
      *PBDR &= 0x7F; //01111111 Turn off green PortB7
      clear_button();
    }
    rt_sem_signal(&light_sem);
  }
}

int init_module(void)
{
  rt_sem_init(&light_sem,1);
  rt_set_periodic_mode(); //set to periodic mode
  period = start_rt_timer(nano2count(1000000));

  rt_task_init(&task1,rt_process1,0,256,2,0,0);
  rt_task_resume(&task1);

  rt_task_init(&task2,rt_process2,0,256,1,0,0);
  rt_task_resume(&task2);

  rt_task_init(&task3,rt_process3,0,256,0,0,0);
  rt_task_resume(&task3);


  return 0;
}

void cleanup_module(void)
{

  rt_sem_delete(&light_sem);
  rt_task_delete(&task1);
  rt_task_delete(&task2);
  rt_task_delete(&task3);
  stop_rt_timer();

  unsigned long *PBDR,*PBDDR;  //create pointer to port B DR/DDR
    unsigned long *ptr;
    ptr = (unsigned long*)__ioremap(0x80840000,4096,0);
    PBDR = ptr+1;
    PBDDR = ptr+5;
    *PBDDR = 0xE0;  // give LEDs output, pbddr 11100000
    *PBDR &= 0x1F; //Turn off red yello green LEDs
}
