di <- read.table("result.csv", sep=",", header=T)
head(di)
plot(
    di$Vertex, 
    di$Time_ms, 
    xlab = "The number of Vertex",
    ylab = "The duration of a search(ms)",
    panel.first=grid(8,8),
    main = "The result of BFS"
    )