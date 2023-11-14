// Rank-1 solution of course project of DataStructure 2019'Spring.
// Copyright (C) 2019 Keyu Tian. All rights reserved.

#ifndef SORT_NODE_H
#define SORT_NODE_H


#include <string.h>
#include "a_hash_table.h"


#define swap(p1, p2) do{AHashNode *tp=p1;p1=p2;p2=tp;} while(0)
#define less(p1, p2) (p1->frequency > p2->frequency || (p1->frequency == p2->frequency && strcmp(p1->str, p2->str) < 0))

void sort_node(AHashNode *a[], const unsigned n);


#endif // SORT_NODE_H
