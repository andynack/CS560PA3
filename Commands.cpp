/* 
 * File:   Commands.cpp
 * Authors: Andy Nack & Aliceann Talley
 * UTK CS560 PA 3
 * 
 * Created on April 12, 2014, 2:50 PM
 */

#include "Commands.h"

#include <iostream>
#include <cstring>


Commands::Commands() {
 /*   std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    std::ifstream ifs("filesystem.dat", std::ios::binary | std::ios::in);*/
    INodePTR = 1000;
    FileSpacePTR = 2000;
    std::cout<< "\nFile System Ready To Receive Commands\n\n";
}

Commands::~Commands() {
    std::cout<< "\nFile System No Longer Able To Receive Commands\n\n";
}

void Commands::mkfs() {
    nametable ctrlnames;
    indexnode ctrlnodes;
        
    //create raw disk
    std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    ofs.seekp(100000000-1);
    ofs.write("", 1); 
    std::cout<< "File Size: "<< ofs.tellp()<< "\n";
    ofs.close(); 
    std::cout<< "\nDisk Created And Ready For Use";
/*      
    // create 5000 entry name table
    ofs.seekp(std::ios_base::beg);
    for (int x = 0; x < 5000; x++) {
        ctrnames.filename = "";
        ctrnames.id = 0;
        ofs.write((char*)&ctrnames,sizeof(ctrnames));
    }
    std::cout<< "\nName Table Initialized";
    
    //define offset for the beginning of the inode block
    //ctrnodeoffset = ofs.tellp();*/
    ofs.seekp(INodePTR, ofs.beg);
    
    //create 1000 inodes to initialize inode block
    for (int j = 0; j < 1000; j++) {
        ctrlnodes.id = j;
        ctrlnodes.valid = 0;
        ctrlnodes.size = 0;
        ctrlnodes.tdcreate = 0;
        ctrlnodes.tdmodify = 0;
        ctrlnodes.linkcount = 0;
        ctrlnodes.fptr_a = 0;
    /*    ctrlnodes.fptr_b = 0;
        ctrlnodes.fptr_c = 0;
        ctrlnodes.fptr_d = 0;
        ctrlnodes.fptr_e = 0;
        ctrlnodes.fptr_f = 0;
        ctrlnodes.fptr_g = 0;
        ctrlnodes.fptr_h = 0;
        ctrlnodes.fptr_i = 0;
        ctrlnodes.fptr_j = 0;
        ctrlnodes.fptr_k = 0;
        ctrlnodes.fptr_l = 0;
        ctrlnodes.fptr_m = 0;
        ctrlnodes.fptr_n = 0;
        ctrlnodes.fptr_o = 0;*/
        ofs.write((char*)&ctrlnodes,sizeof(ctrlnodes));    
    }
    std::cout<< "\nControl Block Initialized\n";
}

void Commands::open(std::string filename, std::string fg){
    bool success = false;
    int datablock = FileSpacePTR;
    nametable ctrlnames;
    indexnode ctrlnodes;
    std::fstream fp;
    fp.open("filesystem.dat", std::ios::in|std::ios::out);
    while(fp.read((char*)&ctrlnames, sizeof(ctrlnames)) && fp.tellg() < INodePTR)
    {
        if(ctrlnames.fname == filename){
            CurrentFileFD = ctrlnames.ntid;
            success = true; 
        }
    }
    if (!success){
        //find unused data block
        fp.seekg(INodePTR, fp.beg);
        while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && fp.tellg() < FileSpacePTR)
        {
            if(ctrlnodes.valid == 1){               
                if (ctrlnodes.fptr_a == datablock){
                    datablock += 1024;
                    fp.beg;
                }/*else if(ctrlnodes.fptr_b == datablock){
                    datablock += 1024;
                    fp.beg;
                }else if(ctrlnodes.fptr_c == datablock){
                    datablock += 1024;
                    fp.beg;
                }else if(ctrlnodes.fptr_d == datablock){
                    datablock += 1024;
                    fp.beg;
                }else if(ctrlnodes.fptr_e == datablock){
                    datablock += 1024;
                    fp.beg;
                }else if(ctrlnodes.fptr_f == datablock){
                    datablock += 1024;
                    fp.beg;
                }else if(ctrlnodes.fptr_g == datablock){
                    datablock += 1024;
                    fp.beg;
                }else if(ctrlnodes.fptr_h == datablock){
                    datablock += 1024;
                    fp.beg;
                }else if(ctrlnodes.fptr_i == datablock){
                    datablock += 1024;
                    fp.beg;
                }else if(ctrlnodes.fptr_j == datablock){
                    datablock += 1024;
                    fp.beg;
                }else if(ctrlnodes.fptr_k == datablock){
                    datablock += 1024;
                    fp.beg;
                }else if(ctrlnodes.fptr_l == datablock){
                    datablock += 1024;
                    fp.beg;
                } */
            }
        }
        
        //find unused inode and initialize
        fp.seekg(INodePTR, fp.beg);
        while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && fp.tellg() < FileSpacePTR)
        {
            if(ctrlnodes.valid == 0){               
                indexnode tmp = ctrlnodes;                
                CurrentFileFD = tmp.id;
                int pos=-1*sizeof(tmp);
                fp.seekp(pos,std::ios::cur);
                tmp.fptr_a = datablock;
                fp.write((char*)&tmp, sizeof(tmp));
                success = true; 
                break;
            }
        }
        //find blank entry in name table and initialize
        fp.beg;
        while(fp.read((char*)&ctrlnames,sizeof(ctrlnames)) && fp.tellg() < INodePTR)
        {
            if(ctrlnames.fname == ""){
                nametable tmp = ctrlnames;
                tmp.fname = filename;
                tmp.ntid = CurrentFileFD;
l                int pos=-1*sizeof(tmp);
                fp.seekp(pos,std::ios::cur);
                fp.write((char*)&tmp, sizeof(tmp));
                success = true; 
                break;
            }
        }
    }
      
    Flag = fg;
    if (Flag.compare("w") == 0){
        std::cout<< "\nFile "<< filename << " open for Writing and fd = " << CurrentFileFD << ".\n";
    }else if (Flag.compare("r") == 0){
        std::cout<< "\nFile "<< filename << " open for Reading and fd = " << CurrentFileFD << ".\n";
    }else if (Flag.compare("rw") == 0){
        std::cout<< "\nFile "<< filename << " open for Reading and Writing and fd = " << CurrentFileFD << ".\n";
    }
    
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