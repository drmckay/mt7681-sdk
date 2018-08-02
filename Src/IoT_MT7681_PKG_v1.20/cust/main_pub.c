/******************************************************************************
 * MODULE NAME:	 main.c
 * PROJECT CODE:	__RT7681__
 * DESCRIPTION:
 * DESIGNER:		Charles Su
 * DATE:			Oct 2011
 *
 * SOURCE CONTROL:
 *
 *最新版本程序我们会在 http://www.ai-thinker.com 发布下载链接
 *	 This source code is copyright (c) 2011 Ralink Tech. Inc.
 *	 深圳市安信可科技 MTK7681串口模块专业生产厂家 .
 *
 * REVISION	 HISTORY:
 *   V1.0.0	 Oct 2011	- Initial Version V1.0
 *   V1.0.1	 Dec 2011	- Add Caution
 *
 *
 * SOURCE:
 * ISSUES:
 *	串口WIFI 价格大于500 30元   大于5K  28元   大于10K  25元.
 * 价格随时在降低，最新报价请致电：15323435161  0755-61195776  4008555368
 *
 ******************************************************************************/
#include "stdio.h"
#include "types.h"
#include "wifi_task.h"
#include "iot_api.h"
#include "iot_custom.h"

/*****************************************************************
  Extern Paramter
******************************************************************/
extern IOT_CUST_OP IoTCustOp;



/*****************************************************************
  Functions
******************************************************************/
/******************************************************************************
 * main
 * Description :
 *  The main function is used to initialize all the functionalities in C.
 ******************************************************************************/
OUT INT32
main (VOID)
{
	/* customer hook function initial */
	IoT_Cust_Ops();

	/* The entry for customization */
	if (IoTCustOp.IoTCustPreInit != NULL)
		IoTCustOp.IoTCustPreInit();

	/* Initialize BSP */
	BSP_Init();

#if (WIFI_SUPPORT==1)
	/* Initialize APP */
	APP_Init();
#endif

	sys_Init();

	/* run customer initial function */
	if (IoTCustOp.IoTCustInit != NULL)
	{
		IoTCustOp.IoTCustInit();
	}

		/* Create Tasks */
	/*******************************************************************************
	 ****** CAUTION : SOFTWARE PROGRAMMER SHALL NOT MODIFY THE FOLLOWING CODES *****
	 ******************************************************************************/
	sysTASK_RegTask(wifiTASK_LowPrioTask);

	/* Start the Kernel process */
	sysKernelStart();

	return 0;
}

