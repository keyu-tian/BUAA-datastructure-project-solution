// Rank-1 solution of course project of DataStructure 2019'Spring.
// Copyright (C) 2019 Keyu Tian. All rights reserved.

#ifndef FILE_READ_H
#define FILE_READ_H


#define is_lower(c) (c>='a' && c<='z')
#define is_upper(c) (c>='A' && c<='Z')

#define _UL(x) if(is_upper(pCurBuf[x])) pCurBuf[x] += 32;
#define up_to_low(who) \
	for(pCurBuf = who##Buf ; pCurBuf < pBufEnd; pCurBuf += 4) \
	{ \
		_UL(0)_UL(1)_UL(2)_UL(3) \
	} \

#define read_file(PATH, who) \
	pFin = fopen(PATH, "rb"), \
	fseek(pFin, 0, SEEK_END), \
	pBufEnd = who##Buf + (fileBytes = ftell(pFin)), \
	fseek(pFin, 0, SEEK_SET), \
	fread(who##Buf, fileBytes, 1, pFin), \
	fclose(pFin)


#endif // FILE_READ_H
