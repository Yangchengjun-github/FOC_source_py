/********************************************************************************
File:			task.c
Author:			songlianjie
Date:			2021-04-19
Version:		V001
Description:				
*********************************************************************************/
#include "task.h"
#if (!USE_HAL)
static uint32_t _systickCounter;
#endif

static taskCb_t taskTab[MAX_TASK_NUM]={0};
/********************************************************************************
Description:  系统滴答中断 1ms
Input:			
Output:			
Return:			
*********************************************************************************/
#if (!USE_HAL)
void SysTick_Handler(void)
{
    _systickCounter++;
}
#endif
/********************************************************************************
Description:  获取系统滴答
Input:			
Output:			
Return:			
*********************************************************************************/
#if (!USE_HAL)
uint32_t getSysTick(void)
{
    return _systickCounter;
}
#endif
/********************************************************************************
Description:    注册任务
Input:			period_Ms 任务函数调用的周期，单位为ms
Output:			
Return:			
*********************************************************************************/
bool taskRegister(void(*cb)(),uint32_t period_Ms)
{
    uint8_t i;
    for (i = 0; i < (sizeof(taskTab) / sizeof(taskCb_t)); i++)
    {
        if(taskTab[i].fun == NULL)
        {   
            taskTab[i].fun = cb;
            taskTab[i].period = period_Ms;
            return true;
        }
    }
    return false;
}
/********************************************************************************
Description:    注销任务
Input:			
Output:			
Return:			
*********************************************************************************/
bool taskUnRegister(void(*cb)())
{
    uint8_t i;
    for (i = 0; i < (sizeof(taskTab) / sizeof(taskCb_t)); i++)
    {
        if(taskTab[i].fun == cb)
        {   
            memset((void *)&taskTab[i],0,sizeof(taskCb_t));
            return true;
        }
    }
    return false;
}
/********************************************************************************
Description:    运行任务
Input:			
Output:			
Return:			
*********************************************************************************/
bool taskRun(void)
{
    uint8_t i;
    for (i = 0; i < (sizeof(taskTab) / sizeof(taskCb_t)); i++)
    {
        if(taskTab[i].fun != NULL)
        {   
			#if (!USE_HAL)
            taskTab[i].currentTick = _systickCounter;
            #else 
            taskTab[i].currentTick = HAL_GetTick();
            #endif
            if(taskTab[i].currentTick > taskTab[i].lastTick)
            {
                if( taskTab[i].currentTick - taskTab[i].lastTick >= taskTab[i].period )
                {
                   taskTab[i].lastTick = taskTab[i].currentTick;
                   taskTab[i].fun();
                   return true;   
                }
            }       
        }
    }
    return false;
}

