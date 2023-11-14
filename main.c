// Rank-1 solution of course project of DataStructure 2019'Spring.
// Copyright (C) 2019 Keyu Tian. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "config.h"
#include "optim.h"
#include "basic_funcs.h"
#include "a_hash_table.h"
#include "d_hash_table.h"
#include "sort_node.h"


//extern int true_main(void) __asm__ ("main2");


//// manuall stack allocation; but finally not used cuz it did not improve performance
//int main()
//{
//	unsigned size_128MB = 64u << 20;
//	char *stack_mem = (char *)malloc(size_128MB) + size_128MB;
//
//	__asm__ __volatile__
//	(
//		"movq  %0, %%rsp\n"
//		"pushq $exit\n"
//		"jmp main2\n"
//		: : "r"(stack_mem)
//	);
//}


int main()
{
	// malloc on heap will introduce more cache misses and increses the overhead due to system calls
	// so i use static segment instead
	static DHashNode dicMemPool[MAX_DIC_WORDS_CNT];	DHashNode *pCurDicMemPool = dicMemPool;
	static DHashNode *dicHashTable[TABLE_SIZE];		DHashNode **pCurDicHashTable, *pDicHashNode;

	static AHashNode atcMemPool[MAX_WR_WORDS_CNT];	AHashNode *pCurAtcMemPool = atcMemPool;
	static AHashNode *atcHashTable[TABLE_SIZE];		AHashNode **pCurAtcHashTable, *pAtcHashNode;

	AHashNode *ansNodes[MAX_WR_WORDS_CNT], **pAnsNodesBegin=ansNodes, **pAnsNodesEnd;

	PosNode posMemPool[MAX_WR_TIMES], *pCurPosMenPool = posMemPool;
	PosNode *pPosNode;

	int fileBytes;
	static char __dicBuf[MAX_DIC_LEN]; char * const dicBuf=__dicBuf + 1;
	static char __atcBuf[MAX_ATC_LEN]; char * const atcBuf=__atcBuf + 1;
	static char outputBuf[MAX_ANS_LEN];
	register char *pCurBuf;
	char *pBufEnd, *pWordBegin;

	FILE *pFin, *pFout;

	{
		register HashInt1 hashCode1;
		register HashInt2 hashCode2;

		read_file("dictionary.txt", dic);	// read and hash the dictionary (vocabulary set)
		up_to_low(dic);
		for (pCurBuf = dicBuf; pCurBuf < pBufEnd; ++pCurBuf)
		{
			if (is_lower(*pCurBuf))
			{
				pWordBegin = pCurBuf, hashCode1 = hashCode2 = 0u;
				do  hashCode1 = (hashCode1 * MUL_VAL + *pCurBuf++);		// i actually tried to use inline assembly to speed up the hash function, with 'leal' command used (9 as hash seed) and it can be 1.2x faster
				while (is_lower(*pCurBuf));								// however for the sake of safety, i did not use 9 as hash seed in the final version (but 131 instead)
				hashCode1 &= AND_VAL;									// for 131, we cannot use assembly like 'leal' to speed up
				*pCurBuf = '\0', insert_dic_node(pWordBegin, hashCode1);
			}
		}

		read_file("article.txt", atc);		// read each word in the article and check if it is misspelled (not in the vocabulary set); if so, insert it into another hash table (for output)
#ifdef _VSC_KEVIN
	{
		char *p;
		for (p=pBufEnd=atcBuf; *p; ++p)
			if (*p != '\r')
				*pBufEnd++=*p;
		*pBufEnd = 0;
		fileBytes = pBufEnd - atcBuf;
	}
#endif
		up_to_low(atc);

		for (pCurBuf = atcBuf; pCurBuf < pBufEnd; ++pCurBuf)
		{
			if (is_lower(*pCurBuf))
			{
				pWordBegin = pCurBuf, hashCode1 = hashCode2 = 0u;		// i actually tried to use inline assembly to speed up the hash function, with 'leal' command used (9 as hash seed) and it can be 1.2x faster
				do  hashCode1 = (hashCode1 * MUL_VAL + *pCurBuf++);		// however for the sake of safety, i did not use 9 as hash seed in the final version (but 131 instead)
				while (is_lower(*pCurBuf));								// for 131, we cannot use assembly like 'leal' to speed up
				hashCode1 &= AND_VAL;
				*pCurBuf = '\0', set_cur_atc_ptr(hashCode1); // set pCurAtcHashTable and pCurDicHashTable

				if (*pCurDicHashTable)	// hash hit; double check if the word is exactly in the vocabulary set
				{
					for (pDicHashNode = *pCurDicHashTable; pDicHashNode; pDicHashNode = pDicHashNode->next)
						if (!strcmp(pDicHashNode->str, pWordBegin))
							goto found;	// exactly the same word
					goto not_found;		// just a hash hit, not exactly the same
				}
				else	// not hit, misspelled
				{
					not_found:
					if (*pCurAtcHashTable)
					{
						for (pAtcHashNode = *pCurAtcHashTable; pAtcHashNode; pAtcHashNode = pAtcHashNode->next)
						{
							if (!strcmp(pAtcHashNode->str, pWordBegin))
							{
								push_back_pos_node(pWordBegin - atcBuf);
								goto found;
							}
						}
						insert_atc_node(pWordBegin, pWordBegin - atcBuf);
					}
					else
						insert_atc_node(pWordBegin, pWordBegin - atcBuf);
				}
				found:;
			}
		}
		set_cur_ans_ptr(); 	// set pAnsNodesEnd and pAnsNodesBegin
	}


	{
		unsigned numStack[8];
		register unsigned tp, top = 0u;
		char *pCurWord;

		sort_node(ansNodes, pAnsNodesEnd - pAnsNodesBegin);		// i found a median-three quick sort with a threshould of 6 can beat qsort in this course project (c++ is not permitted so std::sort is not available)
		for (pCurBuf = outputBuf; pAnsNodesBegin < pAnsNodesEnd; ++pAnsNodesBegin)
		{
			pCurWord = (*pAnsNodesBegin)->str;
			while (*pCurBuf++ = *pCurWord++);
			pCurBuf[-1] = ' ';

			print_integer((*pAnsNodesBegin)->frequency);
			for (pPosNode = (*pAnsNodesBegin)->posHead; pPosNode; pPosNode = pPosNode->next)
			{
				print_integer(pPosNode->pos);		// much faster than printf
			}

#ifdef _VSC_KEVIN
		++pCurBuf;
		pCurBuf[-2] = '\r', pCurBuf[-1] = '\n';
#else
		pCurBuf[-1] = '\n';
#endif
		}
	}

#ifdef _VSC_KEVIN
	printf("tot size:%.2f MB\n", 
	(
		sizeof(dicMemPool)+
		sizeof(dicHashTable)+
		sizeof(atcMemPool)+
		sizeof(atcHashTable)+
		sizeof(ansNodes)+
//		sizeof(posMemPool)+
		sizeof(__dicBuf)+
		sizeof(__atcBuf)+
		sizeof(outputBuf)+200
	) / 1000.0 / 1000.0 
	);
#endif

	fwrite(outputBuf, pCurBuf - outputBuf, 1, pFout = fopen("misspelling.txt", "wb"));
	fclose(pFout);

	return(0);
}

