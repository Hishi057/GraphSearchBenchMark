# データを準備
# 左側の数値（例えば 5000, 10000 など）がデータ量、右側の数値（例えば 4, 7 など）が計測時間(ms)と仮定します。
data_points <- data.frame(
  data_amount = c(
    5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000,
    55000, 60000, 65000, 70000, 75000, 80000, 85000, 90000, 95000, 100000,
    105000, 110000, 115000, 120000, 125000, 130000, 135000, 140000, 145000, 150000,
    155000, 160000, 165000, 170000, 175000, 180000, 185000, 190000, 195000, 200000,
    205000, 210000, 215000, 220000, 225000, 230000, 235000, 240000, 245000, 250000,
    255000, 260000, 265000, 270000, 275000, 280000, 285000, 290000, 295000, 300000
  ),
  measurement_time_ms = c(
    4, 7, 11, 15, 19, 23, 27, 31, 35, 40,
    43, 48, 50, 54, 61, 67, 68, 72, 76, 79,
    84, 86, 94, 98, 102, 105, 111, 114, 127, 122,
    130, 135, 138, 145, 145, 153, 181, 161, 165, 172,
    204, 178, 185, 189, 201, 198, 203, 211, 227, 227,
    234, 235, 240, 246, 250, 258, 264, 266, 277, 281
  )
)

# ggplot2ライブラリをロード (インストールされていない場合は install.packages("ggplot2") でインストールしてください)
if (!requireNamespace("ggplot2", quietly = TRUE)) {
  install.packages("ggplot2")
}
library(ggplot2)

# PNGファイルとして出力開始
png("measurement_time_plot.png", width = 800, height = 600, res = 100) # 解像度を100dpiに設定

# グラフを作成
ggplot(data_points, aes(x = data_amount, y = measurement_time_ms)) +
  geom_line(color = "blue", size = 1) + # 線グラフ
  geom_point(color = "red", size = 2) + # 点プロット
  labs(
    title = "データ量と計測時間(ms)の関係",
    x = "The number of vertexes",
    y = "計測時間 (ms)"
  ) +
  theme_minimal() + # シンプルなテーマ
  theme(
    plot.title = element_text(hjust = 0.5, size = 16, face = "bold"),
    axis.title = element_text(size = 12),
    axis.text = element_text(size = 10)
  ) +
  scale_x_continuous(labels = scales::comma) + # x軸の数値にカンマを追加
  scale_y_continuous(labels = scales::comma)   # y軸の数値にカンマを追加

# 出力デバイスを閉じる（ファイルを保存）
dev.off()

cat("グラフが 'measurement_time_plot.png' として保存されました。\n")