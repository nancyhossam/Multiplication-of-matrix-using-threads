#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
struct args{
int rowno;
int colno;};
sem_t mutex; //binary semaphore by can be unlocked only the thread that locked it
double time1,time2;// time taken by procedure
int mat1row,mat1col,mat2row,mat2col;//rowa and columns
int **mat1, **mat2, **outmat1, **outmat2;//input and output matrices
void FileInput()
{FILE * file=fopen("input.txt","r");//open file input.txt to read from
fscanf(file,"%d",&mat1row);//take row 1
fscanf(file,"%d",&mat1col);//take col one
mat1=malloc(mat1row*sizeof(int*));//allocte place for matrix
for(int i=0;i<mat1row;i++)
{mat1[i]=malloc(mat1col*sizeof(int));//get the matrix elemets
for(int j=0;j<mat1col;j++)
fscanf(file,"%d",&mat1[i][j]);
}
fscanf(file,"%d",&mat2row);//take row 2
fscanf(file,"%d",&mat2col);//take col 2
mat2=malloc(mat2row*sizeof(int*));//allocate memory foe matrix
for(int i=0;i<mat2row;i++)
{mat2[i]=malloc(mat2col*sizeof(int));
for(int j=0;j<mat2col;j++)
fscanf(file,"%d",&mat2[i][j]);
}
fclose(file);//clos file
outmat1=malloc(mat1row*sizeof(int *));//alocate memory for output matrix with size row1*col2
outmat2=malloc(mat1row*sizeof(int *));//alocate memory for output matrix with size row1*col2
for(int i=0;i<mat1row;i++)
outmat1[i]=(int*)calloc(mat2col,sizeof(int));
for(int i=0;i<mat1row;i++)
outmat2[i]=(int*)calloc(mat2col,sizeof(int));
}
void * evaluateMul(void *element)
{ struct args *arg=(struct args*)element;
sem_wait(&mutex);//decrement the mutex so no one runs except it
int sum=0;
for(int i=0;i<mat1col;i++)
sum+=(mat1[arg->rowno][i]*mat2[i][arg->colno]); //multiply element in matrix byelment in another matri
outmat1[arg->rowno][arg->colno]=sum;//put it in theoutput matix
sem_post(&mutex);//increment mutex so another thread can start
}
void procedure1()
{
pthread_t Arrofthread[mat1row*mat2col];//make number of threads equals row1 *col2 so each elemtment has thread
int counter=0;
for(int i=0;i<mat1row;i++)
{
for(int j=0;j<mat2col;j++)
{ struct args* element=malloc(sizeof(struct args));//get the index of the element
element->rowno=i;
element->colno=j;
pthread_create(&Arrofthread[counter++],NULL,evaluateMul,(void*) element);//thread starts in function evaluateMul takent element as an input
}

}
for(int i=0;i<mat1row*mat2col;i++)
{
pthread_join(Arrofthread[i],NULL);//wait fot the threads to finish
}
}
void *evaluatematrow(void *x)
{int* temp=(int*)x;//taking the parametr
int x1=temp;
sem_wait(&mutex);//decrement th mutex
int sum=0;
for(int i=0;i<mat1col;i++)
{for(int j=0;j<mat2col;j++)
{outmat2[x1][j]+=(mat1[x1][i]*mat2[i][j]); //taking the parameter
}
}
sem_post(&mutex);//increment the mutes
}
void procedure2()
{
pthread_t arrofthread[mat1row];//make array of threads of size row1
int *counter=0;
for(int i=0;i<mat1row;i++){
pthread_create(&arrofthread[i],NULL,evaluatematrow,(void *)counter);//go to evalutematroe taking the number of row to evaluate
int temp=counter;
temp++;
counter=temp;}
for(int i=0;i<mat1row;i++)
pthread_join(arrofthread[i],NULL);//wait for the threads to finish

}
void writeOutputFile()//print the output matrices in output.txt
{
    FILE* outputFile = fopen("output.txt","w");


    for(int i = 0; i < mat1row; i++)
    {
        for(int j = 0; j < mat2col; j++)
        {
            fprintf(outputFile,"%d ",outmat1[i][j]);
        }
        fprintf(outputFile,"\n");
    }
    fprintf(outputFile,"\Time1    %f\n\n",time1); //write procedure one spent time

    //write procedure two output matrix
    for(int i = 0; i < mat1row; i++)
    {
        for(int j = 0; j < mat2col; j++)
        {
            fprintf(outputFile,"%d ",outmat2[i][j]);
        }
        fprintf(outputFile,"\n");
    }
    fprintf(outputFile,"\Time2    %f",time2); //write procedure two spent time

    fclose(outputFile);
}


int main()
{ sem_init(&mutex,0,1);
FileInput();
 clock_t start,end;
    start = clock();
    procedure1(); //strat procedure ine
    end = clock();
    time1 = (double)(end - start) / CLOCKS_PER_SEC;

    //evaluate procedure two spent time
    start = clock();
    procedure2(); //start procedure two
    end = clock();
    time2 =(double)(end - start) / CLOCKS_PER_SEC;

    free(mat1);
    free(mat2);

    writeOutputFile(); //write output matrices

    free(outmat1);
    free(outmat2);


    return 0;
}
