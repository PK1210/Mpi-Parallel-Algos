#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>
#include <limits.h>
#include <fstream>
#define ll long long int

int main(){

    ll n,m;
    int rank, numElements=0, numProcesses;
    ll *arr = (ll *)malloc(1100000*sizeof(ll));

    /* start up MPI */
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    if(!rank)
    {
        std::cin>>n>>m;
    
}
