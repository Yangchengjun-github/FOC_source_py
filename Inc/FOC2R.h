/**
  ******************************************************************************
  * @file    FOC2R.h
  * @author  MCU Application Team
  * @Version V1.0.0
  * @Date    2023-03-01
  * @brief   BLDC FOC Sensorless under 1R Sensing Current
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FOC2R_H
#define __FOC2R_H

#include "py32f072_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MOT_DIR_CWC		0
#define MOT_DIR_CCW		1

#define MOT_RUN_STOPING						0x00
#define MOT_RUN_CWC_DIR						0x11
#define MOT_RUN_CCW_DIR						0x22
#define MOT_RUN_CHANGE_DIR					0x33
	
#define MCU_MAIN_FREQUANCY					72000000	//Hz
#define MOTOR_PWM_FREQUANCY					20000//30000//25000	//Hz
#define MOTOR_PWM_TIM1_DUTY					(MCU_MAIN_FREQUANCY/MOTOR_PWM_FREQUANCY/2)
#define MOTOR_PWM_TIM1_ISR_TH				(MOTOR_PWM_TIM1_DUTY-100)
	
#define LL_GPIO_PULL_NO                    (0x00000000U)      /*!< Select I/O no pull */
#define LL_GPIO_PULL_UP                    GPIO_PUPDR_PUPD0_0 /*!< Select I/O pull up */
#define LL_GPIO_PULL_DOWN                  GPIO_PUPDR_PUPD0_1 /*!< Select I/O pull down */

#define LL_GPIO_MODE_INPUT                 (0x00000000U)       /*!< Select input mode */
#define LL_GPIO_MODE_OUTPUT                GPIO_MODER_MODE0_0  /*!< Select output mode */
#define LL_GPIO_MODE_ALTERNATE             GPIO_MODER_MODE0_1  /*!< Select alternate function mode */
#define LL_GPIO_MODE_ANALOG                GPIO_MODER_MODE0    /*!< Select analog mode */
//PB07--PWMU+; PB03--PWMV+; PB06--PWMW+; 
//PF01--PWMU-; PB04--PWMV-; PB05--PWMW-;
#define PwmUH_ONN	WRITE_REG(GPIOB->BSRR, GPIO_PIN_7)	
#define PwmVH_ONN	WRITE_REG(GPIOB->BSRR, GPIO_PIN_3)
#define PwmWH_ONN	WRITE_REG(GPIOB->BSRR, GPIO_PIN_6)
#define PwmUH_OFF	WRITE_REG(GPIOB->BRR, GPIO_PIN_7)	
#define PwmVH_OFF	WRITE_REG(GPIOB->BRR, GPIO_PIN_3)	
#define PwmWH_OFF	WRITE_REG(GPIOB->BRR, GPIO_PIN_6)			
#define PwmUL_ONN	WRITE_REG(GPIOF->BSRR, GPIO_PIN_1)	
#define PwmVL_ONN	WRITE_REG(GPIOB->BSRR, GPIO_PIN_4)		
#define PwmWL_ONN	WRITE_REG(GPIOB->BSRR, GPIO_PIN_5)
#define PwmUL_OFF	WRITE_REG(GPIOF->BRR, GPIO_PIN_1)	
#define PwmVL_OFF	WRITE_REG(GPIOB->BRR, GPIO_PIN_4)	
#define PwmWL_OFF	WRITE_REG(GPIOB->BRR, GPIO_PIN_5)

#define abs(x) ( x < 0 ? ( -x ) : ( x ) )

#define PWM_IS_ONN		1
#define PWM_IS_OFF		0
	
//JHX 47uf
#define VDC_GAIN_COF			2535	
#define VDC_OFSET				12	

//JKR 33uf
// #define VDC_GAIN_COF			2267	
// #define VDC_OFSET				11


#define MIN_RECOVERY_VDC_TH		3950		//3900 错误恢复最电压（持续）

#define MAX_VDC_THRESHOLD		4100//3800	//3600//380.0VDC
#define MIN_VDC_THRESHOLD		1500	//250.0VDC
#define MIN_VDC_TH				550		//500//300//30.0VDC

#define MAX_IS					25000	//1900//2218//250
#define MOTOR_STALL_MAXCNT		65000
#define MAX_CUR_THRESHOLD		MAX_IS

#define MAX_TEMP_TH				600 	//max-50.0Degree
	
#define MOTOR_NO_ERROR			0x0000
#define MOTOR_SOC_ERROR			0x0001	//软件过流
#define MOTOR_HOC_ERROR			0x0002	//硬件过流
#define MOTOR_OV_ERROR			0x0004	//软件过压
#define MOTOR_LV_ERROR			0x0008	//软件欠压
#define MOTOR_OH_ERROR			0x0010	//温度过高
#define MOTOR_STALL_ERROR		0x0020	//堵转错误
#define MOTOR_OVSPD_ERROR		0x0040	//超速
#define MOTOR_Brokn_ERROR		0x0080	//刹车
#define MOTOR_VDC_ERROR			0x0100	//
#define MOTOR_ELSE_ERROR		0x0200	//其他错误
#define NTC_OPEN_ERROR			0x0400	//NTC开路错误
#define NTC_SHORT_ERROR			0x0800	//NTC短路错误

//#define HIGH_SPEED_CMD		15550 	//105000rpm
#define HIGH_SPEED_CMD			15000 	//102000rpm    (15000/32768)*3700Hz*60=RPM
#define MIDL_SPEED_CMD			14083 	//096000rpm
#define LOWW_SPEED_CMD			12432 	//12290 //086000rpm  //12432 // 87000rpm

#define PWM_OUT_IS_ONN			0x1		//PwmOnOfState
#define PWM_OUT_IS_OFF			0x2

/** @defgroup Exported_types  Exported_types
* @{
*/
#define STATE_POWERON           	0000   // System POWERON
#define STATE_STOP_NOMARL           0001   // Stopped without error
#define STATE_STOP_ERROR            0002   // Stopped with error
#define STATE_StartingUp			9001
#define STATE_StableRun				9002
#define STATE_SpdToPwmStp			9003
#define STATE_STOP_NTC_ERROR		9004 	//NTC引起的错误

/** 
  * @brief FOC2R parameters  
  */
typedef struct{    
	  ULONG_UNION ulSitaPu;   	//00    Q15
    UWORD uwSitaPu;   			//00    Q15
    SWORD swSinPu;    			//02    Q15
    SWORD swCosPu;    			//04    Q15
} SIN_COS;	

/** 
  * @brief FOC2R parameters  
  */
typedef struct
{
	int16_t		PwmOnOfState;
	int16_t		VdcAdc;
	int16_t		IdcAdc;
	int16_t		IbusAdc0;
	int16_t		IbusAdc1;
	int16_t		IbusAdc2;
	int16_t		VTmpAdc;
	int16_t		NtcAdc;
	int16_t		IdcLpfCof;
	int16_t		NtcTemp;		//温度C

	SLONG_UNION		slIdcLpf;
	SLONG_UNION		slVdcLpf;
	SLONG_UNION		slVTmpLpf;
	SLONG_UNION		slVdcMean;
	int16_t		TmpX10Degree;
	int16_t   InPutPower;
	uint32_t	Acc_cnt;
	//uint16_t	clk_Secnd;	
	//uint16_t	clk_cnt;	
	uint16_t	Iqclk_cnt;	
	uint32_t	PwmIsr_cnt;	
	uint32_t	PwmIsr_cnt2;	
	
	uint16_t	SysStateId;		
	uint16_t	MotorErrCode;
	int16_t   FulCyCle;
	int16_t   PWM_Status;
	int16_t   PwmIsr_Tmp;	
	int16_t		AdcIb;
	int16_t		Ia;
	int16_t		Ib;	
	int16_t		Ic;
	int16_t		Ialfa;
	int16_t		Ibeta;
	int16_t		Id;
	int16_t		Iq;
	int16_t		wSin;
	int16_t		wCos;	
	int16_t 	Vd;
	int16_t 	Vq;	
	int16_t 	EsFdwCmd;
	int16_t 	EsMinTh;
	int16_t 	EsMaxTh;
	int16_t 	EsTmp;	
	int16_t 	Valfa;
	int16_t 	Vbeta;		
	int16_t 	ValfaLpf;
	int16_t 	VbetaLpf;		
	int16_t		Va;
	int16_t		Vb;
	int16_t		Vc;	
	int16_t		MinVs;
	int16_t		MaxVs;
	int16_t		ZerVs;
	int16_t		dutyVa;
	int16_t		dutyVb;
	int16_t		dutyVc;	
	uint16_t	DutyA;
	uint16_t	DutyB;
	uint16_t	DutyC;

	uint16_t 	SectID;
	int16_t		Ialfa0;
	int16_t		AdcIb0;
	int16_t		DutyMax;
	int16_t		DutyMid;
	
	int16_t 	ErrPLLMinTh;
	int16_t 	ErrPLLMaxTh;
	int16_t		ErrPLLAcc;
	
	int16_t swLpfCofPu;
	SLONG_UNION		slIalfaAdc;
	SLONG_UNION		slIbetaAdc;
	
	//int16_t Cnt_1ms;
	//int16_t Cnt_SW1;
	// int16_t testCode;

	uint32_t errorRecoverCnt;//错误后恢复时间计数
	
	int16_t HOC_Cnt;
	int16_t stable_Cnt;
	int16_t VoltFeedfwdCof;
	int16_t VoltFeedfwdCofCnt;
	int16_t tempVal1;
	int16_t DecWeCmd;
	
	int16_t 	CH4_Sample_ID;
	int16_t 	IbusMaxIndex;
	int16_t 	IbusMax;
	int16_t 	IbusMaxVal;
	SLONG_UNION slIbusMaxVal;
	SLONG_UNION slPowerPu;
	SLONG_UNION slPower2Pu;
	// int16_t SW1_Cmd;
	// int16_t SW1_ID;
	
	int16_t		MotDirCmd;
	int16_t		MotRunCmd;
	uint16_t	LopCnt;

	int16_t 	MaxVal;
	int16_t 	MaxIdcVal;
	int16_t 	MaxIdcIdx;
	uint16_t 	InitAngle[7];
	uint16_t	OnCnt;
	int16_t		ValIa;
	int16_t		ValIb;
	uint16_t	spd_cnt;
	
	uint16_t	VdcVoltX100;	

	uint16_t 	SpdLopCnt;
	int16_t		Tc2PI;
	int16_t		WeCmd;
	int16_t 	WeCmdChange;
	int16_t 	WeCmd0;
	int16_t 	DetaWeCmd;	
	// int16_t 	AccCtrlCnt;

	uint16_t	SpdRefCmd;
	uint16_t	SpdCnt;
	uint16_t	SpdCmdUnderVdc;
	
	SLONG_UNION 	slDuty;
	SLONG_UNION		slIalfa0;
	SLONG_UNION		slAdcIb0;	
	SLONG_UNION 	slValfa;
	SLONG_UNION 	slVbeta;
	SLONG_UNION 	slEsLpf;	
	ULONG_UNION 	CompVal;	
	int8_t   		Comp2Val;
	
	SLONG_UNION slVdLpf;
	SLONG_UNION slVqLpf;		
	
	uint8_t 	SixStep_ID;	

}PUYA_SEMI_MC_FOC1R_Base_TYPE;             /*!< FOC1R Data Structure */


typedef struct
{
	int16_t cnt;
	int16_t cntTh;
	int16_t tabID;
	int16_t WeCmd0;
	int16_t WeCmd;	
	int16_t DetaWeCmd;	
	int16_t state;
}PUYA_SEMI_MC_SCURVE_TYPE;

typedef struct
{
	uint16_t CZ_Cnt;
	uint16_t GridZero0;
	uint16_t GridZero1;
	uint16_t CZ_Cycle;
	uint16_t Status;
}PUYA_SEMI_GridOnOff_TYPE;
#define GRID_OFF 0
#define GRID_ONN 1

typedef struct
{
	int16_t		GridZero1;
	int16_t		GridZero0;
	int16_t		GridZeroCnt;
	int16_t		GridZeroTh;
	int16_t		GridTrigEn;
	int16_t		GridTrigEnCnt;
	int16_t		GridTrigOnTh;	
	int16_t		PTC_Rank;
	int16_t		PTC_RankCof;
}PUYA_SEMI_MC_PtcCtrl_TYPE;

typedef struct
{
	int16_t		Brk_Duty;
	int16_t		Brk_Cnt;
	int16_t		Brk_DutyCmd;
	int16_t		Brk_InitDutyCmd;//Brk_DutyCmd=600
	int16_t		Brk_IdleTh;//Brk_Cnt<400
	int16_t		Brk_PwmTh;//Brk_Cnt<4500)
	int16_t		Brk_AccCnt;
	int16_t		Brk_AccTh;//Brk_AccCnt>130)
	int16_t		Brk_AllTimeTh;//Brk_Cnt>12000)//9000)//8000)
}
PUYA_SEMI_MC_BrkStpCtrl_TYPE;

typedef struct
{
	int16_t swOut;
	
	int16_t Ref;
	int16_t Yk;
	
	int16_t ek;
	int16_t ek_1;	
	int16_t dek;
	
	int16_t Kp;
	int16_t Ki;	
	int16_t Max;
	int16_t Min;
	
	int16_t Cnt;
	SLONG_UNION slOut;
}PUYA_SEMI_MC_FOC1R_PI_TYPE;
/**
  * @} 
  */
extern PUYA_SEMI_MC_PtcCtrl_TYPE stPtc;
extern PUYA_SEMI_MC_BrkStpCtrl_TYPE stBrk;
extern SIN_COS SinCos;
extern PUYA_SEMI_MC_SCURVE_TYPE mc_stSCurve;
extern PUYA_SEMI_GridOnOff_TYPE stGrid;
extern const signed int ArBemfCof[];	

void voTabSinCos(SIN_COS *in);
void MC_HbdgIO_LbdgPwm(void);
void MC_6Step_Pwm_Io(void);
void MC_6Step_Io_Pwm(void);
void PUYA_SEMI_MC_FOC1R_INIT(void);

void PUYA_SEMI_MC_FOC1R_Schedule(void);
void PUYA_SEMI_MC_FOC1R_PWMCtrl(void);
void PUYA_SEMI_MC_FOC1R_IdcSample2(void);

void InnerComp_Init(void); 
void PUYA_SEMI_MC_MotorCtrl(void);
void PUYA_SEMI_MC_SCurve(PUYA_SEMI_MC_SCURVE_TYPE *s);
int16_t PUYA_SEMI_MC_VsCalc(int16_t We);

void PUYA_SEMI_MC_LpfCtrl(int16_t LowPasFilter_Cof);

void USART_Config(void);
void PUYA_SEMI_PtcCtrl(PUYA_SEMI_MC_PtcCtrl_TYPE *s);
void PUYA_SEMI_BrkStpCtrl(PUYA_SEMI_MC_BrkStpCtrl_TYPE *s);
void PwmIsrCallback(void);
void PUYA_SEMI_MC_FocPWM_OFF(void);
void PUYA_SEMI_MC_FocPWM_ONN(void);
void PUYA_SEMI_MC_FocVs1PWMduty_Calc(PUYA_SEMI_MC_FOC1R_Base_TYPE *Foc);

extern PUYA_SEMI_MC_FOC1R_Base_TYPE mc_stFoc2R;
extern SIN_COS FocSinCos;
extern DIV_CalculatedTypeDef Calculatervalue;
extern DIV_HandleTypeDef Divhandle;
extern PUYA_SEMI_MC_FOC1R_PI_TYPE mc_stPidNtc;  
#endif
/*************************************************************************
 End of this File (EOF):
 !!!!!!Do not put anything after this part!!!!!!!!!!!
*************************************************************************/
