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

class Commands {
public:
    Commands(); //constructor
    virtual ~Commands(); //de-constructor
    void mkfs();
    void open(std::string filename, std::string flag);
    void read(std::string fd, int size);
    void write(std::string fd, std::string str);
    void seek(std::string fd, int offset);
    void close(std::string fd);
    void mkdir(std::string dirname);
    void rmdir(std::string dirname);
    void cd(std::string dirname);
    void link(std::string src, std::string dest);
    void unlink(std::string name);
    void stat(std::string name);
    void ls();
    void cat (std::string filename);
    void cp(std::string src, std::string dest);
    void tree();
    void import(std::string src, std::string dest);
    void fexport(std::string src, std::string dest);
    
private:

};

#endif	/* COMMANDS_H */

