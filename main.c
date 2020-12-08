#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>



/*
 * Function definitions
 */
int* traditional_optimized_square(int d, int* a, int* b);
int* traditional_non_optimized_square(int d, int* a, int* b);
int* traditional_square(int dimension, int* a, int* b);
int* strassen(int rowA, int colA, int* a,
              int rowB, int colB, int* b);
int* strassen_square(int dimension, int* a, int* b);
int* add(int rowA, int colA, int* a,
         int rowB, int colB, int* b, bool subtract);
int* add_square(int d, int* a, int* b);
int* sub_square(int d, int* a, int* b);
void print_matrix(int rows, int cols, int* m);
void print_diagonal(int d,  int* m);
float timer(int n, int numtrials, int* (*f)(int, int*, int*));
int* gen_matrix(int n);
int crossOver;



int main(int argc, char* argv[]){
    if(argc != 4){
        printf("Usage: ./strassen cvFlag dimension inputfile\n");
        return 1;
    }
    int cvFlag = atoi(argv[1]); // Let's figure out what cvFlag does
    crossOver = (cvFlag > 0) ? cvFlag : 1;
    int matrixDim = atoi(argv[2]);   // dimension of the matrix should like to convert
    char* matrixInputFile = argv[3];  // input file containing matrix values
    int* matrixA = malloc(matrixDim * matrixDim * sizeof(int)); // pointer storing the address of allocated memory address for nxn dimensional int matrix
    int* matrixB = malloc(matrixDim * matrixDim * sizeof(int)); // Second matrix
    srand(time(NULL));

    // We need to turn this into a function
    FILE* inputFile = fopen(matrixInputFile, "r");  // Below Code is for assigning values to matrix
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    if (inputFile == NULL)
        exit(EXIT_FAILURE);

    int count = 0;
    int* matrixBuffer = matrixA;
    while ((read = getline(&line, &len, inputFile)) != -1) {
        int value = atoi(line);
        if(count == matrixDim * matrixDim){
            count = 0;
            matrixBuffer = matrixB;
        }
        *(matrixBuffer + count) = value;
        count++;
    }

    free(line);     // free memory for null
    fclose(inputFile);   //

    // Somehow convert this into a function


    int* matrixC = strassen(matrixDim, matrixDim, matrixA, matrixDim, matrixDim, matrixB);    // Now what does it too?
    if(matrixC == NULL){
        printf("Strassen failed\n");
        free(matrixA);
        free(matrixB);
        free(matrixC);
        return 1;
    }
    // print_matrix(matrixDim, matrixDim, a);
    // print_matrix(matrixDim, matrixDim, b);
    // print_matrix(matrixDim, matrixDim, c);
    print_diagonal(matrixDim, matrixC);


    /*
    int n = crossOver;
    printf("Crossover = %d\n", crossOver);
    int trials = 10;
    printf("Trials = %d\n", trials);
    printf("Size\tTraditional\tStrassen\n");
    //printf("Size\tTrad_opt\tTrad_non_opt\n");
    while(n < 500){
        if(n > 3 * crossOver)
            trials = 3;
        float trad_opt_time = timer(n, trials, &traditional_optimized_square);
        //float trad_nonopt_time = timer(n, trials, &traditional_non_optimized_square);
        float strassen_time = timer(n, trials, &strassen_square);

        printf("%d\t%f\t%f\t%d\n", n, trad_opt_time, strassen_time, strassen_time < trad_opt_time);
        //printf("%d\t%f\t%f\n", n, trad_opt_time, trad_nonopt_time);
        //n <<= 1;
        n += 1;
    }
    */

    free(matrixA);
    free(matrixB);
    free(matrixC);
    return 0;
}

int* add(int rowA, int colA, int* a,
         int rowB, int colB, int* b, bool subtract){
    /*
     * The function sums two matrices and returns the pointer variable for the resultant matrix.
     * The bool variable specifies whether to perform addition or subtraction. If set to false,
     * it will perform addition, else it performs subtraction operation.
     */
    if(rowA != rowB || colA != colB){
        printf("Invalid dimensions for the add function \n");
        return NULL;
    }

    for(int i = 0; i < rowA; i++){
        for(int j = 0; j < colA; j++){
            if (subtract)
                *(a + i * rowA + j) -= *(b + i * rowA + j);
            else
                *(a + i * rowA + j) += *(b + i * rowA + j);
        }
    }
    return a;
}

int* add_square(int d, int* a, int* b){
    return add(d, d, a, d, d, b, false);
}

int* sub_square(int d, int* a, int* b){
    return add(d, d, a, d, d, b, true);
}

int* traditional_non_optimized(int rowA, int colA, int* a,
                              int rowB, int colB, int* b){

    int* res = calloc(rowA * colB, sizeof(int));

    if(colA != rowB){
        return NULL;
    }
    for(int i = 0; i < rowA; i++){
        for(int j = 0; j < colB; j++){
            int sum = 0;
            for(int k = 0; k < colA; k++){
                sum += *(a + i * rowA + k) * *(b + k * rowB + j);
            }
            *(res + i * rowA + j) = sum;
        }
    }

    return res;
}

// Let's figure this function out
int* traditional_cache_optimized(int rowA, int colA, int* a,
                                 int rowB, int colB, int* b){
    int* res = calloc(rowA * colB, sizeof(int));   // Allocate the memory for the resultant matrix

    if(colA != rowB){  // Checking , again
        return NULL;
    }
    for(int k = 0; k < colA; k++){
        for(int i = 0; i < rowA; i++){
            int tmp = *(a + i * rowA + k);
            for(int j = 0; j < colB; j++){
                *(res + i * rowA + j) += tmp * *(b + k * rowB + j);
            }
        }
    }

    return res;     // return address fo the result.
}

// Optimized version of the traditional matrix multiplication
int* traditional_optimized_square(int d, int* a, int* b){
    return traditional_cache_optimized(d, d, a, d, d, b);   // Call to the optimized version of
}

int* traditional_non_optimized_square(int d, int* a, int* b){
    return traditional_non_optimized(d, d, a, d, d, b);
}

int* strassen_square(int d, int* a, int* b){
    return strassen(d, d, a, d, d, b);
}

int* strassen(int rowA, int colA, int* matrixA, int rowB, int colB, int* matrixB){
    if(rowA != colA || rowB != colB || colA != rowB){     // Checking if matrix multiplication is even possible
        printf("Matrix dimensions not valid.\n");
        return NULL;
    }

    // Got it, if the value of our no of rows is less than cross over, then we fall back on our traditional matrix multiplication method.
    if(rowA <= crossOver){
        int* matrixC = traditional_optimized_square(rowA, matrixA, matrixB);  // We will figure out what this does. Probably optimized version of multiplication via some methods?
        return matrixC;
    }

    bool oddFlag = false;   // to check if row is odd

    int d;        // set the proper value of d
    if(rowA % 2 == 0){
         d = rowA / 2;
    }
    else{
        d = rowA / 2 + 1;
        oddFlag = true;
    }

    int memoryOffsets[4] = {0, d, rowA * d, rowA * d + d};   // Got it, these are the offsets for the memory addresses of the partitioned matrix

    int* block[8];     //  I'm not sure what the block is for ??
    for(int i = 0; i < 8; i ++){
        block[i] = malloc(sizeof(int) * d * d);
    }



    // I'm not sure how we are using block to get the value of the various components
    int* A = block[0];
    int* B = block[1]; //a + (colA / 2);
    int* C = block[2]; //a + (rowA * colA / 2);
    int* D = block[3]; //a + (rowA * colA / 2 + colA / 2);
    int* E = block[4];
    int* F = block[5]; // b + (colB / 2);
    int* G = block[6]; // b + (rowB * colB / 2);
    int* H = block[7]; // b + (rowB * colB / 2 + colB / 2);


    /* A matrix is parititioned blocks this way:
     * | 0  1 |
     * | 2  3 |
     */

    for(int bl = 0; bl < 4; bl++){
        for(int i = 0; i < d; i++){
            for(int j = 0; j < d; j++){
                // Pad bottom row with zeros
                if(oddFlag && i == d - 1 && bl > 1){
                    *(block[bl] + i * d + j) = 0;
                    *(block[bl + 4] + i * d + j) = 0;
                }
                // Pad right column with zeros
                else if(oddFlag && j == d - 1 && (bl == 1 || bl == 3)){
                    *(block[bl] + i * d + j) = 0;
                    *(block[bl + 4] + i * d + j) = 0;
                }
                else{
                    *(block[bl] + i * d + j) = *(matrixA + i * rowA + j + memoryOffsets[bl]);
                    *(block[bl + 4] + i * d + j) = *(matrixB + i * rowA + j + memoryOffsets[bl]);
                }
            }
        }
    }


    // Time to understand this part of the code.
    sub_square(d, F, H);
    int* P1 = strassen(d, d, A, d, d, F);

    add_square(d, A, B);
    int* P2 = strassen(d, d, A, d, d, H);

    add_square(d, C, D);
    int* P3 = strassen(d, d, C, d, d, E);

    sub_square(d, G, E);
    int* P4 = strassen(d, d, D, d, d, G);

    sub_square(d, A, B);
    add_square(d, A, D);
    add_square(d, E, H);
    int* P5 = strassen(d, d, A, d, d, E);

    sub_square(d, B, D);
    add_square(d, G, E);
    int* P6 = strassen(d, d, B, d, d, G);

    sub_square(d, A, C);
    add_square(d, E, F);
    int* P7 = strassen(d, d, A, d, d, E);

    int* c = malloc(sizeof(int) * rowA * rowA);

    int* top_right = add_square(d, P1, P2);
    int* bottom_left = add_square(d, P3, P4);
    int* P5_ = add_square(d, P5, sub_square(d, P4, P2));
    int* top_left = add_square(d, P6, P5_);
    int* bottom_right = sub_square(d, add_square(d, P5_, top_right), add_square(d, P7, bottom_left));

    for(int i = 0; i < d; i++){
        for(int j = 0; j < d; j++){
            *(c + i * rowA + j) = *(top_left + i * d + j);
            if(!oddFlag || (oddFlag && j != d - 1))
                *(c + i * rowA + j + memoryOffsets[1]) = *(top_right + i * d + j);
            if(!oddFlag || (oddFlag && i != d - 1))
                *(c + i * rowA + j + memoryOffsets[2]) = *(bottom_left + i * d + j);
            if(!oddFlag || (oddFlag && i != d - 1 && j != d - 1))
                *(c + i * rowA + j + memoryOffsets[3]) = *(bottom_right + i * d + j);
        }
    }

    free(P1);
    free(P2);
    free(P3);
    free(P4);
    free(P5);
    free(P6);
    free(P7);
    for(int i = 0; i < 8; i++){
        free(block[i]);
    }

    return c;
}


float timer(int n, int numtrials, int* (*f)(int, int*, int*)){

    double time_elapsed = 0.0;
    int count = 0;
    while (count < numtrials){
        int* m1 = gen_matrix(n);
        int* m2 = gen_matrix(n);

        clock_t begin = clock();
        int* result = f(n, m1, m2);
        clock_t end = clock();

        free(result);
        free(m1);
        free(m2);
        time_elapsed += (double)(end - begin) / CLOCKS_PER_SEC;

        count++;
    }
    float avg_time = time_elapsed / (float) numtrials;
    return avg_time;
}


int* gen_matrix(n){
    int* m = malloc(sizeof(int) * n * n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            *(m + i * n + j) = rand() % 10;
            if(rand() % 10 > 5){
                *(m + i * n + j) *= -1;
            }
        }
    }
    return m;
}

void print_matrix(int rows, int cols, int* m){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            printf("%d ", *(m + i * rows + j));
        }
        printf("\n");
    }
    printf("\n");

}

void print_diagonal(int d, int* m){
    for(int i = 0; i < d; i++){
        printf("%d\n", *(m + i * d + i));
    }
}
