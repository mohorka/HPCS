#include <omp.h>
#include <iostream>

using namespace std;

int main(){
    int rows1, cols1, rows2, cols2;
    double **A, **B, **C;

    cout << "Please, input rows and columns number for first matrix separated by space: ";
    cin >> rows1;
    cin >> cols1;
    cout << "Please, input rows and columns number for second separated by space: ";
    cin >> rows2;
    cin >> cols2;
    
    if(cols1 != rows2){
        cout << "Composition is impossible!";
        cin.get();cin.get();
        return 0;
    }

    A = new double* [rows1];
    cout << "Enter elements for first matrix"<<endl;
    for (int i =0;i<rows1;++i){
        A[i] = new double[cols1];
        for(int j=0; j<cols1;++j){
            cout<<"A["<<i<<"]["<<j<<"]=";
            cin >> A[i][j];
        }
    }

    B = new double* [rows2];
    cout << "Enter elements for second matrix"<<endl;
    for (int i =0;i<rows2;++i){
        B[i] = new double[cols2];
        for(int j=0; j<cols2;++j){
            cout<<"B["<<i<<"]["<<j<<"]=";
            cin >> B[i][j];
        }
    }

    C = new double* [rows1];
    for (int i=0; i<rows1; ++i){
        C[i] = new double[cols2];
    }

    #pragma omp parallel for collapse(2) schedule(static) shared(A,B)
    for (int i=0; i<rows1; ++i){
        for(int j=0; j<cols2; ++j){
            for(int k=0;k<cols1;++k){
                #pragma omp critical
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }

    cout<< "Matrix of composition:"<<endl;
    for (int i=0; i<rows1; ++i){
        for(int j=0; j<cols2;++j){
            cout<<C[i][j]<<" ";
        }
        cout<<endl;
    }
    cin.get();cin.get();
    return 0;

}