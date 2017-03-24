#!/bin/bash
gcc -w -static -O3 -lpthread -pthread src/*.c -o loader
