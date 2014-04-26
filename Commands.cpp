/* 
 * File:   Commands.cpp
 * Authors: Andy Nack & Aliceann Talley
 * UTK CS560 PA 3
 * 
 * Created on April 12, 2014, 2:50 PM
 */

#include <iostream>
#include <cstring>
#include <fstream>
#include "index.h"
#include "Commands.h"

Commands::Commands() {
    
    FileSpacePTR = 5000; //Defining start location of datablock
    DiskSize = 100000000; //Defining size of filesystem disk
    DataBlockSize = 10000; //Defining size of datablocks
    strcpy(CurrentDir, "Root"); //intializing root as current directory
    CurrentDirPTR = FileSpacePTR; //intializing root as current directory
    std::cout<< "\nFile System Ready To Receive Commands\n\n";
}

Commands::~Commands() {
 //   std::cout<< "\nFile System No Longer Able To Receive Commands\n\n";
}

void Commands::mkfs() {

    indexnode ctrlnodes;
    directory templatedir;
    int DSize;
        
    //create raw disk
    std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    ofs.seekp(DiskSize-1);
    ofs.write("", 1); 
    std::cout<< "Disk Created And Ready For Use\n";
    DSize = ofs.tellp();
    std::cout<< "Disk Size: "<< DSize << "\n\n";
     
    //initialize Root Directory inode
    ofs.seekp(0); 
    std::cout<< "Start of Inode Block: "<< ofs.tellp()<< "\n";
    ctrlnodes.id = 1;
    ctrlnodes.valid = 1;
    ctrlnodes.linkcount = 1;
    ctrlnodes.size = sizeof(templatedir);
    ctrlnodes.fptr_a = FileSpacePTR; 
    ofs.write((char*)&ctrlnodes,sizeof(ctrlnodes));

    //check that writing inode did not lengthen the file and identify the current directory
    ofs.seekp(0, ofs.end);
    std::cout<< "Disk Size: "<< ofs.tellp()<< "\n\n";
    std::cout<< "Available Disk Space for Data: "<< (DiskSize - FileSpacePTR) << "\n\n";
    std::cout<< "The Current Directory is: "<< CurrentDir << "\n"<< "mkfs() Complete\n\n";
}

void Commands::open(std::string filename, std::string fg){
    
    bool success = false;
    bool success2 = false;
    int i = 1;
    int datablock = FileSpacePTR;
    directory templatdr;
    indexnode ctrlnodes;
    
    //convert inputed strings to char arrays to match the format used in the filesystem
    char file_name[20];
    strcpy(file_name, filename.c_str());
    strcpy(Flag, fg.c_str());
    std::cout<<"Searching for " << file_name<<"\n";
        
    //open filesystem "disk" for reading and writing
    std::fstream fp ("filesystem.dat", std::ios::in|std::ios::out);
    
    //go to current directory
    fp.seekg(CurrentDirPTR);
    
    //Search for requested file to open
    while(fp.read((char*)&templatdr, sizeof(templatdr)) && fp.tellg() < (CurrentDirPTR + DataBlockSize))
    {
//*****add code to abort opening a file that is already open or opening multiple files a one time******        
        if(strcmp(templatdr.dfname, file_name)== 0){
            CurrentFileFD = templatdr.dfid;
            success = true;
            if(templatdr.dir == 1){
                std::cout<< "This is a Directory\n";
//*****add code to abort opening of a directory******                
            }else {
                std::cout<< "File Existed\n";
            }
        }
    }
    
    //if not found and not intended for read only access, create file
    if (!success && (strcmp(Flag, "r") != 0)){
        std::cout<<"Creating " << file_name<<"\n";
        
        //find unused data block
        fp.seekg(0);
        while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && fp.tellg() < FileSpacePTR)
        {
            if(ctrlnodes.valid == 1){               
                if (ctrlnodes.fptr_a == datablock && datablock < DiskSize){
                    datablock += DataBlockSize;
                    fp.seekg(0);
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
                
//*****add code to deal with full disk******
                
            }
        }
        std::cout<< "The next available file block is: "<< datablock << "\n";
        
        //find unused inode and initialize
        fp.seekg(0);
        while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (fp.tellg() < FileSpacePTR))
        {
            if(ctrlnodes.valid == 0){               
                indexnode tmp = ctrlnodes;
                fp.seekp(-sizeof(tmp),std::ios::cur);
                tmp.fptr_a = datablock;
                tmp.valid = 1;
                tmp.id = i;
                tmp.linkcount = 1;
                tmp.size = 0;
                CurrentFileFD = tmp.id;
                fp.write((char*)&tmp, sizeof(tmp));
                success = true; 
                break;
            }
            i++;
//*****add code to deal with full control block******            
        }
        
        //find blank entry in name table and initialize
        fp.seekg(CurrentDirPTR);
        while(fp.read((char*)&templatdr,sizeof(templatdr)) && (fp.tellg() < (CurrentDirPTR + DataBlockSize)))
        {
            if(strcmp(templatdr.dfname, "")== 0){
                directory tmp = templatdr;
                strcpy(tmp.dfname, file_name);
                tmp.dfid = CurrentFileFD;
                tmp.dir = 0;
                fp.seekp(-sizeof(tmp),std::ios::cur);
                fp.write((char*)&tmp, sizeof(tmp));
                std::cout<< tmp.dfname << " added to "<< CurrentDir << "\n";
                success2 = true; 
                break;
            }
        }
        if (!success2){
            std::cout<<"\n****Error: " << file_name << " not added to "<< CurrentDir << " ****\n";
        }
    }

//*****add code to deal with full disk and full control block conditions******
    if (!success && (strcmp(Flag, "r") == 0)){
        std::cout<< file_name << " does not exist for reading\n";
    }
      
    if (success && (strcmp(Flag, "w") == 0)){
        std::cout<< "File "<< file_name << ", from directory, "<< CurrentDir << ", is open for Writing and has fd = " << CurrentFileFD << ".\n";
    }else if (success && (strcmp(Flag, "r") == 0)){
        std::cout<< "File "<< file_name << ", from directory "<< CurrentDir << ", is open for Reading and has fd = " << CurrentFileFD << ".\n";
    }else if (success && (strcmp(Flag, "rw") == 0)){
        std::cout<< "File "<< file_name << ", from directory "<< CurrentDir << ", is open for Reading and Writing and has fd = " << CurrentFileFD << ".\n";
    }
    std::cout<<"open() Complete\n";
}

void Commands::read(int fd, int size){
    
    //add code
}

void Commands::write(int fd, std::string str){
   
    //add code
}

void Commands::seek(int fd, int offset){
    
    //add code
}

void Commands::close(int fd){
    
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