// Rank-1 solution of course project of DataStructure 2019'Spring.
// Copyright (C) 2019 Keyu Tian. All rights reserved.

#ifndef D_HASH_TABLE_H	// dictionary hash table
#define D_HASH_TABLE_H


#include "config.h"

typedef struct __DHashNode
{
	struct __DHashNode *next;
	char *str;
} DHashNode;


#define insert_dic_node(_str, _hashCode1) \
	(++pCurDicMemPool)->next = dicHashTable[_hashCode1], \
	pCurDicMemPool->str = _str, \
	dicHashTable[_hashCode1] = pCurDicMemPool



#endif // D_HASH_TABLE_H
