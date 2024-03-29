#include <iostream>
#include <string>
#include <set>
#include <iomanip>
#include <fstream>
#include <map>

#define INF 0x3f3f3f
using namespace std;
const int maxn = 505;

// 定义遍历邻接节点的方向
int directions[8][2] = {
    {0, -1},  // 上
    {0, 1},   // 下
    {-1, 0},  // 左
    {1, 0},   // 右
    {-1, -1}, // 左上
    {1, -1},  // 右上
    {-1, 1},  // 左下
    {1, 1},   // 右下

};

// 定义节点的结构
struct Node
{
    // 当前节点的状态
    // 0 - new
    // 1 - open
    // -1 - close
    int state;

    // 当前节点的坐标值
    int x;
    int y;

    // 当前节点的 h 值（也就是 dijkstra 中的 dis ）
    double h;

    // 当前节点的 k 值
    double k;

    // 当前节点指向的父节点
    int father;

    // 当前节点的下标
    int index;
};

// 节点列表
Node node[maxn];

// 地图信息
string mp[maxn];

// 地图维数
int n;

map<int, bool> isPath;

// OpenList & CloseList
set<Node *> OpenList;
set<Node *> CloseList;

// 初始化
void init()
{
    // n * n 大小的栅格化地图
    cin >> n;
    // 输入地图数据
    for (int i = 0; i < n; i++)
        cin >> mp[i];

    // 初始化节点数据
    int cnt = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            node[cnt].father = -1;
            node[cnt].h = INF;
            node[cnt].k = INF;
            node[cnt].state = 0;
            node[cnt].x = i;
            node[cnt].y = j;
            node[cnt].index = cnt;
            cnt++;
        }
    }
    // node[0].h = 0;
    // node[0].k = 0;

    node[99].h = 0;
    node[99].k = 0;
}

bool isValidNode(int x, int y)
{
    if (x < 0 || x >= n || y < 0 || y >= n)
        return false;
    if (mp[x][y] == '#')
        return false;

    return true;
}

// 使用 dijkstra 规划最短路径
void dijkstra(int s, int e)
{
    // 将起点 S 放入开放列表
    OpenList.insert(&node[s]);
    node[s].state = 1;

    while (OpenList.size())
    {
        // 遍历 OpenList，找到 h 值最小的节点 n
        int min_h_value = INF;
        // 找到当前正在处理的节点的指针
        Node *N_ptr;
        for (set<Node *>::iterator i = OpenList.begin(); i != OpenList.end(); i++)
        {
            Node *nodePtr = *i;
            if (nodePtr->h < min_h_value)
            {
                min_h_value = nodePtr->h;
                N_ptr = nodePtr;
            }
        }

        // 遍历 N 的邻接节点
        for (int i = 0; i < 8; i++)
        {
            // 先判断当前节点是否存在（合法）
            int n_near_x = N_ptr->x + directions[i][0];
            int n_near_y = N_ptr->y + directions[i][1];
            if (!isValidNode(n_near_x, n_near_y))
                continue;

            Node *N_near_Ptr = &node[n_near_x * n + n_near_y];

            // 当前节点的状态为 new（没有访问过）
            if (N_near_Ptr->state == 0)
            {
                // 加入开放列表
                OpenList.insert(N_near_Ptr);
                N_near_Ptr->state = 1;
                // 更新 h 值
                N_near_Ptr->h = N_ptr->h + (i > 3 ? 1.4 : 1);
                // 将 n 设置为 n_near 的父结点
                N_near_Ptr->father = N_ptr->index;
            }
            // 当前节点的状态为 open（已经访问过但是还没有确定最短路径）
            else if (N_near_Ptr->state == 1)
            {
                double new_h = N_ptr->h + (i > 3 ? 1.4 : 1);
                if (N_near_Ptr->h > new_h)
                {
                    N_near_Ptr->h = new_h;
                    N_near_Ptr->father = N_ptr->index;
                }
            }
            // 当前节点处理完毕，移入CloseList
            N_ptr->state = -1;
            OpenList.erase(N_ptr);
            CloseList.insert(N_ptr);
            // 终点进入 CloseList 算法结束
            if (N_ptr->index == e)
                return;
        }
    }
}

// 输出最终确定的最短路径
void outPut_Path()
{
    ofstream outputFile("output.md", ios::app);

    int index = 99;
    while (index != 0)
    {
        isPath[index] = true;
        index = node[index].father;
    }

    outputFile << "## Path" << endl;
    outputFile << "```" << endl;

    int cnt = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (isPath[cnt])
                outputFile << "·" << ' ';
            else
                outputFile << mp[i][j] << ' ';
            cnt++;
        }
        outputFile << endl;
    }
    outputFile << "```" << endl;
}

// 输出 H 权值图
void outPut_H_graph()
{
    ofstream outputFile("output.md", ios::app);
    outputFile << "## H-Graph" << endl;
    for (int i = -1; i < n; i++)
    {
        if (i >= 0)
            outputFile << "|" << i;
        else
            outputFile << "|/";
    }

    outputFile << "|" << endl;
    for (int i = -1; i < n; i++)
        outputFile << "|-";
    outputFile << "|" << endl;
    int cnt = 0;
    for (int i = 0; i < n; i++)
    {
        outputFile << "|" << i << "|";
        for (int j = 0; j < n; j++)
        {
            if (node[cnt].h == INF)
                outputFile << "###" << '|';
            else
                outputFile << fixed << setprecision(1) << node[cnt].h << '|';
            cnt++;
        }
        outputFile << endl;
    }
    outputFile << endl;
}

void outPut()
{
    ofstream outputFile("output.md");
    outputFile << "# OutPuts" << endl;
    outputFile << endl;
    outPut_H_graph();
    // outPut_Path();
}

int main()
{
    freopen("input.txt", "r", stdin);
    init();
    dijkstra(99, 0);
    outPut();
}