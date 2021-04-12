#include <iostream>
#include <omp.h>
#include <ctime>

using namespace std;

const int SIZE = 1000;
const int MAX_ELEM = 500;
const int THREADS = 2;

void initialize(int* array){
    for(int i = 0; i< SIZE; ++i){
        array[i] = rand() % MAX_ELEM + 1;
    }
}

void odd_even_sort(int* array){
    int i = 0;
    bool sorted = false;
    while(!sorted or i != 0){
        sorted = true;
        for(int j = i; j<SIZE-1; j+=2){
            if(array[j]>array[j+1]){
                swap(array[j],array[j+1]);
                sorted = false;
            }
            
            i = (i == 0)?1:0;

        }
    }
}
void odd_even_sort_parallel(int* array){
    int i = 0;
    bool sorted = false;
    while(!sorted or i != 0){
        sorted = true;
        #pragma omp parallel for
        for(int j = i;j < SIZE -1 ;j+=2){
            if(array[j] > array[j+1]){
                swap(array[j],array[j+1]);
                sorted = false;
            }
        }
        i = (i == 0)?1:0;

    }
}
int main(){
    omp_set_num_threads(THREADS);
    srand(time(0));

    double time = 0;
    double start;
    double end;

    int* array = new int[SIZE];
    for(int i=0;i<20; ++i){
        initialize(array);
        start = omp_get_wtime();
        odd_even_sort(array);
        end = omp_get_wtime();
        time += end - start;
    }
    cout<<"Average time on 20s takes of basic odd-even sort" << time / 20 << endl;
    time = 0;

    int* array = new int[SIZE];
    for(int i=0;i<20; ++i){
        initialize(array);
        start = omp_get_wtime();
        odd_even_sort_parallel(array);
        end = omp_get_wtime();
        time += end - start;
    }
    cout<<"Average time on 20s takes of parallel odd-even sort" << time / 20 << endl;
    return 0;
}