#ifndef MODULE
#define MODULE
#endif
#ifndef __KERNEL__
#define __KERNEL__
#endif
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <rtai_sched.h>
#include <rtai_fifos.h>
#include <linux/time.h>
#include <rtai.h>
#include "ece4220lab3.h"
#include <rtai_sem.h>

MODULE_LICENSE("GPL");

static RT_TASK Trafic_task;
RTIME period;

static void rt_process(int t) //trafic light
{
	unsigned long *PBDR,*PBDDR;  //create pointer to port B DR/DDR
	unsigned long *ptr;
	ptr = (unsigned long*)__ioremap(0x80840000,4096,0);
	PBDR = ptr+1;
	PBDDR = ptr+5;
	*PBDDR = 0xE0;  // give LEDs output, buttons input. pbddr 11100000

	while(1)
	{
		*PBDR |= 0x20; //00100000 Turn on red PortB5
		rt_sleep(2000*period); //RTIME delay
		*PBDR &= 0xDF; //11011111 Turn off red PortB5

		*PBDR |= 0x40; //01000000 Turn on yellow PortB6
		rt_sleep(2000*period); //RTIME delay
		*PBDR &= 0xBF; //10111111 Trun off yellow PortB6

		if(check_button())
		{
			*PBDR |= 0x80; //10000000 Turn on green PortB7
			rt_sleep(3000*period); //RTIME delay
			*PBDR &= 0x7F; //01111111 Turn off green PortB7
			clear_button();
		}
	}

}

int init_module(void)
{
	rt_set_periodic_mode(); //set to periodic mode
	period = start_rt_timer(nano2count(1000000));
	rt_task_init(&Trafic_task,rt_process,0,256,0,0,0);
	rt_task_resume(&Trafic_task);
	return 0;
}

void cleanup_module(void)
{

	    rt_task_delete(&Trafic_task);  //delete real time task
	    stop_rt_timer();          //stop timer

	    unsigned long *PBDR,*PBDDR;  //create pointer to port B DR/DDR
	    unsigned long *ptr;
	    ptr = (unsigned long*)__ioremap(0x80840000,4096,0);
	    PBDR = ptr+1;
	    PBDDR = ptr+5;
	    *PBDDR = 0xE0;  // give LEDs output, pbddr 11100000
	    *PBDR &= 0x1F; //Turn off red yello green LEDs
}
