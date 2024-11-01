
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846


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


int main(int argc, char const *argv[])
{

    // double accuracy = (argc == 2) ? atof(argv[1]) : precission;

    winter_pi();

    printf("\n\n------------------------------------------------\n\n");

    clock_t begin = clock();

    printf("ancient_pi=%f\n", ancient_pi(1000000000));

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("ancient_pi clock time spent: %f secs\n\n", time_spent);

    return 0;
}
