#include "../matrixRepresentation.h"
#include <iostream>


// g++ -std=c++14 ./../matrixRepresentation.cpp  matrixRepresentation_test.cpp -o teste

void printRepresentartionMatrix( matrixRepresentation & M , int n) {
    for (size_t i = 0; i < n; i++)
    {   
        for (size_t j = 0; j < n; j++)
        {
            std::cout<<" "<< M.valueat(i,j);
        }
        std::cout<<"\n"<<std::endl;
    }
}

void printMatrix( std::vector< std::vector<int> > & M , int n) {
    for (size_t i = 0; i < n; i++)
    {   
        for (size_t j = 0; j < n; j++)
        {
            std::cout<<" "<< M[i][j];
        }
        std::cout<<"\n"<<std::endl;
    }
}

void compareValues( std::vector< std::vector<int> > & M , matrixRepresentation & R ,int n) {
    for (size_t i = 0; i < n; i++)
    {   
        for (size_t j = 0; j < n; j++)
        {
            if( M[i][j] != R.valueat(i,j) )
            {
                std::cout<< "<<ERROR>>Matrix not equal value at line: "<<i<<" row: "<<j<<" are not equal."<<std::endl;
                std::cout<< "Matrix : \n"<<std::endl;
                printMatrix(M,n);
                std::cout<< "Representation : \n"<<std::endl;
                printRepresentartionMatrix(R,n);
                return;
            }
        }
    }
    std::cout<< "<<SUCCESS>>Matrix and Representation Matrix are equal."<<std::endl;
}

int main (void) 
{
    std::vector< std::vector<int> > myMatrix(5,std::vector<int>(5));
    std::string lowerRepresentation = ""; // tem representacao dos zeros da diagonal
    std::string upperRepresentation = ""; // diagonal nao representada
    matrixRepresentation m;
    
    for (size_t i = 0; i < 5; i++)
    {   
        myMatrix[i][i] = 0;
        for (size_t j = i+1; j < 5; j++)
        {
            myMatrix[i][j] = abs(i-j) ;
            myMatrix[j][i] = abs(i-j);                
        }
    }
    std::cout<<"test matrix :"<<std::endl;
    printMatrix(myMatrix,5);


    // criando a string que representa a matriz com a parte superior
    for (size_t i = 0; i < 5; i++) 
    {
        for (size_t j = i+1; j < 5; j++)
        {
            upperRepresentation.append( std::to_string(myMatrix[i][j]) + " ");
        }
    }
    std::cout<<upperRepresentation<<std::endl;

    // criando a string que representa a matriz com a parte inferior (diagonais sao representadas)
    for (size_t i = 0; i < 5; i++) 
    {
        for (size_t j = 0; j < i + 1; j++)
        {
            lowerRepresentation.append( std::to_string(myMatrix[i][j]) + " ");
        }
    }
    std::cout<<lowerRepresentation<<std::endl;
    m = matrixRepresentation(5,MatrixType::UPPER_ROW);
    m.readMatrixData(upperRepresentation);
    std::cout<<"Teste com matriz upper row: "<<std::endl;
    compareValues(myMatrix,m,5);

    // m.printVector();
    m = matrixRepresentation(5,MatrixType::LOWER_DIAG_ROW);
    m.readMatrixData(lowerRepresentation);
    std::cout<<"Teste com matriz lower diag row: "<<std::endl;
    compareValues(myMatrix,m,5);


    return 0;
}   