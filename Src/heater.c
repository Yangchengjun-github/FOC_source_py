/**
  ******************************************************************************
  * @file    heater.c
  * @author  MCU Application Team
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "heater.h"
const uint16_t Temp_NTC[]=													//ADC->温度表
{
3858	,	//	0
3847	,	//	1
3835	,	//	2
3822	,	//	3
3809	,	//	4
3795	,	//	5
3781	,	//	6
3767	,	//	7
3752	,	//	8
3736	,	//	9
3720	,	//	10
3703	,	//	11
3686	,	//	12
3668	,	//	13
3650	,	//	14
3631	,	//	15
3612	,	//	16
3592	,	//	17
3571	,	//	18
3550	,	//	19
3528	,	//	20
3506	,	//	21
3483	,	//	22
3459	,	//	23
3435	,	//	24
3410	,	//	25
3385	,	//	26
3359	,	//	27
3332	,	//	28
3305	,	//	29
3277	,	//	30
3249	,	//	31
3220	,	//	32
3191	,	//	33
3161	,	//	34
3131	,	//	35
3100	,	//	36
3069	,	//	37
3037	,	//	38
3004	,	//	39
2972	,	//	40
2939	,	//	41
2905	,	//	42
2871	,	//	43
2837	,	//	44
2802	,	//	45
2767	,	//	46
2732	,	//	47
2697	,	//	48
2661	,	//	49
2625	,	//	50
2589	,	//	51
2553	,	//	52
2516	,	//	53
2479	,	//	54
2443	,	//	55
2406	,	//	56
2369	,	//	57
2332	,	//	58
2295	,	//	59
2259	,	//	60
2222	,	//	61
2185	,	//	62
2149	,	//	63
2112	,	//	64
2076	,	//	65
2040	,	//	66
2004	,	//	67
1968	,	//	68
1932	,	//	69
1897	,	//	70
1862	,	//	71
1827	,	//	72
1793	,	//	73
1759	,	//	74
1725	,	//	75
1691	,	//	76
1658	,	//	77
1626	,	//	78
1593	,	//	79
1561	,	//	80
1530	,	//	81
1498	,	//	82
1468	,	//	83
1437	,	//	84
1408	,	//	85
1378	,	//	86
1349	,	//	87
1321	,	//	88
1292	,	//	89
1265	,	//	90
1238	,	//	91
1211	,	//	92
1185	,	//	93
1159	,	//	94
1133	,	//	95
1109	,	//	96
1084	,	//	97
1060	,	//	98
1037	,	//	99
1014	,	//	100
991	,	//	101
969	,	//	102
947	,	//	103
926	,	//	104
905	,	//	105
884	,	//	106
864	,	//	107
845	,	//	108
826	,	//	109
807	,	//	110
789	,	//	111
771	,	//	112
753	,	//	113
736	,	//	114
719	,	//	115
703	,	//	116
687	,	//	117
671	,	//	118
656	,	//	119
641	,	//	120
626	,	//	121
612	,	//	122
598	,	//	123
584	,	//	124
571	,	//	125
558	,	//	126
545	,	//	127
532	,	//	128
520	,	//	129
508	,	//	130
497	,	//	131
486	,	//	132
475	,	//	133
464	,	//	134
453	,	//	135
443	,	//	136
433	,	//	137
423	,	//	138
414	,	//	139
404	,	//	140
395	,	//	141
387	,	//	142
378	,	//	143
369	,	//	144
361	,	//	145
353	,	//	146
345	,	//	147
338	,	//	148
330	,	//	149
323	,	//	150
316	,	//	151
309	,	//	152
302	,	//	153
295	,	//	154
289	,	//	155
283	,	//	156
277	,	//	157
271	,	//	158
265	,	//	159
259	,	//	160
253	,	//	161
248	,	//	162
243	,	//	163
237	,	//	164
232	,	//	165
227	,	//	166
223	,	//	167
218	,	//	168
213	,	//	169
209	,	//	170
204	,	//	171
200	,	//	172
196	,	//	173
192	,	//	174
188	,	//	175
184	,	//	176
180	,	//	177
177	,	//	178
173	,	//	179
169	,	//	180
166	,	//	181
163	,	//	182
159	,	//	183
156	,	//	184
153	,	//	185
150	,	//	186
147	,	//	187
144	,	//	188
141	,	//	189
138	,	//	190
135	,	//	191
133	,	//	192
130	,	//	193
128	,	//	194
125	,	//	195
123	,	//	196
120	,	//	197
118	,	//	198
116	,	//	199
113	,	//	200
111	,	//	201
109	,	//	202
107	,	//	203
105	,	//	204
103	,	//	205
101	,	//	206
99	,	//	207
97	,	//	208
95	,	//	209
94	,	//	210
92	,	//	211
90	,	//	212
88	,	//	213
87	,	//	214
85	,	//	215
84	,	//	216
82	,	//	217
81	,	//	218
79	,	//	219
78	,	//	220
76	,	//	221
75	,	//	222
74	,	//	223
72	,	//	224
71	,	//	225
70	,	//	226
68	,	//	227
67	,	//	228
66	,	//	229
65	,	//	230
64	,	//	231
63	,	//	232
62	,	//	233
61	,	//	234
59	,	//	235
58	,	//	236
57	,	//	237
56	,	//	238
55	,	//	239
55	,	//	240
54	,	//	241
53	,	//	242
52	,	//	243
51	,	//	244
50	,	//	245
49	,	//	246
48	,	//	247
48	,	//	248
47	,	//	249
46	,	//	250
45	,	//	251
45	,	//	252
44	,	//	253
43	,	//	254
42	,	//	255
42	,	//	256
41	,	//	257
40	,	//	258
40	,	//	259
39	,	//	260
38	,	//	261
38	,	//	262
37	,	//	263
37	,	//	264
36	,	//	265
35	,	//	266
35	,	//	267
34	,	//	268
34	,	//	269
33	,	//	270
33	,	//	271
32	,	//	272
32	,	//	273
31	,	//	274
31	,	//	275
30	,	//	276
30	,	//	277
29	,	//	278
29	,	//	279
29	,	//	280
28	,	//	281
28	,	//	282
27	,	//	283
27	,	//	284
26	,	//	285
26	,	//	286
26	,	//	287
25	,	//	288
25	,	//	289
25	,	//	290
24	,	//	291
24	,	//	292
24	,	//	293
23	,	//	294
23	,	//	295
23	,	//	296
22	,	//	297
22	,	//	298
22	,	//	299
21	,	//	300
};


/********************************************************************************
Description:	折半查找法
Input:			需要查找的数，查找的数组，数组的最大值
Output:			查找数的位置
Return:
*********************************************************************************/
int Binary_Search(uint16_t Search_Num, uint16_t const *Temp_Tab, uint16_t High) // 折半查找法
{
	int Low = 0, Mid;
	if (Search_Num > Temp_Tab[0])
	{
		return 0;
	}
	else
	{
		if (Search_Num < Temp_Tab[High])
		{
			return High + 1;
		}
	}
	while (Low < High)
	{
		Mid = (Low + High) / 2;
		if (Search_Num < Temp_Tab[Mid])
		{
			Low = Mid + 1;
			if (Search_Num > Temp_Tab[Low])
			{
				return Mid;
			}
			else
			{
				if (Search_Num == Temp_Tab[Low])
				{
					return Low;
				}
			}
		}
		else
		{
			if (Search_Num > Temp_Tab[Mid])
			{
				High = Mid - 1;
				if (Search_Num < Temp_Tab[High])
				{
					return Mid;
				}
				else
				{
					if (Search_Num == Temp_Tab[High])
					{
						return High;
					}
				}
			}
			else
			{
				return Mid;
			}
		}
	}
	return Mid;
}

/********************************************************************************
Description:	
Input:
Output:
Return:
*********************************************************************************/
int16_t PUYA_SEMI_MC_Adc2TmpDegreeX100(int16_t NtcAdc)
{
//TEMPERATURE VS RESISTANCE TABLE
//Resistance         				100k Ohms at 25deg. C
//Resistance Tolerance     	+ / -1%
//B Value            				3950K at 25/50deg. C
//B Value Tolerance         + / - 1%
//@	R38=20K, Vcc=5V, ADC_REF=5V
	
	int16_t tmpY,TmpDeg;
	
	if(NtcAdc>4096)NtcAdc=4096;
	if(NtcAdc<13)NtcAdc=13;
	
	if(NtcAdc>4020)//1--(3945-H2)*7306/2^8
	{
		tmpY=3945; TmpDeg=(int32_t)(tmpY-NtcAdc)*7306>>8;
	}
	else if(NtcAdc>3945)//2--(3893-H11)*4223/2^8
	{
		tmpY=3893; TmpDeg=(int32_t)(tmpY-NtcAdc)*4223>>8;
	}
	else if(NtcAdc>3860)//3--(3862-H31)*2686/2^8
	{
		tmpY=3862; TmpDeg=(int32_t)(tmpY-NtcAdc)*2686>>8;
	}
	else if(NtcAdc>3690)//4--(3864-H43)*1823/2^8
	{
		tmpY=3864; TmpDeg=(int32_t)(tmpY-NtcAdc)*1823>>8;
	}
	else if(NtcAdc>3362)//5--(3955-H58)*1190/2^8
	{
		tmpY=3955; TmpDeg=(int32_t)(tmpY-NtcAdc)*1190>>8;
	}
	else if(NtcAdc>2738)//6--(4225-H78)*816/2^8
	{
		tmpY=4225; TmpDeg=(int32_t)(tmpY-NtcAdc)*816>>8;
	}
	else if(NtcAdc>1355)//7--(4365-H118)*732/2^8
	{
		tmpY=4365; TmpDeg=(int32_t)(tmpY-NtcAdc)*732>>8;
	}
	else if(NtcAdc>780)//8--(3384-H143)*1088/2^8
	{
		tmpY=3384; TmpDeg=(int32_t)(tmpY-NtcAdc)*1088>>8;
	}
	else if(NtcAdc>410)//9--(2278-H171)*1887/2^8
	{
		tmpY=2278; TmpDeg=(int32_t)(tmpY-NtcAdc)*1887>>8;
	}
	else if(NtcAdc>200)//10--(1291-J204)*4004/2^8
	{
		tmpY=1291; TmpDeg=(int32_t)(tmpY-NtcAdc)*4004>>8;
	}
	else if(NtcAdc>132)//11--(750-J224)*7951/2^8
	{
		tmpY=750; TmpDeg=(int32_t)(tmpY-NtcAdc)*7951>>8;
	}
	else if(NtcAdc>99)//12--(560-J239)*11498/2^8
	{
		tmpY=560; TmpDeg=(int32_t)(tmpY-NtcAdc)*11498>>8;
	}
	else if(NtcAdc>75)//13--(430-J254)*16025/2^8
	{
		tmpY=430; TmpDeg=(int32_t)(tmpY-NtcAdc)*16025>>8;
	}
	else if(NtcAdc>56)//14--(328-J271)*22499/2^8
	{
		tmpY=328; TmpDeg=(int32_t)(tmpY-NtcAdc)*22499>>8;
	}
	else if(NtcAdc>43)//15--(252-J286)*31219/2^8
	{
		tmpY=252; TmpDeg=(int32_t)(tmpY-NtcAdc)*31219>>8;
	}
	else if(NtcAdc>33)//16--(196-J303)*21308/2^7
	{
		tmpY=196; TmpDeg=(int32_t)(tmpY-NtcAdc)*21308>>7;
	}
	else if(NtcAdc>26)//17--(154-J318)*28800/2^7
	{
		tmpY=154; TmpDeg=(int32_t)(tmpY-NtcAdc)*28800>>7;
	}
	else //if(NtcAdc>20)//18--(125-J319)*2336/2^3
	{
		tmpY=125; TmpDeg=(int32_t)(tmpY-NtcAdc)*2336>>3;
	}
	
	return(TmpDeg);
}
 
/********************************************************************************
Description:	读取NTC并转为温度 NtcTemp 或返回 NTC状态
Input:
Output:
Return:
*********************************************************************************/
void Read_NTC_To_Temp(void)														//读取ADC值 
{
	unsigned int ADC_Value;

	ADC_Value = mc_stFoc2R.slVTmpLpf.sw.hi;										//采到的ADC
	mc_stFoc2R.NtcTemp = Binary_Search(ADC_Value,Temp_NTC,300);					
}
/********************************************************************************
Description:
Input:
Output:
Return:
*********************************************************************************/
void heaterInit(void)
{
	mc_stPidNtc.Kp=200;
	mc_stPidNtc.Ki=200;
	mc_stPidNtc.Max=29;
	mc_stPidNtc.Min=1;  
	mc_stPidNtc.ek=0;
	mc_stPidNtc.ek_1=0;
	mc_stPidNtc.dek=0;
	mc_stPidNtc.swOut=0;
}

#define NTC_ALAM_TH												(200)				//温度过热保护温度阈值 （如果用拟合的温度就要放大100）	
#define NTC_RESTORE_TH											(NTC_ALAM_TH - 80)	//恢复温度阈值	
/********************************************************************************
Description:	发热丝 控制处理  温度控制调节
Input:
Output:
Return:
*********************************************************************************/
void heaterTask(void)
{
	#if 0
	//拟合方式 算温度
	mc_stFoc2R.TmpX10Degree = 100; //((SLONG)mc_stFoc2R.slVTmpLpf.sw.hi*21378>>15)-204;
	mc_stFoc2R.NtcTemp = PUYA_SEMI_MC_Adc2TmpDegreeX100(mc_stFoc2R.slVTmpLpf.sw.hi);	//拟合方式 NtcTemp 已放大了100倍
	//NTC PI Ctrl
	if(mc_stPidNtc.Ref>userPara.targetTemperature)mc_stPidNtc.Ref-=100;
	if(mc_stPidNtc.Ref<userPara.targetTemperature)mc_stPidNtc.Ref+=100;		
	mc_stPidNtc.Yk = mc_stFoc2R.NtcTemp; //注意NtcTemp是放大了100倍
	PUYA_SEMI_MC_FOC1R_PI_Ctrl(&mc_stPidNtc);
	stPtc.PTC_Rank = mc_stPidNtc.swOut+5;//PI输出

	if(mc_stFoc2R.NtcTemp > (NTC_MAX*100))//过温保护
	{
		userPara.sw_heat_en = HEAT_DIS;
		updateLed();	//刷新LED
		userPara.tempAlam = TEMP_HIGH;	//温度过高
	}
	else if(mc_stFoc2R.NtcTemp < ((NTC_MAX-60)*100) )
	{
		userPara.tempAlam = TEMP_OK;	//温度正常
		//userPara.sw_heat_en = HEAT_EN;
		//updateLed();	//刷新LED
	}
	#else
	//查表方式 算温度
	Read_NTC_To_Temp(); 																//查表方式 NtcTemp
	//stPtc.PTC_Rank=22;// 22---1244W
	//stPtc.PTC_Rank=20;// 19---1073W   20---1122W  
	//stPtc.PTC_Rank=18;// 18---1019W
	
	//stPtc.PTC_Rank=17;// 16---903W 17---950W
	//stPtc.PTC_Rank=16;// 16---903W 
	//stPtc.PTC_Rank=15;// 16---903W 

	if(userPara.ntcSta == NTC_OK)
	{
		if(mc_stFoc2R.NtcTemp > (NTC_ALAM_TH))//过温保护
		{
			userPara.sw_heat_en = HEAT_DIS;
			updateLed();	//刷新LED
			userPara.tempAlam = TEMP_HIGH;	//温度过高
		}
		else if(mc_stFoc2R.NtcTemp < NTC_RESTORE_TH )//温度恢复
		{
			if(userPara.tempAlam == TEMP_HIGH)
			{
				userPara.tempAlam = TEMP_OK;	//温度正常
				userPara.sw_heat_en = HEAT_EN;
				updateLed();	//刷新LED 这里只能调用1次，否则会闪(造成关机时微弱的亮)				
			}
		}		
	}
	#endif

	//printf("%d,%d,%d,%d\n",userPara.ntcSta,stPtc.PTC_Rank,userPara.targetTemperature,mc_stFoc2R.NtcTemp);
	//printf("%d,%d,%d,%d\n",userPara.ntcSta,mc_stFoc2R.NtcTemp,mc_stFoc2R.SysStateId,mc_stFoc2R.MotorErrCode);
	printf("%d,%d,%d,%d,%d\n",userPara.sw_heat_en,stPtc.PTC_Rank,mc_stFoc2R.NtcTemp,mc_stFoc2R.SysStateId,mc_stFoc2R.MotorErrCode);
}


/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
