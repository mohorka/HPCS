#include <iostream>
#include <ctime>
#include <random>
#include <iomanip>
#include <chrono>
#include "/usr/include/x86_64-linux-gnu/mpich/mpi.h"

void merge(int* array, int left, int m, int right){
    int i,j,k;
    int size1 = m - left + 1;
    int size2 = right - m;
    int* left_array = new int[size1];
    int* right_array = new int[size2];
    for(i = 0; i<size1; i++){
        left_array[i] = array[left+i];

    }
    for(j = 0; j<size2; j++){
        right_array[i] = array[m+1+j];

    }
    
    i = 0;
    j = 0;
    k = left;

    while (i < size1 || j<size2) {
         if (j >= size2 || (i < size1 && left_array[i] <= right_array[j])) {
            array[k] = left_array[i];
            i++;
        }
        else {
            array[k] = right_array[j];
            j++;
        }
        k++;
    }

    delete[] left_array;
    delete[] right_array;


}

void merge_sort(int* array, int left, int right){
    if (left < right) {
        int m = left + (right - left)/2;
        merge_sort(array,left,m);
        merge_sort(array,m+1,right);
        merge(array,left,m,right);
    }

}

int* create_array(int size, int a = 1000){
    int* array = new int[size];
    for (int i = 0; i<size; i++){
        array[i] = rand() % a + 1;
    }
    return array;
}

bool sorted_array (int* array, int size){
    for (int i = 0; i< size - 1; i++){
        if (array[i]>array[i+1]){
            return false;
        }

    }
    return true;
}
void MPI_run (int argc, char** argv){
    int max_threads, thread_num;
    MPI_Status msg_status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &max_threads);
    MPI_Comm_rank(MPI_COMM_WORLD,&thread_num);
    
    if(thread_num == 0){
        return;
    }
    else {
        while(true){
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &msg_status);

            int tag = msg_status.MPI_TAG;
            int source = msg_status.MPI_SOURCE;

            if(tag == 0){
                int dummy;
                MPI_Recv(&dummy,0,MPI_INT,source,tag,MPI_COMM_WORLD,&msg_status);
                MPI_Finalize();
                exit(0);
            }
            else {
                int* array;
                int size;

                MPI_Get_count(&msg_status,MPI_INT,&size);
                array = new int[size];
                MPI_Recv(array,size,MPI_INT, source, tag,MPI_COMM_WORLD,&msg_status);

                MPI_merge_sort(array,size, MPI_INT, source, tag, MPI_COMM_WORLD);
                delete[] array;
            }
        }
    }
    
}
void MPI_merge_sort(int* array, int left, int right, int thread_num, int max_threads, int thread_offset){
    if (left >= right) {
        return;
    }

    int m = left + (right - left) / 2;
    int next_thread_num = thread_num + thread_offset;

    if (next_thread_num < max_threads){
        MPI_Request msg_req;
        MPI_Status msg_status;

        MPI_Isend(array+m+1, right-m,MPI_INT,next_thread_num,thread_offset*2,MPI_COMM_WORLD,&msg_req);
        MPI_merge_sort(array,left,m,thread_num,max_threads,thread_offset*2);
        MPI_Recv(array+m+1, right-m, MPI_INT,next_thread_num,thread_offset*2,MPI_COMM_WORLD,&msg_status);
        MPI_Wait(&msg_req, &msg_status);  
    }
    else {
        MPI_merge_sort(array,left,m,thread_num,max_threads,thread_offset*2);
        MPI_merge_sort(array,m+1,right,thread_num,max_threads,thread_offset*2);
        merge(array,left,m,right);
    }
}
void test_merge (int size, int iters, int c){
    clock_t CPU_time = 0;
    double real_time =0;
    for (int i = 0; i<iters; i++){
        int* array = create_array(size,c);
        std::clock_t cpu_start = std::clock();
        auto time_start = std::chrono::high_resolution_clock::now();
        merge_sort(array,0,size - 1);
        std::clock_t cpu_end = std::clock();
        auto time_end = std::chrono::high_resolution_clock::now();
        if (!sorted_array(array,size)){
            std::cout << "Some troubles with array's size"<<size<<std::endl;
        }
        delete[] array;

        CPU_time += (cpu_end - cpu_start);
        real_time += std::chrono::duration<double,std::milli>(time_end-time_start).count();

    }
    std::cout<< "Result for array which size is " << size << "; number of iterations: "<< iters<< std::endl;
    std::cout<<"CPU: "<< 1000.0 * CPU_time / (iters * CLOCKS_PER_SEC) << " ms"<< std::endl;
    std::cout<< "Real time is "<< real_time / iters << " ms"<< std::endl;

}
void test_MPI_merge(int size, int iters, int c){
    int max_threads, thread_num;
    MPI_Comm_size(MPI_COMM_WORLD, &max_threads);
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_num);

    clock_t CPU_time = 0;
    double real_time = 0;

    for (int i =0; i<iters; i++){
        int* array = create_array(size,c);
        std::clock_t cpu_start = std::clock();
        auto time_start = std::chrono::high_resolution_clock::now();

        MPI_merge_sort(array,0,size-1, thread_num,max_threads,1);

        std::clock_t cpu_end = std::clock();
        auto time_end = std::chrono::high_resolution_clock::now();

        if (!sorted_array(array,size)) {
            std::cout<< "Some troubles with array's size " << size << std::endl;
        }
        delete[] array;

        CPU_time += (cpu_end = cpu_start);
        real_time += std::chrono::duration<double, std::milli>(time_end - time_start).count();
    }

    std::cout<< "Result for array (MPI) with size " << size << ", number of iterations: "<< iters << std::endl;
    std::cout << "CPU: " << 1000.0 * CPU_time / iters / CLOCKS_PER_SEC <<" ms"<< std::endl;
    std::cout << "Real time: " << real_time/ iters  <<" ms"<< std::endl;
}

int main(int argc, char** argv) {
    std::cout << "Non-parallel merge sort"<<std::endl;
    test_merge(20000,100,100);
    test_merge(25000,100,200);
    test_merge(50000,100,300);
    test_merge(70000,100,400);
    test_merge(90000,100,500);

    MPI_run(argc,argv);
    int max_threads;
    MPI_Comm_size(MPI_COMM_WORLD, &max_threads);

    std::cout << "Parallel merge sort"<<std::endl;
    test_MPI_merge(20000,100,100);
    test_MPI_merge(25000,100,200);
    test_MPI_merge(50000,100,300);
    test_MPI_merge(70000,100,400);
    test_MPI_merge(90000,100,500);

    for (int i = 1; i < max_threads; i++){
        MPI_Send(0,0,MPI_INT,i,0,MPI_COMM_WORLD);
    }
    MPI_Finalize();
    
    std::cin.get(); std::cin.get();
    return 0;

}