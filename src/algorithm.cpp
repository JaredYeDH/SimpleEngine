#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
#define OCX_MAX_VERTEX_NUM 500

int min_int(int m, int n)
{
	return m < n ? m : n;
}
vector<int> gVec;

vector<vector<int>> gMap;
int gVecNumber;
struct Node
{
	int w;
	int v;
	int d[2];
	int p;
};

struct Graph
{
	enum
	{
		PRINT_D,
		PRINT_W,
		PRINT_P
	};
	int vecNum;
	vector<vector<Node>> g;
	map<int, string> vecName;
	int edgeNum;
	int dIndexer;
	void setDistanceIndexer(int d) { dIndexer = d; };

	vector<Node> &operator[](int i)
	{
		return g[i];
	}
	void init(int num)
	{
		vecNum = num;
		g.clear();
		g.resize(vecNum);
		for (int i = 0; i < vecNum; i++)
		{
			int v;
			string name;
			cin >> v >> name;
			vecName[v - 1] = name;
			g[i].resize(vecNum);
			cout << v << ":" << name << endl;
		}

		for (int i = 0; i < vecNum; i++)
		{
			for (int j = 0; j < vecNum; j++)
			{
				if (i != j)
				{
					g[i][j].w = 9999;
					g[i][j].d[0] = 9999;
					g[i][j].d[1] = 9999;
				}
				else
				{
					g[i][j].w = 0;
					g[i][j].d[0] = 0;
					g[i][j].d[1] = 0;
				}
				g[i][j].p = -1;
			}
		}
	}
	void print(int kind)
	{
		for (int i = -1; i < vecNum; i++)
		{
			for (int j = -1; j < vecNum; j++)
			{
				if (i == -1)
				{
					if (j != -1)
						cout << vecName[j] << " ";
					else
						cout << "\t\t";
				}
				else
				{
					if (j == -1)
					{
						cout << vecName[i] << "   \t" << (i + 1) << "|";
					}
					else
					{
						int res = 0;
						switch (kind)
						{
						case PRINT_W:
							res = g[i][j].w;
							break;
						case PRINT_D:
							res = g[i][j].d[dIndexer];
							break;
						case PRINT_P:
							res = g[i][j].p;
							break;
						default:
							break;
						}
						cout << res << '\t';
					}
				}
			}
			cout << endl;
		}
	}

	void initEdge(int edgeNum)
	{
		edgeNum = edgeNum;
		for (int e = 0; e < edgeNum; e++)
		{
			int i, j;
			cin >> i >> j;
			cout << i << '\t' << j << endl;
			g[i - 1][j - 1].w = 1;
		}
	}

	void query(int u, int v)
	{
		cout << vecName[u] << " 到 " << vecName[v]
			 << " 最短距离：" << g[u][v].d[dIndexer] << endl;
	}
};

void Floyd_Warshall(Graph &g)
{
	g.print(Graph::PRINT_W);
	int switcher = 0;
	for (int k = -1; k < g.vecNum - 1; k++)
	{
		int next = (switcher++) % 2;
		int prev = (next + 1) % 2;
		for (int i = 0; i < g.vecNum; i++)
		{
			for (int j = 0; j < g.vecNum; j++)
			{
				if (k == -1)
				{
					g[i][j].d[0] = g[i][j].w;
					if (i == j)
					{
						g[i][j].p = -1;
					}
					else
					{
						g[i][j].p = i;
					}
					switcher = 1;
				}
				else
				{
					g[i][j].d[next] = min_int(g[i][j].d[prev], g[i][k].d[prev] + g[k][j].d[prev]);
					if (g[i][j].d[prev] <= g[i][k].d[prev] + g[k][j].d[prev])
					{
						g[i][j].p = g[i][j].p;
					}
					else
					{
						g[i][j].p = g[k][j].p;
					}
					//cout << prev << '\t' << next << '\t' << g[i][j].d[next];
				}
			}
		}
	}

	int next = (switcher + 1) % 2;
	g.setDistanceIndexer(next);
	g.print(Graph::PRINT_D);
	g.print(Graph::PRINT_P);
}

int main()
{
	freopen("C:/Users/wangfang/Desktop/DevLog/test.map", "r", stdin);
	int n;
	cin >> n;
	cout << "VecNumber:" << n << endl;
	Graph g;
	g.init(n);

	cin >> n;
	g.initEdge(n);
	Floyd_Warshall(g);

	cin >> n;
	for (auto i = 0; i < n; i++)
	{
		int u, v;
		cin >> u >> v;
		g.query(u - 1, v - 1);
	}
	while (true)
		;
	return 0;
}
