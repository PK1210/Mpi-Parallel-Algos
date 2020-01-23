#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
void swap(int * x, int * y){
    int temp = *x;
    *x = *y;
    *y = temp;
}
int partition(int *arr, int l, int r){
    int a = rand()%(r-l+1)+l;
    int b = rand()%(r-l+1)+l;
    int c = rand()%(r-l+1)+l;
    int index,pivot;
    if(arr[a] <= arr[b])
        if(arr[b] <= arr[c])
            index = b;
        else if(arr[a] <= arr[c])
            index = c;
        else
            index = a;
    else if(arr[a] <= arr[c])
        index = a;
    else if(arr[b] <= arr[c])
        index = c;
    else
        index = b;
    pivot = arr[index];
    swap(&arr[index],&arr[r]);
    index = l;
    for(int i=l;i<r;i++)
        if(arr[i]<pivot)
            swap(&arr[index++],&arr[i]);
    swap(&arr[index],&arr[r]);
    return index;
}
void quickSort(int *arr, int l, int r){
    if(l>=r)
        return;
    int pivot = partition(arr,l,r);
    quickSort(arr,l,pivot-1);
    quickSort(arr,pivot+1,r);
}
int main(int argc,char **argv){

    int pid, numElements, numProcesses = 4;

    /* start up MPI */
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    int arr[10002];
    scanf("%d",&numElements);
    for(int i=0;i<numElements;i++)
        scanf("%d",&arr[i]);

    // Divide into parallel batches
    const int batch_size = numElements/numProcesses;
    const int residual_size = numElements % numProcesses;
    int * batches = (int *)malloc(batch_size*sizeof(int));
    MPI_Scatter(arr, batch_size, MPI_INT, batches, batch_size, MPI_INT, 0, MPI_COMM_WORLD);

    quickSort(batches, 0, numElements - 1);
    printf("Sorted array of rank:%d \n",pid);
    for(int i=0;i<numElements;i++)
        printf("%d ",arr[i]);
    printf("\n");

    MPI_Finalize();
    return 0;
}
