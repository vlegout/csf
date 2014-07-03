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

#define PERIOD 10.0
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

int main(void)
{
	int i, schedulable;

	struct task t[2] = { {7.0, 50.0}, {9.0, 75.0} };	// 2.8, 10
	/* struct task t[2] = { {5.0, 40.0}, {4.0, 25.0} };     // 3.1, 10 */
	/* struct task t[2] = { {3.1, 10.0}, {4.4, 10.0} };     // 4.4, 5 */

	int n_tasks = 2;

	struct server s = { 0.0, PERIOD };

	double time;
	double b;

	double dbf, sbf;
	double k;

	double hyperperiod = get_hyperperiod(n_tasks, t);

	for (b = 0; b < PERIOD; b += 0.1) {
		s.budget = b;

		schedulable = 1;

		for (time = 0; time <= hyperperiod; time++) {
			dbf = 0;
			sbf = 0;

			for (i = 0; i < n_tasks; i++) {
				dbf += floor(time / t[i].period) * t[i].wcet;
			}

			k = ceil((time - (s.period - s.budget)) / s.period);

			if (k < 1)
				k = 1;

			if (time >= ((k + 1) * s.period - 2 * s.budget) &&
			    time <= ((k + 1) * s.period - s.budget)) {
				sbf = time - (k + 1) * (s.period - s.budget);
			} else {
				sbf = (k - 1) * s.budget;
			}

			if (dbf > sbf) {
				schedulable = 0;
				break;
			}

			/* printf("b %lf, time %.0lf: dbf %lf, sbf %lf, k %lf\n", b, time, dbf, sbf, k); */
		}

		if (schedulable) {
			printf("budget = %lf, period = %lf\n", b, PERIOD);
			break;
		}
	}

	return 0;
}
