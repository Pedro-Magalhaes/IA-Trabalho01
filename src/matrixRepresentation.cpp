#include "matrixRepresentation.h"
#include <fstream>
#include <iostream>
#include <sstream>



matrixRepresentation::matrixRepresentation(unsigned int n)
{
    // total de posiçoes é n-1 M[0][0] faz parte da diagonal e é 0
    matrixData.resize((n * (n - 1)) / 2);
    nodeNumber = n;
}
matrixRepresentation::matrixRepresentation(unsigned int n, MatrixType m)
{
    // total de posiçoes é n-1 M[0][0] faz parte da diagonal e é 0
    matrixData.resize(n);
    for (int i = 0; i < n ; i++) {
        matrixData[i].resize(n);
    }
    mType = m;
    nodeNumber = n;
}

matrixRepresentation::matrixRepresentation(const matrixRepresentation &other)
{
    this->matrixData = other.matrixData;
    this->mType = other.mType;
    this->nodeNumber = other.nodeNumber;
}

matrixRepresentation::~matrixRepresentation()
{
}

bool matrixRepresentation::readMatrixData(std::string &matrixDataString)
{

    std::stringstream ss(matrixDataString);
    int x , maior = 0;
    // dados separados por 0's
    // como vamos armazenar os dados como uma matriz inferior basta ler direto
    if (this->mType == MatrixType::LOWER_DIAG_ROW)
    {
        unsigned int nextPosition = 0;
        for (unsigned int i = this->nodeNumber; i > 0; i--)
        {
            while (ss >> x)
            {
                if (x != 0)
                {
                    this->matrixData[i][nextPosition].first = nextPosition; // guardando o "id" do no, para não perder a info ao ordenar
                    this->matrixData[i][nextPosition].second = x; // peso do no
                    if( x > maior)
                    {
                        maior = x;
                    }
                    nextPosition++;
                }
            }
        }
    }
    // dados separados por linha, nao contem 0's
    // temos que transformar para saber a linha do vetor
    else if (this->mType == MatrixType::UPPER_ROW)
    {
        unsigned int contador = 0;
        unsigned int linha = 0;
        for (unsigned int i = this->nodeNumber - 1; i > 0; i--)
        {
            // preenchendo a diagonal 
            //OBS: VAMOS TER QUE TESTAR SE O PESO EH 0 quando ordenar porque o peso 0 sempre vai ser o primeiro do vetor ordenado
            this->matrixData[i][i].second = 0;
            this->matrixData[i][i].first = i;
            while (contador < i)
            { 
                ss >> x;
                printf("contador %u , i: %u \n",contador,i);
                // colocando os dados primeiro na coluna
                this->matrixData[contador][linha].second = x;
                this->matrixData[contador][linha].first = contador;
                // agora na parte transtposta (na linha)
                this->matrixData[linha][contador].second = x;
                this->matrixData[linha][contador].first = contador;

                contador++;
                if( x > maior)
                {
                    maior = x;
                }
            }
            linha++;
            contador = 0;
        }
    }
    std::cout<<"Maior: "<< maior <<" First = "<<this->matrixData[0][0].second<<" Last = "<< this->matrixData.back().back().second << std::endl;
    return true;
}

