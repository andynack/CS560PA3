/* 
 * File:   index.h
 * Authors: Andy Nack & Aliceann Talley
 * UTK CS560 PA 3
 *
 * Created on April 6, 2014, 9:39 PM
 */

#ifndef INDEX_H
#define	INDEX_H

#include <string>

struct nametable
{
    char fname[20];
    int ntid;
};

struct directory
{
    char dfname[20];
    int dfid;
};

struct indexnode
{ 
    //file attributes (all files are type .txt)
    int id; //inode identifier
    int valid;
    int size; //file size
    int tdcreate; //time & date of original creation
    int tdmodify; //time & date of last modification
    int linkcount; //number of names linked to this file
    
    //file block locations (block size is 10KB)
    int fptr_a; //1st direct pointer
 /*   int fptr_b; //2nd direct pointer
    int fptr_c; //3rd direct pointer
    int fptr_d; //4th direct pointer
    int fptr_e; //5th direct pointer
    int fptr_f; //6th direct pointer
    int fptr_g; //7th direct pointer
    int fptr_h; //8th direct pointer
    int fptr_i; //9th direct pointer
    int fptr_j; //10th direct pointer
    int fptr_k; //11th direct pointer
    int fptr_l; //12th direct pointer
    int fptr_m; //indirect pointer
    int fptr_n; //double indirect pointer
    int fptr_o; //triple indirect pointer*/
    
};

#endif	/* INDEX_H */