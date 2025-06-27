#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <fstream>
#include <chrono>
#include <filesystem>
using namespace std;

struct GraphData
{
    int N; // 頂点の数
    int M; // 辺の数
    vector<vector<int>> G; //隣接リスト
    int searchDuration = -1; // 探索に要した時間
    bool isReachable = false; // ゴールできるかどうか
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
// 経路を復元して出力する関数
//
string rebuildPath(int start, int goal, vector<int> from){
    string path = "";
    int current = goal;
    path = to_string(current) + "<-";
    while(true){
        path += to_string(from[current]);
        current = from[current];
        if (current == start || current == -1) break;
        path += "<-";
    }

    return path;
}

//
// 幅優先探索
// 最短経路を求めている
// 辿った経路をstring型で表示させることも可能
//
bool BFS(GraphData graph){

    int N = graph.N;
    int M = graph.M;
    vector<vector<int>> G = graph.G;

    vector<int64_t> ways(N + 1, 0);  // 最短での到着経路の本数
    vector<int64_t> dist(N + 1, -1); // 最短距離
    vector<int> from(N+1, -1); // 最短経路を辿る用

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

    if(ways[N] == 0) return false;
    cout << "最短経路の道筋: " << rebuildPath(1, N, from) << endl;
    return true;
}

//
// # 深さ優先探索
// explore(int N, bool flag)
// 再帰的に調べるための関数。
// flagは経路を辿る用
//
bool DFS(GraphData& graph){
    int N = graph.N;
    int M = graph.M;
    const vector<vector<int>> G = graph.G;

    vector<int64_t> dist(N + 1, -1); // 最短距離
    vector<int> from(N+1, -1); // 最短経路を辿る用

    stack<int> st;

    dist[1] = 0;
    st.push(1);

    while (!st.empty())
    {
        int v = st.top();
        st.pop();

        for (int nv : G[v])
        {
            if (dist[nv] == -1)
            {
                dist[nv] = dist[v] + 1;
                from[nv] = v;
                st.push(nv);
            }
        }
    }

    if(dist[N] == -1) return false;
    // cout << dist[N] << endl;
    // cout << "最短経路の道筋: " << rebuildPath(1, N, from) << endl;
    return true;
}

//
// Graphs/i.txt を入力データとして受け取ったグラフをBFSで探索するのに要した時間を計測。
//
GraphData measure(int i){
    string filename = "Graphs/" + to_string(i) + ".txt";
    
    if(filesystem::exists(filename)){
        auto start = chrono::high_resolution_clock::now();

        GraphData graph = input(filename);
        
        if(DFS(graph)){
            graph.isReachable = true;
            cout << "スタートからゴールに辿り着けた" << endl;
        }else{
            graph.isReachable = false;
            cout << "スタートからゴールに辿り着けなかった" << endl;
        }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cout << i << ": 計測時間(ms): " << duration.count() << endl;
        graph.searchDuration = duration.count();

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
    ofstream ofs(filename);
    ofs << "Vertex,Edge,Time_ms\n";
    for(GraphData g : graphs){
        ofs << g.N << "," << g.M << "," << g.searchDuration << "\n";
    }

    ofs.close();

    return 0;
}

//
// main関数
//
int main()
{
    /*
    int number = 300000;

    for(int i = 1;i <= number; i++){
        GraphData graph = measureBFS(i);
        if(graph.N != 0) graphs.push_back(graph);
    }

    output();
    */

    for(int i = 1;i <= 150000; i++){
        GraphData graph = measure(i);
        if(graph.N != 0) {
            graphs.push_back(graph);
        }
    }


    return 0;

}