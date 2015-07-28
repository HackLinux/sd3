#ifndef _SD3_H
#define _SD3_H

#include "typedef.h"

/* --------------------------Exported types ---------------------------------*/
typedef enum
{
/** 
  * @brief  SDIO specific error defines  
  */   
  SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
  SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
  SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
  SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
  SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
  SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
  SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
  SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
  SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
  SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
  SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
  SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
  SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
  SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
  SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
  SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
  SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
  SD_CC_ERROR                        = (18), /*!< Internal card controller error */
  SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
  SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
  SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
  SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
  SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
  SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
  SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
  SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
  SD_INVALID_VOLTRANGE               = (27),
  SD_ADDR_OUT_OF_RANGE               = (28),
  SD_SWITCH_ERROR                    = (29),
  SD_SDIO_DISABLED                   = (30),
  SD_SDIO_FUNCTION_BUSY              = (31),
  SD_SDIO_FUNCTION_FAILED            = (32),
  SD_SDIO_UNKNOWN_FUNCTION           = (33),
  SD_UNUSABLE_CARD                   = (34),/*!< the card no response or invalid voltrange. */

/** 
  * @brief  Standard error defines   
  */ 
  SD_INTERNAL_ERROR, 
  SD_NOT_CONFIGURED,
  SD_REQUEST_PENDING, 
  SD_REQUEST_NOT_APPLICABLE, 
  SD_INVALID_PARAMETER,  
  SD_UNSUPPORTED_FEATURE,  
  SD_UNSUPPORTED_HW,  
  SD_ERROR,  
  SD_OK = 0 
} SD_Error;

/** 
  * @brief  SDIO Transfer state  
  */   
typedef enum
{
  SD_TRANSFER_OK  = 0,
  SD_TRANSFER_BUSY = 1,
  SD_TRANSFER_ERROR
} SDTransferState;

/** 
  * @brief  SD Card States 
  */   
typedef enum
{
  SD_CARD_READY                  = ((uint32_t)0x00000001),
  SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
  SD_CARD_STANDBY                = ((uint32_t)0x00000003),
  SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
  SD_CARD_SENDING                = ((uint32_t)0x00000005),
  SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
  SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
  SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
  SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
} SDCardState;
/*----------------------------------sd card type--------------------------------*/
/*
 *
 * */
typedef enum
{
	NOT_SD, SD_10, SD_20_SC, SD_20_HC, MMC
} SD_VERSION;

typedef struct {
	unsigned short dwRCA;
    unsigned int dwCap;
    unsigned int dwNextLBA;
    enum sd_version	eVer;
    CARD_STATE	eCardState;
} SD_INFO;

/** 
  * @brief SD Card information 
  */
typedef struct
{
//  SD_CSD SD_csd;
//  SD_CID SD_cid;
  uint32_t CardCapacity;  			/*!< Card Capacity */
  uint32_t CardBlockSize; 			/*!< Card Block Size */
  uint32_t RCA;
  SD_VERSION sd_version;
} _SD_CardInfo;

/*-------------------------------------#define----------------------------------*/
#define SD_CHECK_POWER_UP				((uint32_t)0x80000000)			/*ACMD41 response check*/
#define SD_CHECK_CCS					((uint32_t)0x40000000)			/*ACMD41 response check*/
/*-----------------------------------sd card cmd--------------------------------*/
#define CMD0															/* GO_IDLE_STATE */
#define CMD1															/* SEND_OP_COND (MMC) */
#define CMD2															/* ALL_SEND_CID */
#define CMD3															/* SEND_RELATIVE_ADDR */
#define CMD8															/* SEND_IF_COND */
#define CMD55
#define CMD41
/*---------------------------------sd card argument-----------------------------*/
#define SD_CHECK_PATTERN				((uint32_t)0x000001AA)			/*user define CMD8 argument*/
#define SD_VOLTAGE_WINDOW				((uint32_t)0x40100000)			/*user define ACMD41 argument*/

/*-------------------------------sd card response type--------------------------*/
#define NO_RESP
#define RESP1
#define RESP1b
#define RESP2
#define RESP3
#define RESP6
#define RESP7




/*-------------------------------public functions--------------------------------*/
SD_Error sd_send_cmd(u32 cmd,u32 arg,u32 restype);
SD_Error sd_get_resp1(u32 * res1);
SD_Error sd_get_resp1b(u32 * res1b);
SD_Error sd_get_resp2(u32 * res2);
SD_Error sd_get_resp3(u32 * res3);
SD_Error sd_get_resp6(u32 * res6);
SD_Error sd_get_resp7(u32 * res7);


#endif
