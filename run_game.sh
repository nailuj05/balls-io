#!/bin/bash

cc game/game.c -g -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o game/game
./game/game
