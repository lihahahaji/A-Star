#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <string>
#include <cstring>
#include <complex>
#include <iterator>
using namespace std;

#define INF 0x3f3f3f3f
int m;
string mp[20];
int directions[4][2] = {
	{0, -1}, // 上
	{0, 1},	 // 下
	{-1, 0}, // 左
	{1, 0}	 // 右
};

struct Node
{
	// 定义构造函数
	Node() {}
	Node(int x, int y)
	{
		this->x = x;
		this->y = y;
		this->index = x * m + y;
		this->father = 0;
	}
	int x;
	int y;
	int index;
	int father;
};

Node node[200];

Node s, e;

// 定义评价函数
int f[200];
int g[200];
int h[200];

// 定义开放列表和关闭列表
set<int> OpenList;
set<int> CloseList;

// 下标转为坐标
pair<int, int> indexToCoordinates(int index)
{
	int x = index / m;
	int y = index % m;
	return pair<int, int>(x, y);
}

// 坐标转为下标
int CoordinatesToindex(int x, int y)
{
	return x * m + y;
}

// 计算两个点之间的曼哈顿距离
int manhattanDistance(int point1, int point2)
{
	pair<int, int> p1 = indexToCoordinates(point1);
	pair<int, int> p2 = indexToCoordinates(point2);
	return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}

// 判断节点是否可达
bool isValidNode(int x, int y)
{
	if (x < 0 || x >= m || y < 0 || y >= m)
		return false;
	if (mp[x][y] == '#')
		return false;
	return true;
}

// 输出开放列表中的元素
void print_OpenList()
{
	cout << "OpenList: ";
	for (set<int>::iterator i = OpenList.begin(); i != OpenList.end(); i++)
	{
		cout << *i << ' ';
	}
	cout << endl;
}

void A_star()
{
	// 将起点 S 放入开放列表
	OpenList.insert(s.index);

	// 当开放列表不为空
	while (OpenList.size())
	{

		// print_OpenList();
		// 终点进入开放列表，找到路径，算法结束。
		if (OpenList.find(e.index) != OpenList.end())
			return;

		// 遍历开放列表，找到评价函数 f 的值最小的节点
		int min_f = INF;
		int n_ind = -1;

		for (set<int>::iterator i = OpenList.begin(); i != OpenList.end(); i++)
		{
			if (f[*i] < min_f)
			{
				min_f = f[*i];
				n_ind = *i;
			}
		}

		// 找到当前需要处理的节点 N 的下标 n_ind
		// 遍历节点 N 的邻接节点(上下左右)
		pair<int, int> n_coord = indexToCoordinates(n_ind);
		for (int i = 0; i < 4; i++)
		{
			int n_near_x = n_coord.first + directions[i][1];
			int n_near_y = n_coord.second + directions[i][0];
			int n_near_ind = CoordinatesToindex(n_near_x, n_near_y);

			if (!isValidNode(n_near_x, n_near_y) || CloseList.find(n_near_ind) != CloseList.end())
			{
				// 当前节点不可达 或者 已经在关闭列表中
				// 忽略
				// cout<<n_near_ind<<endl;
			}
			else if (OpenList.find(n_near_ind) == OpenList.end())
			{
				// 不在开放列表中（未被访问过），则将其加入开放列表
				OpenList.insert(n_near_ind);

				// 计算评估函数
				g[n_near_ind] = g[n_ind] + 1;
				f[n_near_ind] = g[n_ind] + 1 + manhattanDistance(n_near_ind, e.index);
				// 将 n 设为 n_near 的父结点
				node[n_near_ind].father = n_ind;
			}
			else
			{
				// 已经在开放列表中，更新 f
				if (g[n_near_ind] > g[n_ind] + 1)
				{
					// 如果有更优的路径
					g[n_near_ind] = g[n_ind] + 1;
					// 更新 f 的值
					f[n_near_ind] = g[n_near_ind] + manhattanDistance(n_near_ind, e.index);
					// 将 n 设为 n_near 的父结点
					node[n_near_ind].father = n_ind;
				}
			}
		}
		// 节点 n 已经处理完毕，移动到关闭列表。
		OpenList.erase(n_ind);
		CloseList.insert(n_ind);
	}
}

void init()
{
	cin >> m;
	int x, y;
	// 输入起点和终点的坐标
	cin >> x >> y;
	s = Node(x, y);
	cin >> x >> y;
	e = Node(x, y);
	// 输入地图数据
	for (int i = 0; i < m; i++)
		cin >> mp[i];

	memset(f, INF, sizeof(f));
	memset(g, INF, sizeof(g));
	memset(h, INF, sizeof(h));

	f[s.index] = manhattanDistance(s.index, e.index);
	g[s.index] = 0;
	h[s.index] = manhattanDistance(s.index, e.index);

	for (int i = 0; i < m; i++)
	{
		node[i].index = i;
		node[i].x = indexToCoordinates(i).first;
		node[i].y = indexToCoordinates(i).second;
		node[i].father = -1;
	}
}

void solve()
{
	init();
	A_star();


	// 输出访问过的节点
	cout<<"被访问过的节点:"<<endl;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (CloseList.find(CoordinatesToindex(i, j)) == CloseList.end())
				cout << mp[i][j]<<' ';
			else
				cout << 'A'<<' ';
		}
		cout << endl;
	}

	// 输出最短路径
	cout<<endl<<"最短路径:"<<endl;
	set<int> path;
	int p = 99;
	while (p != 0)
	{
		path.insert(p);
		p = node[p].father;
	}

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (path.find(CoordinatesToindex(i, j)) == path.end())
				cout << mp[i][j]<<' ';
			else
				cout << '*'<<' ';
		}
		cout << endl;
	}
}

int main()
{
	ios::sync_with_stdio(0);
	cin.tie(0);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
#endif
	solve();
}