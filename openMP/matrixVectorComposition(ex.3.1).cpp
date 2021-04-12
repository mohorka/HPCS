#include <omp.h>
#include <iostream>
using namespace std;

int main(){
    int rows,cols,vec_size;
    double **A, *vec, *vec_str, *vec_cols, *vec_block;

    cout<< "Please enter amount of rows and columns for matrix separated by space:";
    cin >> rows;
    cin >> cols;
    cout << "Now enter size of vector:";
    cin >> vec_size;

    if(cols != vec_size){
        cout<< "Composition is impossible";
        cin.get(); cin.get();
        return 0;
    }

    A = new double* [rows];
    cout<<"Enter elements for matrix:"<<endl;
    for(int i=0; i<rows;++i){
        A[i] = new double[cols];
        for(int j=0;j<cols;++j){
            cout<<"A["<<i<<"]["<<j<<"]= ";
            cin >>A[i][j];
        }
    }

    vec = new double [vec_size];
    cout<< "Enter elements for vector:"<<endl;
    for (int i=0;i<vec_size;++i){
        cout<<"vec["<<i<<"]";
        cin>>vec[i];
    }

    vec_str = new double[vec_size];
    vec_cols= new double[vec_size];
    vec_block = new double[vec_size];

    double runtime;

    #pragma omp parallel for
        for(int i=0;i<vec_size;++i){
            vec_str[i]=0;
            vec_block[i]=0;
            vec_cols[i]=0;
        }
    int counter = 1;

    do{
        runtime = omp_get_wtime()*1000;
        #pragma omp parallel for shared (A,vec,vec_str) num_threads(counter)
        for(int i=0;i<rows;++i){
            for(int j=0;j<cols;++j){
                #pragma omp atomic
                vec_str[i] +=  A[i][j] * vec[j];
            }
        }
        runtime = omp_get_wtime()*1000 - runtime;
        cout<<counter<<" threads, lines time is "<<runtime<<endl;

        runtime= omp_get_wtime()*1000;
        #pragma omp parallel for shared(A,vec,vec_cols) num_threads(counter)
        for(int j=0;j<cols;++j){
            for(int i=0;i<cols;++i){
                #pragma omp atomic
                vec_cols[i] += A[i][j] * vec[j];
            }
        }
        runtime = omp_get_wtime()*1000 - runtime;
        cout<<counter<<" threads, columns time is"<<runtime<<endl;

        runtime = omp_get_wtime()*1000;
        int h = rows / counter;
        int w = cols / counter;
        #pragma omp parallel shared(A,vec,vec_block) num_threads(counter)
        for(int blocks = 0; blocks < counter*counter; ++blocks){
            int i = blocks/counter;
            int j = blocks % counter;
            for(int k = i*h;k<(i+1)*h;++k){
                for(int l = j*w;l<(j+1)*w;++l){
                    #pragma omp critial
                    vec_block[k] = A[k][l] * vec[l];
                }
            }
        }
        runtime = omp_get_wtime()*1000 - runtime;
        cout<<counter<<" threads, blocks time is "<<runtime*1000<<endl;
    }while (counter<omp_get_thread_num());
    
    cin.get(); cin.get();
    return 0;
    
}