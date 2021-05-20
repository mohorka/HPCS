#include "/usr/include/x86_64-linux-gnu/mpich/mpi.h"
#include <iostream>
#include <cstring>

#define SIZE 32
#define TASK_LIMIT 8
#define MSG "Hello from process "

int main (int argc, char** argv) {
    int rank, tasks_number;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&tasks_number);
    MPI_Status status;

    int p = 0;
    char send_buff[SIZE];
    strcpy(send_buff,MSG);
    int len = strlen(MSG);
    send_buff[len] = '0' + rank;
    send_buff[len+1] = '\0';

    if (rank == 0){
        char recv_buff[TASK_LIMIT - 1][SIZE];
        for (int i = 0; i<tasks_number -1; i++){
            MPI_Recv(recv_buff[i],SIZE, MPI_CHAR,MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,&status);
            MPI_Send(send_buff,strlen(send_buff) + 1, MPI_CHAR,status.MPI_SOURCE,0,MPI_COMM_WORLD);
            std::cout<<"Process received: "<<recv_buff[i]<<std::endl;

        }
    }
    else {
        char recv_from_master[SIZE];
        MPI_Send(send_buff,strlen(send_buff)+1,MPI_CHAR,p,0,MPI_COMM_WORLD);
        MPI_Recv(recv_from_master,SIZE,MPI_CHAR,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        std::cout<<"Received from master process: "<<recv_from_master<<std::endl;
    }
    MPI_Finalize();
    
    std::cin.get();std::cin.get();
    return 0;



}