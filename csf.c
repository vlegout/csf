/*
 *
 * Compositional Real-Time Scheduling Framework with Periodic Model
 *
 * INSIK SHIN and INSUP LEE, University of Pennsylvania
 *
 * Vincent Legout <vincent@legout.info>
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

int main(void)
{
	int i, j, schedulable;
	double b, dbf, time, sbf;

	struct server s = { 0.0, 0.0 };

	struct task t[2] = { {7.0, 50.0}, {9.0, 75.0} };	// 2.8, 10
	/* struct task t[2] = { {5.0, 40.0}, {4.0, 25.0} };     // 3.1, 10 */
	/* struct task t[2] = { {3.1, 10.0}, {4.4, 10.0} };     // 4.4, 5 */

	int n_tasks = 2;

	double period = 10.0;
#ifdef SOLUTION_SPACE
	double period_max = 75;
#endif

#ifndef RM
	double hyperperiod = get_hyperperiod(n_tasks, t);
#endif

#ifdef SOLUTION_SPACE
	for (period = 1; period < period_max; period++) {
#endif

		s.period = period;

		for (b = 0; b < period; b += INC) {
			s.budget = b;

			schedulable = 0;

#ifdef RM
			for (j = 0; j < n_tasks; j++) {
				for (time = 1; time <= t[j].period; time++) {

					dbf = t[j].wcet;

					for (i = 0; i < n_tasks; i++) {
						if (i != j && (t[i].period >= t[j].period || (t[i].period == t[j].period && j < i))) {
							dbf += ceil(time / t[i].period) * t[i].wcet;
						}
					}

					sbf = compute_sbf(time, &s);

					if (dbf <= sbf) {
						schedulable = 1;
						goto end;
					}
				}
			}
end:

#else
			for (time = 0; time <= hyperperiod; time++) {

				dbf = 0;

				for (i = 0; i < n_tasks; i++) {
					dbf += floor(time / t[i].period) * t[i].wcet;
				}

				sbf = compute_sbf(time, &s);

				if (dbf > sbf) {
					schedulable = 1;
					break;
				}
			}
#endif

#ifdef RM
			if (schedulable) {
#else
			if (!schedulable) {
#endif
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
