 #include "sd3.h"
 
 /*-------------------------------public variable--------------------------------------*/
_SD_CardInfo	SD_CardInfo;
static 	u32		csdtemp[4],cidtemp[4];
 /*------------------------------------------------------------------------------------*/

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
/*
 * the struct SD_CardInfo initialization
 */
void sd_info_init(_SD_CardInfo * pSD_CardInfo)
{
	
}

SD_Error sdio_handware_init(void)
{
	/*初始化SDIO的引脚、时钟、DMA等硬件*/
	
	/*!< SDIO_CK for initialization should not exceed 400 KHz */
}

/* @arg rca: the card RCA
 * @arg SDIO_BUS_4BIT: 4-bit data transfer
 * @arg SDIO_BUS_1BIT: 1-bit data transfer (sd card default)
 */
SD_Error sdio_bus_width(u32 rca,u32 bus_width)
{
	SD_Error errorstatus;
	u32 res_short_temp;
	
	/*send ACMD6 set the card's bus width*/
	errorstatus=sd_send_cmd(CMD55,(u32)(rca<<16),RESP1);
	if(errorstatus!=SD_OK) return errorstatus;								/*check cmd55 send success*/
	errorstatus=sd_get_resp1(&res_short_temp);
	if(errorstatus!=SD_OK) return errorstatus;

	errorstatus=sd_send_cmd(CMD6,bus_width,RESP1);
	if(errorstatus!=SD_OK) return errorstatus;								/*check cmd6 send success*/
	errorstatus=sd_get_resp1(&res_short_temp);
	if(errorstatus!=SD_OK) return errorstatus;
	
	/*config sdio of the mcu*/
	if(bus_width==SDIO_BUS_4BIT) SET_SDIO_BUS_WIDTH_4BIT;
	if(bus_width==SDIO_BUS_1BIT) SET_SDIO_BUS_WIDTH_1BIT;
	
	return SD_OK;
}

SD_Error sd_check_card_version(_SD_CardInfo * pSD_CardInfo)
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
		if(res_short_temp==SD_CHECK_PATTERN) pSD_CardInfo->sd_version=SD_20_SC;	/*默认为SD_20_SC，之后进一步检测*/
		else return SD_INVALID_VOLTRANGE;										/*不支持当前电压范围*/
	}
	else																		/*无响应，说明是1.x的或mmc的卡*/
	{
		pSD_CardInfo->sd_version=SD_10;											/*默认为SD_10，之后进一步检测*/
	}
	
	/*The RCA to be used for CMD55 in idle_state shall be the card’s default RCA = 0x0000*/
	errorstatus=sd_send_cmd(CMD55,0X00,RESP1);
	if(errorstatus!=SD_OK) return errorstatus;									/*check cmd55 send success*/
	errorstatus=sd_get_resp1(&res_short_temp);
	if(errorstatus!=SD_OK) 
	{
		pSD_CardInfo->sd_version=MMC;											/*此句是瞎编的，本lib不支持MMC*/
		return errorstatus;
	}
	
	while(wait_cnt--)															/*循环发送ACMD41以等待卡POWERUP*/
	{
		errorstatus=sd_send_cmd(CMD55,0X00,RESP1);
		if(errorstatus!=SD_OK) return errorstatus;								/*check cmd55 send success*/
		errorstatus=sd_get_resp1(&res_short_temp);
		if(errorstatus!=SD_OK) 
		{
			pSD_CardInfo->sd_version=MMC;										/*此句是瞎编的，本lib不支持MMC*/
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
	
	if(pSD_CardInfo->sd_version==SD_10) return SD_OK;							/*the card is SD V1.X*/
	
	if(res_short_temp & SD_CHECK_CCS) pSD_CardInfo->sd_version=SD_20_HC;		/*CCS=1 sd card is High Capacity*/
	else pSD_CardInfo->sd_version=SD_20_SC;
	return SD_OK;
}

SD_Error sd_get_cid(u32 * cid)
{
	SD_Error errorstatus;
	
	errorstatus=sd_send_cmd(CMD2,0X00,RESP2);
	if(errorstatus!=SD_OK) return errorstatus;									/*check cmd2 send success*/
	errorstatus=sd_get_resp2(cid);
	if(errorstatus!=SD_OK) return errorstatus;
	
	return SD_OK;
}

SD_Error sd_get_rca(u32 * rca)
{
	SD_Error errorstatus;
	
	errorstatus=sd_send_cmd(CMD3,0X00,RESP6);
	if(errorstatus!=SD_OK) return errorstatus;									/*check cmd3 send success*/
	errorstatus=sd_get_resp6(rca);												/*返回值包括RCA和card status*/
	if(errorstatus!=SD_OK) return errorstatus;
	/*-----------check the card status----------*/
	
	
	*rca >>= 16;																/*保留RCA*/
	return SD_OK;
}

SD_Error sd_get_csd(u32 rcd,u32 * csd)
{
	SD_Error errorstatus;
	
	errorstatus=sd_send_cmd(CMD9,(u32)(rcd<<16),RESP2);
	if(errorstatus!=SD_OK) return errorstatus;									/*check cmd9 send success*/
	errorstatus=sd_get_resp2(csd);
	if(errorstatus!=SD_OK) return errorstatus;
	
	return SD_OK;
}

SD_Error sd_select_card(u32 rca)
{
	SD_Error errorstatus;
	u32 res_short_temp;
	
	errorstatus=sd_send_cmd(CMD7,(u32)(rcd<<16),RESP1b);
	if(errorstatus!=SD_OK) return errorstatus;									/*check cmd7 send success*/
	errorstatus=sd_get_resp1b(&res_short_temp);
	if(errorstatus!=SD_OK) return errorstatus;
	
	return SD_OK;
}
/*
 * 
 */
SD_Error sd_init(void)
{
	SD_Error errorstatus;
	
	sd_info_init(&SD_CardInfo);													/*init sd card infomation default*/
	
	errorstatus=sdio_handware_init();											/*init hardware*/
	if(errorstatus!=SD_OK) return errorstatus;
	
	errorstatus=sd_check_card_version(&SD_CardInfo);							/*check sd card voltrange,and check sd card type*/
	if(errorstatus!=SD_OK) return errorstatus;
	
	/*!< Send CMD2 to get CID ,and the card will be in Identification State. */
	errorstatus=sd_get_cid(&csdtemp);
	if(errorstatus!=SD_OK) return errorstatus;
	
	/*get a new RCA,and card state changes to the Stand-by State */
	errorstatus=sd_get_rca(&SD_CardInfo.RCA);
	if(errorstatus!=SD_OK) return errorstatus;
	
	/*get CSD*/
	errorstatus=sd_get_csd(SD_CardInfo.RCA,&csdtemp);
	if(errorstatus!=SD_OK) return errorstatus;
	
	/*get CSR,send ACMD51*/
	
	
	/* send CMD7,CMD7 is used to select one card and put it into the Transfer State
	 * the card is selected by its own relative address and gets deselected by any other address;
	 */
	errorstatus=sd_select_card(SD_CardInfo.RCA);
	if(errorstatus!=SD_OK) return errorstatus;
	
	/*上电识别，卡初始化都完成后，进入数据传输模式，提高读写速度,并开启4bits模式*/
	errorstatus=sdio_bus_width(SD_CardInfo.RCA,SDIO_BUS_4BIT);
	if(errorstatus!=SD_OK) return errorstatus;
	
	return SD_OK;
}




