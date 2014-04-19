/* 
 * File:   main.cpp
 * Authors: Andy Nack & Aliceann Talley
 * UTK CS560 PA 3
 *
 * Created on April 2, 2014, 10:23 PM
 */

#include <cstdlib>
#include <stdio.h>
#include "index.h"
#include "Commands.h"

using namespace std;

int main(int argc, char** argv) {
    
    Commands customfilesys; //test
    customfilesys.mkfs(); //test
    customfilesys.open("test.txt", "rw");
    
    return 0;
}