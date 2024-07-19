#!/bin/bash
echo "Rebuilding Server"
gcc server/server.c -Iinclude -o server/server
echo "Starting Server"
./server/server
