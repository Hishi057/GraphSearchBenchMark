#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <chrono>
using namespace std;

int N, M;
vector<vector<int> > G;

void input(string filename){
    ifstream inputFile(filename);

    inputFile >> N >> M;
    G.assign(N+1, vector<int>());
    for (int i = 1; i <= M; i++)
    {
        int A, B;
        inputFile >> A >> B;
        G[A].push_back(B);
        G[B].push_back(A);
    }

    inputFile.close();
}

int BFS(){
    vector<int64_t> ways(N + 1, 0);  // 最短での到着経路の本数
    vector<int64_t> dist(N + 1, -1); // 最短距離
    queue<int> que;

    ways[1] = 1;
    dist[1] = 0;
    que.push(1);

    while (!que.empty())
    {
        int v = que.front();
        que.pop();

        for (int nv : G[v])
        {
            if (dist[nv] == -1)
            {
                dist[nv] = dist[v] + 1;
                que.push(nv);
            }

            if (dist[nv] == dist[v] + 1)
            {
                ways[nv] += ways[v] % 1000000007;
            }
        }
    }

    return ways[N] % 1000000007;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();
    input("input.txt");
    cout << BFS() << endl;
    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "計測時間: " << duration.count() << endl;

}