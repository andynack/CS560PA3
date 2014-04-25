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
 /*   std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    std::ifstream ifs("filesystem.dat", std::ios::binary | std::ios::in);*/
 //   INodePTR = 5000; //defining start of inode block
    FileSpacePTR = 5000; //defining start location of datablock
    CurrentDirPTR = FileSpacePTR;
    strcpy(CurrentDir, "Root");
    std::cout<< "\nFile System Ready To Receive Commands\n\n";
}

Commands::~Commands() {
    std::cout<< "\nFile System No Longer Able To Receive Commands\n\n";
}

void Commands::mkfs() {
//    nametable ctrlnames;
    indexnode ctrlnodes;
    directory templatedir;
    int DiskSize;
        
    //create raw disk
    std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    ofs.seekp(100000000-1);
    ofs.write("", 1); 
    std::cout<< "Disk Created And Ready For Use\n";
    DiskSize = ofs.tellp();
    std::cout<< "Disk Size: "<< DiskSize << "\n\n";
        
    ofs.seekp(0);
 /*   std::cout<< "Start of Name Table: "<< ofs.tellp()<< "\n";
    strcpy(ctrlnames.fname, CurrentDir);
    ctrlnames.ntid = 1;
    ofs.write((char*)&ctrlnames,sizeof(ctrlnames));
    
    ofs.seekp(5000);*/
    
    std::cout<< "Start of Inode Block: "<< ofs.tellp()<< "\n";
    ctrlnodes.id = 1;
    ctrlnodes.valid = 1;
    ctrlnodes.linkcount = 1;
    ctrlnodes.size = sizeof(templatedir);
    ctrlnodes.fptr_a = FileSpacePTR; 
    ofs.write((char*)&ctrlnodes,sizeof(ctrlnodes));
/*    
    ofs.seekp(ctrlnodes.fptr_a);
    std::cout<< "Start of Data Block: "<< ofs.tellp()<< "\n";
    strcpy(templatedir.dfname, "");
    templatedir.dfid = 0;
    ofs.write((char*)&templatedir,sizeof(templatedir));
    
    std::cout<< "Root Directory Initialized\n";
*/    
    ofs.seekp(0, ofs.end);
    std::cout<< "Disk Size: "<< ofs.tellp()<< "\n\n";
    std::cout<< "Available Disk Space for Data: "<< (DiskSize - FileSpacePTR) << "\n\n";
    std::cout<< "The Current Directory is: "<< CurrentDir << "\n";
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
    //ctrnodeoffset = ofs.tellp();
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
        ctrlnodes.fptr_b = 0;
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
        ctrlnodes.fptr_o = 0;
        ofs.write((char*)&ctrlnodes,sizeof(ctrlnodes));    
    }
    std::cout<< "\nControl Block Initialized\n";*/
}

void Commands::open(std::string filename, std::string fg){
    char file_name[20];
    strcpy(file_name, filename.c_str());
    bool success = false;
    int i = 1;
    int datablock = FileSpacePTR;
 //   nametable ctrlnames;
    directory templatdr;
    indexnode ctrlnodes;
    std::fstream fp ("filesystem.dat", std::ios::in|std::ios::out);
    fp.seekg(CurrentDirPTR);
    while(fp.read((char*)&templatdr, sizeof(templatdr)) && fp.tellg() < (CurrentDirPTR + 10000))
    {
        if(strcmp(templatdr.dfname, file_name)== 0){
            CurrentFileFD = templatdr.dfid;
            success = true;
            if(templatdr.dir == 0){
                std::cout<< "This is a Directory\n";
            }else {
                std::cout<< "File Existed\n";
            }
        }
    }
    if (!success){
        //find unused data block
        fp.seekg(0);
        while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && fp.tellg() < FileSpacePTR)
        {
            if(ctrlnodes.valid == 1){               
                if (ctrlnodes.fptr_a == datablock && datablock < 100000000){
                    datablock += 10000;
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
            }
        }
        std::cout<< "The next available file block is: "<< datablock << "\n";
        
        //find unused inode and initialize
        fp.seekg(0);
        while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && fp.tellg() < FileSpacePTR)
        {
          //  int i = 1;
            if(ctrlnodes.valid == 0){               
                indexnode tmp = ctrlnodes;                
        //        long pos=-1*sizeof(tmp);
                fp.seekp(-sizeof(tmp),std::ios::cur);
                tmp.fptr_a = datablock;
                tmp.valid = 1;
                tmp.id = i;
                tmp.linkcount = 1;                
                CurrentFileFD = tmp.id;
                fp.write((char*)&tmp, sizeof(tmp));
                success = true; 
                break;
            }
            i++;
        }
        //find blank entry in name table and initialize
        fp.seekg(CurrentDirPTR);
        while(fp.read((char*)&templatdr,sizeof(templatdr)) && fp.tellg() < (CurrentDirPTR + 10000))
        {
            if(templatdr.dfname == ""){
                directory tmp = templatdr;
                strcpy(tmp.dfname, file_name);
                tmp.dfid = CurrentFileFD;
                tmp.dir = 0;
        //        long pos=-sizeof(tmp);
                fp.seekp(-sizeof(tmp),std::ios::cur);
                fp.write((char*)&tmp, sizeof(tmp));
                success = true; 
                break;
            }
        }
    }
      
    strcpy(Flag, fg.c_str());
    if (strcmp(Flag, "w") == 0){
        std::cout<< "\nFile "<< file_name << " open for Writing and fd = " << CurrentFileFD << ".\n";
    }else if (strcmp(Flag, "r") == 0){
        std::cout<< "\nFile "<< file_name << " open for Reading and fd = " << CurrentFileFD << ".\n";
    }else if (strcmp(Flag, "rw") == 0){
        std::cout<< "\nFile "<< file_name << " open for Reading and Writing and fd = " << CurrentFileFD << ".\n";
    }
    
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