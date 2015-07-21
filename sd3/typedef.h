#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#ifdef _WIN32	/* FatFs development platform */

#include <windows.h>
#include <tchar.h>

#else			/* Embedded platform */

/* This type MUST be 8 bit */
typedef unsigned char	BYTE,u8,uint8_t;

/* These types MUST be 16 bit */
typedef short			SHORT,s16,int16_t;
typedef unsigned short	WORD,u16,uint16_t;
typedef unsigned short	WCHAR;

/* These types MUST be 16 bit or 32 bit */
typedef int				INT;
typedef unsigned int	UINT;

/* These types MUST be 32 bit */
typedef long			LONG,s32,int32_t;
typedef unsigned long	DWORD,u32,uint32_t;

#endif

#endif
