#include "unionfind.h"
#include <iostream>

UnionFind::UnionFind(int n)
    : parent( std::vector<int>(n, -1) )
    , size( std::vector<int>(n, 1) )
    , numSets(n)
{
}


UnionFind::~UnionFind()
{
}


int UnionFind::find(int u)
{
	if (parent[u]==-1)
		return u;
	parent[u]=find(parent[u]);
	return parent[u];
}


void UnionFind::makeUnion(int u, int v)
{
	u=find(u);
	v=find(v);
	if (v!=u)
	{
		if (size[v]>size[u])
		{
			size[v]=size[v]+size[u];
			size[u]=0;
			parent[u]=v;
		}
		else
		{
			size[u]=size[u]+size[v];
			size[v]=0;
			parent[v]=u;
		}
		numSets--;
	}
	else
	{
		std::cout<<"pertencem ao mesmo grupo!"<<std::endl;
	}
}


int UnionFind::getNumSets()
{
    return numSets;
}
