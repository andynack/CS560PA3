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
    void open(std::string filename, std::string fg);
    void read(int fd, int size);
    void write(int fd, std::string str);
    void seek(int fd, int offset);
    void close(int fd);
    void mkdir(std::string dirname);
    void rmdir(std::string dirname);
    void cd(std::string dirname);
    void link(std::string src, std::string dest);
    void unlink(std::string name);
    void stat(std::string name);
    void ls();
    void cat(std::string filename);
    void cp(std::string src, std::string dest);
    void tree();
    void import(std::string src, std::string dest);
    void fexport(std::string src, std::string dest);
    
private:
    int INodePTR;
    int FileSpacePTR;
    int OpenForWritePTR;
    int OpenForReadPTR;
    int CurrentDirPTR;
    int CurrentFileFD;
    std::string Flag;
    std::string CurrentDir;
};

#endif	/* COMMANDS_H */

