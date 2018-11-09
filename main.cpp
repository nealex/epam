#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include "reader.cpp"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        exit(EXIT_FAILURE);
    }
    cout<< endl;
    if (reader(argv[1]))
    {
        cout << "Done!";
    }
     cout<< endl;
}