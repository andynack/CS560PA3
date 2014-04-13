/* 
 * File:   index.h
 * Authors: Andy Nack & Aliceann Talley
 * UTK CS560 PA 3
 *
 * Created on April 6, 2014, 9:39 PM
 */

#ifndef INDEX_H
#define	INDEX_H

#include <stdio.h>

struct indexnode
{ 
    //file attributes (all files are type .txt)
    int id; //inode identifier
    int size; //file size
    int tdcreate; //time & date of original creation
    int tdmodify; //time & date of last modification
    
    //file block locations (??block size is 1KB, 1024 Bytes??)
    FILE *fptr; //1st direct pointer
 /*   FILE *b; //2nd direct pointer
    FILE *c; //3rd direct pointer
    FILE *d; //4th direct pointer
    FILE *e; //5th direct pointer
    FILE *f; //6th direct pointer
    FILE *g; //7th direct pointer
    FILE *h; //8th direct pointer
    FILE *i; //9th direct pointer
    FILE *j; //10th direct pointer
    FILE *k; //11th direct pointer
    FILE *l; //12th direct pointer
    FILE *m; //indirect pointer
    FILE *n; //double indirect pointer
    FILE *o; //triple indirect pointer
*/    
};

#endif	/* INDEX_H */