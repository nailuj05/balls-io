#!/bin/bash
echo "Rebuilding Server"
gcc server/server.c -o server/server
echo "Starting Server"
./server/server
