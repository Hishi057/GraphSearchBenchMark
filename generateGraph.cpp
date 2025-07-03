#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>  // std::shuffle
#include <numeric>    // std::iota
#include <string>     // std::string
#include <filesystem> // std::filesystem::create_directory (C++17以降)
#include <set>        // 辺の重複チェック用

/**
 * @brief 指定された頂点数Nと辺の数Mを持つ連結グラフを生成し、ファイルに出力します。
 * 頂点は1からNまでで表現されます。
 * 出力ファイルは "Graphs/(Nの頂点数)_(Mの辺数).txt" の形式で保存されます。
 *
 * @param N 最終的な頂点の数 (1以上の整数)
 * @param M 最終的な辺の数 (N-1以上、N*(N-1)/2以下の整数)
 * @return true グラフの生成とファイル出力が成功した場合
 * @return false グラフの生成条件が満たされない場合 (Mが小さすぎる、大きすぎるなど)
 */
bool generateConnectedGraph(int N, int M)
{
    // 連結グラフの条件チェック
    // 連結グラフであるためには、N-1本以上の辺が必要
    if (M < N - 1)
    {
        std::cerr << "エラー: 連結グラフを生成するには、辺の数 M は N-1 以上である必要があります。" << std::endl;
        return false;
    }
    // 辺の最大数 N*(N-1)/2 を超える場合はエラー
    if (M > (long long)N * (N - 1) / 2)
    {
        std::cerr << "エラー: 辺の数 M が最大辺数 N*(N-1)/2 を超えています。" << std::endl;
        return false;
    }
    if (N <= 0)
    {
        std::cerr << "エラー: 頂点の数 N は1以上である必要があります。" << std::endl;
        return false;
    }

    // N=1, M=0 の特殊ケース
    if (N == 1 && M == 0)
    {
        std::string directory_name = "Graphs";
        if (!std::filesystem::exists(directory_name))
        {
            if (!std::filesystem::create_directory(directory_name))
            {
                std::cerr << "エラー: ディレクトリ '" << directory_name << "' を作成できませんでした。" << std::endl;
                return false;
            }
        }
        std::string filename = directory_name + "/" + std::to_string(N) + "_" + std::to_string(M) + ".txt";
        std::ofstream ofs(filename);
        if (!ofs)
        {
            std::cerr << "エラー: ファイルを開けませんでした。" << std::endl;
            return false;
        }
        ofs << N << " " << M << std::endl;
        ofs.close();
        std::cout << "グラフがファイル '" << filename << "' に出力されました。" << std::endl;
        std::cout << "頂点の数: " << N << std::endl;
        std::cout << "辺の数: " << M << std::endl;
        return true;
    }
    else if (N == 1 && M != 0)
    {
        std::cerr << "エラー: 頂点が1つの場合、辺の数 M は0である必要があります。" << std::endl;
        return false;
    }

    // 乱数生成器の初期化
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::pair<int, int>> edges;       // 辺を格納するベクタ
    std::set<std::pair<int, int>> existing_edges; // 辺の重複を避けるためのセット

    // 1. 連結性を保証するために、まずパスグラフを生成 (N-1本の辺)
    // 頂点1-2-3-...-N のようにパスを張る
    for (int i = 1; i < N; ++i)
    {
        edges.push_back({i, i + 1});
        existing_edges.insert({i, i + 1}); // 順序を考慮して挿入 (小さい方をfirst)
    }

    // 残りの辺の数を計算
    int remaining_edges = M - (N - 1);

    // 2. 残りの辺をランダムに追加
    while (remaining_edges > 0)
    {
        int u = std::uniform_int_distribution<>(1, N)(gen); // 1からNまでのランダムな頂点U
        int v = std::uniform_int_distribution<>(1, N)(gen); // 1からNまでのランダムな頂点V

        // 同じ頂点同士の辺や既に存在する辺はスキップ
        if (u == v)
            continue;

        // 辺の順序を正規化 (常に小さい方をfirstにする)
        if (u > v)
            std::swap(u, v);

        // 既に存在する辺でなければ追加
        if (existing_edges.find({u, v}) == existing_edges.end())
        {
            edges.push_back({u, v});
            existing_edges.insert({u, v});
            remaining_edges--;
        }
    }

    // ディレクトリとファイル名の設定
    std::string directory_name = "Graphs";
    // フォルダが存在しない場合は作成 (C++17のstd::filesystemを使用)
    if (!std::filesystem::exists(directory_name))
    {
        if (!std::filesystem::create_directory(directory_name))
        {
            std::cerr << "エラー: ディレクトリ '" << directory_name << "' を作成できませんでした。" << std::endl;
            return false;
        }
    }

    std::string filename = directory_name + "/" + std::to_string(N) + ".txt";
    std::ofstream ofs(filename);

    if (!ofs)
    {
        std::cerr << "エラー: ファイルを開けませんでした。" << std::endl;
        return false;
    }

    // グラフ情報をファイルに書き込み
    ofs << N << " " << M << std::endl;
    for (const auto &edge : edges)
    {
        ofs << edge.first << " " << edge.second << std::endl;
    }

    ofs.close();

    std::cout << "グラフがファイル '" << filename << "' に出力されました。" << std::endl;
    std::cout << "頂点の数: " << N << std::endl;
    std::cout << "辺の数: " << M << std::endl;

    return true;
}

// generateGraph 関数
// 必ずしも連結でなくてもよいグラフを生成し、指定されたファイルに出力します。
// N: 頂点数 (1以上)
// M: 辺数 (0以上、最大 N*(N-1)/2)
bool generateGraph(int N, int M)
{
    // エラーチェック
    if (N <= 0)
    {
        std::cerr << "エラー: 頂点の数 N は1以上である必要があります。" << std::endl;
        return false;
    }
    if (M < 0)
    {
        std::cerr << "エラー: 辺の数 M は0以上である必要があります。" << std::endl;
        return false;
    }
    // 辺の最大数 N*(N-1)/2 を超える場合はエラー
    // (N=1の場合、N*(N-1)/2 は0になるので、M=0はOK)
    if (M > (long long)N * (N - 1) / 2)
    {
        std::cerr << "エラー: 辺の数 M が最大辺数 N*(N-1)/2 を超えています。" << std::endl;
        return false;
    }

    // N=1 の特殊ケース
    if (N == 1)
    {
        if (M != 0)
        {
            std::cerr << "エラー: 頂点が1つの場合、辺の数 M は0である必要があります。" << std::endl;
            return false;
        }
        // N=1, M=0 のグラフを生成
        std::string directory_name = "Graphs";
        if (!std::filesystem::exists(directory_name))
        {
            if (!std::filesystem::create_directory(directory_name))
            {
                std::cerr << "エラー: ディレクトリ '" << directory_name << "' を作成できませんでした。" << std::endl;
                return false;
            }
        }
        std::string filename = directory_name + "/" + std::to_string(N) + ".txt";
        std::ofstream ofs(filename);
        if (!ofs)
        {
            std::cerr << "エラー: ファイル '" << filename << "' を開けませんでした。" << std::endl;
            return false;
        }
        ofs << N << " " << M << std::endl;
        ofs.close();
        std::cout << "グラフがファイル '" << filename << "' に出力されました。" << std::endl;
        std::cout << "頂点の数: " << N << std::endl;
        std::cout << "辺の数: " << M << std::endl;
        return true;
    }

    // 乱数生成器の初期化
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::pair<int, int>> edges;       // 辺を格納するベクタ
    std::set<std::pair<int, int>> existing_edges; // 辺の重複を避けるためのセット

    // M本の辺をランダムに追加
    int current_edges_count = 0;
    while (current_edges_count < M)
    {
        int u = std::uniform_int_distribution<>(1, N)(gen); // 1からNまでのランダムな頂点U
        int v = std::uniform_int_distribution<>(1, N)(gen); // 1からNまでのランダムな頂点V

        // 同じ頂点同士の辺や既に存在する辺はスキップ
        if (u == v)
            continue;

        // 辺の順序を正規化 (常に小さい方をfirstにする)
        if (u > v)
            std::swap(u, v);

        // 既に存在する辺でなければ追加
        if (existing_edges.find({u, v}) == existing_edges.end())
        {
            edges.push_back({u, v});
            existing_edges.insert({u, v});
            current_edges_count++; // 辺の数をインクリメント
        }
        // 念のため、最大辺数に達していないかチェック (非常に稀だが、ループが続く可能性を考慮)
        if (current_edges_count >= (long long)N * (N - 1) / 2 && current_edges_count < M)
        {
            std::cerr << "警告: 生成可能な最大辺数に達しました。要求された辺数 M=" << M << " は達成できませんでした。実際の辺数: " << current_edges_count << std::endl;
            break; // 最大辺数に達したらループを抜ける
        }
    }

    // ディレクトリとファイル名の設定
    std::string directory_name = "Graphs";
    // フォルダが存在しない場合は作成 (C++17のstd::filesystemを使用)
    if (!std::filesystem::exists(directory_name))
    {
        if (!std::filesystem::create_directory(directory_name))
        {
            std::cerr << "エラー: ディレクトリ '" << directory_name << "' を作成できませんでした。" << std::endl;
            return false;
        }
    }

    // ファイル名にNとMの両方を含めるように変更
    std::string filename = directory_name + "/" + std::to_string(N) + ".txt";
    std::ofstream ofs(filename);

    if (!ofs)
    {
        std::cerr << "エラー: ファイル '" << filename << "' を開けませんでした。" << std::endl;
        return false;
    }

    // グラフ情報をファイルに書き込み
    ofs << N << " " << current_edges_count << std::endl; // 実際に生成された辺の数を出力
    for (const auto &edge : edges)
    {
        ofs << edge.first << " " << edge.second << std::endl;
    }

    ofs.close();

    std::cout << "グラフがファイル '" << filename << "' に出力されました。" << std::endl;
    std::cout << "頂点の数: " << N << std::endl;
    std::cout << "辺の数: " << current_edges_count << std::endl; // 実際に生成された辺の数を出力

    return true;
}

// 関数の動作を確認するための main 関数
int main() {
    std::cout << "グラフを生成します。\n";

    for(int i = 1; i<= 100; i++){
        int vertex = i * 5000;
        int N = vertex;
        // int M = 0.1 * N * log(N);
        int M = 2 * N;
        if(generateGraph(N, M)){
            std::cout << "頂点の数: "<< vertex <<" のグラフ生成に成功しました。\n";
        }
    }

    return 0;
}