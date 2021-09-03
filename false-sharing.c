#include <stdio.h>

// We want many iterations just to have many accesses to data.
// It turned out that this value results in a reasonable runtime and is still able to show the effects of false sharing.
// Feel free to adjust it to your machine speed.
#define N 1073741823

// The line size (in bytes) of the last cache level which is private to one core, which is usually the L2 cache.
// With the command 'lscpu -C' you can find the highest level private cache by searching for the highest level where ONE-SIZE != ALL_SIZE.
// From this row, use the value of the COHERENCY-SIZE column here.
#define CACHE_LINE_SIZE 64

typedef struct {
    int a;

    #ifdef PADDED
    // We remove one sizeof(int) from the padding because that's the size of the 'a' variable. sizeof(a) doesn't work here unfortunately
    char padding[CACHE_LINE_SIZE - sizeof(int)];
    #endif

    int b;

} mytype_t;

int main(int argc, char **argv) {
    // volatile, otherwise the compiler would remove the loops through optimization (already with -O1)
    volatile mytype_t data;
    data.a = 0;
    data.b = 0;

    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        {
            for (int i = 0; i < N; i++) {
                data.a += i;
            }
        }

        #pragma omp section
        {
            for (int i = 0; i < N; i++) {
                data.b += i;
            }
        }
    }

    // These values should be same at the end. It's just there to have some output.
    // There happens a lot of overflowing in this program. But doesn't matter for the purpose of this program. 
    printf("Final Value of data.a=%d\n", data.a);
    printf("Final Value of data.b=%d\n", data.b);
}
