/* 
 * File:   index.h
 * Authors: Andy Nack & Aliceann Talley
 * UTK CS560 PA 3
 *
 * Created on April 6, 2014, 9:39 PM
 */

#ifndef INDEX_H
#define	INDEX_H

struct directory
{
    char dfname[20];
    int dir;
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
    
    //file block locations (block size is 1KB, Max File size is 10KB)
    int fptr_a; //1st direct pointer
    int fptr_b; //2nd direct pointer
    int fptr_c; //3rd direct pointer
    int fptr_d; //4th direct pointer
    int fptr_e; //5th direct pointer
    int fptr_f; //6th direct pointer
    int fptr_g; //7th direct pointer
    int fptr_h; //8th direct pointer
    int fptr_i; //9th direct pointer
    int fptr_j; //10th direct pointer
    
};
int FileSpacePTR; //Defines the end of the inode section and the beginning of the data section
int DataBlockSize; //defines the size of the file system data block
int DiskSize; //defines the size of the simulated disk of the file system
bool CurrentFileStatus; //Status of whether the current file is open (true) or closed (false)
char Flag[2]; //identifies the mode of the open file: r, w, or rw
char CurrentFile[20]; //Name of current file
int CurrentFileOffset; //Offset # of bytes from beginning of current file
int CurrentFileFD; //inode id# for opened file
int CurrentFileFDPTR; //pointer to inode for opened file
int CurrentFileSize; //Size of current file
int CurrentFilePTR; //Pointer for current file
int CurrentFilePTR_B; //2nd Pointer for current file
int CurrentFilePTR_C; //3rd Pointer for current file
int CurrentFilePTR_D; //4th Pointer for current file
int CurrentFilePTR_E; //5th Pointer for current file
int CurrentFilePTR_F; //6th Pointer for current file
int CurrentFilePTR_G; //7th Pointer for current file
int CurrentFilePTR_H; //8th Pointer for current file
int CurrentFilePTR_I; //9th Pointer for current file
int CurrentFilePTR_J; //10th Pointer for current file
char CurrentDir[20]; //Name of current directory
int CurrentDirOffset; //Offset # of bytes from beginning of current directory
int CurrentDirID; //inode id# for opened file
int CurrentDirSize; //Size of current directory
int CurrentDirPTR; //Pointer for current directory
int CurrentDirPTR_B; //2nd Pointer for current directory
int CurrentDirPTR_C; //3rd Pointer for current directory
int CurrentDirPTR_D; //4th Pointer for current directory
int CurrentDirPTR_E; //5th Pointer for current directory
int CurrentDirPTR_F; //6th Pointer for current directory
int CurrentDirPTR_G; //7th Pointer for current directory
int CurrentDirPTR_H; //8th Pointer for current directory
int CurrentDirPTR_I; //9th Pointer for current directory
int CurrentDirPTR_J; //10th Pointer for current directory

#endif	/* INDEX_H */