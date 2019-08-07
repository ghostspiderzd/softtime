#ifndef __SOFTTIME_H
#define __SOFTTIME_H




#define ERROR_OFB 0xFF
#define ERROR_DNE 0xFE


#define TIMER_NUM 10






typedef void callback(void);

typedef enum
{
	SOFT_TIMER_STOPPED = 0,
	SOFT_TIMER_RUNNING,
	SOFT_TIMER_TIMEOUT,
}softtime_state;

typedef enum
{
	MODE_ONE_SHOT = 0,
	MODE_PERIODIC,
}softtime_mode;

typedef struct  
{
	softtime_state state;           //状态
	softtime_mode mode;            //模式
	unsigned int match;          //到期时间
	unsigned int period;         //定时周期
	callback* handler;            //回调函数指针
}softTimer;



unsigned int SysTick_Get(void);
void softTimer_Init(void);
unsigned char softTimer_Create(unsigned char id, tmrMode mode, unsigned int delay, callback* handler);
void softTimer_Update(void);
unsigned char softTimer_GetState(unsigned char id);
void softTimer_Delete(unsigned char id);

#endif
