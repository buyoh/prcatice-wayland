#!/bin/bash

mkdir -p out
g++ -o out/simple src/simple.cpp -lwayland-client
