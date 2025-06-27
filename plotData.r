di <- read.table("result.csv", sep=",", header=T)
head(di)
plot(
    di$Vertex, 
    di$BFS_Time_ms, 
    xlab = "The number of Vertex",
    ylab = "The duration of BFS",
    panel.first=grid(8,8)
    )