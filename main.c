/*
    Authors: 20907541, 30939506
    Due Date: 3/24/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include "main.h"

#define threshold 15


int **newMatrix(int d) {
    int **m = (int**) malloc(d * sizeof(int*));

    // allocating memory for each row in the matrix
    for (int i = 0; i < d; i++){
        m[i] = (int *) malloc(d * sizeof(int));
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


int **standardmult(int **a, int **b, int d) {
    int **c = newMatrix(d);
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
int **strassen(int **x, int **y, int dim) {

    int **z = newMatrix(dim);
    
    if (dim <= threshold)
        return standardmult(x, y, dim);

    else {
        int half = dim / 2;

        int **a = newMatrix(half);
        int **b = newMatrix(half);
        int **c = newMatrix(half);
        int **d = newMatrix(half);
        int **e = newMatrix(half);
        int **f = newMatrix(half);
        int **g = newMatrix(half);
        int **h = newMatrix(half);

        for (int i = 0; i < half; i++) {
            for (int j = 0; j < half; j++) {
                a[i][j] = x[i][j];
                b[i][j] = x[i][j + half];
                c[i][j] = x[i + half][j];
                d[i][j] = x[i + half][j + half];
                e[i][j] = y[i][j];
                f[i][j] = y[i][j + half];
                g[i][j] = y[i + half][j];
                h[i][j] = y[i + half][j + half];
            }
        }

        int **p1 = strassen(a, add_matrix(f, h, half, -1), half); // A(F - H)
        int **p2 = strassen(add_matrix(a, b, half, 1), h, half); // (A + B)H
        int **p3 = strassen(add_matrix(c, d, half, 1), e, half); // (C + D)H
        int **p4 = strassen(d, add_matrix(g, e, half, -1), half); // D(G - E)
        int **p5 = strassen(add_matrix(a, d, half, 1), add_matrix(e, h, half, 1), half); // (A + D)(E + H)
        int **p6 = strassen(add_matrix(b, d, half, -1), add_matrix(g, h, half, 1), half); // (B - D)(G + H)
        int **p7 = strassen(add_matrix(a, c, half, -1), add_matrix(e, f, half, 1), half); // (A - C)(E + F)

        // AE + BG = P5 + P4 - P2 + P6
        int **z11 = add_matrix(p5, add_matrix(p4, add_matrix(p6, p2, half, -1), half, 1), half, 1);
         
        // AF + BH = P1 + P2
        int **z12 = add_matrix(p1, p2, half, 1);
        
        // CE + DG = P3 + P4
        int **z21 = add_matrix(p3, p4, half, 1);

        // CF + DH = P5 + P1 - P3 - P7
        int **z22 = add_matrix(add_matrix(p5, p1, half, 1), add_matrix(p3, p7, half, 1), half, -1);

        for (int i = 0; i < half; i++) {
            for (int j = 0; j < half; j++) {
                z[i][j] = z11[i][j];
                z[i][j + half] = z12[i][j];
                z[i + half][j] = z21[i][j];
                z[i + half][j + half] = z22[i][j];
            }
        }

    return z;
    }
}

// Pad to the nearest power of 2
int padding(int d) {

    int p = 1;

    while(p < d)
        p *= 2;

    return p;
}

int main(int argc, char *argv[]) {

    // Error Handling
    if (argc != 4) {
        printf("Format is: ./strassen flag dimension inputfile");
        return 1;
    }

    int flag = atoi(argv[1]);
    int dimension = atoi(argv[2]);

    int old = dimension;

    FILE *file;

    file = fopen(argv[3], "r");

    if (dimension > 2) {
        dimension = padding(dimension);
    }

    int **a = newMatrix(dimension);
    int **b = newMatrix(dimension);

    for (int i = 0; i < dimension; i++){
        for(int j =0; j < dimension; j++){
            if(i < old && j< old){
                fscanf(file, "%d", &a[i][j]);
            }
            else{
                a[i][j]=0;
            }
        }
    }

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


    int **c = strassen(a, b, dimension);

    // only print the diagonals, ignore the paddings
    for (int i = 0; i < dimension; i++){
        if (c[i][i] == 0)
            continue;
        printf("%d\n", c[i][i]);
    }

    freeMatrix(dimension, a);
    freeMatrix(dimension, b);
    freeMatrix(dimension, c);

}