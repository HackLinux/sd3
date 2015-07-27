 #include "sd3.h"
 
 /*
  * 
  */
 SD_Error sd_send_cmd(u32 cmd,u32 arg,u32 restype)
 {
	 SD_Error errorstatus = SD_OK;
	 
	 /*将cmd、arg、restype写入相应寄存器,SDIO发送命令*/
	 
	 /*等待cmd发送完毕，清除pending*/
	 
	 return errorstatus;
 }
 
/*
 * 
 */
inline SD_Error sd_get_resp_short(u32 * res_short)
{
	 SD_Error errorstatus = SD_OK;
	 
	 /*等待接收完毕，清除pending*/
	 
	 /*接收到的值->res_short*/
	 
	 return errorstatus;
}

/*
 *
 */
inline SD_Error sd_get_resp_long(u32 * res_long)
{
	 SD_Error errorstatus = SD_OK;
	 
	 /*等待接收完毕，清除pending*/
	 
	 /*接收到的值->*res_long*/
	 
	 return errorstatus;
}

/*-------------------------------------------------------------------------*/

/*
 * 
 */
 SD_Error sd_get_resp1(u32 * res1)
 {
	return sd_get_resp_short(res1);
 }

/*
 * 
 */
SD_Error sd_get_resp1b(u32 * res1b)
{
	 /*check busy*/
	 
	 return sd_get_resp_short(res1b);
}
 
/*
 * 
 */
SD_Error sd_get_resp2(u32 * res2)
{
	return sd_get_resp_long(res2);
}

/*
 * 
 */
SD_Error sd_get_resp3(u32 * res3)
{
	return sd_get_resp_short(res3);
}

/*
 * 
 */
SD_Error sd_get_resp6(u32 * res6)
{
	return sd_get_resp_short(res6);
}

/*
 * 
 */
SD_Error sd_get_resp7(u32 * res7)
{
	return sd_get_resp_short(res7);
}

/*---------------------------------------------------------------------------*/

SD_Error sdio_handware_init(void)
{
	/*初始化SDIO的引脚、时钟、DMA等硬件*/
}

SD_Error sd_check_card_version(_SD_CardInfo * SD_CardInfo)
{
	SD_Error errorstatus;
	u32 res_short_temp;															/*short response temp*/
	u32 wait_cnt=100;															/*循环发送的次数*/
	
	errorstatus=sd_send_cmd(CMD0,0X00,NO_RESP);
	if(errorstatus!=SD_OK) return errorstatus;									/*check cmd0 send success*/
	
	errorstatus=sd_send_cmd(CMD8,SD_CHECK_PATTERN,RESP7);
	if(errorstatus!=SD_OK) return errorstatus;									/*check cmd8 send success*/
	errorstatus=sd_get_resp7(&res_short_temp);
	if (errorstatus == SD_OK)													/*有响应则card遵循sd协议2.0版本*/
	{
		if(res_short_temp==SD_CHECK_PATTERN) SD_CardInfo->sd_version=SD_20_SC;	/*默认为SD_20_SC，之后进一步检测*/
		else return SD_INVALID_VOLTRANGE;										/*不支持当前电压范围*/
	}
	else																		/*无响应，说明是1.x的或mmc的卡*/
	{
		SD_CardInfo->sd_version=SD_10;											/*默认为SD_10，之后进一步检测*/
	}
	
	/*The RCA to be used for CMD55 in idle_state shall be the card’s default RCA = 0x0000*/
	errorstatus=sd_send_cmd(CMD55,0X00,RESP1);
	if(errorstatus!=SD_OK) return errorstatus;									/*check cmd55 send success*/
	errorstatus=sd_get_resp1(&res_short_temp);
	if(errorstatus!=SD_OK) 
	{
		SD_CardInfo->sd_version=MMC;											/*此句是瞎编的*/
		return errorstatus;
	}
	
	while(wait_cnt--)															/*循环发送ACMD41以等待卡POWERUP*/
	{
		errorstatus=sd_send_cmd(CMD55,0X00,RESP1);
		if(errorstatus!=SD_OK) return errorstatus;								/*check cmd55 send success*/
		errorstatus=sd_get_resp1(&res_short_temp);
		if(errorstatus!=SD_OK) 
		{
			SD_CardInfo->sd_version=MMC;
			return errorstatus;
		}
		
		errorstatus=sd_send_cmd(CMD41,SD_VOLTAGE_WINDOW,RESP3);
		if(errorstatus!=SD_OK) return errorstatus;								/*check acmd41 send success*/
		errorstatus=sd_get_resp3(&res_short_temp);
		if(errorstatus==SD_OK)
		{
			if(res_short_temp & SD_CHECK_POWER_UP) break;						/*check sd card power up status =1*/
		}
		/*no response for ACMD41,or no power up,continue */
	}
	
	if(wait_cnt==0)																/*time over*/
	{
		return SD_UNUSABLE_CARD;												/*the card no response or invalid voltrange*/
	}
	
	if(SD_CardInfo->sd_version==SD_10) return SD_OK;							/*the card is SD V1.X*/
	
	if(res_short_temp & SD_CHECK_CCS) SD_CardInfo->sd_version=SD_20_HC;			/*CCS=1 sd card is High Capacity*/
	else SD_CardInfo->sd_version=SD_20_SC;
}


/*
 * 
 */
SD_Error sd_init(void)
{
	
}




