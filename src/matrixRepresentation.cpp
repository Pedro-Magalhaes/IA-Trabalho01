#include "matrixRepresentation.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

// cada nó é composto por um pair e possui seu indice (id da cidade) no FIRST e o peso da aresta no SECOND

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
    for (unsigned int i = 0; i < n ; i++) {
        matrixData[i].resize(n);
    }
    mType = m;
    nodeNumber = n;
}

// retorna todos as arestas do grafo
std::vector<matrixRepresentation::edge> matrixRepresentation::getAllEdges()
{
    std::vector<matrixRepresentation::edge> arestas;
    unsigned int n = this->nodeNumber;
    arestas.reserve(n*(n-1)/2); // reservando sem inicializar (push fica otimizado)
    
    for(unsigned int i = 0; i < n; i++)
    {
        for(unsigned int j = 0; j < n; j++)
        {
            if(i!=j)
            {
                edge e;
                e.from = i;
                e.to = j;
                e.weight = this->matrixData[i][j].second;
                arestas.emplace_back(e);
            }
            
        }
    }
    return arestas;
    

}

matrixRepresentation::matrixRepresentation(const matrixRepresentation &other)
{
    this->matrixData = other.matrixData;
    this->mType = other.mType;
    this->nodeNumber = other.nodeNumber;
}

unsigned int matrixRepresentation::getNodeNumber()
{
	return this->nodeNumber;
}

unsigned int matrixRepresentation::getNumberNeighbours(int i)
{
	return this->sortedMatrixData[i].size();
}

matrixRepresentation::~matrixRepresentation()
{
}

int matrixRepresentation::valueat(const int line, const int col) {
    
    if( line == col ) { // diagonal
        return 0;
    }
    return this->matrixData[line][col].second;
}

std::pair<int,int> matrixRepresentation::nodeAt(const int line, const int col) {
    
    return this->matrixData[line][col];
}

bool matrixRepresentation::readMatrixData(std::string &matrixDataString)
{
    std::stringstream ss(matrixDataString);
    int x , maior = 0;
    // dados separados por 0's
    // como vamos armazenar os dados como uma matriz inferior basta ler direto
    if (this->mType == MatrixType::LOWER_DIAG_ROW)
    {
        for (unsigned int i = 0 ; i < this->nodeNumber; i++)
        {
            for (unsigned int j = 0; j < i+1; j++)
            {
                ss >> x;
                // colocando os dados na parte da linha
                this->matrixData[i][j].second = x;
                this->matrixData[i][j].first = j;

                // colocando os dados da parte transtposta
                this->matrixData[j][i].second = x;
                this->matrixData[j][i].first = i;
                if( x > maior)
                {
                    maior = x;
                }
                
            }
        }
    }
    // dados separados por linha, nao contem 0's
    // temos que transformar para saber a linha do vetor
    else if (this->mType == MatrixType::UPPER_ROW)
    {
        for (unsigned int i = 0; i < this->nodeNumber; i++)
        {
            
            // preenchendo a diagonal 
            //OBS: VAMOS TER QUE TESTAR SE O PESO EH 0 quando ordenar porque o peso 0 sempre vai ser o primeiro do vetor ordenado
            this->matrixData[i][i].second = 0;
            this->matrixData[i][i].first = i;         

            for (unsigned int j = i+1; j < this->nodeNumber; j++)
            { 
                ss >> x;               
                // colocando os dados na parte da linha
                this->matrixData[i][j].second = x;
                this->matrixData[i][j].first = j;

                // colocando os dados da parte transtposta
                this->matrixData[j][i].second = x;
                this->matrixData[j][i].first = i;

                if( x > maior)
                {
                    maior = x;
                }
            }
            
        }
    }
	this->sortedMatrixData = this->constructSortedMatrix(this->matrixData);
    std::cout<<"last x: "<< x << " num nodes "<< this->nodeNumber <<std::endl;
    std::cout<<"Maior: "<< maior <<" First = "<<this->matrixData[0][0].second<<" Last = "<< this->matrixData.back().back().second << std::endl;
    return true;
}

// retornando uma cópia do vetor da matriz original
std::vector < std::pair<int,int>> matrixRepresentation::getNeighbours(int node)
{
    return std::vector<std::pair<int,int>>(this->matrixData[node]);
}

// retornando uma cópia do vetor da matriz original ordenado 
std::vector < std::pair<int,int>> matrixRepresentation::getSortedNeighbours(int node)
{
    return std::vector<std::pair<int,int>>(this->sortedMatrixData[node]);
}

// construindo a matriz com a vizinhança ordenada
std::vector < std::vector < std::pair<int,int> > > matrixRepresentation::constructSortedMatrix(std::vector < std::vector < std::pair<int,int> > > originalM)
{
    std::vector < std::vector < std::pair<int,int> > > sorted(originalM.size());

    for(unsigned int i = 0; i < originalM.size(); i++) 
    {
        // fazendo uma cópia do vetor linha da matriz original
        sorted[i] = std::vector< std::pair<int,int>> (originalM[i].begin(),originalM[i].end());
        // fazendo o sort do vetor "sorted" , passando o inicio e o fim do vetor, além de uma funçao de comparação
        std::sort( sorted[i].begin(),sorted[i].end(), [](std::pair<int,int> a, std::pair<int,int> b) {
        return a.second < b.second;   
        } );
    }
	for (int i = 0; i < sorted.size();i++)
	{
		printf("%d ", i);
		for (int j = 0;j < sorted[i].size();j++)
		{
			printf("%d ", sorted[i][j].first);
		}
		printf("\n\n\n");
	}
    return sorted;
}

