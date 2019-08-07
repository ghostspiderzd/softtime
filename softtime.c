#include "softtime.h"

static softTimer softtimer[TIMER_NUM]; 

unsigned int SysTick_Get(void)
{
	/*  */
}

/*
 soft timer init
 para: timer 		: 	Timer array
 	   timer_size 	: 	timer array size
*/
void softTimer_Init(void)
{
	unsigned char i;
	for(i = 0; i < TIMER_NUM; i++) 
	{
		softtimer[i].state = SOFT_TIMER_STOPPED;
		softtimer[i].mode = MODE_ONE_SHOT;
		softtimer[i].match = 0;
		softtimer[i].period = 0;
		softtimer[i].handler = NULL;
	}
}

/*
 para： id		:	timer ID,When id == 0, an id is randomly assigned and the assigned id is returned.
		mode	:	run mode
		delay	:	time
		cb		: 	call-back function 
		argv	: 	call-back function para
		argc	: 	call-back function num.
 return : Returns the id that was created successfully
*/
unsigned char softTimer_Create(unsigned char id, softtime_mode mode, unsigned int delay, callback* handler)
{
	unsigned char i = 0;
	
	if(id >= TIMER_NUM) return ERROR_OFB;
	if(mode != MODE_ONE_SHOT && mode != MODE_PERIODIC) return ERROR_DNE;
	/* 
		Random allocation ：Query the timer that does not currently return the function
	*/
	if(id == 0)
	{
		for(i==0;i<TIMER_NUM,i++)
		{
			if(softtimer[i].handler == NULL)
			{
				softtimer[i].handler = handler;
				break;
			}
		}
		softtimer[i].match = SysTick_Get() + delay;
		softtimer[i].period = delay;
		softtimer[i].state = SOFT_TIMER_RUNNING;
		softtimer[i].mode = mode;
		return i;
	}
	else
	{
		softtimer[id].handler = handler;
		softtimer[id].match = SysTick_Get() + delay;
		softtimer[id].period = delay;
		softtimer[id].state = SOFT_TIMER_RUNNING;
		softtimer[id].mode = mode;
		return id;
	}
}

void softTimer_Update(void)
{
	static unsigned char i = 0;

	if(i < TIMER_NUM)
	{
		switch (softtimer[i].state) {
          case SOFT_TIMER_STOPPED:
			  break;
		
		  case SOFT_TIMER_RUNNING:
			  if(softtimer[i].match <= SysTick_Get()) {
				  softtimer[i].state = SOFT_TIMER_TIMEOUT;
				  softtimer[i].handler();
			  }
			  break; //? 这里如果break,那么刚执行结束的这个函数，要等到一轮结束后，下一轮到达的时候，才会重新计算时间
		  case SOFT_TIMER_TIMEOUT:
			  if(softtimer[i].mode == MODE_ONE_SHOT) {
			      softtimer[i].state = SOFT_TIMER_STOPPED;
			  } else {
				  softtimer[i].match = SysTick_Get() + softtimer[i].period;
			      softtimer[i].state = SOFT_TIMER_RUNNING;
			  }
			  break;
		  default:
			 // printf("timer[%d] state error!\r\n", i);
			  break;
	  }
		i++;
	}
	else
	{
		i = 0;
	}
}

/*
	Return the status of a timer
*/
unsigned char softTimer_GetState(unsigned char id)
{
	return softtimer[id].state;
}

/*
	Delete an id 
 */
void softTimer_Delete(unsigned char id)
{
	softtimer[id].state = SOFT_TIMER_STOPPED;
	softtimer[id].handler = NULL;
}

