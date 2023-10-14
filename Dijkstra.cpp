#include <iostream>
#include <set>
#include <algorithm>
using namespace std;
#define INF 0x3f3f3f3f

int m;
string mp[20];
const int maxn = 200;

int directions[4][2] = {
    {0, -1}, // 上
    {0, 1},  // 下
    {-1, 0}, // 左
    {1, 0}   // 右
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

Node node[maxn];

Node s, e;

// 定义评价函数
int dis[maxn];

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

void Dijkstra()
{
    
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

    memset(dis, INF, sizeof(dis));

    dis[0] = 0;

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
    Dijkstra();

    // 输出访问过的节点
    cout << "被访问过的节点:" << endl;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (CloseList.find(CoordinatesToindex(i, j)) == CloseList.end())
                cout << mp[i][j] << ' ';
            else
                cout << 'A' << ' ';
        }
        cout << endl;
    }

    // 输出最短路径
    cout << endl
         << "最短路径:" << endl;
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
                cout << mp[i][j] << ' ';
            else
                cout << '%' << ' ';
        }
        cout << endl;
    }
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
#ifndef ONLINE_JUDGE
    freopen("/Applications/CppRunner.app/Contents/Resources/cpp/input.txt", "r", stdin);
#endif
    solve();
}