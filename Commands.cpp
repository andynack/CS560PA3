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
    
    FileSpacePTR = 64000; //Defining start location of datablock
    DiskSize = 100000000; //Defining size of filesystem disk
    DataBlockSize = 1024; //Defining size of datablocks
    strcpy(CurrentDir, "Root"); //intializing root as current directory
    CurrentDirPTR = FileSpacePTR; //intializing root as current directory
    CurrentDirOffset = 0;
    CurrentDirID = 1;
    CurrentFileStatus = false;
    std::cout<< "\nFile System Ready To Receive Commands\n\n";
}

Commands::~Commands() {
 //   std::cout<< "\nFile System No Longer Able To Receive Commands\n\n";
}

void Commands::mkfs() {

    indexnode ctrlnodes;
    directory templatedir;
            
    //create raw disk
    std::ofstream ofs("filesystem.dat", std::ios::binary | std::ios::out);
    
    //ensure "disk" opened correctly
    if (ofs){
        ofs.seekp(DiskSize-1);
        ofs.write("", 1); 
        std::cout<< "Disk Created And Ready For Use\n";
        std::cout<< "Disk Size: "<< ofs.tellp() << "\n";

        //initialize Root Directory inode
        ofs.seekp(0); 
        ctrlnodes.id = 1;
        ctrlnodes.valid = 1;
        ctrlnodes.linkcount = 1;
        ctrlnodes.size = sizeof(templatedir);
        ctrlnodes.fptr_a = FileSpacePTR;
        ctrlnodes.fptr_b = 0;
        ctrlnodes.fptr_c = 0;
        ctrlnodes.fptr_d = 0;
        ctrlnodes.fptr_e = 0;
        ctrlnodes.fptr_f = 0;
        ctrlnodes.fptr_g = 0;
        ctrlnodes.fptr_h = 0;
        ctrlnodes.fptr_i = 0;
        ctrlnodes.fptr_j = 0;
        ofs.write((char*)&ctrlnodes,sizeof(ctrlnodes));
        std::cout<< "Inode Size: "<< ofs.tellp()<< "\n";

        //check that writing inode did not lengthen the file and identify the current directory
        ofs.seekp(0, ofs.end);
        std::cout<< "Available Disk Space for Data: "<< (DiskSize - FileSpacePTR) << "\n\n";
        std::cout<< "The Current Directory is: "<< CurrentDir << "\n"<< "mkfs() Complete\n\n";
        
    }else{
        std::cout<<"****Error Accessing File System Disk****\n\n";
    }
}

void Commands::open(std::string filename, std::string fg){
    if (CurrentFileStatus){
        std::cout<<"File "<<CurrentFile<<" has already been opened. Only one file may be opened at a time.\n";
    }else {
        bool error = false;
        int datablock = FileSpacePTR;
        directory templatdr;
        indexnode ctrlnodes;

        //convert inputed strings to char arrays to match the format used in the filesystem
        strcpy(CurrentFile, filename.c_str());
        strcpy(Flag, fg.c_str());
        std::cout<<"Searching for " << CurrentFile<<"\n";

        //open filesystem "disk" for reading and writing
        std::fstream fp ("filesystem.dat", std::ios::in | std::ios::out | std::ios::binary);
        
        //ensure "disk" opened correctly
        if (fp){
            
            //get info of current directory
            fp.seekg(0);
            bool success = false;
            while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (fp.tellg() < FileSpacePTR))
            {
                if(ctrlnodes.id == CurrentDirID){               
                    CurrentDirSize = ctrlnodes.size;
                    CurrentDirPTR = ctrlnodes.fptr_a;
                    if(CurrentDirSize > DataBlockSize){
                        CurrentDirPTR_B = ctrlnodes.fptr_b;
                    }else{
                        CurrentDirPTR_B = 0;
                    }
                    if(CurrentDirSize > (2 * DataBlockSize)){
                        CurrentDirPTR_C = ctrlnodes.fptr_c;
                    }else{
                        CurrentDirPTR_C = 0;
                    }
                    if(CurrentDirSize > (3 * DataBlockSize)){
                        CurrentDirPTR_D = ctrlnodes.fptr_d;
                    }else{
                        CurrentDirPTR_D = 0;
                    }
                    if(CurrentDirSize > (4 * DataBlockSize)){
                        CurrentDirPTR_E = ctrlnodes.fptr_e;
                    }else{
                        CurrentDirPTR_E = 0;
                    }
                    if(CurrentDirSize > (5 * DataBlockSize)){
                        CurrentDirPTR_F = ctrlnodes.fptr_f;
                    }else{
                        CurrentDirPTR_F = 0;
                    }
                    if(CurrentDirSize > (6 * DataBlockSize)){
                        CurrentDirPTR_G = ctrlnodes.fptr_g;
                    }else{
                        CurrentDirPTR_G = 0;
                    }
                    if(CurrentDirSize > (7 * DataBlockSize)){
                        CurrentDirPTR_H = ctrlnodes.fptr_h;
                    }else{
                        CurrentDirPTR_H = 0;
                    }
                    if(CurrentDirSize > (8 * DataBlockSize)){
                        CurrentDirPTR_I = ctrlnodes.fptr_i;
                    }else{
                        CurrentDirPTR_I = 0;
                    }
                    if(CurrentDirSize > (9 * DataBlockSize)){
                        CurrentDirPTR_J = ctrlnodes.fptr_j;
                    }else{
                        CurrentDirPTR_J = 0;
                    }
                    success = true; 
                    break;
                }
            }
            if (!success){
                CurrentFileStatus = false;
                std::cout<< "****Error inode for current directory not found****\n";
                error = true;
            }
            
            //go to current directory
            fp.seekg(CurrentDirPTR);

            //Search for requested file to open
            while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR + DataBlockSize)))
            {
                if(strcmp(templatdr.dfname, CurrentFile)== 0){
                    if(templatdr.dir == 1){
                        std::cout<< "****Error: This is a Directory and therefore can not be opened for reading and/or writing.****\n";
                        CurrentFileStatus = false;
                        error = true;              
                    }else {
                        std::cout<< "File Existed\n";
                        CurrentFileFD = templatdr.dfid;
                        CurrentFileStatus = true;
                    }
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_B && !CurrentFileStatus){
                fp.seekg(CurrentDirPTR_B);

                //Search for requested file to open
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_B + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, CurrentFile)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "****Error: This is a Directory and therefore can not be opened for reading and/or writing.****\n";
                            CurrentFileStatus = false;
                            error = true;              
                        }else {
                            std::cout<< "File Existed\n";
                            CurrentFileFD = templatdr.dfid;
                            CurrentFileStatus = true;
                        }
                    }
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search           
            if(CurrentDirPTR_C && !CurrentFileStatus){
                fp.seekg(CurrentDirPTR_C);

                //Search for requested file to open
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_C + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, CurrentFile)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "****Error: This is a Directory and therefore can not be opened for reading and/or writing.****\n";
                            CurrentFileStatus = false;
                            error = true;              
                        }else {
                            std::cout<< "File Existed\n";
                            CurrentFileFD = templatdr.dfid;
                            CurrentFileStatus = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_D && !CurrentFileStatus){
                fp.seekg(CurrentDirPTR_D);

                //Search for requested file to open
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_D + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, CurrentFile)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "****Error: This is a Directory and therefore can not be opened for reading and/or writing.****\n";
                            CurrentFileStatus = false;
                            error = true;              
                        }else {
                            std::cout<< "File Existed\n";
                            CurrentFileFD = templatdr.dfid;
                            CurrentFileStatus = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_E && !CurrentFileStatus){
                fp.seekg(CurrentDirPTR_E);

                //Search for requested file to open
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_E + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, CurrentFile)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "****Error: This is a Directory and therefore can not be opened for reading and/or writing.****\n";
                            CurrentFileStatus = false;
                            error = true;              
                        }else {
                            std::cout<< "File Existed\n";
                            CurrentFileFD = templatdr.dfid;
                            CurrentFileStatus = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_F && !CurrentFileStatus){
                fp.seekg(CurrentDirPTR_F);

                //Search for requested file to open
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_F + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, CurrentFile)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "****Error: This is a Directory and therefore can not be opened for reading and/or writing.****\n";
                            CurrentFileStatus = false;
                            error = true;              
                        }else {
                            std::cout<< "File Existed\n";
                            CurrentFileFD = templatdr.dfid;
                            CurrentFileStatus = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_G && !CurrentFileStatus){
                fp.seekg(CurrentDirPTR_G);

                //Search for requested file to open
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_G + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, CurrentFile)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "****Error: This is a Directory and therefore can not be opened for reading and/or writing.****\n";
                            CurrentFileStatus = false;
                            error = true;              
                        }else {
                            std::cout<< "File Existed\n";
                            CurrentFileFD = templatdr.dfid;
                            CurrentFileStatus = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_H && !CurrentFileStatus){
                fp.seekg(CurrentDirPTR_H);

                //Search for requested file to open
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_H + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, CurrentFile)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "****Error: This is a Directory and therefore can not be opened for reading and/or writing.****\n";
                            CurrentFileStatus = false;
                            error = true;              
                        }else {
                            std::cout<< "File Existed\n";
                            CurrentFileFD = templatdr.dfid;
                            CurrentFileStatus = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_I && !CurrentFileStatus){
                fp.seekg(CurrentDirPTR_I);

                //Search for requested file to open
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_I + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, CurrentFile)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "****Error: This is a Directory and therefore can not be opened for reading and/or writing.****\n";
                            CurrentFileStatus = false;
                            error = true;              
                        }else {
                            std::cout<< "File Existed\n";
                            CurrentFileFD = templatdr.dfid;
                            CurrentFileStatus = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_J && !CurrentFileStatus){
                fp.seekg(CurrentDirPTR_J);

                //Search for requested file to open
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_J + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, CurrentFile)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "****Error: This is a Directory and therefore can not be opened for reading and/or writing.****\n";
                            CurrentFileStatus = false;
                            error = true;              
                        }else {
                            std::cout<< "File Existed\n";
                            CurrentFileFD = templatdr.dfid;
                            CurrentFileStatus = true;
                        }
                    }
                }
            }
            
            //if not found and not intended for read only access, create file
            if (!CurrentFileStatus && (strcmp(Flag, "r") != 0) && !error){
                std::cout<<"Creating " << CurrentFile<<"\n";

                //find unused data block
                datablock= GetNextDataBlock();

                //error checking
                if(!error){
                    
                    //find unused inode and initialize                    
                    fp.seekg(0);
                    success = false;
                    int i = 1;
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
                    }
                    if (!success){
                        CurrentFileStatus = false;
                        std::cout<< "****Error Control Block is Full****\n";
                        error = true;
                    }
                    
                    //error checking
                    if(!error){
                        
                        //find next blank entry in directory and initialize
                        if(CurrentDirSize < DataBlockSize){
                            fp.seekg(CurrentDirPTR);
                        }
                        if((CurrentDirSize > DataBlockSize) && (CurrentDirSize < (2 * DataBlockSize))){
                            fp.seekg(CurrentDirPTR_B);
                        }
                        if((CurrentDirSize > (2 * DataBlockSize)) && (CurrentDirSize < (3 * DataBlockSize))){
                            fp.seekg(CurrentDirPTR_C);
                        }
                        if((CurrentDirSize > (3 * DataBlockSize)) && (CurrentDirSize < (4 * DataBlockSize))){
                            fp.seekg(CurrentDirPTR_D);
                        }
                        if((CurrentDirSize > (4 * DataBlockSize)) && (CurrentDirSize < (5 * DataBlockSize))){
                            fp.seekg(CurrentDirPTR_E);
                        }
                        if((CurrentDirSize > (5 * DataBlockSize)) && (CurrentDirSize < (6 * DataBlockSize))){
                            fp.seekg(CurrentDirPTR_F);
                        }
                        if((CurrentDirSize > (6 * DataBlockSize)) && (CurrentDirSize < (7 * DataBlockSize))){
                            fp.seekg(CurrentDirPTR_G);
                        }
                        if((CurrentDirSize > (7 * DataBlockSize)) && (CurrentDirSize < (8 * DataBlockSize))){
                            fp.seekg(CurrentDirPTR_H);
                        }
                        if((CurrentDirSize > (8 * DataBlockSize)) && (CurrentDirSize < (9 * DataBlockSize))){
                            fp.seekg(CurrentDirPTR_I);
                        }
                        if((CurrentDirSize > (9 * DataBlockSize)) && (CurrentDirSize < (10 * DataBlockSize))){
                            fp.seekg(CurrentDirPTR_J);
                        }
                        success = false;
                        
                        int temp = fp.tellg();
                        while(fp.read((char*)&templatdr,sizeof(templatdr)) && (fp.tellg() <= (temp + DataBlockSize)))
                        {
                            if(strcmp(templatdr.dfname, "")== 0){
                                directory tmp = templatdr;
                                strcpy(tmp.dfname, CurrentFile);
                                tmp.dfid = CurrentFileFD;
                                tmp.dir = 0;
                                fp.seekp(-sizeof(tmp),std::ios::cur);
                                fp.write((char*)&tmp, sizeof(tmp));
                                std::cout<< tmp.dfname << " added to "<< CurrentDir << "\n";
                                success = true; 
                                break;
                            }          
                        }
                        if (!success){
                            CurrentFileStatus = false;
                            std::cout<< "****Error Directory is Full****\n";
                            error = true;
                        }else{
                            int temp2 = fp.tellg();
                            CurrentDirSize = CurrentDirSize + (temp2 - temp);
                        }
                        //update directory size data in inode
                        fp.seekg(0);
                        success = false;
                        while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (fp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == CurrentDirID){
                                indexnode tmp2 = ctrlnodes;
                                fp.seekp(-sizeof(tmp2),std::ios::cur);
                                tmp2.size = CurrentDirSize;
                                fp.write((char*)&tmp2, sizeof(tmp2));
                                success = true; 
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error: Directory size data could not be updated****\n";
                            error = true;
                        }
                    }
                    if (error){
                        std::cout<<"\n****Error: " << CurrentFile << " not added to "<< CurrentDir << " ****\n";
                        error = true;
                    }
                }
                if (!error){
                    CurrentFileStatus = true;
                }
            }

            if (!CurrentFileStatus && (strcmp(Flag, "r") == 0) && !error){
                std::cout<< CurrentFile << " does not exist for reading\n";
            }

            if (CurrentFileStatus && (strcmp(Flag, "w") == 0) && !error){
                std::cout<< "File "<< CurrentFile << ", from directory, "<< CurrentDir << ", is open for Writing and has fd = " << CurrentFileFD << ".\n";
            }else if (CurrentFileStatus && (strcmp(Flag, "r") == 0) && !error){
                std::cout<< "File "<< CurrentFile << ", from directory "<< CurrentDir << ", is open for Reading and has fd = " << CurrentFileFD << ".\n";
            }else if (CurrentFileStatus && (strcmp(Flag, "rw") == 0) && !error){
                std::cout<< "File "<< CurrentFile << ", from directory "<< CurrentDir << ", is open for Reading and Writing and has fd = " << CurrentFileFD << ".\n";
            }
        }else {
            std::cout<<"****Error Accessing File System Disk****\n";
        }
    }    
    std::cout<<"open Complete\n\n";
}

void Commands::read(int fd, int size){
    
    //add code
}

void Commands::write(int fd, std::string str){
    bool error = false;
    indexnode ctrlnodes;
    if (CurrentFileStatus && ((strcmp(Flag, "w") == 0) || (strcmp(Flag, "rw") == 0)) && (fd == CurrentFileFD)){
        
        //open filesystem "disk" for reading and writing
        std::fstream fp ("filesystem.dat", std::ios::in|std::ios::out|std::ios::binary);
        
        //ensure "disk" opened correctly
        if (fp){
            
            //convert inputed string to char array to match the format used in the filesystem
            int len = str.length();
            char buffer[len];
            strcpy(buffer, str.c_str());
            
            //get info of current file
            fp.seekg(0);
            bool success = false;
            while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (fp.tellg() < FileSpacePTR))
            {
                if(ctrlnodes.id == CurrentFileFD){               
                    CurrentFileFDPTR = fp.tellg();
                    CurrentFileFDPTR = CurrentFileFDPTR - sizeof(ctrlnodes);
                    CurrentFileSize = ctrlnodes.size;
                    CurrentFilePTR = ctrlnodes.fptr_a;
                    if(CurrentFileSize > DataBlockSize){
                        CurrentFilePTR_B = ctrlnodes.fptr_b;
                    }else{
                        CurrentFilePTR_B = 0;
                    }
                    if(CurrentFileSize > (2 * DataBlockSize)){
                        CurrentFilePTR_C = ctrlnodes.fptr_c;
                    }else{
                        CurrentFilePTR_C = 0;
                    }
                    if(CurrentFileSize > (3 * DataBlockSize)){
                        CurrentFilePTR_D = ctrlnodes.fptr_d;
                    }else{
                        CurrentFilePTR_D = 0;
                    }
                    if(CurrentFileSize > (4 * DataBlockSize)){
                        CurrentFilePTR_E = ctrlnodes.fptr_e;
                    }else{
                        CurrentFilePTR_E = 0;
                    }
                    if(CurrentFileSize > (5 * DataBlockSize)){
                        CurrentFilePTR_F = ctrlnodes.fptr_f;
                    }else{
                        CurrentFilePTR_F = 0;
                    }
                    if(CurrentFileSize > (6 * DataBlockSize)){
                        CurrentFilePTR_G = ctrlnodes.fptr_g;
                    }else{
                        CurrentFilePTR_G = 0;
                    }
                    if(CurrentFileSize > (7 * DataBlockSize)){
                        CurrentFilePTR_H = ctrlnodes.fptr_h;
                    }else{
                        CurrentFilePTR_H = 0;
                    }
                    if(CurrentFileSize > (8 * DataBlockSize)){
                        CurrentFilePTR_I = ctrlnodes.fptr_i;
                    }else{
                        CurrentFilePTR_I = 0;
                    }
                    if(CurrentFileSize > (9 * DataBlockSize)){
                        CurrentFilePTR_J = ctrlnodes.fptr_j;
                    }else{
                        CurrentFilePTR_J = 0;
                    }
                    success = true; 
                    break;
                }
            }
            if (!success){
                CurrentFileStatus = false;
                std::cout<< "****Error inode for current file not found****\n";
                error = true;
            }
            
            //Determine new size of file after write
            CurrentFileSize = CurrentFileSize + len;
            
            //Allocate space for write if needed
            if((CurrentFileSize > DataBlockSize) && !CurrentFilePTR_B){
                CurrentFilePTR_B = GetNextDataBlock();
            }
            if((CurrentFileSize > (2 * DataBlockSize)) && !CurrentFilePTR_C){
                CurrentFilePTR_C = GetNextDataBlock();
            }
            if((CurrentFileSize > (3 * DataBlockSize)) && !CurrentFilePTR_D){
                CurrentFilePTR_D = GetNextDataBlock();
            }
            if((CurrentFileSize > (4 * DataBlockSize)) && !CurrentFilePTR_E){
                CurrentFilePTR_E = GetNextDataBlock();
            }
            if((CurrentFileSize > (5 * DataBlockSize)) && !CurrentFilePTR_F){
                CurrentFilePTR_F = GetNextDataBlock();
            }
            if((CurrentFileSize > (6 * DataBlockSize)) && !CurrentFilePTR_G){
                CurrentFilePTR_G = GetNextDataBlock();
            }
            if((CurrentFileSize > (7 * DataBlockSize)) && !CurrentFilePTR_H){
                CurrentFilePTR_H = GetNextDataBlock();
            }
            if((CurrentFileSize > (8 * DataBlockSize)) && !CurrentFilePTR_I){
                CurrentFilePTR_I = GetNextDataBlock();
            }
            if((CurrentFileSize > (9 * DataBlockSize)) && !CurrentFilePTR_J){
                CurrentFilePTR_J = GetNextDataBlock();
            }
            
            //write string to file
            int t;
            if((CurrentFileOffset < DataBlockSize) && (t < len)){
                fp.seekp(CurrentFilePTR + CurrentFileOffset);
                while((CurrentFileOffset < DataBlockSize) && (t < len)){
                    fp.write((char*)&buffer[t],sizeof(buffer[t]));
                    t++;
                    CurrentFileOffset++;
                }
            }
            if((CurrentFileOffset > DataBlockSize) && (CurrentFileOffset < (2 * DataBlockSize)) && (t < len)){
                fp.seekp(CurrentFilePTR_B + CurrentFileOffset - DataBlockSize);
                while((CurrentFileOffset > DataBlockSize) && (CurrentFileOffset < (2 * DataBlockSize)) && (t < len)){
                    char tmp = buffer[t];
                    fp.write((char*)&buffer[t],sizeof(buffer[t]));
                    t++;
                    CurrentFileOffset++;
                }
            }
            if((CurrentFileOffset > (2 * DataBlockSize)) && (CurrentFileOffset < (3 * DataBlockSize)) && (t < len)){
                fp.seekp(CurrentFilePTR_C + CurrentFileOffset - (2 * DataBlockSize));
                while((CurrentFileOffset > (2 * DataBlockSize)) && (CurrentFileOffset < (3 * DataBlockSize)) && (t < len)){
                    char tmp = buffer[t];
                    fp.write((char*)&buffer[t],sizeof(buffer[t]));
                    t++;
                    CurrentFileOffset++;
                }
            }
            if((CurrentFileOffset > (3 * DataBlockSize)) && (CurrentFileOffset < (4 * DataBlockSize)) && (t < len)){
                fp.seekp(CurrentFilePTR_D + CurrentFileOffset - (3 * DataBlockSize));
                while((CurrentFileOffset > (3 * DataBlockSize)) && (CurrentFileOffset < (4 * DataBlockSize)) && (t < len)){
                    char tmp = buffer[t];
                    fp.write((char*)&buffer[t],sizeof(buffer[t]));
                    t++;
                    CurrentFileOffset++;
                }
            }
            if((CurrentFileOffset > (4 * DataBlockSize)) && (CurrentFileOffset < (5 * DataBlockSize)) && (t < len)){
                fp.seekp(CurrentFilePTR_E + CurrentFileOffset - (4 * DataBlockSize));
                while((CurrentFileOffset > (4 * DataBlockSize)) && (CurrentFileOffset < (5 * DataBlockSize)) && (t < len)){
                    char tmp = buffer[t];
                    fp.write((char*)&buffer[t],sizeof(buffer[t]));
                    t++;
                    CurrentFileOffset++;
                }
            }
            if((CurrentFileOffset > (5 * DataBlockSize)) && (CurrentFileOffset < (6 * DataBlockSize)) && (t < len)){
                fp.seekp(CurrentFilePTR_F + CurrentFileOffset - (5 * DataBlockSize));
                while((CurrentFileOffset > (5 * DataBlockSize)) && (CurrentFileOffset < (6 * DataBlockSize)) && (t < len)){
                    char tmp = buffer[t];
                    fp.write((char*)&buffer[t],sizeof(buffer[t]));
                    t++;
                    CurrentFileOffset++;
                }
            }
            if((CurrentFileOffset > (6 * DataBlockSize)) && (CurrentFileOffset < (7 * DataBlockSize)) && (t < len)){
                fp.seekp(CurrentFilePTR_G + CurrentFileOffset - (6 * DataBlockSize));
                while((CurrentFileOffset > (6 * DataBlockSize)) && (CurrentFileOffset < (7 * DataBlockSize)) && (t < len)){
                    char tmp = buffer[t];
                    fp.write((char*)&buffer[t],sizeof(buffer[t]));
                    t++;
                    CurrentFileOffset++;
                }
            }
            if((CurrentFileOffset > (7 * DataBlockSize)) && (CurrentFileOffset < (8 * DataBlockSize)) && (t < len)){
                fp.seekp(CurrentFilePTR_H + CurrentFileOffset - (7 * DataBlockSize));
                while((CurrentFileOffset > (7 * DataBlockSize)) && (CurrentFileOffset < (8 * DataBlockSize)) && (t < len)){
                    char tmp = buffer[t];
                    fp.write((char*)&buffer[t],sizeof(buffer[t]));
                    t++;
                    CurrentFileOffset++;
                }
            }
            if((CurrentFileOffset > (8 * DataBlockSize)) && (CurrentFileOffset < (9 * DataBlockSize)) && (t < len)){
                fp.seekp(CurrentFilePTR_I + CurrentFileOffset - (8 * DataBlockSize));
                while((CurrentFileOffset > (8 * DataBlockSize)) && (CurrentFileOffset < (9 * DataBlockSize)) && (t < len)){
                    char tmp = buffer[t];
                    fp.write((char*)&buffer[t],sizeof(buffer[t]));
                    t++;
                    CurrentFileOffset++;
                }
            }
            if((CurrentFileOffset > (9 * DataBlockSize)) && (CurrentFileOffset < (10 * DataBlockSize)) && (t < len)){
                fp.seekp(CurrentFilePTR_J + CurrentFileOffset - (9 * DataBlockSize));
                while((CurrentFileOffset > (9 * DataBlockSize)) && (CurrentFileOffset < (10 * DataBlockSize)) && (t < len)){
                    char tmp = buffer[t];
                    fp.write((char*)&buffer[t],sizeof(buffer[t]));
                    t++;
                    CurrentFileOffset++;
                }
            }
            
            //Update inode for current file
            fp.seekp(CurrentFileFDPTR); 
            ctrlnodes.size = CurrentFileSize;
            ctrlnodes.fptr_b = CurrentFilePTR_B;
            ctrlnodes.fptr_c = CurrentFilePTR_C;
            ctrlnodes.fptr_d = CurrentFilePTR_D;
            ctrlnodes.fptr_e = CurrentFilePTR_E;
            ctrlnodes.fptr_f = CurrentFilePTR_F;
            ctrlnodes.fptr_g = CurrentFilePTR_G;
            ctrlnodes.fptr_h = CurrentFilePTR_H;
            ctrlnodes.fptr_i = CurrentFilePTR_I;
            ctrlnodes.fptr_j = CurrentFilePTR_J;
            fp.write((char*)&ctrlnodes,sizeof(ctrlnodes));
        
        }else {
            std::cout<<"****Error Accessing File System Disk****\n";
        }
        
    }else {
        std::cout<<"File is not open for writing\n";
    }
    std::cout<<"write Complete\n\n";
}

void Commands::seek(int fd, int offset){
    
    //add code
}

void Commands::close(int fd){
    if(CurrentFileStatus){
        if(fd== CurrentFileFD){
            CurrentFileStatus = false;
            std::cout<<"File has been closed\n";
        }else{
            std::cout<<"File is not open\n";
        }
    }else{
        std::cout<<"No files are open\n";
    }
    std::cout<<"close Complete\n\n";
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
    
    bool error = false;
    directory templatdr;
    indexnode ctrlnodes;
    
    //open filesystem "disk" for reading and writing
    std::ifstream ifp ("filesystem.dat", std::ios::in|std::ios::binary);

    //ensure "disk" opened correctly
    if (ifp){

        //get info of current directory
        ifp.seekg(0);
        bool success = false;
        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
        {
            if(ctrlnodes.id == CurrentDirID){               
                CurrentDirSize = ctrlnodes.size;
                CurrentDirPTR = ctrlnodes.fptr_a;
                if(CurrentDirSize > DataBlockSize){
                    CurrentDirPTR_B = ctrlnodes.fptr_b;
                }else{
                    CurrentDirPTR_B = 0;
                }
                if(CurrentDirSize > (2 * DataBlockSize)){
                    CurrentDirPTR_C = ctrlnodes.fptr_c;
                }else{
                    CurrentDirPTR_C = 0;
                }
                if(CurrentDirSize > (3 * DataBlockSize)){
                    CurrentDirPTR_D = ctrlnodes.fptr_d;
                }else{
                    CurrentDirPTR_D = 0;
                }
                if(CurrentDirSize > (4 * DataBlockSize)){
                    CurrentDirPTR_E = ctrlnodes.fptr_e;
                }else{
                    CurrentDirPTR_E = 0;
                }
                if(CurrentDirSize > (5 * DataBlockSize)){
                    CurrentDirPTR_F = ctrlnodes.fptr_f;
                }else{
                    CurrentDirPTR_F = 0;
                }
                if(CurrentDirSize > (6 * DataBlockSize)){
                    CurrentDirPTR_G = ctrlnodes.fptr_g;
                }else{
                    CurrentDirPTR_G = 0;
                }
                if(CurrentDirSize > (7 * DataBlockSize)){
                    CurrentDirPTR_H = ctrlnodes.fptr_h;
                }else{
                    CurrentDirPTR_H = 0;
                }
                if(CurrentDirSize > (8 * DataBlockSize)){
                    CurrentDirPTR_I = ctrlnodes.fptr_i;
                }else{
                    CurrentDirPTR_I = 0;
                }
                if(CurrentDirSize > (9 * DataBlockSize)){
                    CurrentDirPTR_J = ctrlnodes.fptr_j;
                }else{
                    CurrentDirPTR_J = 0;
                }
                success = true;
                break;
            }
        }
        if (!success){
            CurrentFileStatus = false;
            std::cout<< "****Error inode for current directory not found****\n";
            error = true;
        }else{
            //go to current directory
            ifp.seekg(CurrentDirPTR);
            std::cout<< "\n";
            while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR + DataBlockSize)))
            {
                if (strcmp(templatdr.dfname, "")!= 0){
                    if(templatdr.dir == 1){
                        std::cout<< "Directory: "<< templatdr.dfname << "\n";
                    }else {
                        std::cout<< "File: "<< templatdr.dfname << "\n";
                    }
                }
            }

            //go to next block of current directory, if it exists, and continue to display contents
            if(CurrentDirPTR_B){
                ifp.seekg(CurrentDirPTR_B);
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_B + DataBlockSize)))
                {
                    if (strcmp(templatdr.dfname, "")!= 0){
                        if(templatdr.dir == 1){
                            std::cout<< "Directory: "<< templatdr.dfname << "\n";
                        }else {
                            std::cout<< "File: "<< templatdr.dfname << "\n";
                        }
                    }
                }
            }

            //go to next block of current directory, if it exists, and continue to display contents
            if(CurrentDirPTR_C){
                ifp.seekg(CurrentDirPTR_C);
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_C + DataBlockSize)))
                {
                    if (strcmp(templatdr.dfname, "")!= 0){
                        if(templatdr.dir == 1){
                            std::cout<< "Directory: "<< templatdr.dfname << "\n";
                        }else {
                            std::cout<< "File: "<< templatdr.dfname << "\n";
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, and continue to display contents
            if(CurrentDirPTR_D){
                ifp.seekg(CurrentDirPTR_D);
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_D + DataBlockSize)))
                {
                    if (strcmp(templatdr.dfname, "")!= 0){
                        if(templatdr.dir == 1){
                            std::cout<< "Directory: "<< templatdr.dfname << "\n";
                        }else {
                            std::cout<< "File: "<< templatdr.dfname << "\n";
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, and continue to display contents
            if(CurrentDirPTR_E){
                ifp.seekg(CurrentDirPTR_E);
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_E + DataBlockSize)))
                {
                    if (strcmp(templatdr.dfname, "")!= 0){
                        if(templatdr.dir == 1){
                            std::cout<< "Directory: "<< templatdr.dfname << "\n";
                        }else {
                            std::cout<< "File: "<< templatdr.dfname << "\n";
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, and continue to display contents
            if(CurrentDirPTR_F){
                ifp.seekg(CurrentDirPTR_F);
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_F + DataBlockSize)))
                {
                    if (strcmp(templatdr.dfname, "")!= 0){
                        if(templatdr.dir == 1){
                            std::cout<< "Directory: "<< templatdr.dfname << "\n";
                        }else {
                            std::cout<< "File: "<< templatdr.dfname << "\n";
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, and continue to display contents
            if(CurrentDirPTR_G){
                ifp.seekg(CurrentDirPTR_G);
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_G + DataBlockSize)))
                {
                    if (strcmp(templatdr.dfname, "")!= 0){
                        if(templatdr.dir == 1){
                            std::cout<< "Directory: "<< templatdr.dfname << "\n";
                        }else {
                            std::cout<< "File: "<< templatdr.dfname << "\n";
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, and continue to display contents
            if(CurrentDirPTR_H){
                ifp.seekg(CurrentDirPTR_H);
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_H + DataBlockSize)))
                {
                    if (strcmp(templatdr.dfname, "")!= 0){
                        if(templatdr.dir == 1){
                            std::cout<< "Directory: "<< templatdr.dfname << "\n";
                        }else {
                            std::cout<< "File: "<< templatdr.dfname << "\n";
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, and continue to display contents
            if(CurrentDirPTR_I){
                ifp.seekg(CurrentDirPTR_I);
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_I + DataBlockSize)))
                {
                    if (strcmp(templatdr.dfname, "")!= 0){
                        if(templatdr.dir == 1){
                            std::cout<< "Directory: "<< templatdr.dfname << "\n";
                        }else {
                            std::cout<< "File: "<< templatdr.dfname << "\n";
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, and continue to display contents
            if(CurrentDirPTR_J){
                ifp.seekg(CurrentDirPTR_J);
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_J + DataBlockSize)))
                {
                    if (strcmp(templatdr.dfname, "")!= 0){
                        if(templatdr.dir == 1){
                            std::cout<< "Directory: "<< templatdr.dfname << "\n";
                        }else {
                            std::cout<< "File: "<< templatdr.dfname << "\n";
                        }
                    }
                }
            }
        }
        
    }else {
        std::cout<<"****Error Accessing File System Disk****\n";
    }
    std::cout<<"ls Complete\n\n";
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

int Commands::GetNextDataBlock(){
    
    int datablock = FileSpacePTR;
    indexnode ctrlnodes;
    
    //open filesystem "disk" for reading
    std::ifstream ifp ("filesystem.dat", std::ios::in | std::ios::binary);
    
    if(ifp){
        //find next unused data block
        ifp.seekg(0);
        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && ifp.tellg() < FileSpacePTR)
        {
            if(ctrlnodes.valid == 1){               
                if (ctrlnodes.fptr_a == datablock && datablock < DiskSize){
                    datablock += DataBlockSize;
                    ifp.seekg(0);
                }else if(ctrlnodes.fptr_b == datablock && datablock < DiskSize){
                    datablock += DataBlockSize;
                    ifp.seekg(0);
                }else if(ctrlnodes.fptr_c == datablock && datablock < DiskSize){
                    datablock += DataBlockSize;
                    ifp.seekg(0);
                }else if(ctrlnodes.fptr_d == datablock && datablock < DiskSize){
                    datablock += DataBlockSize;
                    ifp.seekg(0);
                }else if(ctrlnodes.fptr_e == datablock && datablock < DiskSize){
                    datablock += DataBlockSize;
                    ifp.seekg(0);
                }else if(ctrlnodes.fptr_f == datablock && datablock < DiskSize){
                    datablock += DataBlockSize;
                    ifp.seekg(0);
                }else if(ctrlnodes.fptr_g == datablock && datablock < DiskSize){
                    datablock += DataBlockSize;
                    ifp.seekg(0);
                }else if(ctrlnodes.fptr_h == datablock && datablock < DiskSize){
                    datablock += DataBlockSize;
                    ifp.seekg(0);
                }else if(ctrlnodes.fptr_i == datablock && datablock < DiskSize){
                    datablock += DataBlockSize;
                    ifp.seekg(0);
                }else if(ctrlnodes.fptr_j == datablock && datablock < DiskSize){
                    datablock += DataBlockSize;
                    ifp.seekg(0);
                }
            }
        }
        if (datablock > (DiskSize)){
            std::cout<< "****Error File System Disk is Full****\n";
            return 0;
        }else {
            std::cout<< "The next available file block is: "<< datablock << "\n";
            return datablock;
        }
    }else{
        std::cout<<"****Error Accessing File System Disk****\n";
    }
}