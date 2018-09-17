#pragma once

#include <vector>
#include <string>
#include <utility>

enum MatrixType {LOWER_DIAG_ROW,UPPER_ROW,ERRO};  // Tipo de matriz descrita nos arquivos

class matrixRepresentation
{
  private:
      
  public:
    matrixRepresentation(){}
    matrixRepresentation(unsigned int n); // n = number of nodes
    matrixRepresentation(unsigned int n, MatrixType m); 
    matrixRepresentation(const matrixRepresentation & other);
    bool readMatrixData ( std::string & matrixDataString );
    ~matrixRepresentation();
    
  private:
    unsigned int nodeNumber;
    MatrixType mType; // tipo da matriz a ser lida
    std::vector < std::vector < std::pair<int,int> > > matrixData; // dados relevantes da matriz first = "id" da cidade second = peso da aresta
};

