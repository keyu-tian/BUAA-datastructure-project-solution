// Rank-1 solution of course project of DataStructure 2019'Spring.
// Copyright (C) 2019 Keyu Tian. All rights reserved.

#ifndef A_HASH_TABLE_H	// artical hash table
#define A_HASH_TABLE_H


typedef struct __PosNode
{
	struct __PosNode *next;
	unsigned int pos;
} PosNode;

typedef struct __AHashNode
{
	struct __AHashNode *next;
	struct __PosNode *posHead, *posTail;
	unsigned int frequency;
	char *str;
} AHashNode;


#define set_cur_atc_ptr(_hashCode1) \
	pCurAtcHashTable = atcHashTable + _hashCode1, pCurDicHashTable = dicHashTable + _hashCode1

#define set_cur_ans_ptr() \
	pAnsNodesEnd = pAnsNodesBegin, pAnsNodesBegin = ansNodes;


//pAtcHashNode->posTail->next = NULL;
#define push_back_pos_node(_pos) \
	++pAtcHashNode->frequency, \
	pAtcHashNode->posTail = pAtcHashNode->posTail->next = ++pCurPosMenPool, \
	pAtcHashNode->posTail->pos = _pos



//	pCurAtcMemPool->posTail->next = NULL,
#define insert_atc_node(_str, _pos) \
	(++pCurAtcMemPool)->next = *pCurAtcHashTable, \
	++pCurAtcMemPool->frequency, \
	pCurAtcMemPool->str = _str, \
	pCurAtcMemPool->posHead = pCurAtcMemPool->posTail = ++pCurPosMenPool, \
	pCurAtcMemPool->posTail->pos = _pos, \
	*pAnsNodesBegin++ = pCurAtcMemPool, \
	*pCurAtcHashTable = pCurAtcMemPool


#endif // A_HASH_TABLE_H
