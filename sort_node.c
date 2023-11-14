
// Copyright (C) 2019 Keyu Tian. All rights reserved.
#include "sort_node.h"


// use a threshold of 6 can beat qsort in this course project (c++ is not permitted so std::sort is not available)
#define QSORT_THRESHOLD 6u

// median-three quick sort
void sort_node(AHashNode *a[], const unsigned n)
{
	if (n < QSORT_THRESHOLD)
	{
		// insertion sort
		AHashNode *t;
		unsigned i=1, j;
		for(; i<n; ++i)
		{
			j = i;
			t = a[j];
			while (j && less(t, a[j-1]))
				a[j] = a[j-1], --j;
			a[j] = t;
		}
		return;
	}

	// pick the pivot
	unsigned int mid = n >> 1;
	if (less(a[mid], a[0]))
		swap(a[mid], *a);
	if (less(a[n-1], a[0]))
		swap(a[n-1], *a);
	if (less(a[n-1], a[mid]))
		swap(a[n-1], a[mid]);
	AHashNode *pivot = a[mid];
	a[mid] = a[n-2];
	a[n-2] = pivot;

	// partition
	unsigned int pLeft = 0, pRight = n-2;
	while (1)
	{
		do ++pLeft;
		while (less(a[pLeft], pivot));
		do --pRight;
		while (less(pivot, a[pRight]));
		if (pLeft < pRight)
			swap(a[pLeft], a[pRight]);
		else
			break;
	}

	swap(a[pLeft], a[n-2]);
	sort_node(a, pLeft);
	sort_node(a+pLeft+1, n-1-pLeft);
}
