#pragma once

#include <vector>
#include <string>
#include <utility>

enum MatrixType {LOWER_DIAG_ROW,UPPER_ROW,ERRO};  // Tipo de matriz descrita nos arquivos

class matrixRepresentation
{
  private:
      std::vector < std::vector < std::pair<int,int> > > constructSortedMatrix(std::vector < std::vector < std::pair<int,int> > > originalM);
  public:
    
    struct edge
    {
        int from;
        int to;
        int weight;
    };
    

    matrixRepresentation(){}
    matrixRepresentation(unsigned int n); // n = number of nodes
    matrixRepresentation(unsigned int n, MatrixType m); 
    matrixRepresentation(const matrixRepresentation & other);
	unsigned int getNodeNumber();
	unsigned int getNumberNeighbours(int i);
    bool readMatrixData ( std::string & matrixDataString );
    int valueat(const int line, const int col);
    std::vector < std::pair<int,int>> getNeighbours(int node);
    std::vector < std::pair<int,int>> getSortedNeighbours(int node); // retorna o vetor em ordem crescente, sem o proprio no que tem aresta 0
    std::pair<int,int> nodeAt(const int line, const int col);
    void printVector();
    std::vector<matrixRepresentation::edge> getAllEdges();
    ~matrixRepresentation();

  private:
    unsigned int nodeNumber;
    MatrixType mType; // tipo da matriz a ser lida
    std::vector < std::vector < std::pair<int,int> > > matrixData; // dados relevantes da matriz first = "id" da cidade second = peso da aresta
    std::vector < std::vector < std::pair<int,int> > > sortedMatrixData;

};

