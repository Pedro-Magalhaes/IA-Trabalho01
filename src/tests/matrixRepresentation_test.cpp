#include "../matrixRepresentation.h"
#include <iostream>


int main (void) 
{
    int myMatrix[5][5];
    std::string lowerRepresentation = ""; // tem representacao dos zeros da diagonal
    std::string upperRepresentation = ""; // diagonal nao representada
    matrixRepresentation m;
    for (size_t i = 0; i < 5; i++)
    {   
        for (size_t j = i; j < 5; j++)
        {
            if( i != j) 
            {
                myMatrix[i][j] = j+1;
                myMatrix[j][i] = j+1;
            }
            else
            {
                myMatrix[i][j] = 0;
            }
            
            
        }
    }

    for (size_t i = 0; i < 5; i++)
    {   
        for (size_t j = 0; j < 5; j++)
        {
            std::cout<<" "<<myMatrix[i][j];
        }
        std::cout<<"\n"<<std::endl;
    }

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

    m.printVector();

    std::cout<<"\n"<<std::endl;
    
    for (size_t i = 0; i < 5; i++)
    {   
        for (size_t j = 0; j < 5; j++)
        {
            std::cout<<" "<<m.valueat(j,i);
        }
        std::cout<<"\n"<<std::endl;
    }
    std::cout<<"\n Passada a representacao lower: "<<std::endl;

    m = matrixRepresentation(5,MatrixType::LOWER_DIAG_ROW);
    m.readMatrixData(lowerRepresentation);

    for (size_t i = 0; i < 5; i++)
    {   
        for (size_t j = 0; j < 5; j++)
        {
            std::cout<<" "<<m.valueat(j,i);
        }
        std::cout<<"\n"<<std::endl;
    }

    return 0;
}   