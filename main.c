/*
    Authors: 20907541, 30939506
    Due Date: 3/24/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

//#define threshold 1


int **newMatrix(int d) {
    int **m = (int**) malloc(d * sizeof(int*));

    // allocating memory for each row in the matrix
    for (int i = 0; i < d; i++){
        m[i] = (int *) calloc(d, sizeof(int));
    }

    return m;
}

/* Function that generates a random
   matrix with dimension d
   For testing purposes
*/
int **genrand_Matrix(int d) {
    //time_t t;
    //srand(time(NULL));

    int **m = newMatrix(d);

    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            m[i][j] = rand() % 3;
        }
    }

    return m;
}

// Included in neg so that it's not necessary to create subtraction func
int **add_matrix(int **a, int **b, int d, int neg) {

    int **c = newMatrix(d);
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            c[i][j] = a[i][j] + neg * b[i][j];
        }
    }

    return c;
}


int **standardmult(int **a, int **b, int **c, int d) {
    //int **c = newMatrix(d);
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            for (int k = 0; k < d; k++) {
                c[i][k] += a[i][j] * b[j][k];
            }
        }
    }
    return c;
}

// Freeing the matrix
void freeMatrix(int d, int **a) {
    for (int i = 0; i < d; i++) {
        free(a[i]);
    }

    free(a);
}

// Strassen's algorithm
int **strassenMod(int **x, int **y, int **z, int dim, int k) {

    //int **z = newMatrix(dim);
    
    if (dim <= k)
        return standardmult(x, y, z, dim);

    else if (dim == 1) {
        z[0][0] = x[0][0] * y[0][0];
        return z;
    }

    else {

        int half = dim / 2;

        int **a, **c, **e, **g;
        int **b = newMatrix(half);
        int **d = newMatrix(half);
        int **f = newMatrix(half);
        int **h = newMatrix(half);

        int **p1 = newMatrix(half);
        int **p2 = newMatrix(half);
        int **p3 = newMatrix(half);
        int **p4 = newMatrix(half);
        int **p5 = newMatrix(half);
        int **p6 = newMatrix(half);
        int **p7 = newMatrix(half);

        for (int i = 0; i < half; i++) {
            for (int j = 0; j < half; j++) {
                b[i][j] = x[i][j + half];
                d[i][j] = x[i + half][j + half];
                f[i][j] = y[i][j + half];
                h[i][j] = y[i + half][j + half];
            }
        }

        a = x;
        c = &x[half];

        e = y;
        g = &y[half];


        strassenMod(a, add_matrix(f, h, half, -1), p1, half, k); // A(F − H)
        strassenMod(add_matrix(a, b, half, 1), h, p2, half, k); // (A + B)H
        strassenMod(add_matrix(c, d, half, 1), e, p3, half, k); // (C + D)E
        strassenMod(d, add_matrix(g, e, half, -1), p4, half, k); // D(G - E)
        strassenMod(add_matrix(a, d, half, 1), add_matrix(e, h, half, 1), p5, half, k); // (A + D)(E + H)
        strassenMod(add_matrix(b, d, half, -1), add_matrix(g, h, half, 1), p6, half, k); // (B - D)(G + H)
        strassenMod(add_matrix(a, c, half, -1), add_matrix(e, f, half, 1), p7, half, k); // (A - C)(E + F)

        for (int i = 0; i < half; i++) {
            for (int j = 0; j < half; j++) {
                // AE + BG = P5 + P4 - P2 + P6
                z[i][j] = p5[i][j] + p4[i][j] - p2[i][j] + p6[i][j]; 
                // AF + BH = P1 + P2
                z[i][j + half] = p1[i][j] + p2[i][j];
                // CE + DG = P3 + P4 
                z[i + half][j] = p3[i][j] + p4[i][j]; 
                // CF + DH = P5 + P1 - P3 - P7
                z[i + half][j + half] = p5[i][j] + p1[i][j] - p3[i][j] - p7[i][j]; 
            }
        }

        freeMatrix(half, b); freeMatrix(half, d);
        freeMatrix(half, f); freeMatrix(half, h);
        freeMatrix(half, p1); freeMatrix(half, p2); freeMatrix(half, p3); freeMatrix(half, p4);
        freeMatrix(half, p5); freeMatrix(half, p6); freeMatrix(half, p7);
        return z;

    }
}

// Padding 
int padding(int d, int threshold) {
    int tmp = d;
    while (tmp > threshold) {
        tmp = (tmp + 1) / 2;
    }

    while (tmp < d) {
        tmp *= 2;
    }

    return tmp;
}

void printDiags(int d, int **a) {
// only print the diagonals, ignore the paddings
    for (int i = 0; i < d; i++){
        printf("%d\n", a[i][i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {

    // Error Handling
    if (argc != 5) {
        printf("Format is: ./strassen flag dimension inputfile threshold");
        return 1;
    }

    int flag = atoi(argv[1]);
    int dimension = atoi(argv[2]);
    int t = atoi(argv[4]);

    int old = dimension;
    clock_t t1;

    FILE *file;

    file = fopen(argv[3], "r");

    float d[t + 1];


    int avgTrial = 0;
    float avgSecs = 0.0;
    int numtrials = 1;
    int iterator = 1;

    for (int p = 0; p < numtrials; p++) {
        printf("hehe\n");
        for (int k = 1; k <= t; k++) {

            if (dimension > 2) {
                dimension = padding(dimension, k);
            }

            int **a = newMatrix(dimension);
            int **b = newMatrix(dimension);
            int **c = newMatrix(dimension);

            // Takes care of padding for first matrix
            for (int i = 0; i < dimension; i++){
                for(int j = 0; j < dimension; j++){
                    if(i < old && j< old){
                        fscanf(file, "%d", &a[i][j]);
                    }
                    else{
                        a[i][j]=0;
                    }
                }
            }

            // Takes care of padding for second matrix
            for (int i = 0; i < dimension; i++){
                for(int j =0; j < dimension; j++){
                    if(i < old && j< old){
                        fscanf(file, "%d", &b[i][j]);
                    }
                    else{
                        b[i][j]=0;
                    }
                }
            }

            t1 = clock();
            for (int wow = 0; wow < iterator; wow++)
                c = strassenMod(a, b, c, dimension, k);
            t1 = clock() - t1;
            double secs = ((double)t1) / CLOCKS_PER_SEC;

            double tmp2 = secs / iterator;

            d[k] = tmp2;

            printf("%d, %f, %d \n", old, d[k], k);

            //printf("strassenMod() took %f seconds \n", secs);
            
            /*printf("wow %f, \n", d[0]);
            printf("wow %f, \n", d[1]);*/
            //printDiags(old, c);

            freeMatrix(dimension, a);
            freeMatrix(dimension, b);
            freeMatrix(dimension, c);
        }
        int location = 1;


        float minimum = d[1];
     
            for (int x = 2 ; x <= t ; x++ ) {
                if (d[x] < minimum ) {
                    minimum = d[x];
                    location = x;
                }  
            }

        avgSecs += minimum;
        avgTrial += location;

        //printf("coolio %d\n", avgTrial);
    }
    printf("%d, %f, %d \n", avgTrial / numtrials, avgSecs / numtrials, 0);


    return 0;
}
