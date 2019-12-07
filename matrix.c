#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 4

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 
int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX];

typedef void* (*Operators)(void*);

void fillMatrix(int matrix[MAX][MAX]) 
{
    for(int i = 0; i<MAX; i++) 
    {
        for(int j = 0; j<MAX; j++) 
        {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) 
{
    for(int i = 0; i<MAX; i++) 
    {
        for(int j = 0; j<MAX; j++) 
        {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void* computeSum(void* args) 
{
    int *index = (int*) args;
    int row = (*index)/MAX;
    int col = (*index)%MAX;
	
    matSumResult[row][col] = matA[row][col] + matB[row][col];
}

void* computeDiff(void* args) 
{
    int *index = (int*) args;
    int row = (*index)/MAX;
    int col = (*index)%MAX;
    matDiffResult[row][col] = matA[row][col] - matB[row][col];
}

void* computeProduct(void* args) 
{
    int *index = (int*) args;
    int row = (*index)/MAX;
    int col = (*index)%MAX;
    matProductResult[row][col] = matA[row][col] * matB[row][col];
}
int main() 
{
    srand(time(0));  
    fillMatrix(matA);
    fillMatrix(matB);
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    pthread_t objects[3][MAX*MAX];
    Operators operations[3];
		operations[0] = &computeProduct;
    operations[1] = &computeSum;
    operations[2] = &computeDiff;
    for(int i = 0; i < 3; i++)
    {
			for(int j = 0; j < MAX*MAX; j++)
      {
        int *operation_id = (int*)malloc(sizeof(int));
        int *index = (int*)malloc(sizeof(int));
        *operation_id = i;
				*index = j; 
        pthread_create(&objects[*operation_id][*index], NULL, operations[*operation_id], (void*)index);
      }
    }
    for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < MAX*MAX; j++)
      {
        pthread_join(objects[i][j], NULL);
      }
    }
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
}