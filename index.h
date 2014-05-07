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
int CurrentDirID; //inode id# for current directory
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

int PrevDir[20]; //record of last 20 previous directories
int PrevDirPtr; //Pointer to next spot in previous directory array

char SrcDir[20]; //Name of source directory
int SrcDirID; //inode id# for source directory
int SrcDirSize; //Size of source directory
int SrcDirPTR; //Pointer for source directory
int SrcDirPTR_B; //2nd Pointer for source directory
int SrcDirPTR_C; //3rd Pointer for source directory
int SrcDirPTR_D; //4th Pointer for source directory
int SrcDirPTR_E; //5th Pointer for source directory
int SrcDirPTR_F; //6th Pointer for source directory
int SrcDirPTR_G; //7th Pointer for source directory
int SrcDirPTR_H; //8th Pointer for source directory
int SrcDirPTR_I; //9th Pointer for source directory
int SrcDirPTR_J; //10th Pointer for source directory

char DestDir[20]; //Name of destination directory
int DestDirID; //inode id# for destination directory
int DestDirSize; //Size of destination directory
int DestDirPTR; //Pointer for destination directory
int DestDirPTR_B; //2nd Pointer for destination directory
int DestDirPTR_C; //3rd Pointer for destination directory
int DestDirPTR_D; //4th Pointer for destination directory
int DestDirPTR_E; //5th Pointer for destination directory
int DestDirPTR_F; //6th Pointer for destination directory
int DestDirPTR_G; //7th Pointer for destination directory
int DestDirPTR_H; //8th Pointer for destination directory
int DestDirPTR_I; //9th Pointer for destination directory
int DestDirPTR_J; //10th Pointer for destination directory

char SrcFile[20]; //Name of source file
int SrcFileID; //inode id# for source file
int SrcFileSize; //Size of source file
int SrcFileOffset; //Offset # of bytes from beginning of source file
int SrcFilePTR; //Pointer for source file
int SrcFilePTR_B; //2nd Pointer for source file
int SrcFilePTR_C; //3rd Pointer for source file
int SrcFilePTR_D; //4th Pointer for source file
int SrcFilePTR_E; //5th Pointer for source file
int SrcFilePTR_F; //6th Pointer for source file
int SrcFilePTR_G; //7th Pointer for source file
int SrcFilePTR_H; //8th Pointer for source file
int SrcFilePTR_I; //9th Pointer for source file
int SrcFilePTR_J; //10th Pointer for source file

char DestFile[20]; //Name of destination file
int DestFileID; //inode id# for destination file
int DestFileSize; //Size of destination file
int DestFileOffset; //Offset # of bytes from beginning of destination file
int DestFilePTR; //Pointer for destination file
int DestFilePTR_B; //2nd Pointer for destination file
int DestFilePTR_C; //3rd Pointer for destination file
int DestFilePTR_D; //4th Pointer for destination file
int DestFilePTR_E; //5th Pointer for destination file
int DestFilePTR_F; //6th Pointer for destination file
int DestFilePTR_G; //7th Pointer for destination file
int DestFilePTR_H; //8th Pointer for destination file
int DestFilePTR_I; //9th Pointer for destination file
int DestFilePTR_J; //10th Pointer for destination file

//char Buff[];

#endif	/* INDEX_H */