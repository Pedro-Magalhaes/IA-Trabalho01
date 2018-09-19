#pragma once
#include <vector>

#include "matrixRepresentation.h"


class Solver
{
private:
	std::vector<int> generateRandomSolution();
	int funcaoObjetiva(std::vector<int> & sol);
	enum Color
	{
		WHITE,
		GRAY,
		BLACK
	};
public:
	Solver();
	Solver(int n,matrixRepresentation & m);
	std::vector<int> BAHIA();
	std::vector<std::vector<int>> CalculaVizinhancaSwap(std::vector<int>& sol);
	std::vector<std::vector<int>> CalculaVizinhancaRelocate(std::vector<int>& sol);
	std::vector<std::vector<int>> CalculaVizinhancakOpt(std::vector<int>& sol, int k = 2);
	std::vector<std::vector<int>> CalculaVizinhanca(std::vector<int>& sol);
	//std::vector<std::vector<int>> CalculaVizinhanca(std::vector<int>& sol);
	std::vector<int> BuscaLocal();
	int dfs_limitada(int s, int currentDepth, int k, std::vector<Color> & nodeStates);
	~Solver();

private:

	matrixRepresentation _m;
	std::vector<int> _finalSolution;
	int _node_number;
};

