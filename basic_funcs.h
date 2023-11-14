// Rank-1 solution of course project of DataStructure 2019'Spring.
// Copyright (C) 2019 Keyu Tian. All rights reserved.

#ifndef BASIC_FUNCS_H
#define BASIC_FUNCS_H


#include "file_read.h"

// much faster than printf
#define print_integer(_val) \
	tp = _val; \
	do numStack[top++] = tp % 10u; \
	while (tp /= 10u); \
	while (top--) \
		*pCurBuf++ = '0'+numStack[top]; \
	++top, *pCurBuf++ = ' '


#endif // BASIC_FUNCS_H
