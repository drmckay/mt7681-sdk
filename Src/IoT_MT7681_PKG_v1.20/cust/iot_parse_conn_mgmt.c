#include <stdio.h>
#include "string.h"
#include "types.h"
#include "iot_api.h"
#include "config.h"
#if 0 //CFG_SUPPORT_TCPIP
#include "uip.h"
#endif
#include "flash_map.h"
#include "uip-conf.h"

/******************************************************************************
* MODULE NAME:     iot_parse_conn_mgmt.c
* PROJECT CODE:    __MT7681__
* DESCRIPTION:     
* DESIGNER:        
* DATE:            Jan 2013
*
* SOURCE CONTROL:
*
*最新版本程序我们会在 http://www.ai-thinker.com 发布下载链接
*     This source code is copyright (c) 2011 Ralink Tech. Inc.
*     深圳市安信可科技 MTK7681串口模块专业生产厂家 .
*
* REVISION     HISTORY:
*   V1.0.0     Jan 2012    - Initial Version V1.0
*
*
* SOURCE:
* ISSUES:
*    串口WIFI 价格大于500 30元   大于5K  28元   大于10K  25元.
* 价格随时在降低，最新报价请致电：15323435161  0755-61195776  4008555368
*
******************************************************************************/

#if ENABLE_DATAPARSING_SEQUENCE_MGMT

struct cp_app_conn cp_app_conns[UIP_CONF_MAX_CONNECTIONS];

#define INVALID_SOCK_NUM 255


VOID IoT_cp_app_connection_init(VOID)
{
	UCHAR i = 0;
	DBGPRINT(RT_DEBUG_TRACE, ("IoT_cp_app_connection_init\n"));
	
	for (i=0; i<UIP_CONF_MAX_CONNECTIONS; i++) 
	{
		cp_app_conns[i].soket_num = INVALID_SOCK_NUM;
		cp_app_conns[i].prev_seq = 0;

	}

	return;
}

INT16 IoT_cp_app_connection_connected(UINT8 fd
#if (NO_USED_CODE_REMOVE==0)
											,UINT16 lport, 
											PUCHAR raddr, 
											UINT16 rport
#endif
											)
{
	UCHAR i = 0;
	DBGPRINT(RT_DEBUG_TRACE, ("IoT_cp_app_connection_connected:%d\n",fd));
	
	for (i=0; i<UIP_CONF_MAX_CONNECTIONS; i++) 
	{
		if(cp_app_conns[i].soket_num == INVALID_SOCK_NUM)
		{
			cp_app_conns[i].soket_num = fd;
			cp_app_conns[i].prev_seq = 0;
			return 0;
		}	
	}
	
	return -1;
}

INT16 IoT_cp_app_connection_closed(UINT8 fd)
{

	UCHAR i = 0;
	DBGPRINT(RT_DEBUG_TRACE, ("IoT_cp_app_connection_closed:%d\n",fd));

	for (i=0; i<UIP_CONF_MAX_CONNECTIONS; i++) 
	{
		if(cp_app_conns[i].soket_num == fd)
		{
			cp_app_conns[i].soket_num = INVALID_SOCK_NUM;
			cp_app_conns[i].prev_seq = 0;
			return 0;
		}	
	}

	return -1;
}

UINT16 IoT_cp_app_search_seq(UINT8 fd)
{

	UCHAR i = 0;

	UINT16 seq = 0;
	DBGPRINT(RT_DEBUG_TRACE, ("IoT_cp_app_search_seq:%d\n",fd));

	for (i=0; i<UIP_CONF_MAX_CONNECTIONS; i++) 
	{
		if(cp_app_conns[i].soket_num == fd)
		{
			seq = cp_app_conns[i].prev_seq;
			return seq;
		}	
	}

	return 0;
}

INT16 IoT_cp_app_set_seq(UINT8 fd, UINT16 seq)
{

	UCHAR i = 0;
	DBGPRINT(RT_DEBUG_TRACE, ("IoT_cp_app_set_seq:%d,%d\n", fd, seq));

	for (i=0; i<UIP_CONF_MAX_CONNECTIONS; i++) 
	{
		if(cp_app_conns[i].soket_num == fd)
		{
			cp_app_conns[i].prev_seq = seq;
			return 0;
		}	
	}

	return -1;
}


struct cp_app_conn * IoT_cp_app_search_connection(UINT8 fd)
{

	UCHAR i = 0;

	struct cp_app_conn *conn = NULL;

	for (i=0; i<UIP_CONF_MAX_CONNECTIONS; i++) 
	{
		if(cp_app_conns[i].soket_num == fd)
		{
			conn = &cp_app_conns[i];
				return conn;;
		}	
	}
	
	return NULL;

}

#endif



