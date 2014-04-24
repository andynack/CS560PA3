/* 
 * File:   Commands.h
 * Authors: Andy Nack & Aliceann Talley
 * UTK CS560 PA 3
 *
 * Created on April 12, 2014, 2:50 PM
 */

#ifndef COMMANDS_H
#define	COMMANDS_H

#include <string>
#include <fstream>
#include "index.h"

class Commands {
public:
    Commands(); //constructor
    virtual ~Commands(); //de-constructor
    void mkfs();
    void open(char filename[20], char fg[5]);
    void read(int fd, int size);
    void write(int fd, char str[100]);
    void seek(int fd, int offset);
    void close(int fd);
    void mkdir(char dirname[20]);
    void rmdir(char dirname[20]);
    void cd(char dirname[20]);
    void link(char src[20], char dest[20]);
    void unlink(char name[20]);
    void stat(char name[20]);
    void ls();
    void cat(char filename[20]);
    void cp(char src[20], char dest[20]);
    void tree();
    void import(char src[20], char dest[20]);
    void fexport(char src[20], char dest[20]);
    
private:
    int INodePTR;
    int FileSpacePTR;
    int OpenForWritePTR;
    int OpenForReadPTR;
    int CurrentDirPTR;
    int CurrentFileFD;
    char Flag[5];
    char CurrentDir[20];
};

#endif	/* COMMANDS_H */

