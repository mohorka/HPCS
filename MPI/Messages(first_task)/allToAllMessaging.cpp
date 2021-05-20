#include "/usr/include/x86_64-linux-gnu/mpich/mpi.h"
#include <iostream>
#include <cstring>

#define MSG "Hello from process "
#define SIZE 32
#define TASKS_LIMIT 8

int main(int argc, char** argv){
    int rank, task_num;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &task_num);
	MPI_Status status;
	MPI_Request request;

    char send_buff[SIZE];
    char recv_buff[TASKS_LIMIT][SIZE];
    strcpy(send_buff,MSG);
    int len = strlen(MSG);

    for (int i =0; i< task_num;++i){
        MPI_Send(send_buff, strlen(send_buff) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
    for (int i =0; i< task_num;++i) {
        MPI_Recv(recv_buff[i], SIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        std::cout<<"Process "<<rank<<"received: "<<recv_buff[i]<<std::endl;
    }

    MPI_Finalize();
    std::cin.get();std::cin.get();
    return 0;
}