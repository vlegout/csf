/*
 *
 * Copyright 2014 Vincent Legout <vincent@legout.info>
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
 * Based on the following paper:
 *
 * Compositional Real-Time Scheduling Framework with Periodic Model
 * INSIK SHIN and INSUP LEE, University of Pennsylvania
 * ACM Trans. Embed. Comput. Syst., April 2008
 *
 */

#include <math.h>
#include <stdio.h>

#define INC 0.001

struct task {
	double wcet;
	double period;
};

struct server {
	double budget;
	double period;
};

static double gcd(double a, double b)
{
	double div = b / a;
	int d = ((int)b) / ((int)a);

	if (div - d < 0 || div - d > 0)
		return gcd(b - d * a, a);
	else
		return a;
}

__attribute__ ((unused))
static double get_hyperperiod(int n_tasks, struct task *t)
{
	int i;
	double h = 1, g;

	for (i = 0; i < n_tasks; i++) {
		g = gcd(h, t[i].period);

		h = h * t[i].period / g;
	}

	return h;
}

static double compute_sbf(double time, struct server *s)
{
	double sbf;
	int k = ceil((time - (s->period - s->budget)) / s->period);

	if (k < 1)
		k = 1;

	if (time >= ((k + 1) * s->period - 2 * s->budget) &&
	    time <= ((k + 1) * s->period - s->budget)) {
		sbf = time - (k + 1) * (s->period - s->budget);
	} else {
		sbf = (k - 1) * s->budget;
	}

	return sbf;
}

__attribute__ ((unused))
static int is_sched_edf(struct task *t, int n_tasks, struct server *s)
{
	int i;
	double dbf, sbf, time;

	double hyperperiod = get_hyperperiod(n_tasks, t);

	for (time = 0; time <= hyperperiod; time++) {

		dbf = 0;

		for (i = 0; i < n_tasks; i++) {
			dbf += floor(time / t[i].period) * t[i].wcet;
		}

		sbf = compute_sbf(time, s);

		if (dbf > sbf)
			return 0;
	}

	return 1;
}

__attribute__ ((unused))
static int is_sched_rm(struct task *t, int n_tasks, struct server *s)
{
	int i, j, schedulable;
	double dbf, sbf, time;

	for (j = 0; j < n_tasks; j++) {

		schedulable = 0;

		for (time = 1; time <= t[j].period; time++) {

			dbf = t[j].wcet;

			for (i = 0; i < n_tasks; i++) {
				if (i != j &&
				    (t[i].period >= t[j].period ||
				     (t[i].period == t[j].period && j < i))) {
					dbf += ceil(time / t[i].period) * t[i].wcet;
				}
			}

			sbf = compute_sbf(time, s);

			if (dbf <= sbf)
				schedulable = 1;
		}

		if (schedulable == 0)
			return 0;
	}

	return 1;
}

int main(void)
{
	int schedulable;
	double b;

	struct server s = { 0.0, 0.0 };

	struct task t[2] = { {7.0, 50.0}, {9.0, 75.0} };	// 2.8, 10
	/* struct task t[2] = { {5.0, 40.0}, {4.0, 25.0} };     // 3.1, 10 */
	/* struct task t[2] = { {3.1, 10.0}, {4.4, 10.0} };     // 4.4, 5 */

	int n_tasks = 2;

	double period = 10.0;

#ifdef SOLUTION_SPACE
	double period_max = 75;

	for (period = 1; period < period_max; period++) {
#endif

		s.period = period;

		for (b = 0; b < period; b += INC) {
			s.budget = b;

#ifdef RM
			schedulable = is_sched_rm(t, n_tasks, &s);
#else
			schedulable = is_sched_edf(t, n_tasks, &s);
#endif

			if (schedulable) {
#ifndef SOLUTION_SPACE
				printf("budget = %lf, period = %lf\n", b, period);
#endif
				break;
			}
		}

#ifdef SOLUTION_SPACE
		printf("%lf %lf\n", period, b / period);

	}
#endif

	return 0;
}
