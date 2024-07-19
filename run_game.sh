#!/bin/bash

cc game/game.c -g -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o game/game
./game/game
