/* 
 * File:   Commands.cpp
 * Authors: Andy Nack & Aliceann Talley
 * UTK CS560 PA 3
 * 
 * Created on April 12, 2014, 2:50 PM
 */

#include "Commands.h"
#include <fstream>


Commands::Commands() {
}

Commands::~Commands() {
}

void Commands::mkfs() {
    std::ofstream ofs("filesystem", std::ios::binary | std::ios::out);
    ofs.seekp((100<<20) - 1);
    ofs.write("", 1); 
    //add writing of control block inodes
}

void Commands::open(std::string filename, std::string flag){
    //add code
}

void Commands::read(std::string fd, int size){
    //add code
}

void Commands::write(std::string fd, std::string str){
    //add code
}

void Commands::seek(std::string fd, int offset){
    //add code
}

void Commands::close(std::string fd){
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
    //add code
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