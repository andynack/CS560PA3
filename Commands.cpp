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
        
    //create raw disk
    std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    ofs.seekp(100000000-1);
    ofs.write("", 1); 
    std::cout<< "Disk Created And Ready For Use\n";
    std::cout<< "Disk Size: "<< ofs.tellp()<< "\n\n";
        
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
    
    ofs.seekp(ctrlnodes.fptr_a);
    std::cout<< "Start of Data Block: "<< ofs.tellp()<< "\n";
    strcpy(templatedir.dfname, "");
    templatedir.dfid = 0;
    ofs.write((char*)&templatedir,sizeof(templatedir));
    
    std::cout<< "Root Directory Initialized\n";
    
    ofs.seekp(0, ofs.end);
    std::cout<< "Disk Size: "<< ofs.tellp()<< "\n\n";
    
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

void Commands::open(char filename[20], char fg[5]){
    bool success = false;
    int datablock = FileSpacePTR;
 //   nametable ctrlnames;
    directory templatdr;
    indexnode ctrlnodes;
    std::fstream fp ("filesystem.dat", std::ios::in|std::ios::out);
    fp.seekg(CurrentDirPTR);
    while(fp.read((char*)&templatdr, sizeof(templatdr)) && fp.tellg() < (CurrentDirPTR + 10000))
    {
        if(strcmp(templatdr.dfname, filename)== 0){
            CurrentFileFD = templatdr.dfid;
            success = true; 
        }
    }
    if (!success){
        //find unused data block
        fp.seekg(INodePTR);
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
        
        //find unused inode and initialize
        fp.seekg(INodePTR);
        while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && fp.tellg() < FileSpacePTR)
        {
            int i = 1;
            if(ctrlnodes.valid == 0){               
                indexnode tmp = ctrlnodes;                
                CurrentFileFD = tmp.id;
        //        long pos=-1*sizeof(tmp);
                fp.seekp(-sizeof(tmp),std::ios::cur);
                tmp.fptr_a = datablock;
                tmp.valid = 1;
                tmp.id = i;
                tmp.linkcount = 1;                
                fp.write((char*)&tmp, sizeof(tmp));
                success = true; 
                break;
            }
            i++;
        }
        //find blank entry in name table and initialize
        fp.seekg(0);
        while(fp.read((char*)&templatdr,sizeof(templatdr)) && fp.tellg() < INodePTR)
        {
            if(templatdr.dfname == ""){
                directory tmp = templatdr;
                strcpy(tmp.dfname, filename);
                tmp.dfid = CurrentFileFD;
        //        long pos=-sizeof(tmp);
                fp.seekp(-sizeof(tmp),std::ios::cur);
                fp.write((char*)&tmp, sizeof(tmp));
                success = true; 
                break;
            }
        }
    }
      
    strcpy(Flag, fg);
    if (strcmp(Flag, "w") == 0){
        std::cout<< "\nFile "<< filename << " open for Writing and fd = " << CurrentFileFD << ".\n";
    }else if (strcmp(Flag, "r") == 0){
        std::cout<< "\nFile "<< filename << " open for Reading and fd = " << CurrentFileFD << ".\n";
    }else if (strcmp(Flag, "rw") == 0){
        std::cout<< "\nFile "<< filename << " open for Reading and Writing and fd = " << CurrentFileFD << ".\n";
    }
    
}

void Commands::read(int fd, int size){
    
    //add code
}

void Commands::write(int fd, char str[100]){
   
    //add code
}

void Commands::seek(int fd, int offset){
    
    //add code
}

void Commands::close(int fd){
    
    //add code
}

void Commands::mkdir(char dirname[20]){
    //add code
}

void Commands::rmdir(char dirname[20]){
    //add code
}

void Commands::cd(char dirname[20]){
    //add code
}

void Commands::link(char src[20], char dest[20]){
    //add code
}

void Commands::unlink(char name[20]){
    //add code
}

void Commands::stat(char name[20]){
    //add code
}

void Commands::ls(){
    //add code
}

void Commands::cat(char filename[20]){
    //add code
}

void Commands::cp(char src[20], char dest[20]){
  
}

void Commands::tree(){
    //add code
}

void Commands::import(char src[20], char dest[20]){
    //add code
}

void Commands::fexport(char src[20], char dest[20]){
    //add code
}