/* 
 * File:   main.cpp
 * Authors: Andy Nack & Aliceann Talley
 * UTK CS560 PA 3
 *
 * Created on April 2, 2014, 10:23 PM
 */

#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include "index.h"
#include "Commands.h"

using namespace std;

int main(int argc, char** argv) {
    
    Commands customfilesys; //test
    customfilesys.mkfs(); //test
    char file[20];
    char flg[5];
    strcpy(file, "test.txt");
    strcpy(flg, "rw");
    customfilesys.open(file, flg);
    
    return 0;
}