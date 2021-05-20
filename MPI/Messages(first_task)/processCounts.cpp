#include "/usr/include/x86_64-linux-gnu/mpich/mpi.h"
#include <iostream>


int main(int argc, char* argv[]){
    int id, threads_number;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &threads_number);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);

    std::cout<<"Process "<<id<<"of "<<threads_number<<std::endl;

    MPI_Finalize();
    std::cin.get();std::cin.get();

    return 0;



}