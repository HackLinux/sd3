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
 SD_Error sd_get_resp1(u32 res1)
 {
	 SD_Error errorstatus = SD_OK;
	 
	 /*等待接收完毕，清除pending*/
	 
	 /*接收到的值->res1*/
	 
	 return errorstatus;
 }

/*
 * 
 */
SD_Error sd_get_resp1b(u32 res1b)
{
	 SD_Error errorstatus = SD_OK;
	 
	 /*check busy*/
	 
	 /*等待接收完毕，清除pending*/
	 
	 /*接收到的值->res1*/
	 
	 return errorstatus;
}
 
/*
 * 
 */
SD_Error sd_get_resp2(u32 * res2)
{
	 SD_Error errorstatus = SD_OK;
	 
	 /*等待接收完毕，清除pending*/
	 
	 /*接收到的值->res2*/
	 
	 return errorstatus;
}


