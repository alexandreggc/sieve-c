#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// Define this to enable parallelization
unsigned long long sieveParallel(unsigned long long n) {
    unsigned long long nEven = n / 2;

    char* isPrime = (char*)malloc((nEven + 1) * sizeof(char));
    if (isPrime == 0) return 0;

    for (unsigned long long i = 0; i <= nEven; i++) {
        isPrime[i] = 1;
    }

    if (n >= 0) isPrime[0] = 0;

    unsigned long long limit = sqrt((double)n);

    #pragma omp parallel for schedule(dynamic)
    for (unsigned long long value = 1; value <= limit; value+=2) {
        if (isPrime[value / 2]) {
            for (unsigned long long mulValue = value * value; mulValue <= n; mulValue += value) {
                if (mulValue % 2)
                    isPrime[mulValue / 2] = 0;
            }
        }
    }

    unsigned long long primeCount = 0;

    #pragma omp parallel for reduction(+:primeCount)
    for (unsigned long long i = 1; i < nEven+1; i++) {
        if (isPrime[i]) {
            primeCount++;
        }
    }

    free(isPrime);

    return primeCount;
    
}

unsigned long long sieveSequential(unsigned long long n) {
    unsigned long long nEven = n / 2;

    char* isPrime = (char*)malloc((nEven + 1) * sizeof(char));
    if (isPrime == 0) return 0;

    for (unsigned long long i = 0; i <= nEven; i++) {
        isPrime[i] = 1;
    }

    if (n >= 0) isPrime[0] = 0;

    unsigned long long limit = sqrt((double)n);

    for (unsigned long long value = 1; value <= limit; value+=2) {
        if (isPrime[value / 2]) {
            for (unsigned long long mulValue = value * value; mulValue <= n; mulValue += value) {
                if (mulValue % 2)
                    isPrime[mulValue / 2] = 0;
            }
        }
    }

    unsigned long long primeCount = 0;
    for (unsigned long long i = 1; i < nEven+1; i++) {
        if (isPrime[i]) {
            primeCount++;
        }
    }

    free(isPrime);

    return primeCount;
    
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }

    unsigned long long n = strtoull(argv[1], NULL, 10);
    int iterations = 5;

    double startTime, endTime;
    double timeTaken;
    unsigned long long result;

    double execTimes[iterations];
    for (int i=0; i< iterations; i++) {
        startTime = omp_get_wtime();
        result = sieveSequential(n);
        endTime = omp_get_wtime();
        timeTaken = ((double)(endTime - startTime));
        execTimes[i] = timeTaken;
        printf("Sequential Time: %lf seconds, N primes: %llu\n", timeTaken, result);
    }
    double avgTime1 = (execTimes[2] + execTimes[3] + execTimes[4]) / 3;

    printf("Average Sequential Time: %lf seconds\n", (execTimes[0] + execTimes[1] + execTimes[2] + execTimes[3] + execTimes[4]) / 5);

    for (int i=0; i< iterations; i++) {
        startTime = omp_get_wtime();
        result = sieveParallel(n);
        endTime = omp_get_wtime();
        timeTaken = ((double)(endTime - startTime));
        execTimes[i] = timeTaken;
        printf("Parallel Time: %lf seconds, N primes: %llu\n", timeTaken, result);
    }
    printf("Average Parallel Time: %lf seconds\n", (execTimes[0] + execTimes[1] + execTimes[2] + execTimes[3] + execTimes[4]) / 5);
    double avgTime2 = (execTimes[2] + execTimes[3] + execTimes[4]) / 3;

    printf("Speedup: %f\n", avgTime1 / avgTime2);

    return 0;
}
