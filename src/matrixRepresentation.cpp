#include "matrixRepresentation.h"
#include <fstream>
#include <iostream>
#include <sstream>

matrixRepresentation::matrixRepresentation(unsigned int n)
{
    // total de posiçoes é n-1 M[0][0] faz parte da diagonal e é 0
    matrixData.resize( (n * (n-1))/2 ) ;
    nodeNumber = n;
}
matrixRepresentation::matrixRepresentation(unsigned int n, MatrixType m)
{
    // total de posiçoes é n-1 M[0][0] faz parte da diagonal e é 0
    matrixData.resize( (n * (n-1))/2 );
    mType = m;
    nodeNumber = n;
}

matrixRepresentation::matrixRepresentation(const matrixRepresentation & other) {
    this->matrixData = other.matrixData;
    this->mType = other.mType;
    this->nodeNumber = other.nodeNumber;
}

matrixRepresentation::~matrixRepresentation()
{
}

bool matrixRepresentation::readMatrixData(std::string & matrixDataString)
{
    std::stringstream ss(matrixDataString);
    int x;
    // dados separados por 0's
    // como vamos armazenar os dados como uma matriz inferior basta ler direto
    if (this->mType == MatrixType::LOWER_DIAG_ROW)
    {
        unsigned int nextPosition = 0;
        for(unsigned int i = this->nodeNumber; i > 0; i--)
        {
            while (ss >> x)
        {
            if (x != 0)
            {
                this->matrixData[nextPosition++] = x;
            }
        }
        }
        
        std::cout<<"First = "<<this->matrixData[0]<<std::endl;
    }
    // dados separados por linha, nao contem 0's
    // temos que transformar para saber a linha do vetor
    else if ( this->mType == MatrixType::UPPER_ROW )
    {
        unsigned int contador = 0;
        unsigned int linha = 0;
        for(unsigned int i = this->nodeNumber-1; i > 0; i--)
        {
            while (ss >> x)
            {
                this->matrixData[this->vectorPosition(linha,contador)] = x;
                contador++;
            }
            linha++;
            contador = 0;
        }          
        
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
