#pragma once
#include <vector>

#include "matrixRepresentation.h"
#include "unionfind.h"

class Solver
{
private:
	enum Color
		{
			WHITE,
			GRAY,
			BLACK
		};
	std::vector<int> generateRandomSolution();	
	std::vector<matrixRepresentation::edge> getSortedEdges();
	std::vector<int> TwoOptSwap(const std::vector<int>& currentSol, int i, int k);
	std::vector<int> kruskalGetBestPath(std::vector<matrixRepresentation::edge> pathA);
public:
	Solver();
	Solver(int n,matrixRepresentation & m);
	int funcaoObjetiva(std::vector<int> & sol);
	std::vector<int> solucaoGulosa();
	std::vector<int> solucaoKruskalAdaptada();
	std::vector<int> dfs_latencia(int s, int p,int k);
	int dfs_soma_latencia_visit(int u, int p, int latencia, int * melhorLatencia,std::vector<Color> inPath, int pAtual, int somaLatencia,int k);
	std::vector<std::vector<int>> CalculaVizinhancaSwap(std::vector<int>& sol,int limit = 0);
	std::vector<std::vector<int>> CalculaVizinhancaRelocate(std::vector<int>& sol);
	std::vector<std::vector<int>> CalculaVizinhanca2Opt(std::vector<int>& sol);
	std::vector<int> BuscaLocal();
	std::vector<int> SimAnn();
	~Solver();

private:
	
	matrixRepresentation _m;
	std::vector<int> _finalSolution;
	int _node_number;
};


