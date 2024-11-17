
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>


#define PI 3.14159265358979323846

#define ANCIENT_PI_PASSES 1000000000


/*
As you did not ask for the "fastest" or otherwise constrain your request, 
I offer this simple C code for an ancient method 1 - 1/3 + 1/5 - 1/7 + 1/9 - 1/11 ...
https://www.quora.com/What-is-the-method-for-calculating-Pi-in-the-programming-language-C
*/
double ancient_pi(double N) {

    double sign[2] = { +1.0, -1.0 };

    double divisor = 1.0, estimate = 0.0;

    // sign with bit manipulation only works if n is int, not double
    // for(int n=0; n<N; n++, divisor+=2) estimate += sign[n&1] / divisor;

    for(int n=0; n<N; n++, divisor+=2) estimate +=  ((n % 2) ? -1 : 1) / divisor;

    return 4.0 * estimate;
}


/*
Dik T. Winter's 160-byte C program to compute the first 800 digits of pi.
https://cs.uwaterloo.ca/~alopez-o/math-faq/mathtext/node12.html
https://crypto.stanford.edu/pbc/notes/pi/code.html
*/

int winter_pi() {

    int r[2800 + 1];
    int i, k;
    int b, d;
    int c = 0;

    for (i = 0; i < 2800; i++) {
	r[i] = 2000;
    }
    r[i] = 0;

    for (k = 2800; k > 0; k -= 14) {
	d = 0;

	i = k;
	for (;;) {
	    d += r[i] * 10000;
	    b = 2 * i - 1;

	    r[i] = d % b;
	    d /= b;
	    i--;
	    if (i == 0) break;
	    d *= i;
	}
	printf("%.4d", c + d / 10000);
	c = d % 10000;
    }

    return 0;
}


/*
TODO: Look into calculating PI with Monte Carlo simulation.
see: https://github.com/BaseMax/pi
*/


int repeatMeasure(int n) {
    double total_time = 0.0;
    for (int i=0; i<n; ++i) {
        clock_t begin = clock();
        double a_pi = ancient_pi(ANCIENT_PI_PASSES - i);  // insignificant change in argument value to hopefully prevent compiler optimizations
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("run #%d: ancient_pi=%f, clock_time_spent=%f secs\n", i+1, a_pi, time_spent);
        total_time += time_spent;
    }
    printf("ancient_pi %d rounds finished in total clock time: %f secs\n\n", n, total_time);
}



int main(int argc, char const *argv[])
{
    int n_repeat = 1;
    int default_repeat = 1;
    // double accuracy = (argc == 2) ? atof(argv[1]) : precission;

    if ((argc>2)||((argc==2)&&((strcmp(argv[1], "-h")==0)||(strcmp(argv[1], "--help")==0)))) {
        printf("\n\n--------------------------------------------------------------------------------------\n");
        printf("This program calculates PI in a ridiculously slow way in order to stress the CPU with floating point operations");
        printf("\n--------------------------------------------------------------------------------------\n\n");
        printf("But to not leave anybody wanting for PI:\nHere are 800 digits of PI calculated with black magic by mathematician Dik T. Winter:\n");

        winter_pi();

        printf("\n\nHow to use this program:\n");
        printf("\tRun computation:\t%s [n_repeat]\n", argv[0]);
        printf("\tGet help message:\t%s --help|-h\n", argv[0]);
        printf("\t\t-If argument n_repeat is not provided it is assumed to be %d\n\n", default_repeat);

        exit(1);
    }

    n_repeat = (argc == 2) ? atoi(argv[1]) : default_repeat;

    printf("\n\n-------- Running PI computation with acient PI method repeated %d times ---------------------------\n\n", n_repeat);

    repeatMeasure(n_repeat);

    return 0;
}
