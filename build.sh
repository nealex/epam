#!/bin/bash
clear
rm -rf epam
#rm -rf log.txt
g++ main.cpp -o epam; ./epam traf_with_proxy.txt
#traf_original.txt
#traf_with_proxy.txt 