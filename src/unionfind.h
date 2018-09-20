#pragma once

#include <vector>

class UnionFind
{
public:
    //Inicializa a UnionFind com o numero de elementos
    UnionFind(int n);

    //Destrutor
    ~UnionFind();

    //Retorna a raiz do conjunto a que u pertence
    int find(int u);

    //Une os dois conjuntos que contém u e v
    void makeUnion(int u, int v);

    //Retorna o numero de conjuntos diferentes
    int getNumSets();

private:
    //Armazena o pai de cada elemento
    std::vector<int> parent;

    //Armazena o tamanho de cada conjunto
    std::vector<int> size;

    //Quantidade de conjuntos atual
    int numSets;
};
