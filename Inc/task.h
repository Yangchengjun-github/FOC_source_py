#ifndef __TASK_H__
#define __TASK_H__

#include "stdint.h"
#include "stdbool.h"
#include "string.h"


#define USE_HAL                             (1)                                     //使用Hal库
#define DEBUG_LOG_EN                		(1)           							//1:使用颜色LOG 0:不用LOG

#if (USE_HAL)
#include "main.h"
#endif

#define C_NONE "\033[m"
#define C_RED "\033[0;32;31m"       //红色
#define C_LIGHT_RED "\033[1;31m"    //浅红
#define C_GREEN "\033[0;32;32m"     //绿色
#define C_LIGHT_GREEN "\033[1;32m"  //浅绿
#define C_BLUE "\033[0;32;34m"      //蓝色
#define C_LIGHT_BLUE "\033[1;34m"   //浅蓝
#define C_DARY_GRAY "\033[1;30m"    //深灰
#define C_CYAN "\033[0;36m"         //蓝绿
#define C_LIGHT_CYAN "\033[1;36m"   //浅青绿
#define C_PURPLE "\033[0;35m"       //紫色
#define C_LIGHT_PURPLE "\033[1;35m" //浅紫
#define C_BROWN "\033[0;33m"        //棕色
#define C_YELLOW "\033[1;33m"       //黄色
#define C_LIGHT_GRAY "\033[0;37m"   //浅灰
#define C_WHITE "\033[1;37m"        //白色


#if DEBUG_LOG_EN
#define LOG_W(M, ...)  printf("\r\n"C_YELLOW "[%s:%4d]" M "\r\n" C_NONE, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_I(M, ...)  printf("\r\n"C_WHITE "[%s:%4d]" M "\r\n" C_NONE, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_V(M, ...)  printf("\r\n"C_GREEN "[%s:%4d]" M "\r\n" C_NONE, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_E(M, ...)  printf("\r\n"C_RED "[%s:%4d]" M "\r\n" C_NONE, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_D(M, ...)  printf("\r\n"C_BLUE "[%s:%4d]" M "\r\n" C_NONE, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#if USE_HAL
#define LOG_T_W(M, ...)  printf("\r\n"C_YELLOW "[%s:%4d](%d)" M "\r\n" C_NONE, __FUNCTION__, __LINE__, HAL_GetTick(),##__VA_ARGS__)
#define LOG_T_I(M, ...)  printf("\r\n"C_WHITE "[%s:%4d](%d)" M "\r\n" C_NONE, __FUNCTION__, __LINE__,HAL_GetTick(), ##__VA_ARGS__)
#define LOG_T_V(M, ...)  printf("\r\n"C_GREEN "[%s:%4d](%d)" M "\r\n" C_NONE, __FUNCTION__, __LINE__,HAL_GetTick(), ##__VA_ARGS__)
#define LOG_T_E(M, ...)  printf("\r\n"C_RED "[%s:%4d](%d)" M "\r\n" C_NONE, __FUNCTION__, __LINE__,HAL_GetTick(), ##__VA_ARGS__)
#define LOG_T_D(M, ...)  printf("\r\n"C_BLUE "[%s:%4d](%d)" M "\r\n" C_NONE, __FUNCTION__, __LINE__, HAL_GetTick(),##__VA_ARGS__)
#else 
#define LOG_T_W(M, ...)  printf("\r\n"C_YELLOW "[%s:%4d](%d)" M "\r\n" C_NONE, __FUNCTION__, __LINE__, getSysTick(),##__VA_ARGS__)
#define LOG_T_I(M, ...)  printf("\r\n"C_WHITE "[%s:%4d](%d)" M "\r\n" C_NONE, __FUNCTION__, __LINE__,getSysTick(), ##__VA_ARGS__)
#define LOG_T_V(M, ...)  printf("\r\n"C_GREEN "[%s:%4d](%d)" M "\r\n" C_NONE, __FUNCTION__, __LINE__,getSysTick(), ##__VA_ARGS__)
#define LOG_T_E(M, ...)  printf("\r\n"C_RED "[%s:%4d](%d)" M "\r\n" C_NONE, __FUNCTION__, __LINE__,getSysTick(), ##__VA_ARGS__)
#define LOG_T_D(M, ...)  printf("\r\n"C_BLUE "[%s:%4d](%d)" M "\r\n" C_NONE, __FUNCTION__, __LINE__, getSysTick(),##__VA_ARGS__)
#endif /*USE_HAL*/
#else
#define LOG_W(M, ...) 
#define LOG_I(M, ...) 
#define LOG_V(M, ...)  
#define LOG_E(M, ...) 
#define LOG_D(M, ...) 
#define LOG_T_W(M, ...)  
#define LOG_T_I(M, ...) 
#define LOG_T_V(M, ...)  
#define LOG_T_E(M, ...) 
#define LOG_T_D(M, ...) 
#endif /*DEBUG_LOG_EN*/

typedef struct
{
    uint32_t currentTick;
    uint32_t lastTick;
    uint32_t period;
    void(*fun)(void);
}taskCb_t;

#define MAX_TASK_NUM            7 

extern bool taskRegister(void(*cb)(),uint32_t period_Ms);
extern bool taskUnRegister(void(*cb)());
extern bool taskRun(void);
#if (!USE_HAL)
extern uint32_t getSysTick(void);
#endif

#endif 

