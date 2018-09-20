#pragma once
#include <vector>

#include "matrixRepresentation.h"


class Solver
{
private:
	std::vector<int> generateRandomSolution();
	int funcaoObjetiva(std::vector<int> & sol);
	std::vector<matrixRepresentation::edge> getSortedEdges();
public:
	Solver();
	Solver(int n,matrixRepresentation & m);
	std::vector<int> solucaoGulosa();
	std::vector<int> solucaoKruskalAdaptada();
	std::vector<int> BAHIA();
	std::vector<std::vector<int>> CalculaVizinhancaSwap(std::vector<int>& sol);
	std::vector<std::vector<int>> CalculaVizinhancaRelocate(std::vector<int>& sol);
	std::vector<std::vector<int>> CalculaVizinhancakOpt(std::vector<int>& sol, int k = 2);
	//std::vector<std::vector<int>> CalculaVizinhanca(std::vector<int>& sol);
	std::vector<int> BuscaLocal();
	~Solver();

private:
	matrixRepresentation _m;
	std::vector<int> _finalSolution;
	int _node_number;
};

