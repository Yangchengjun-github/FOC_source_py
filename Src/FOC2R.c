/**
 ******************************************************************************
 * @file    FOC2R.c
 * @author  MCU Application Team
 * @Version V1.0.0
 * @Date    2023-03-01
 * @brief   BLDC FOC Sensorless under 1R Sensing Current
 ******************************************************************************
 **/

/************************************************************************
 Included File:
*************************************************************************/
#include "main.h"

const signed int ArBemfCof[11] = {
	#if 0
	// 0 to 32275
	5000,  //	0
	7750,  //	1
	10500, //	2
	13250, //	3
	16000, //	4
	18750, //	5
	21500, //	6
	24250, //	7
	26500, //	8
	29750, //	9
	32500, //	10
	#else 
	4000 		,//	0
	6750 		,//	1
	9500 	,//	2
	12250 	,//	3
	15000 	,//	4
	17750 	,//	5
	20500 	,//	6
	23250 	,//	7
	25500 	,//	8
	28750 	,//	9
	31500 	,//	10
	#endif
};

PUYA_SEMI_MC_FOC1R_Base_TYPE mc_stFoc2R;
SIN_COS FocSinCos;
DIV_CalculatedTypeDef Calculatervalue;
DIV_HandleTypeDef Divhandle;
PUYA_SEMI_MC_SCURVE_TYPE mc_stSCurve;
PUYA_SEMI_MC_PtcCtrl_TYPE stPtc;				//PTC 发热丝
PUYA_SEMI_MC_BrkStpCtrl_TYPE stBrk;
PUYA_SEMI_GridOnOff_TYPE stGrid;
PUYA_SEMI_MC_FOC1R_PI_TYPE mc_stPidNtc;  


/********************************************************************************
Description:    Motor MC_FOC2R_Schedule
Input:
Output:
Return:
*********************************************************************************/
void PUYA_SEMI_MC_FOC1R_Schedule(void)
{
	mc_PLL.Valfa = mc_stFoc2R.ValfaLpf;
	mc_PLL.Vbeta = mc_stFoc2R.VbetaLpf;
	mc_PLL.Ialfa = mc_stFoc2R.Ialfa;
	mc_PLL.Ibeta = mc_stFoc2R.Ibeta;
	PUYA_SEMI_MC_PLL_Calc(&mc_PLL); // 观测器为锁相环PLL，调用实时更新电机电角度

	// Error Scan
	//if (mc_stFoc2R.SysStateId > 0)
	if (mc_stFoc2R.SysStateId == STATE_StableRun)
	{
		//if (mc_stFoc2R.IbusAdc1 > 4000)
		if(mc_stFoc2R.slIdcLpf.sw.hi > 4000)					//改为滤波后的电流
		{
			mc_stFoc2R.MotorErrCode = MOTOR_SOC_ERROR;			//软件过流
			mc_stFoc2R.SysStateId = STATE_STOP_ERROR;
		}
	}

	mc_stFoc2R.CompVal.ul = COMP2->CSR;
	mc_stFoc2R.Comp2Val = (COMP2->CSR & 0X40000000) >> 30;		//硬件过流
	if (mc_stFoc2R.Comp2Val)
	{
		mc_stFoc2R.MotorErrCode = MOTOR_HOC_ERROR;
		mc_stFoc2R.SysStateId = STATE_STOP_ERROR;
	}

	/******************************************************************************************************/
	// StateMachine Control
	/******************************************************************************************************/
	switch (mc_stFoc2R.SysStateId)
	{
		/*************************************************************************************************************************************************************************************************************/
	case STATE_POWERON:
		/******************************************************************************************************/
		// 1.Mission Service of Current System-State
		/******************************************************************************************************/
		// PUYA_SEMI_MC_FocPWM_OFF();
		PUYA_SEMI_MC_PLL_Init(&mc_PLL);
		CLEAR_BIT(TIM1->BDTR, TIM_BDTR_MOE);
		mc_stFoc2R.PwmOnOfState = PWM_OUT_IS_OFF;

		/******************************************************************************************************/
		// 2.Current Offset Calc
		/******************************************************************************************************/
		mc_stFoc2R.slVdcLpf.sw.hi = mc_stFoc2R.VdcAdc;
		mc_stFoc2R.slVdcMean.sw.hi = mc_stFoc2R.VdcAdc;
		mc_stFoc2R.slIdcLpf.sw.hi = mc_stFoc2R.IdcAdc;
		//mc_stFoc2R.slVTmpLpf.sw.hi = mc_stFoc2R.NtcAdc;

		/******************************************************************************************************/
		// 2.System-State-Transition Control
		/******************************************************************************************************/
		if ((userPara.ntcSta == NTC_OK) && (mc_stFoc2R.slVdcLpf.sw.hi > 2170)) // 3000)//Delay and waiting for VDC Stable after Power-on
		{
			mc_stFoc2R.SysStateId = STATE_STOP_NOMARL;
			mc_stFoc2R.PwmIsr_cnt = 0;
			mc_stFoc2R.PwmIsr_cnt2 = 0;
		}
		break;

		/*************************************************************************************************************************************************************************************************************/
	case STATE_STOP_NOMARL:
		/******************************************************************************************************/
		// 1.Mission Service of Current System-State
		/******************************************************************************************************/
		PUYA_SEMI_MC_FocPWM_OFF();

		/******************************************************************************************************/
		// 2.System-State-Transition Control
		/******************************************************************************************************/
		if (mc_stFoc2R.PwmIsr_cnt2 > 1000)
		{
			mc_stFoc2R.PwmIsr_cnt = 0;

			// According to current DC Bus voltage to set the Starting-up SpdCmd 根据DCBus电压设定启动速度指令
			mc_stFoc2R.SpdCmdUnderVdc = (mc_stFoc2R.slVdcMean.sw.hi * 8969 >> 10) - 7651; //-8051;//=D2*8969/2^10-8051
			if (mc_stFoc2R.SpdCmdUnderVdc > HIGH_SPEED_CMD)			//限最大
				mc_stFoc2R.SpdCmdUnderVdc = HIGH_SPEED_CMD;
			if (mc_stFoc2R.SpdCmdUnderVdc < 10000)					//限最小
				mc_stFoc2R.SpdCmdUnderVdc = 10000;
			mc_stFoc2R.SpdRefCmd = mc_stFoc2R.SpdCmdUnderVdc;		//给速度参考指令

			if (userPara.sw_speed_cmd == SPEED_CMD_HIGH)			//根据电压算出启动速度还需 判断档位 是否在条件内
			{
				if (mc_stFoc2R.SpdRefCmd > HIGH_SPEED_CMD)
					mc_stFoc2R.SpdRefCmd = HIGH_SPEED_CMD;
			}
			else if (userPara.sw_speed_cmd == SPEED_CMD_MIDL)
			{
				if (mc_stFoc2R.SpdRefCmd > MIDL_SPEED_CMD)
					mc_stFoc2R.SpdRefCmd = MIDL_SPEED_CMD;
			}
			else if (userPara.sw_speed_cmd == SPEED_CMD_LOWW)
			{
				if (mc_stFoc2R.SpdRefCmd > LOWW_SPEED_CMD)
					mc_stFoc2R.SpdRefCmd = LOWW_SPEED_CMD;
			}
			mc_stFoc2R.SysStateId = STATE_StartingUp;				//转到启动状态
			MC_6Step_Io_Pwm();			 		 					//恢复可能由于刹车PWM上桥的设置
			FocSinCos.uwSitaPu +=8192;
			
			updateLed();			 //刷新LED

			PUYA_SEMI_MC_FOC1R_INIT();
			PUYA_SEMI_MC_PLL_Init(&mc_PLL);

			mc_stSCurve.DetaWeCmd = mc_stFoc2R.SpdRefCmd; 			// 设定速度阶跃量
			mc_stSCurve.WeCmd = 0;
			mc_stSCurve.WeCmd0 = 0;
			mc_stSCurve.tabID = 0;
			mc_stSCurve.cnt = 0;	 								// 这些量初始化为加速前的速度指令值 变量须清零
			mc_stSCurve.cntTh = 100; // 55;//70;//cntTh会影响加速度；越大加速越慢   //速度RPM值=60*3700*WeCmd/32768

			mc_stFoc2R.VoltFeedfwdCof = 3100; // 3200;//3000;//3200; 电压默认3100，代表310V 电机无法启动时，可微调此系数，建议以50为步进进行微调尝试
			mc_stFoc2R.DecWeCmd = 0;
		}

		/******************************************************************************************************/
		// 3.NTC check
		/******************************************************************************************************/
		#if 1
		//NTC 开短路检测
		if(mc_stFoc2R.NtcAdc < 15) 													//NTC短路
		{
			userPara.ntcSta = NTC_SHORT;											//温度的LED会闪
			mc_stFoc2R.MotorErrCode = NTC_SHORT_ERROR; 			
			mc_stFoc2R.SysStateId = STATE_STOP_NTC_ERROR;	
			userPara.sw_heat_en = HEAT_DIS;											//禁止加温				
		}
			
		else if(mc_stFoc2R.NtcAdc > (4096-20))										//NTC开路
		{
			userPara.ntcSta = NTC_OPEN;												//温度的LED会闪
			mc_stFoc2R.MotorErrCode = NTC_OPEN_ERROR;
			mc_stFoc2R.SysStateId = STATE_STOP_NTC_ERROR;
			userPara.sw_heat_en = HEAT_DIS;											//禁止加温
		}
		else 
			userPara.ntcSta = NTC_OK;
		#endif		

		if (mc_stFoc2R.slVdcLpf.sw.hi < MIN_VDC_THRESHOLD)		//用小电源时 注意一下！
		{ // When using small-power DC-Power-Supply, the Vdc increasing is slow,which will trig FW MOTOR_LV_ERROR. In this case, just disable this error
			mc_stFoc2R.MotorErrCode = MOTOR_LV_ERROR;
			mc_stFoc2R.SysStateId = STATE_STOP_ERROR;
		}
		break;

		/*************************************************************************************************************************************************************************************************************/
	case STATE_StartingUp:
		/******************************************************************************************************/
		// 1.MotorCtrl Service
		/******************************************************************************************************/
		PUYA_SEMI_MC_SCurve(&mc_stSCurve); 						// 当调速阶跃指令比较大时，调用进行加减速控制
		mc_stFoc2R.WeCmd = mc_stSCurve.WeCmd;

		
		PUYA_SEMI_MC_MotorCtrl();		  						//电机控制
		PUYA_SEMI_MC_FocPWM_ONN();		  						//使能PWM输出

		/******************************************************************************************************/
		// 2.System-State-Transition Control
		/******************************************************************************************************/
		if (mc_stFoc2R.stable_Cnt > 2000) // 20000)
		{
			mc_stFoc2R.SysStateId = STATE_StableRun;

			userPara.save = SAVE_DIS;//重新准备使用记录
			updatHeatSet();			 //更新一下档位
		}

		if (abs(mc_stFoc2R.slVdcMean.sw.hi - mc_stFoc2R.slVdcLpf.sw.hi) > MIN_VDC_TH)
		{
			mc_stFoc2R.SysStateId = STATE_SpdToPwmStp;
		}

		if (mc_stFoc2R.slVdcLpf.sw.hi < MIN_VDC_THRESHOLD)
		{ // When using small-power DC-Power-Supply, the Vdc increasing is slow,which will trig FW MOTOR_LV_ERROR. In this case, just disable this error
			mc_stFoc2R.SysStateId = STATE_SpdToPwmStp;
		}
		break;

		/*************************************************************************************************************************************************************************************************************/
	case STATE_StableRun:
		/******************************************************************************************************/
		// 1.MotorCtrl Service
		/******************************************************************************************************/
		mc_stFoc2R.SpdCnt++;
		if (mc_stFoc2R.SpdCnt > 10)
		{
			mc_stFoc2R.SpdCnt = 0;
			if (mc_stFoc2R.SpdRefCmd > mc_stFoc2R.WeCmd)
				mc_stFoc2R.WeCmd++;
			if (mc_stFoc2R.SpdRefCmd < mc_stFoc2R.WeCmd)
				mc_stFoc2R.WeCmd--;
		}

		PUYA_SEMI_MC_MotorCtrl();		 //电机控制
		PUYA_SEMI_MC_FocPWM_ONN();		 //使能PWM输出	

		/******************************************************************************************************/
		// 2.System-State-Transition Control
		/******************************************************************************************************/
		if (abs(mc_stFoc2R.slVdcMean.sw.hi - mc_stFoc2R.slVdcLpf.sw.hi) > MIN_VDC_TH)							//电压差
		{
			mc_stFoc2R.SysStateId = STATE_SpdToPwmStp;
		}

		if (mc_stFoc2R.slVdcLpf.sw.hi < MIN_VDC_THRESHOLD)														//关电时电压往下 触分刹车
		{ // When using small-power DC-Power-Supply, the Vdc increasing is slow,which will trig FW MOTOR_LV_ERROR. In this case, just disable this error
			mc_stFoc2R.SysStateId = STATE_SpdToPwmStp;
		}
		break;

		/*************************************************************************************************************************************************************************************************************/
	case STATE_SpdToPwmStp:
		/******************************************************************************************************/
		// 1.Mission Service of Current System-State
		/******************************************************************************************************/
		PUYA_SEMI_BrkStpCtrl(&stBrk);																			//刹车

		/******************************************************************************************************/
		// 2.System-State-Transition Control
		/******************************************************************************************************/
		if (stBrk.Brk_Cnt < stBrk.Brk_AllTimeTh)																//刹车总时间
			stBrk.Brk_Cnt++;
		else
		{
			if ((mc_stFoc2R.VdcAdc > 2170) && (stGrid.Status == GRID_ONN)) // if((mc_stFoc2R.VdcAdc>3000)&&(stGrid.Status==GRID_ONN))
			{
				mc_stFoc2R.PwmIsr_cnt = 0;
				stBrk.Brk_Cnt = 0;

				mc_stFoc2R.SysStateId = STATE_STOP_NOMARL;
				mc_stFoc2R.EsTmp = 0;
				PUYA_SEMI_MC_FocPWM_OFF();						//停止PWM输出
				mc_stFoc2R.stable_Cnt = 0;
				MC_6Step_Io_Pwm();								//IO还原成PWM输出
				mc_stFoc2R.PwmIsr_cnt2 = 0;
			}
			if(userPara.save == SAVE_DIS)userPara.save = SAVE_EN;	//开始记录

			allLedOff();													//LED全灭
		}
		break;

		/*************************************************************************************************************************************************************************************************************/
	case STATE_STOP_NTC_ERROR:
		//NTC 开短路检测
		if(mc_stFoc2R.NtcAdc < 10) 													//NTC短路
		{
			userPara.ntcSta = NTC_SHORT;											//温度的LED会闪
			mc_stFoc2R.MotorErrCode = NTC_SHORT_ERROR; 			
			userPara.sw_heat_en = HEAT_DIS;											//禁止加温				
		}
		else if(mc_stFoc2R.NtcAdc > (4096-20))										//NTC开路
		{
			userPara.ntcSta = NTC_OPEN;												//温度的LED会闪
			mc_stFoc2R.MotorErrCode = NTC_OPEN_ERROR;
			userPara.sw_heat_en = HEAT_DIS;											//禁止加温
		}
		else 
		{
			userPara.ntcSta = NTC_OK;
		}
		break;
	case STATE_STOP_ERROR:
		/******************************************************************************************************/
		// 1.Mission Service of Current System-State
		/******************************************************************************************************/
		// PUYA_SEMI_MC_FocPWM_OFF();
		CLEAR_BIT(TIM1->BDTR, TIM_BDTR_MOE);																	//停止PWM输出
		mc_stFoc2R.PwmOnOfState = PWM_OUT_IS_OFF;

		allLedOff();													//LED全灭
		/******************************************************************************************************/
		// 2.System-State-Transition Control
		/******************************************************************************************************/
		if (mc_stFoc2R.MotorErrCode == MOTOR_LV_ERROR)
		{ 
			if (mc_stFoc2R.VdcAdc > 2170)
			{
				mc_stFoc2R.SysStateId = STATE_STOP_NOMARL;
				mc_stFoc2R.MotorErrCode = 0;
				mc_stFoc2R.PwmIsr_cnt2 = 0;
				if(userPara.save == SAVE_DIS)userPara.save = SAVE_EN;	//开始记录
			}
		}

		if(mc_stFoc2R.MotorErrCode != MOTOR_LV_ERROR) 														//快速开关引起过压错误
		{
			if (mc_stFoc2R.slVdcLpf.sw.hi < MIN_RECOVERY_VDC_TH)
			{
				if(++mc_stFoc2R.errorRecoverCnt > (100000/50)) 		//持续100ms
				{
					mc_stFoc2R.errorRecoverCnt = 0;
					mc_stFoc2R.SysStateId = STATE_POWERON;			//恢复继续跑
					mc_stFoc2R.MotorErrCode = 0;					//恢复错误代码	
					mc_stFoc2R.PwmIsr_cnt = 0;
					mc_stFoc2R.PwmIsr_cnt2 = 0;
					mc_stFoc2R.IbusAdc1=0;

					HAL_FLASH_OB_Launch(); 
				}
			}
			else 
				mc_stFoc2R.errorRecoverCnt = 0;					
		}

		break;

		/*************************************************************************************************************************************************************************************************************/
	default:
		break;
	}
}

/********************************************************************************
Description:   20K 中断回调 50uS
Input:
Output:
Return:
*********************************************************************************/
extern uint32_t gADCxConvertedData[4];
void PwmIsrCallback(void)
{
	/*******************************************************************************/
	// GET REGULAR ADC VALUES
	/*******************************************************************************/
	//    gADCxConvertedData[0]-ADC_REGULAR_RANK_1---OPA2--Idc or IdcMean
	//    gADCxConvertedData[1]-ADC_REGULAR_RANK_2---AN2--Vbus
	//    gADCxConvertedData[2]-ADC_REGULAR_RANK_3---AN0--VTmp of IPM-Module(Temperature)
	//    gADCxConvertedData[3]-ADC_REGULAR_RANK_4---AN1--NTC
	mc_stFoc2R.IdcAdc = gADCxConvertedData[0];
	mc_stFoc2R.VdcAdc = ((SLONG)gADCxConvertedData[1] * VDC_GAIN_COF >> 10) + VDC_OFSET;
	mc_stFoc2R.VTmpAdc = gADCxConvertedData[2]; // Temperature of IPM-Module
	mc_stFoc2R.NtcAdc = gADCxConvertedData[3];

	/***********************************************************************************************/
	// Software-Start-Trig the ADC_REGULAR Channels
	/***********************************************************************************************/
	SET_BIT(ADC1->CR2, (ADC_CR2_SWSTART | ADC_CR2_EXTTRIG));						

	/***********************************************************************************************/
	//CTRL of FaReSi 发热丝
	/***********************************************************************************************/
	if((mc_stFoc2R.SysStateId==STATE_StableRun) && (userPara.sw_heat_en==HEAT_EN))						//在风机稳定状态下才使用发热丝
		PUYA_SEMI_PtcCtrl(&stPtc);
	else
		PTC_CTR_OFF;

	/***********************************************************************************************/
	// Grid Status Detecting
	/***********************************************************************************************/
	if (stGrid.CZ_Cnt < 30000)
		stGrid.CZ_Cnt++;
	stGrid.GridZero1 = ZERO_VAL;
	if (stGrid.GridZero1 != stGrid.GridZero0)
	{
		stGrid.CZ_Cycle = stGrid.CZ_Cnt;
		stGrid.CZ_Cnt = 0;
	}
	if (stGrid.CZ_Cycle < stGrid.CZ_Cnt)
		stGrid.CZ_Cycle = stGrid.CZ_Cnt;
	stGrid.GridZero0 = stGrid.GridZero1;

	if (stGrid.CZ_Cycle > 600)
		stGrid.Status = GRID_OFF;
	if (stGrid.CZ_Cycle < 400)
		stGrid.Status = GRID_ONN;

	/***********************************************************************************************/
	// AN LowPassFilter Ctrl
	/***********************************************************************************************/
	// Cof: LowPasFilter_Cof=12000
	// Input:mc_stFoc2R.VdcAdc, mc_stFoc2R.IdcAdc, mc_stFoc2R.VTmpAdc
	PUYA_SEMI_MC_LpfCtrl(12000);
	// Output:mc_stFoc2R.slVdcLpf.sw.hi, mc_stFoc2R.slIdcLpf.sw.hi, mc_stFoc2R.slVTmpLpf.sw.hi

	/***********************************************************************************************/
	// When DC Bus voltage is smaller than its norminal-value, the fan target speed command need to be smaller, changing to mc_stFoc2R.SpdCmdUnderVdc
	/***********************************************************************************************/
	mc_stFoc2R.SpdCmdUnderVdc = (mc_stFoc2R.slVdcMean.sw.hi * 8861 >> 10) - 6893; //=E2*8861/2^10-6893
	if (mc_stFoc2R.SpdCmdUnderVdc > HIGH_SPEED_CMD)
		mc_stFoc2R.SpdCmdUnderVdc = HIGH_SPEED_CMD;
	if (mc_stFoc2R.SpdCmdUnderVdc < 10000)
		mc_stFoc2R.SpdCmdUnderVdc = 10000;

	/***********************************************************************************************/
	// FW Timers
	/***********************************************************************************************/
	mc_stFoc2R.PwmIsr_cnt++; // PWM Isr count for control in PUYA_SEMI_MC_FOC1R_Schedule()
	//mc_stFoc2R.clk_cnt++;	 // FW timer for 1000ms in main-while-loop
	mc_stFoc2R.PwmIsr_cnt2++;

	/***********************************************************************************************/
	// MotorCTRL
	/***********************************************************************************************/
	PUYA_SEMI_MC_FOC1R_Schedule();
}

/********************************************************************************
Description:    参数初始化
Input:
Output:
Return:
*********************************************************************************/
void PUYA_SEMI_MC_FOC1R_INIT(void)
{
	mc_stFoc2R.Tc2PI = 24248;

	mc_stFoc2R.EsMinTh = 6000;
	mc_stFoc2R.EsMaxTh = 32760;

	mc_stFoc2R.ErrPLLMinTh = 2000;
	mc_stFoc2R.ErrPLLMaxTh = 32000;

	mc_stFoc2R.ErrPLLAcc = 4;

	stPtc.PTC_RankCof = 1;														//发热丝 相关
	stPtc.PTC_Rank = 1;
	stPtc.GridTrigOnTh = 150; // 300;//Should be larger than 10ms to be sure connducting at one grid cycle

	stBrk.Brk_InitDutyCmd = 1150;//900;//600;												//刹车 相关
	stBrk.Brk_IdleTh = 200;//1000;
	stBrk.Brk_PwmTh = 500;//2000;												//PWM刹车时间
	stBrk.Brk_AccTh = 30;
	stBrk.Brk_AllTimeTh = 6000;//6000;													//刹车 相关
	stBrk.Brk_Cnt = 0;
}

/********************************************************************************
Description:    停止PWM输出
Input:
Output:
Return:
*********************************************************************************/
void PUYA_SEMI_MC_FocPWM_OFF(void)
{
	if (mc_stFoc2R.PwmOnOfState == PWM_OUT_IS_ONN)
	{
		CLEAR_BIT(TIM1->BDTR, TIM_BDTR_MOE);
		mc_stFoc2R.PwmOnOfState = PWM_OUT_IS_OFF;
	}
}

/********************************************************************************
Description:    使能PWM输出
Input:
Output:
Return:
*********************************************************************************/
void PUYA_SEMI_MC_FocPWM_ONN(void)
{
	if ((mc_stFoc2R.PwmOnOfState == PWM_OUT_IS_OFF) && (mc_stFoc2R.MotorErrCode == 0) && (mc_stFoc2R.MotorErrCode != STATE_STOP_ERROR))
	{
		SET_BIT(TIM1->BDTR, TIM_BDTR_MOE);
		mc_stFoc2R.PwmOnOfState = PWM_OUT_IS_ONN;
	}
}

/*************************************************************************
 End of this File (EOF):
 Do not put anything after this part!
*************************************************************************/
