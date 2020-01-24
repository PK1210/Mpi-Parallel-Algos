#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>
#include <limits.h>
#include <fstream>
#define ll long long int
// void swap(ll * x, ll * y);
// ll partition(ll *arr, ll l, ll r);
void quickSort(ll *arr, int l, int r);
void merge(ll *buffer_a,int buffer_a_size,ll *buffer_b, int buffer_b_size, ll *merged_array);

int main(int argc,char **argv){

    int rank, numElements=0, numProcesses;
    ll *arr = (ll *)malloc(1100000*sizeof(ll));

    /* start up MPI */
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    // if(!rank)
    // {
        std::ifstream fin;
        fin.open(argv[1]);
        // printf("Taylor\n");
        ll num;
        while(fin>>num)
        {
            // printf("%d ",num);
            arr[numElements++]=num;
        }
        fin.close();
        // numElements--;
        printf("Number of elements = %d\n",numElements);
        // ll * arr = (ll *)malloc((numElements+numProcesses)*sizeof(int));
        // for(ll i=0;i<numElements;i++)
            // arr[i] = atoi(argv[i+1]);

        // for(ll i=0;i<numElements;i++)
        //     printf("%d ",arr[i]);
        // printf("\n");
    // }

    // Divide into parallel bufferes
    int buffer_size = numElements/numProcesses;
    // printf("Masoom = %d\n",buffer_size);
    const int residual_size = numElements % numProcesses;
    if(residual_size)
    {
        buffer_size++;
        for(int i=0;i<numProcesses-residual_size;i++)
            arr[numElements+i]=LLONG_MAX;
    }
    ll * buffer = (ll *)malloc(buffer_size*sizeof(ll));
    MPI_Scatter(arr,buffer_size,MPI_LONG_LONG_INT,buffer,buffer_size,MPI_LONG_LONG_INT,0,MPI_COMM_WORLD);
    // printf("Before sorted array rank = %d\n",rank);
    // for(ll i=0;i<buffer_size;i++)
    //     printf("%d ",buffer[i]);
    // printf("\n");
    quickSort(buffer, 0, buffer_size - 1);
    free(arr);
    // printf("After final sorted array rank = %d\n", rank);
    // for(ll i=0;i<buffer_size;i++)
    //     printf("%d ",buffer[i]);
    // printf("\n");

    // Merge results form odd and even pairs recusively
    int i = 1;
    while(i<numProcesses){
        // printf("Hello %d\n",rank);
        if(rank%(i<<1)){
            // printf("Send %d %d\n",rank,rank-i);
            MPI_Send(&buffer_size,1,MPI_INT,rank-i,1,MPI_COMM_WORLD);
            MPI_Send(buffer,buffer_size,MPI_LONG_LONG_INT,rank-i,0,MPI_COMM_WORLD);
            break;
        }
        else if(rank+i>=numProcesses)
        {
            i=i<<1;
            continue;
        }
        // printf("Waiting %d %d\n",rank,rank+i);
        int recieved_buffer_size;
        MPI_Recv(&recieved_buffer_size,1,MPI_INT,rank+i,1,MPI_COMM_WORLD,NULL);
        ll * recieved_buffer = (ll *)malloc(recieved_buffer_size*sizeof(ll));
        MPI_Recv(recieved_buffer,recieved_buffer_size,MPI_LONG_LONG_INT,rank+i,0,MPI_COMM_WORLD,NULL);
        ll * merged_array = (ll *)malloc((buffer_size+recieved_buffer_size)*sizeof(ll));
        merge(recieved_buffer,recieved_buffer_size,buffer,buffer_size,merged_array);
        free(recieved_buffer);
        free(buffer);
        buffer=merged_array;
        buffer_size=recieved_buffer_size+buffer_size;
        // printf("Merged %d\n",rank);
        i=i<<1;
    }

    //
    MPI_Barrier(MPI_COMM_WORLD);
    if(!rank){
        std::ofstream fout;
        fout.open(argv[2]);
        // printf("Final sorted array\n");
        for(ll i=0;i<numElements;i++)
            fout<<buffer[i]<<" ";
        fout<<'\n';
        fout.close();
    }
    MPI_Finalize();
    return 0;
}
void swap(ll * x, ll * y){
    ll temp = *x;
    *x = *y;
    *y = temp;
}
int partition(ll *arr, ll l, ll r){
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
    swap(&arr[index],&arr[r]);
    index = l;
    for(int i=l;i<r;i++)
        if(arr[i]<arr[r])
            swap(&arr[index++],&arr[i]);
    swap(&arr[index],&arr[r]);
    return index;
}
void quickSort(ll *arr, int l, int r){
    if(l>=r)
        return;
    int pivot = partition(arr,l,r);
    quickSort(arr,l,pivot-1);
    quickSort(arr,pivot+1,r);
}
void merge(ll *buffer_a,int buffer_a_size,ll *buffer_b, int buffer_b_size, ll *merged_array){
    int a=0,b=0,c=0;
    while(a<buffer_a_size || b<buffer_b_size){
        if(a<buffer_a_size && b<buffer_b_size)
            if(buffer_a[a]<=buffer_b[b])
                merged_array[c++]=buffer_a[a++];
            else
                merged_array[c++]=buffer_b[b++];
        else if(a<buffer_a_size)
            merged_array[c++]=buffer_a[a++];
        else
            merged_array[c++]=buffer_b[b++];
    }
}
