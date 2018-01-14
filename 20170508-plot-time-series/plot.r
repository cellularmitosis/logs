#!/usr/bin/Rscript

# sudo apt-get install r-base

data <- read.csv("temp.csv", header=T)
data
z <- strptime(paste(), "%Y-%m-%d %H:%M:%OS")
dates = as.POSIXct(strptime(
