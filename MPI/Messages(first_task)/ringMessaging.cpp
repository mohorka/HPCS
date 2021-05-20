#include "/usr/include/x86_64-linux-gnu/mpich/mpi.h"
#include <iostream>
#include <cstring>

#define SIZE 32
#define MSG "Hello from process "

int main(int argc, char** argv){
    int rank, task_num;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &task_num);
	MPI_Status status;

    int process_rec = (rank + 1) % task_num;
    int process_send = rank == 0? task_num - 1 : rank - 1;
    char send_buff[SIZE];
    strcpy(send_buff,MSG);
    int len = strlen(MSG);
    send_buff[len] = '0' + rank;
    send_buff[len + 1] = '\0';
    char recv_buff[SIZE];

    MPI_Send(send_buff, strlen(send_buff) + 1, MPI_CHAR, process_rec, 0, MPI_COMM_WORLD);
    MPI_Recv(recv_buff, SIZE, MPI_CHAR, process_send, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    std::cout<<"Process "<< rank << "received: "<< recv_buff<<std::endl;

    MPI_Finalize();
    std::cin.get();std::cin.get();
    return 0;

}