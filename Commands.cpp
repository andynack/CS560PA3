/* 
 * File:   Commands.cpp
 * Authors: Andy Nack & Aliceann Talley
 * UTK CS560 PA 3
 * 
 * Created on April 12, 2014, 2:50 PM
 */

#include "Commands.h"
#include "index.h"

#include <iostream>


Commands::Commands() {
 /*   std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    std::ifstream ifs("filesystem.dat", std::ios::binary | std::ios::in);*/
    int ctrnodeoffset = 5000;
    int filespaceoffset = 10000;
}

Commands::~Commands() {
}

void Commands::mkfs() {
    //initialize data structures
    nametable ctrnames;
    indexnode ctrnode;
        
    //create raw disk
    std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    ofs.seekp((100<<20)-1);
    ofs.write("", 1); 
    std::cout<< "\nDisk Created";
      
    // create 5000 entry name table
    ofs.seekp(std::ios_base::beg);
    for (int x = 0; x < 5000; x++) {
        ctrnames.filename = "";
        ctrnames.id = 0;
        ofs.write((char*)&ctrnames,sizeof(ctrnames));
    }
    std::cout<< "\nName Table Initialized";
    
    //define offset for the beginning of the inode block
    //ctrnodeoffset = ofs.tellp();
    ofs.seekp(ctrnodeoffset);
    
    //create 1000 inodes to initialize inode block
    for (int j = 0; j < 1000; j++) {
        ctrnode.id = j;
        ctrnode.size = 0;
        ctrnode.tdcreate = 0;
        ctrnode.tdmodify = 0;
        ctrnode.linkcount = 0;
        ctrnode.fptr_a = NULL;
        ctrnode.fptr_b = NULL;
        ctrnode.fptr_c = NULL;
        ctrnode.fptr_d = NULL;
        ctrnode.fptr_e = NULL;
        ctrnode.fptr_f = NULL;
        ctrnode.fptr_g = NULL;
        ctrnode.fptr_h = NULL;
        ctrnode.fptr_i = NULL;
        ctrnode.fptr_j = NULL;
        ctrnode.fptr_k = NULL;
        ctrnode.fptr_l = NULL;
        ctrnode.fptr_m = NULL;
        ctrnode.fptr_n = NULL;
        ctrnode.fptr_o = NULL;
        ofs.write((char*)&ctrnode,sizeof(ctrnode));    
    }
    std::cout<< "\nControl Block Initialized\n";
}

void Commands::open(std::string filename, std::string flag){
    std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    ofs.seekp(std::ios_base::beg);
    //add code
    std::cout<< "\nFile "<< filename << " open.\n";
}

void Commands::read(int fd, int size){
    std::ifstream ifs("filesystem.dat", std::ios::binary | std::ios::in);
    //ifs.seekp(std::ios_base::beg);
    //add code
}

void Commands::write(int fd, std::string str){
    std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    ofs.seekp(std::ios_base::beg);
    //add code
}

void Commands::seek(int fd, int offset){
    std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    ofs.seekp(std::ios_base::beg);
    //add code
}

void Commands::close(int fd){
    std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    ofs.seekp(std::ios_base::beg);
    //add code
}

void Commands::mkdir(std::string dirname){
    //add code
}

void Commands::rmdir(std::string dirname){
    //add code
}

void Commands::cd(std::string dirname){
    //add code
}

void Commands::link(std::string src, std::string dest){
    //add code
}

void Commands::unlink(std::string name){
    //add code
}

void Commands::stat(std::string name){
    //add code
}

void Commands::ls(){
    //add code
}

void Commands::cat(std::string filename){
    //add code
}

void Commands::cp(std::string src, std::string dest){
  
}

void Commands::tree(){
    //add code
}

void Commands::import(std::string src, std::string dest){
    //add code
}

void Commands::fexport(std::string src, std::string dest){
    //add code
}