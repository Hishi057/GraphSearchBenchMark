#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <chrono>
#include <filesystem>
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

string BFS(){
    vector<int64_t> ways(N + 1, 0);  // 最短での到着経路の本数
    vector<int64_t> dist(N + 1, -1); // 最短距離
    vector<int64_t> from(N+1, -1); // 最短経路を辿る用

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
                from[nv] = v;
                que.push(nv);
            }

            if (dist[nv] == dist[v] + 1)
            {
                ways[nv] += ways[v] % 1000000007;
            }
        }
    }

    // スタートからゴールまでの手順を辿って表示する
    string path = "";
    if(ways[N] != 0){
        int current = N;
        path = to_string(current) + "<-";
        while(true){
            path += to_string(from[current]);
            current = from[current];
            if (current == 1) break;
            path += "<-";
        }
    }
    

    return path;
}

int measure(int i){
    string filename = "Graphs/" + to_string(i) + ".txt";
    if(filesystem::exists(filename)){
        auto start = chrono::high_resolution_clock::now();
        input(filename);
        // cout << "最短経路の道筋: " << BFS() << endl;
        BFS();
        auto end = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cout << i << ": 計測時間(ms): " << duration.count() << endl;

        return duration.count();
    }else{
        return -1;
    }
}


int main()
{
    int number = 100000;
    vector<int> durations(number+1, -1);

    for(int i = 1;i <= number; i++){
        durations[i] = measure(i);
    }

}