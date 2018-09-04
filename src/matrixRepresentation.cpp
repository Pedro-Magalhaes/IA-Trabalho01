#include "matrixRepresentation.h"
#include <fstream>
#include <iostream>
#include <sstream>

matrixRepresentation::matrixRepresentation(unsigned int n)
{
    // total de posiçoes é n-1 M[0][0] faz parte da diagonal e é 0
    matrixData.resize(n - 1);
    nodeNumber = n;
}
matrixRepresentation::matrixRepresentation(unsigned int n, MatrixType m)
{
    // total de posiçoes é n-1 M[0][0] faz parte da diagonal e é 0
    matrixData.resize(n - 1);
    mType = m;
    nodeNumber = n;
}

matrixRepresentation::~matrixRepresentation()
{
}

bool matrixRepresentation::readMatrixData(std::string &matrixData)
{
    std::stringstream ss(matrixData);
    int x;
    // dados separados por 0's
    // como vamos armazenar os dados como uma matriz inferior basta ler direto
    if (this->mType == MatrixType::LOWER_DIAG_ROW)
    {
        while (ss >> x)
        {
            if (x == 0)
            {
                std::cout << "\n"
                          << std::endl;
            }
            else
            {
                std::cout << x << " ";
            }
        }
    }
    // dados separados por linha, nao contem 0's
    // temos que transformar para saber a linha do vetor
    else if ( this->mType == MatrixType::UPPER_ROW )
    {
        unsigned int limite = this->nodeNumber;
        unsigned int contador = 0;
        unsigned int linha = 0;
        while( ss >> x ) 
        {
            if(contador == limite)
            {
                contador = 0;
                limite--;
                linha++;
            }
            this->matrixData[this->vectorPosition(linha,contador)] = x;
        }
        std::cout<< " ultimo elemento "<< matrixData.back()<<std::endl;

    }

    return true;
}

// assume que nenhuma entrada vai ter line == row
// Vai receber um par linha coluna da matriz e retornar o indice correspondente
unsigned int matrixRepresentation::vectorPosition(unsigned int line, unsigned int row)
{
    unsigned int x, y;
    if (line < row)
    {
        x = row;
        y = line;
    }
    else
    {
        y = row;
        x = line;
    }

    return x + ((y - 1) * y) / 2;
}
