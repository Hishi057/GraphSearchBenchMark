#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <chrono>
#include <filesystem>
using namespace std;

struct GraphData
{
    int N; // 頂点の数
    int M; // 辺の数
    vector<vector<int>> G; //隣接リスト
    int BFSduration = -1; // BFSでの探索に要した時間
    int DFSduration = -1; // DFSでの探索に要した時間
};

vector<GraphData> graphs; 

//
// 入力用の関数
//
GraphData input(string filename){
    ifstream inputFile(filename);

    int N,M;
    inputFile >> N >> M;
    vector<vector<int>> G(N+1);

    for (int i = 1; i <= M; i++)
    {
        int A, B;
        inputFile >> A >> B;
        G[A].push_back(B);
        G[B].push_back(A);
    }

    inputFile.close();
    return {N, M, G};
}

//
// BFSで探索
// 辿った経路をstring型で表示させることも可能
//
void BFS(GraphData graph){

    int N = graph.N;
    int M = graph.M;
    vector<vector<int>> G = graph.G;

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

    cout << "最短経路の道筋: " << path << endl;
}

//
// Graphs/i.txt を入力データとして受け取ったグラフをBFSで探索するのに要した時間を計測。
//
GraphData measureBFS(int i){
    string filename = "Graphs/" + to_string(i) + ".txt";
    
    if(filesystem::exists(filename)){
        auto start = chrono::high_resolution_clock::now();

        GraphData graph = input(filename);
        BFS(graph);

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cout << i << ": 計測時間(ms): " << duration.count() << endl;
        graph.BFSduration = duration.count();

        return graph;
    }else{
        return {0, 0, std::vector<std::vector<int>>()};
    }
}

//
// 結果をcsvファイルを出力する
//
int output(){
    string filename = "result.csv";
    // ofstream ofs(filename);
    // ofs << "頂点の数,辺の数,計算時間(ms)\n";
    for(GraphData g : graphs){
        cout << g.N << "," << g.M << "," << g.BFSduration << "," << g.DFSduration << endl;
    }

    return 0;
}

//
// main関数
//
int main()
{
    int number = 300000;

    for(int i = 1;i <= number; i++){
        GraphData graph = measureBFS(i);
        if(graph.N != 0) graphs.push_back(graph);
    }

    output();
    return 0;
}