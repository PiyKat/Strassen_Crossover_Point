#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define _GNU_SOURCE




/*
 * Function definitions
 */
void readFile(FILE * file, int matrixDim, int* matrixA, int*matrixB);
void strassenTest();
int* optClassicMultiply(int rowA, int colA, int* a,int rowB, int colB, int* b);
//int* nonOptClassicMultiply_square(int d, int* a, int* b);
int* nonOptClassicMultiply(int rowA, int colA, int* a,int rowB, int colB, int* b);
int* traditional_square(int dimension, int* a, int* b);
int* strassen(int rowA, int colA, int* a,
              int rowB, int colB, int* b);
int* addSquareMatrix(int rowA, int colA, int* a,int rowB, int colB, int* b);
int* subSquareMatrix(int rowA, int colA, int* a,int rowB, int colB, int* b);
void printMatrix(int rows, int cols, int* m);
void printDiagonal(int d,  int* m);
float timer(int n, int numtrials, int* (*f)(int, int , int*, int , int , int*));
int* generateMatrix(int n);
int* generateRandomGraph(int vertices , double prob);
int diagonalSum(int d, int* m);
int expectedTriangles(int p);
int crossOver;


void readFile(FILE* inputFile , int matrixDim, int* matrixA , int* matrixB){

    char line[512];
    int count = 0;
    int* matrixBuffer = matrixA;
    while ( fgets ( line, sizeof line, inputFile ) != NULL ) {

            //printf("%s", line);
            int value = atoi(line);
            if(count == matrixDim * matrixDim){
                count = 0;
                matrixBuffer = matrixB;
            }
            *(matrixBuffer + count) = value;
            count++;
        }


    if (inputFile == NULL)
        exit(EXIT_FAILURE);
}


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

    readFile(inputFile, matrixDim, matrixA, matrixB);
    //printMatrix(matrixDim, matrixDim, matrixA);
    int* matrixC = strassen(matrixDim, matrixDim, matrixA, matrixDim, matrixDim, matrixB);
    if(matrixC == NULL){
        printf("Strassen failed\n");
        free(matrixA);
        free(matrixB);
        free(matrixC);
        return 1;
    }
    //printMatrix(matrixDim, matrixDim, matrixA);
    //printMatrix(matrixDim, matrixDim, matrixB);
    //printMatrix(matrixDim, matrixDim, matrixC);
    printDiagonal(matrixDim, matrixC);

    free(matrixA);
    free(matrixB);
    free(matrixC);


    /*
    int n = crossOver;
    FILE* csv;
    csv = fopen("result.csv", "w");
    printf("Crossover = %d\n", crossOver);
    int trials = 10;
    printf("Trials = %d\n", trials);


    //printf("Size\tTraditional\tStrassen\n");
    //printf("Size\tTrad_opt\tTrad_non_opt\n");
    fprintf(csv,"%s,%s,%s,%s\n", "Dimension", "Optimal ", "Strasse","Crossover");
    while(n <= 512){
        if(n > 3 * crossOver)
            trials = 3;
        float trad_opt_time = timer(n, trials, &optClassicMultiply);
        //float trad_nonopt_time = timer(n, trials, &nonOptClassicMultiply);
        float strassen_time = timer(n, trials, &strassen);

        printf("%d\t%f\t%f\t%d\n", n, trad_opt_time, strassen_time, strassen_time < trad_opt_time);
        //printf("%d\t%f\t%f\n", n, trad_opt_time, trad_nonopt_time);
        fprintf(csv,"%d,%f,%f,%d\n", n, trad_opt_time, strassen_time, strassen_time < trad_opt_time);
        //n <<= 1;
        n += 1;
    }
    fclose(csv);


    return 0;
    */

    // Turn this into a function

    // Testing our random graph

    /*
    double prob = 0.05;
    int vertices = 1024;
    int numTrials = 10;
    int totalTriangles = 0;
    int count = 0;
    while (count < numTrials){
        int* randMatrix = generateRandomGraph(vertices,prob);
        //printMatrix(vertices,vertices,randMatrix);

        // Now, to calculate the A^2 matrix via Strassen method !!!
        int* a2 = strassen(vertices,vertices,randMatrix,vertices,vertices,randMatrix);
        // Calculate A^3 matrix via Strassen !!!
        int* a3 = strassen(vertices,vertices, randMatrix, vertices, vertices, a2);
        int sumDiagonal = diagonalSum(vertices,a3);
        int triangles = sumDiagonal/6;
        printf("No of triangles calculated : %d \n",triangles);
        totalTriangles += triangles;
        count++;
    }

    //printf("Calculated A^3 : ");
    //printMatrix(vertices,vertices,a3);
    printf("\n\n");
    printf("Avg no of triangles : %d\n\n",totalTriangles/10);
    */
}


int expectedTriangles(int p){

    return 178433024*p*p*p;
}

int* addSquareMatrix(int rowA, int colA, int* a,
                     int rowB, int colB, int* b){

    /*
     * The function sums two matrices and returns the pointer variable for the resultant matrix.
     */

    if(rowA != rowB || colA != colB){
        printf("Invalid dimensions for the add function \n");
        return NULL;
    }

    for(int i = 0; i < rowA; i++){
        for(int j = 0; j < colA; j++){
                *(a + i * rowA + j) += *(b + i * rowA + j);
        }
    }
    return a;
}


int* subSquareMatrix(int rowA, int colA, int* a,
         int rowB, int colB, int* b){
    /*
     * The function subtracts two matrices and returns the pointer variable for the resultant matrix.
     */
    if(rowA != rowB || colA != colB){
        printf("Invalid dimensions for the substract function \n");
        return NULL;
    }

    for(int i = 0; i < rowA; i++){
        for(int j = 0; j < colA; j++){

                *(a + i * rowA + j) -= *(b + i * rowA + j);
        }
    }
    return a;
}


int* nonOptClassicMultiply(int rowA, int colA, int* a,
                              int rowB, int colB, int* b){
    /*
     * This function performs matrix multiplication using
     */
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
int* optClassicMultiply(int rowA, int colA, int* a,
                                 int rowB, int colB, int* b){
    int* res = calloc(rowA * colB, sizeof(int));

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


int* strassen(int rowA, int colA, int* matrixA, int rowB, int colB, int* matrixB){
    if(rowA != colA || rowB != colB || colA != rowB){
        printf("Matrix dimensions not valid.\n");
        return NULL;
    }


    if(rowA <= crossOver){
        int* matrixC = optClassicMultiply(rowA, colA, matrixA, rowB, colB, matrixB);
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

    int memoryOffsets[4] = {0, d, rowA * d, rowA * d + d};

    int* block[8];
    for(int i = 0; i < 8; i ++){
        block[i] = malloc(sizeof(int) * d * d);
    }




    int* A = block[0];
    int* B = block[1]; //a + (colA / 2);
    int* C = block[2]; //a + (rowA * colA / 2);
    int* D = block[3]; //a + (rowA * colA / 2 + colA / 2);
    int* E = block[4];
    int* F = block[5]; // b + (colB / 2);
    int* G = block[6]; // b + (rowB * colB / 2);
    int* H = block[7]; // b + (rowB * colB / 2 + colB / 2);




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



    subSquareMatrix(d,d,F,d,d,H);
    int* P1 = strassen(d, d, A, d, d, F);

    addSquareMatrix(d,d,A,d,d,B);
    int* P2 = strassen(d, d, A, d, d, H);

    addSquareMatrix(d, d, C, d, d,D);
    int* P3 = strassen(d, d, C, d, d, E);

    subSquareMatrix(d, d, G,d, d, E);
    int* P4 = strassen(d, d, D, d, d, G);

    subSquareMatrix(d, d, A, d, d, B);
    addSquareMatrix(d, d,A, d, d, D);
    addSquareMatrix(d, d,E, d, d, H);
    int* P5 = strassen(d, d, A, d, d, E);

    subSquareMatrix(d, d, B, d, d,D);
    addSquareMatrix(d, d, G, d, d,E);
    int* P6 = strassen(d, d, B, d, d, G);

    subSquareMatrix(d, d, A, d, d,C);
    addSquareMatrix(d, d,E,  d, d,F);
    int* P7 = strassen(d, d, A, d, d, E);

    int* c = malloc(sizeof(int) * rowA * rowA);

    // I would REALLY like to see differentiate this properly
    int* top_right = addSquareMatrix(d, d,P1, d, d,P2);
    int* bottom_left = addSquareMatrix(d, d, P3, d, d, P4);
    int* P5_ = addSquareMatrix(d, d,P5, d, d,subSquareMatrix(d, d,P4,d, d,P2));
    int* top_left = addSquareMatrix(d, d,P6,d, d, P5_);
    int* bottom_right = subSquareMatrix(d,d, addSquareMatrix(d, d, P5_, d, d,top_right), d, d,addSquareMatrix(d, d, P7, d, d, bottom_left));

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


float timer(int n, int numtrials, int* (*f)(int, int, int*, int, int ,int*)){

    double time_elapsed = 0.0;
    int count = 0;
    while (count < numtrials){
        int* m1 = generateMatrix(n);
        int* m2 = generateMatrix(n);

        clock_t begin = clock();
        int* result = f(n, n , m1, n , n , m2);
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



int* generateMatrix(int n){
    /*
     * Generate square matrices with random weights
     */
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


int bernoulliDistributed(double p){

    return (p > ((double)rand())/ RAND_MAX);
}

int* generateRandomGraph(int vertices, double prob){

    //int vertices = 32;
    int* m = malloc(sizeof(int) *vertices *vertices);

    for(int i = 0 ; i < vertices ; i++){
        for(int j = 0; j <vertices ; j++){
            if (bernoulliDistributed(prob)){
                *(m + i*vertices +j) = 1;
            } else {
                *(m + i*vertices +j) = 0;
            }
        }

    }
    return m;
}


void printMatrix(int rows, int cols, int* m){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            printf("%d ", *(m + i * rows + j));
        }
        printf("\n");
    }
    printf("\n");

}


int diagonalSum(int d, int *m){

    int sum = 0;
    for(int i = 0; i < d; i++){
        sum += *(m + i * d + i);
    }
    return sum;
}


void printDiagonal(int d, int* m){
    for(int i = 0; i < d; i++){
        printf("%d\n", *(m + i * d + i));
    }
}
