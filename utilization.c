/*
 *
 * Copyright 2015 Vincent Legout <vincent@legout.info>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <limits.h>
#include <math.h>
#include <stdio.h>

#define U_INC 0.001

/* Set the number of tasks to +inf */
#define N ((double)INT_MAX)

#define BUDGET 5.0
#define PERIOD 10.0
#define P_MIN 20.0

int get_k(double p_min)
{
	int k = 0;
	double tmp;

	do {
		k++;

		tmp = (k + 1) * PERIOD - BUDGET;

#ifndef RM
		tmp -= (k * BUDGET) / (k + 2);
#endif

	} while (tmp < p_min);

	return k - 1;
}

double get_edf_bound(double u, int k)
{
	if (k <= 0)
		k = (double)get_k(P_MIN);

	/* return (k * BUDGET / PERIOD) / (k + 2 * (1 - (BUDGET / PERIOD))); */

	return (k * u) / (k + 2 * (1 - u));
}

double get_rm_bound(double u, int k)
{
	if (k <= 0)
		k = (double)get_k(P_MIN);

	double r;

	r = (2 * k + 2 * (1 - u)) / (k + 2 * (1 - u));
	r = pow(r, 1 / N);
	r = r - 1;
	r = u * N * r;

	return r;
}

int main(void)
{
	double bound, i;
	int j;

	int k[] = {1, 2, 4, 16};
	int len_k = 4;

	printf("U");
	for (j = 0; j < len_k; j++)
		printf(" k=%d", k[j]);
	printf("\n");

	for (i = 0; i < 1.0; i += U_INC) {

		printf("%lf", i);

		for (j = 0; j < len_k; j++) {

#ifdef RM
			bound = get_rm_bound(i, k[j]);
#else
			bound = get_edf_bound(i, k[j]);
#endif

			printf(" %lf", bound);
		}

		printf("\n");

	}

	return 0;
}
