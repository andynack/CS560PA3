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
//    CurrentDirOffset = 0;
    CurrentDirID = 0;
    PrevDirPtr = 0;
    PrevDir[PrevDirPtr]= 0;
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
        ctrlnodes.id = 0;
        ctrlnodes.valid = 1;
        ctrlnodes.linkcount = 1;
        ctrlnodes.size = 0;
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

int Commands::open(std::string filename, std::string fg){
    bool error = false;
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
            int status = GetCurDirInfo();
            if (!status){
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
                        CurrentFileOffset = 0;
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
                            CurrentFileOffset = 0;
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
                            CurrentFileOffset = 0;
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
                            CurrentFileOffset = 0;
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
                            CurrentFileOffset = 0;
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
                            CurrentFileOffset = 0;
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
                            CurrentFileOffset = 0;
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
                            CurrentFileOffset = 0;
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
                            CurrentFileOffset = 0;
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
                            CurrentFileOffset = 0;
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
                    bool success = false;
                    int i = 0;
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
                    CurrentFileOffset = 0;
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
    std::cout<<"\n";
    if (!error){
        return CurrentFileFD;
    }else{
        return -1;
    }
}

void Commands::read(int fd, int size){
    bool error = false;
    indexnode ctrlnodes;
    if (CurrentFileStatus && ((strcmp(Flag, "r") == 0) || (strcmp(Flag, "rw") == 0)) && (fd == CurrentFileFD)){
        
        //open filesystem "disk" for reading and writing
        std::fstream ifp ("filesystem.dat", std::ios::in|std::ios::binary);
        
        //ensure "disk" opened correctly
        if (ifp){
                        
            //get info of current file
            ifp.seekg(0);
            bool success = false;
            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
            {
                if(ctrlnodes.id == CurrentFileFD){               
                    CurrentFileFDPTR = ifp.tellg();
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
            char buffer;
            if(!error){
                int t= 0;
                if((CurrentFileOffset < DataBlockSize) && (t < size)){
                    ifp.seekg(CurrentFilePTR + CurrentFileOffset);
                    while((CurrentFileOffset < DataBlockSize) && (t < size) && ifp.read((char*)&buffer,sizeof(buffer))){
                        std::cout<< buffer;
                        t++;
                        CurrentFileOffset++;
                    }
                }
                if((CurrentFileOffset > DataBlockSize) && (CurrentFileOffset < (2 * DataBlockSize)) && (t < size)){
                    ifp.seekg(CurrentFilePTR_B + CurrentFileOffset - DataBlockSize);
                    while((CurrentFileOffset > DataBlockSize) && (CurrentFileOffset < (2 * DataBlockSize)) && (t < size) && ifp.read((char*)&buffer,sizeof(buffer))){
                        std::cout<< buffer;
                        t++;
                        CurrentFileOffset++;
                    }
                }
                if((CurrentFileOffset > (2 * DataBlockSize)) && (CurrentFileOffset < (3 * DataBlockSize)) && (t < size)){
                    ifp.seekg(CurrentFilePTR_C + CurrentFileOffset - (2 * DataBlockSize));
                    while((CurrentFileOffset > (2 * DataBlockSize)) && (CurrentFileOffset < (3 * DataBlockSize)) && (t < size) && ifp.read((char*)&buffer,sizeof(buffer))){
                        std::cout<< buffer;
                        t++;
                        CurrentFileOffset++;
                    }
                }
                if((CurrentFileOffset > (3 * DataBlockSize)) && (CurrentFileOffset < (4 * DataBlockSize)) && (t < size)){
                    ifp.seekg(CurrentFilePTR_D + CurrentFileOffset - (3 * DataBlockSize));
                    while((CurrentFileOffset > (3 * DataBlockSize)) && (CurrentFileOffset < (4 * DataBlockSize)) && (t < size) && ifp.read((char*)&buffer,sizeof(buffer))){
                        std::cout<< buffer;
                        t++;
                        CurrentFileOffset++;
                    }
                }
                if((CurrentFileOffset > (4 * DataBlockSize)) && (CurrentFileOffset < (5 * DataBlockSize)) && (t < size)){
                    ifp.seekg(CurrentFilePTR_E + CurrentFileOffset - (4 * DataBlockSize));
                    while((CurrentFileOffset > (4 * DataBlockSize)) && (CurrentFileOffset < (5 * DataBlockSize)) && (t < size) && ifp.read((char*)&buffer,sizeof(buffer))){
                        std::cout<< buffer;
                        t++;
                        CurrentFileOffset++;
                    }
                }
                if((CurrentFileOffset > (5 * DataBlockSize)) && (CurrentFileOffset < (6 * DataBlockSize)) && (t < size)){
                    ifp.seekg(CurrentFilePTR_F + CurrentFileOffset - (5 * DataBlockSize));
                    while((CurrentFileOffset > (5 * DataBlockSize)) && (CurrentFileOffset < (6 * DataBlockSize)) && (t < size) && ifp.read((char*)&buffer,sizeof(buffer))){
                        std::cout<< buffer;
                        t++;
                        CurrentFileOffset++;
                    }
                }
                if((CurrentFileOffset > (6 * DataBlockSize)) && (CurrentFileOffset < (7 * DataBlockSize)) && (t < size)){
                    ifp.seekg(CurrentFilePTR_G + CurrentFileOffset - (6 * DataBlockSize));
                    while((CurrentFileOffset > (6 * DataBlockSize)) && (CurrentFileOffset < (7 * DataBlockSize)) && (t < size) && ifp.read((char*)&buffer,sizeof(buffer))){
                        std::cout<< buffer;
                        t++;
                        CurrentFileOffset++;
                    }
                }
                if((CurrentFileOffset > (7 * DataBlockSize)) && (CurrentFileOffset < (8 * DataBlockSize)) && (t < size)){
                    ifp.seekg(CurrentFilePTR_H + CurrentFileOffset - (7 * DataBlockSize));
                    while((CurrentFileOffset > (7 * DataBlockSize)) && (CurrentFileOffset < (8 * DataBlockSize)) && (t < size) && ifp.read((char*)&buffer,sizeof(buffer))){
                        std::cout<< buffer;
                        t++;
                        CurrentFileOffset++;
                    }
                }
                if((CurrentFileOffset > (8 * DataBlockSize)) && (CurrentFileOffset < (9 * DataBlockSize)) && (t < size)){
                    ifp.seekg(CurrentFilePTR_I + CurrentFileOffset - (8 * DataBlockSize));
                    while((CurrentFileOffset > (8 * DataBlockSize)) && (CurrentFileOffset < (9 * DataBlockSize)) && (t < size) && ifp.read((char*)&buffer,sizeof(buffer))){
                        std::cout<< buffer;
                        t++;
                        CurrentFileOffset++;
                    }
                }
                if((CurrentFileOffset > (9 * DataBlockSize)) && (CurrentFileOffset < (10 * DataBlockSize)) && (t < size)){
                    ifp.seekg(CurrentFilePTR_J + CurrentFileOffset - (9 * DataBlockSize));
                    while((CurrentFileOffset > (9 * DataBlockSize)) && (CurrentFileOffset < (10 * DataBlockSize)) && (t < size) && ifp.read((char*)&buffer,sizeof(buffer))){
                        std::cout<< buffer;
                        t++;
                        CurrentFileOffset++;
                    }
                }
            }else {
                std::cout<<"****Error Accessing File System Disk****\n";
            }
        }
        
    }else {
        std::cout<<"File is not open for reading\n";
    }
    if(!error)
        std::cout<<"\nread Complete\n\n";
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
            
            if(!error){
                //Determine new size of file after write
                CurrentFileSize = CurrentFileSize + len;

                //Allocate space for write if needed
                if((CurrentFileSize > DataBlockSize) && !CurrentFilePTR_B){
                    int ndb = GetNextDataBlock();
                    if(ndb){
                        CurrentFilePTR_B = ndb;
                    }else {
                        error = true;
                    }
                }
                if((CurrentFileSize > (2 * DataBlockSize)) && !CurrentFilePTR_C && !error){
                    int ndb = GetNextDataBlock();
                    if(ndb){
                        CurrentFilePTR_C = ndb;
                    }else {
                        error = true;
                    }
                }
                if((CurrentFileSize > (3 * DataBlockSize)) && !CurrentFilePTR_D && !error){
                    int ndb = GetNextDataBlock();
                    if(ndb){
                        CurrentFilePTR_D = ndb;
                    }else {
                        error = true;
                    }
                }
                if((CurrentFileSize > (4 * DataBlockSize)) && !CurrentFilePTR_E && !error){
                    int ndb = GetNextDataBlock();
                    if(ndb){
                        CurrentFilePTR_E = ndb;
                    }else {
                        error = true;
                    }
                }
                if((CurrentFileSize > (5 * DataBlockSize)) && !CurrentFilePTR_F && !error){
                    int ndb = GetNextDataBlock();
                    if(ndb){
                        CurrentFilePTR_F = ndb;
                    }else {
                        error = true;
                    }
                }
                if((CurrentFileSize > (6 * DataBlockSize)) && !CurrentFilePTR_G && !error){
                    int ndb = GetNextDataBlock();
                    if(ndb){
                        CurrentFilePTR_G = ndb;
                    }else {
                        error = true;
                    }
                }
                if((CurrentFileSize > (7 * DataBlockSize)) && !CurrentFilePTR_H && !error){
                    int ndb = GetNextDataBlock();
                    if(ndb){
                        CurrentFilePTR_H = ndb;
                    }else {
                        error = true;
                    }
                }
                if((CurrentFileSize > (8 * DataBlockSize)) && !CurrentFilePTR_I && !error){
                    int ndb = GetNextDataBlock();
                    if(ndb){
                        CurrentFilePTR_I = ndb;
                    }else {
                        error = true;
                    }
                }
                if((CurrentFileSize > (9 * DataBlockSize)) && !CurrentFilePTR_J && !error){
                    int ndb = GetNextDataBlock();
                    if(ndb){
                        CurrentFilePTR_J = ndb;
                    }else {
                        error = true;
                    }
                }
                if(!error){
                    //write string to file
                    int t= 0;
                    if((CurrentFileOffset < DataBlockSize) && (t < (len))){
                        fp.seekp(CurrentFilePTR + CurrentFileOffset);
                        while((CurrentFileOffset < DataBlockSize) && (t < (len))){
                            fp.write((char*)&buffer[t],sizeof(buffer[t]));
                            t++;
                            CurrentFileOffset++;
                        }
                    }
                    if((CurrentFileOffset > DataBlockSize) && (CurrentFileOffset < (2 * DataBlockSize)) && (t < (len))){
                        fp.seekp(CurrentFilePTR_B + CurrentFileOffset - DataBlockSize);
                        while((CurrentFileOffset > DataBlockSize) && (CurrentFileOffset < (2 * DataBlockSize)) && (t < (len))){
                            fp.write((char*)&buffer[t],sizeof(buffer[t]));
                            t++;
                            CurrentFileOffset++;
                        }
                    }
                    if((CurrentFileOffset > (2 * DataBlockSize)) && (CurrentFileOffset < (3 * DataBlockSize)) && (t < (len))){
                        fp.seekp(CurrentFilePTR_C + CurrentFileOffset - (2 * DataBlockSize));
                        while((CurrentFileOffset > (2 * DataBlockSize)) && (CurrentFileOffset < (3 * DataBlockSize)) && (t < (len))){
                            fp.write((char*)&buffer[t],sizeof(buffer[t]));
                            t++;
                            CurrentFileOffset++;
                        }
                    }
                    if((CurrentFileOffset > (3 * DataBlockSize)) && (CurrentFileOffset < (4 * DataBlockSize)) && (t < (len))){
                        fp.seekp(CurrentFilePTR_D + CurrentFileOffset - (3 * DataBlockSize));
                        while((CurrentFileOffset > (3 * DataBlockSize)) && (CurrentFileOffset < (4 * DataBlockSize)) && (t < (len))){
                            fp.write((char*)&buffer[t],sizeof(buffer[t]));
                            t++;
                            CurrentFileOffset++;
                        }
                    }
                    if((CurrentFileOffset > (4 * DataBlockSize)) && (CurrentFileOffset < (5 * DataBlockSize)) && (t < (len))){
                        fp.seekp(CurrentFilePTR_E + CurrentFileOffset - (4 * DataBlockSize));
                        while((CurrentFileOffset > (4 * DataBlockSize)) && (CurrentFileOffset < (5 * DataBlockSize)) && (t < (len))){
                            fp.write((char*)&buffer[t],sizeof(buffer[t]));
                            t++;
                            CurrentFileOffset++;
                        }
                    }
                    if((CurrentFileOffset > (5 * DataBlockSize)) && (CurrentFileOffset < (6 * DataBlockSize)) && (t < (len))){
                        fp.seekp(CurrentFilePTR_F + CurrentFileOffset - (5 * DataBlockSize));
                        while((CurrentFileOffset > (5 * DataBlockSize)) && (CurrentFileOffset < (6 * DataBlockSize)) && (t < (len))){
                            fp.write((char*)&buffer[t],sizeof(buffer[t]));
                            t++;
                            CurrentFileOffset++;
                        }
                    }
                    if((CurrentFileOffset > (6 * DataBlockSize)) && (CurrentFileOffset < (7 * DataBlockSize)) && (t < (len))){
                        fp.seekp(CurrentFilePTR_G + CurrentFileOffset - (6 * DataBlockSize));
                        while((CurrentFileOffset > (6 * DataBlockSize)) && (CurrentFileOffset < (7 * DataBlockSize)) && (t < (len))){
                            fp.write((char*)&buffer[t],sizeof(buffer[t]));
                            t++;
                            CurrentFileOffset++;
                        }
                    }
                    if((CurrentFileOffset > (7 * DataBlockSize)) && (CurrentFileOffset < (8 * DataBlockSize)) && (t < (len))){
                        fp.seekp(CurrentFilePTR_H + CurrentFileOffset - (7 * DataBlockSize));
                        while((CurrentFileOffset > (7 * DataBlockSize)) && (CurrentFileOffset < (8 * DataBlockSize)) && (t < (len))){
                            fp.write((char*)&buffer[t],sizeof(buffer[t]));
                            t++;
                            CurrentFileOffset++;
                        }
                    }
                    if((CurrentFileOffset > (8 * DataBlockSize)) && (CurrentFileOffset < (9 * DataBlockSize)) && (t < (len))){
                        fp.seekp(CurrentFilePTR_I + CurrentFileOffset - (8 * DataBlockSize));
                        while((CurrentFileOffset > (8 * DataBlockSize)) && (CurrentFileOffset < (9 * DataBlockSize)) && (t < (len))){
                            fp.write((char*)&buffer[t],sizeof(buffer[t]));
                            t++;
                            CurrentFileOffset++;
                        }
                    }
                    if((CurrentFileOffset > (9 * DataBlockSize)) && (CurrentFileOffset < (10 * DataBlockSize)) && (t < (len))){
                        fp.seekp(CurrentFilePTR_J + CurrentFileOffset - (9 * DataBlockSize));
                        while((CurrentFileOffset > (9 * DataBlockSize)) && (CurrentFileOffset < (10 * DataBlockSize)) && (t < (len))){
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
                }
            }
        
        }else {
            std::cout<<"****Error Accessing File System Disk****\n";
        }
        
    }else {
        std::cout<<"File is not open for writing\n";
    }
    if(!error)
        std::cout<<"write Complete\n\n";
}

void Commands::seek(int fd, int offset){
    if(CurrentFileStatus){
        if(fd== CurrentFileFD){
            CurrentFileOffset += offset;
            std::cout<<"Offset has been adjusted to "<< CurrentFileOffset <<"\n";
        }else{
            std::cout<<"File is not open\n";
        }
    }else{
        std::cout<<"No files are open\n";
    }
    std::cout<<"seek Complete\n\n";
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
    
    indexnode ctrlnodes;
    directory templatdr;
    int datablock;
    bool error = false;
    char newdirname[20];
    
    //open disk
    std::fstream fp("filesystem.dat", std::ios::in|std::ios::out|std::ios::binary);
    
    //ensure "disk" opened correctly
    if (fp){
        
        //find next available data block
        int ndb = GetNextDataBlock();
        if(ndb){
            datablock = ndb;
        }else {
            error = true;
        }
        
        if(!error){
            
            //convert inputed name to char array
            strcpy(newdirname, dirname.c_str());
            
            //find spot in control block for new directory inode
            fp.seekp(0); 
            bool success = false;
            int newdirID;
            int i = 0;
            while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (fp.tellg() < FileSpacePTR))
            {
                if(ctrlnodes.valid == 0){               
                    indexnode tmp = ctrlnodes;
                    fp.seekp(-sizeof(tmp),std::ios::cur);
                    
                    //Create inode for new directory
                    tmp.id = i;
                    tmp.valid = 1;
                    tmp.size = 0;
                    tmp.linkcount = 1;
                    tmp.fptr_a = datablock;
                    tmp.fptr_b = 0;
                    tmp.fptr_c = 0;
                    tmp.fptr_d = 0;
                    tmp.fptr_e = 0;
                    tmp.fptr_f = 0;
                    tmp.fptr_g = 0;
                    tmp.fptr_h = 0;
                    tmp.fptr_i = 0;
                    tmp.fptr_j = 0;
                    fp.write((char*)&tmp, sizeof(tmp));
                    
                    newdirID = tmp.id;
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
            
            //get info of current directory
            int status = GetCurDirInfo();
            if (!status){
                CurrentFileStatus = false;
                std::cout<< "****Error inode for current directory not found****\n";
                error = true;
            }
            
            //error checking
            if(!error){

                //find next blank entry in directory and initialize
                if(CurrentDirSize < DataBlockSize){
                    fp.seekg(CurrentDirPTR);
                }
                if((CurrentDirSize >= DataBlockSize) && (CurrentDirSize < (2 * DataBlockSize))){
                    fp.seekg(CurrentDirPTR_B);
                }
                if((CurrentDirSize >= (2 * DataBlockSize)) && (CurrentDirSize < (3 * DataBlockSize))){
                    fp.seekg(CurrentDirPTR_C);
                }
                if((CurrentDirSize >= (3 * DataBlockSize)) && (CurrentDirSize < (4 * DataBlockSize))){
                    fp.seekg(CurrentDirPTR_D);
                }
                if((CurrentDirSize >= (4 * DataBlockSize)) && (CurrentDirSize < (5 * DataBlockSize))){
                    fp.seekg(CurrentDirPTR_E);
                }
                if((CurrentDirSize >= (5 * DataBlockSize)) && (CurrentDirSize < (6 * DataBlockSize))){
                    fp.seekg(CurrentDirPTR_F);
                }
                if((CurrentDirSize >= (6 * DataBlockSize)) && (CurrentDirSize < (7 * DataBlockSize))){
                    fp.seekg(CurrentDirPTR_G);
                }
                if((CurrentDirSize >= (7 * DataBlockSize)) && (CurrentDirSize < (8 * DataBlockSize))){
                    fp.seekg(CurrentDirPTR_H);
                }
                if((CurrentDirSize >= (8 * DataBlockSize)) && (CurrentDirSize < (9 * DataBlockSize))){
                    fp.seekg(CurrentDirPTR_I);
                }
                if((CurrentDirSize >= (9 * DataBlockSize)) && (CurrentDirSize < (10 * DataBlockSize))){
                    fp.seekg(CurrentDirPTR_J);
                }

                success = false;
                int temp = fp.tellg();
                while(fp.read((char*)&templatdr,sizeof(templatdr)) && (fp.tellg() <= (temp + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, "")== 0){
                        directory tmp = templatdr;
                        strcpy(tmp.dfname, dirname.c_str());
                        tmp.dfid = newdirID;
                        tmp.dir = 1;
                        fp.seekp(-sizeof(tmp),std::ios::cur);
                        fp.write((char*)&tmp, sizeof(tmp));
                        std::cout<<"Directory "<< tmp.dfname << " added to "<< CurrentDir << "\n\n";
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
                    CurrentDirSize += (temp2 - temp);
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
        }
    }else{
        std::cout<<"****Error Accessing File System Disk****\n\n";
    }
}

void Commands::rmdir(std::string dirname){
    
    indexnode ctrlnodes;
    directory templatdr;
    bool error = false;
    bool success = false;
    char deldirname[20];
    int deldirID;
    
    //open disk
    std::fstream fp("filesystem.dat", std::ios::in|std::ios::out|std::ios::binary);
    
    //ensure "disk" opened correctly
    if (fp){
        
        //get info of current directory
        int status = GetCurDirInfo();
        if (!status){
            CurrentFileStatus = false;
            std::cout<< "****Error inode for current directory not found****\n";
            error = true;
        }
        
        if(!error){
            //convert inputed name to char array
            strcpy(deldirname, dirname.c_str());
            
            //go to current directory
            fp.seekg(CurrentDirPTR);

            //Search for directory to be deleted
//add code to check to see if directory is empty or not, only delete if empty
            while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR + DataBlockSize)))
            {
                if(strcmp(templatdr.dfname, deldirname)== 0){
                    if(templatdr.dir == 1){
                        directory tmp = templatdr;
                        deldirID = tmp.dfid;
                        strcpy(tmp.dfname, "");
                        tmp.dfid = 0;
                        fp.seekp(-sizeof(tmp),std::ios::cur);
                        fp.write((char*)&tmp, sizeof(tmp));
                        std::cout<<"Directory "<< deldirname << " removed from "<< CurrentDir << "\n\n";
                        CurrentDirSize -= sizeof(templatdr);
                        success = true; 
                        break;
                    }else {
                        std::cout<< "****Error: This is a File****\n";
                        error = true;
                    }
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_B && !success && !error){
                fp.seekg(CurrentDirPTR_B);

                //Search for directory to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_B + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, deldirname)== 0){
                        if(templatdr.dir == 1){
                            directory tmp = templatdr;
                            deldirID = tmp.dfid;
                            strcpy(tmp.dfname, "");
                            tmp.dfid = 0;
                            fp.seekp(-sizeof(tmp),std::ios::cur);
                            fp.write((char*)&tmp, sizeof(tmp));
                            std::cout<<"Directory "<< deldirname << " removed from "<< CurrentDir << "\n\n";
                            CurrentDirSize -= sizeof(templatdr);
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search           
            if(CurrentDirPTR_C && !success && !error){
                fp.seekg(CurrentDirPTR_C);

                //Search for directory to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_C + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, deldirname)== 0){
                        if(templatdr.dir == 1){
                            directory tmp = templatdr;
                            deldirID = tmp.dfid;
                            strcpy(tmp.dfname, "");
                            tmp.dfid = 0;
                            fp.seekp(-sizeof(tmp),std::ios::cur);
                            fp.write((char*)&tmp, sizeof(tmp));
                            std::cout<<"Directory "<< deldirname << " removed from "<< CurrentDir << "\n\n";
                            CurrentDirSize -= sizeof(templatdr);
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_D && !success && !error){
                fp.seekg(CurrentDirPTR_D);

                //Search for directory to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_D + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, deldirname)== 0){
                        if(templatdr.dir == 1){
                            directory tmp = templatdr;
                            deldirID = tmp.dfid;
                            strcpy(tmp.dfname, "");
                            tmp.dfid = 0;
                            fp.seekp(-sizeof(tmp),std::ios::cur);
                            fp.write((char*)&tmp, sizeof(tmp));
                            std::cout<<"Directory "<< deldirname << " removed from "<< CurrentDir << "\n\n";
                            CurrentDirSize -= sizeof(templatdr);
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_E && !success && !error){
                fp.seekg(CurrentDirPTR_E);

                //Search for directory to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_E + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, deldirname)== 0){
                        if(templatdr.dir == 1){
                            directory tmp = templatdr;
                            deldirID = tmp.dfid;
                            strcpy(tmp.dfname, "");
                            tmp.dfid = 0;
                            fp.seekp(-sizeof(tmp),std::ios::cur);
                            fp.write((char*)&tmp, sizeof(tmp));
                            std::cout<<"Directory "<< deldirname << " removed from "<< CurrentDir << "\n\n";
                            CurrentDirSize -= sizeof(templatdr);
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_F && !success && !error){
                fp.seekg(CurrentDirPTR_F);

                //Search for directory to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_F + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, deldirname)== 0){
                        if(templatdr.dir == 1){
                            directory tmp = templatdr;
                            deldirID = tmp.dfid;
                            strcpy(tmp.dfname, "");
                            tmp.dfid = 0;
                            fp.seekp(-sizeof(tmp),std::ios::cur);
                            fp.write((char*)&tmp, sizeof(tmp));
                            std::cout<<"Directory "<< deldirname << " removed from "<< CurrentDir << "\n\n";
                            CurrentDirSize -= sizeof(templatdr);
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_G && !success && !error){
                fp.seekg(CurrentDirPTR_G);

                //Search for directory to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_G + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, deldirname)== 0){
                        if(templatdr.dir == 1){
                            directory tmp = templatdr;
                            deldirID = tmp.dfid;
                            strcpy(tmp.dfname, "");
                            tmp.dfid = 0;
                            fp.seekp(-sizeof(tmp),std::ios::cur);
                            fp.write((char*)&tmp, sizeof(tmp));
                            std::cout<<"Directory "<< deldirname << " removed from "<< CurrentDir << "\n\n";
                            CurrentDirSize -= sizeof(templatdr);
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_H && !success && !error){
                fp.seekg(CurrentDirPTR_H);

                //Search for directory to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_H + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, deldirname)== 0){
                        if(templatdr.dir == 1){
                            directory tmp = templatdr;
                            deldirID = tmp.dfid;
                            strcpy(tmp.dfname, "");
                            tmp.dfid = 0;
                            fp.seekp(-sizeof(tmp),std::ios::cur);
                            fp.write((char*)&tmp, sizeof(tmp));
                            std::cout<<"Directory "<< deldirname << " removed from "<< CurrentDir << "\n\n";
                            CurrentDirSize -= sizeof(templatdr);
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_I && !success && !error){
                fp.seekg(CurrentDirPTR_I);

                //Search for directory to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_I + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, deldirname)== 0){
                        if(templatdr.dir == 1){
                            directory tmp = templatdr;
                            deldirID = tmp.dfid;
                            strcpy(tmp.dfname, "");
                            tmp.dfid = 0;
                            fp.seekp(-sizeof(tmp),std::ios::cur);
                            fp.write((char*)&tmp, sizeof(tmp));
                            std::cout<<"Directory "<< deldirname << " removed from "<< CurrentDir << "\n\n";
                            CurrentDirSize -= sizeof(templatdr);
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_J && !success && !error){
                fp.seekg(CurrentDirPTR_J);

                //Search for directory to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_J + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, deldirname)== 0){
                        if(templatdr.dir == 1){
                            directory tmp = templatdr;
                            deldirID = tmp.dfid;
                            strcpy(tmp.dfname, "");
                            tmp.dfid = 0;
                            fp.seekp(-sizeof(tmp),std::ios::cur);
                            fp.write((char*)&tmp, sizeof(tmp));
                            std::cout<<"Directory "<< deldirname << " removed from "<< CurrentDir << "\n\n";
                            CurrentDirSize -= sizeof(templatdr);
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            
            if(!error){
                //find inode of deleted directory to invalidate
                fp.seekg(0); 
                success = false;
                while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (fp.tellg() < FileSpacePTR))
                {
                    if(ctrlnodes.id == deldirID){               
                        indexnode tmp = ctrlnodes;
                        fp.seekp(-sizeof(tmp),std::ios::cur);

                        //invalidate inode for directory
                        tmp.valid = 0;
                        fp.write((char*)&tmp, sizeof(tmp));
                        success = true; 
                        break;
                    }
                }
                if (!success){
                    std::cout<< "****Inode of deleted directory was not found to invalidate****\n";
                    error = true;
                }

                //update current directory size data in inode
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
        }
    }else{
        std::cout<<"****Error Accessing File System Disk****\n\n";
    }
}

void Commands::cd(std::string dirname){
    
    indexnode ctrlnodes;
    directory templatdr;
    bool error = false;
    bool success = false;
        
    //convert inputed name to char array
    char desiredirname[20];
    strcpy(desiredirname, dirname.c_str());
    
    //search for special character
    if(strcmp(desiredirname, "~")== 0){
        if (PrevDir[PrevDirPtr - 1]== 0){
            strcpy(CurrentDir, "Root"); //set root as current directory
            CurrentDirPTR = FileSpacePTR; //set root as current directory
            CurrentDirID = 0;
            //get info of current directory
            int status = GetCurDirInfo();
            if (!status){
                CurrentFileStatus = false;
                std::cout<< "****Error information for current directory not found****\n";
                error = true;
            }
            if(!error){
                std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
            }
        } else {
            if (CurrentDirID != 0){
                PrevDir[PrevDirPtr]= CurrentDirID;
                PrevDirPtr -= 2;
                CurrentDirID = PrevDir[PrevDirPtr];
                PrevDirPtr++;
                int status = SearchForDirectoryID(PrevDir[PrevDirPtr]);
                if (!status){
                    CurrentFileStatus = false;
                    std::cout<< "****Error information for current directory not found****\n";
                    error = true;
                }else{
                    success = true;
                }
            }else{
                std::cout<< "****Error current directory is already Root****\n\n";
            }
        }
    }else{
        PrevDir[PrevDirPtr]= CurrentDirID;
        SearchForDirectoryName(desiredirname);
        PrevDirPtr++;
        std::cout<< "Next spot in Array: "<<PrevDir[PrevDirPtr]<< "\n";
    }
}

void Commands::link(std::string src, std::string dest){
    //add code
}

void Commands::unlink(std::string name){
    
    indexnode ctrlnodes;
    directory templatdr;
    bool error = false;
    bool success = false;
    char linkname[20];
    int linkID;
    
    //open disk
    std::fstream fp("filesystem.dat", std::ios::in|std::ios::out|std::ios::binary);
    
    //ensure "disk" opened correctly
    if (fp){
        
        //get info of current directory
        int status = GetCurDirInfo();
        if (!status){
            CurrentFileStatus = false;
            std::cout<< "****Error inode for current directory not found****\n";
            error = true;
        }
        
        if(!error){
            //convert inputed name to char array
            strcpy(linkname, name.c_str());
            
            //go to current directory
            fp.seekg(CurrentDirPTR);

            //Search for link to be deleted
            while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR + DataBlockSize)))
            {
                if(strcmp(templatdr.dfname, linkname)== 0){
                    directory tmp = templatdr;
                    linkID = tmp.dfid;
                    strcpy(tmp.dfname, "");
                    tmp.dfid = 0;
                    fp.seekp(-sizeof(tmp),std::ios::cur);
                    fp.write((char*)&tmp, sizeof(tmp));
                    std::cout<<"Link "<< linkname << " removed from "<< CurrentDir << "\n\n";
                    CurrentDirSize -= sizeof(templatdr);
                    success = true; 
                    break;
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_B && !success && !error){
                fp.seekg(CurrentDirPTR_B);

                //Search for link to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_B + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, linkname)== 0){
                        directory tmp = templatdr;
                        linkID = tmp.dfid;
                        strcpy(tmp.dfname, "");
                        tmp.dfid = 0;
                        fp.seekp(-sizeof(tmp),std::ios::cur);
                        fp.write((char*)&tmp, sizeof(tmp));
                        std::cout<<"Link "<< linkname << " removed from "<< CurrentDir << "\n\n";
                        CurrentDirSize -= sizeof(templatdr);
                        success = true; 
                        break;
                    }
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search           
            if(CurrentDirPTR_C && !success && !error){
                fp.seekg(CurrentDirPTR_C);

                //Search for link to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_C + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, linkname)== 0){
                        directory tmp = templatdr;
                        linkID = tmp.dfid;
                        strcpy(tmp.dfname, "");
                        tmp.dfid = 0;
                        fp.seekp(-sizeof(tmp),std::ios::cur);
                        fp.write((char*)&tmp, sizeof(tmp));
                        std::cout<<"Link "<< linkname << " removed from "<< CurrentDir << "\n\n";
                        CurrentDirSize -= sizeof(templatdr);
                        success = true; 
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_D && !success && !error){
                fp.seekg(CurrentDirPTR_D);

                //Search for link to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_D + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, linkname)== 0){
                        directory tmp = templatdr;
                        linkID = tmp.dfid;
                        strcpy(tmp.dfname, "");
                        tmp.dfid = 0;
                        fp.seekp(-sizeof(tmp),std::ios::cur);
                        fp.write((char*)&tmp, sizeof(tmp));
                        std::cout<<"Link "<< linkname << " removed from "<< CurrentDir << "\n\n";
                        CurrentDirSize -= sizeof(templatdr);
                        success = true; 
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_E && !success && !error){
                fp.seekg(CurrentDirPTR_E);

                //Search for link to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_E + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, linkname)== 0){
                        directory tmp = templatdr;
                        linkID = tmp.dfid;
                        strcpy(tmp.dfname, "");
                        tmp.dfid = 0;
                        fp.seekp(-sizeof(tmp),std::ios::cur);
                        fp.write((char*)&tmp, sizeof(tmp));
                        std::cout<<"Link "<< linkname << " removed from "<< CurrentDir << "\n\n";
                        CurrentDirSize -= sizeof(templatdr);
                        success = true; 
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_F && !success && !error){
                fp.seekg(CurrentDirPTR_F);

                //Search for link to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_F + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, linkname)== 0){
                        directory tmp = templatdr;
                        linkID = tmp.dfid;
                        strcpy(tmp.dfname, "");
                        tmp.dfid = 0;
                        fp.seekp(-sizeof(tmp),std::ios::cur);
                        fp.write((char*)&tmp, sizeof(tmp));
                        std::cout<<"Link "<< linkname << " removed from "<< CurrentDir << "\n\n";
                        CurrentDirSize -= sizeof(templatdr);
                        success = true; 
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_G && !success && !error){
                fp.seekg(CurrentDirPTR_G);

                //Search for link to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_G + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, linkname)== 0){
                        directory tmp = templatdr;
                        linkID = tmp.dfid;
                        strcpy(tmp.dfname, "");
                        tmp.dfid = 0;
                        fp.seekp(-sizeof(tmp),std::ios::cur);
                        fp.write((char*)&tmp, sizeof(tmp));
                        std::cout<<"Link "<< linkname << " removed from "<< CurrentDir << "\n\n";
                        CurrentDirSize -= sizeof(templatdr);
                        success = true; 
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_H && !success && !error){
                fp.seekg(CurrentDirPTR_H);

                //Search for link to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_H + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, linkname)== 0){
                        directory tmp = templatdr;
                        linkID = tmp.dfid;
                        strcpy(tmp.dfname, "");
                        tmp.dfid = 0;
                        fp.seekp(-sizeof(tmp),std::ios::cur);
                        fp.write((char*)&tmp, sizeof(tmp));
                        std::cout<<"Link "<< linkname << " removed from "<< CurrentDir << "\n\n";
                        CurrentDirSize -= sizeof(templatdr);
                        success = true; 
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_I && !success && !error){
                fp.seekg(CurrentDirPTR_I);

                //Search for link to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_I + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, linkname)== 0){
                        directory tmp = templatdr;
                        linkID = tmp.dfid;
                        strcpy(tmp.dfname, "");
                        tmp.dfid = 0;
                        fp.seekp(-sizeof(tmp),std::ios::cur);
                        fp.write((char*)&tmp, sizeof(tmp));
                        std::cout<<"Link "<< linkname << " removed from "<< CurrentDir << "\n\n";
                        CurrentDirSize -= sizeof(templatdr);
                        success = true; 
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_J && !success && !error){
                fp.seekg(CurrentDirPTR_J);

                //Search for link to be deleted
                while(fp.read((char*)&templatdr, sizeof(templatdr)) && (fp.tellg()< (CurrentDirPTR_J + DataBlockSize)))
                {
                    if(strcmp(templatdr.dfname, linkname)== 0){
                        directory tmp = templatdr;
                        linkID = tmp.dfid;
                        strcpy(tmp.dfname, "");
                        tmp.dfid = 0;
                        fp.seekp(-sizeof(tmp),std::ios::cur);
                        fp.write((char*)&tmp, sizeof(tmp));
                        std::cout<<"Link "<< linkname << " removed from "<< CurrentDir << "\n\n";
                        CurrentDirSize -= sizeof(templatdr);
                        success = true; 
                        break;
                    }
                }
            }
            
            if(success){
                //find inode of link to be updated
                fp.seekg(0); 
                success = false;
                while(fp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (fp.tellg() < FileSpacePTR))
                {
                    if(ctrlnodes.id == linkID){               
                        indexnode tmp = ctrlnodes;
                        fp.seekp(-sizeof(tmp),std::ios::cur);

                        //decrement linkcount
                        tmp.linkcount -= 1;
                        if(tmp.linkcount == 0){
                            tmp.valid = 0;
                            tmp.id = -1;
                            tmp.size = 0;
                        }
                        fp.write((char*)&tmp, sizeof(tmp));
                        success = true; 
                        break;
                    }
                }
                if (!success){
                    std::cout<< "****Inode of deleted link was not found to update****\n";
                    error = true;
                }

                if(!error){
                    //update current directory size data in inode
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
            }
        }
    }else{
        std::cout<<"****Error Accessing File System Disk****\n\n";
    }
}

void Commands::stat(std::string name){
    //Search current directory for desired file or directory name
    indexnode ctrlnodes;
    directory templatdr;
    
    bool error = false;
    bool success = false;
    
    //convert inputed name to char array
    char fordname[20]; //name of file or directory
    strcpy(fordname, name.c_str());
    
    //open disk
    std::fstream ifp("filesystem.dat", std::ios::in|std::ios::binary);
    
    //ensure "disk" opened correctly
    if (ifp){
        
        //get info of current directory
        int status = GetCurDirInfo();
        if (!status){
            CurrentFileStatus = false;
            std::cout<< "****Error information for current directory not found****\n";
            error = true;
        }
        
        if(!error){
            
            //go to current directory
            ifp.seekg(CurrentDirPTR);

            //Search for desired item
            while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR + DataBlockSize)) && !error)
            {
                if(strcmp(templatdr.dfname, fordname)== 0){
                    if(templatdr.dir == 1){
                        std::cout<< "\nDirectory\n";
                    }else{
                        std::cout<< "\nFile\n";
                    }
                    
                    //find inode for desired item
                    ifp.seekg(0);
                    success = false;
                    while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                    {
                        if(ctrlnodes.id == templatdr.dfid){               
                            std::cout<<"Inode #: " <<ctrlnodes.id<<"\n";
                            std::cout<<"Item Size: " <<ctrlnodes.size<<" Bytes\n";
                            std::cout<<"Link Count: " <<ctrlnodes.linkcount<<"\n";
                            if(ctrlnodes.valid == 1){
                                std::cout<<"Item is valid\n\n";
                            }else{
                                std::cout<<"Item is invalid\n\n";
                            }
                            success = true; 
                            break;
                        }
                    }
                    if (!success){
                        std::cout<< "****Error inode not found for desired item****\n";
                        error = true;
                    }
                    break;
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_B && !success && !error){
                ifp.seekg(CurrentDirPTR_B);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_B + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fordname)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "\nDirectory\n";
                        }else{
                            std::cout<< "\nFile\n";
                        }
                        //find inode for desired item
                        ifp.seekg(0);
                        success = false;
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){               
                                std::cout<<"Inode #: " <<ctrlnodes.id<<"\n";
                                std::cout<<"Item Size: " <<ctrlnodes.size<<" Bytes\n";
                                std::cout<<"Link Count: " <<ctrlnodes.linkcount<<"\n";
                                if(ctrlnodes.valid == 1){
                                    std::cout<<"Item is valid\n\n";
                                }else{
                                    std::cout<<"Item is invalid\n\n";
                                }
                                success = true; 
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                        break;
                    }
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search           
            if(CurrentDirPTR_C && !success && !error){
                ifp.seekg(CurrentDirPTR_C);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_C + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fordname)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "\nDirectory\n";
                        }else{
                            std::cout<< "\nFile\n";
                        }
                        //find inode for desired item
                        ifp.seekg(0);
                        success = false;
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){               
                                std::cout<<"Inode #: " <<ctrlnodes.id<<"\n";
                                std::cout<<"Item Size: " <<ctrlnodes.size<<" Bytes\n";
                                std::cout<<"Link Count: " <<ctrlnodes.linkcount<<"\n";
                                if(ctrlnodes.valid == 1){
                                    std::cout<<"Item is valid\n\n";
                                }else{
                                    std::cout<<"Item is invalid\n\n";
                                }
                                success = true; 
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_D && !success && !error){
                ifp.seekg(CurrentDirPTR_D);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_D + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fordname)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "\nDirectory\n";
                        }else{
                            std::cout<< "\nFile\n";
                        }
                        //find inode for desired item
                        ifp.seekg(0);
                        success = false;
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){               
                                std::cout<<"Inode #: " <<ctrlnodes.id<<"\n";
                                std::cout<<"Item Size: " <<ctrlnodes.size<<" Bytes\n";
                                std::cout<<"Link Count: " <<ctrlnodes.linkcount<<"\n";
                                if(ctrlnodes.valid == 1){
                                    std::cout<<"Item is valid\n\n";
                                }else{
                                    std::cout<<"Item is invalid\n\n";
                                }
                                success = true; 
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_E && !success && !error){
                ifp.seekg(CurrentDirPTR_E);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_E + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fordname)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "\nDirectory\n";
                        }else{
                            std::cout<< "\nFile\n";
                        }
                        //find inode for desired item
                        ifp.seekg(0);
                        success = false;
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){               
                                std::cout<<"Inode #: " <<ctrlnodes.id<<"\n";
                                std::cout<<"Item Size: " <<ctrlnodes.size<<" Bytes\n";
                                std::cout<<"Link Count: " <<ctrlnodes.linkcount<<"\n";
                                if(ctrlnodes.valid == 1){
                                    std::cout<<"Item is valid\n\n";
                                }else{
                                    std::cout<<"Item is invalid\n\n";
                                }
                                success = true; 
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_F && !success && !error){
                ifp.seekg(CurrentDirPTR_F);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_F + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fordname)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "\nDirectory\n";
                        }else{
                            std::cout<< "\nFile\n";
                        }
                        //find inode for desired item
                        ifp.seekg(0);
                        success = false;
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){               
                                std::cout<<"Inode #: " <<ctrlnodes.id<<"\n";
                                std::cout<<"Item Size: " <<ctrlnodes.size<<" Bytes\n";
                                std::cout<<"Link Count: " <<ctrlnodes.linkcount<<"\n";
                                if(ctrlnodes.valid == 1){
                                    std::cout<<"Item is valid\n\n";
                                }else{
                                    std::cout<<"Item is invalid\n\n";
                                }
                                success = true; 
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_G && !success && !error){
                ifp.seekg(CurrentDirPTR_G);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_G + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fordname)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "\nDirectory\n";
                        }else{
                            std::cout<< "\nFile\n";
                        }
                        //find inode for desired item
                        ifp.seekg(0);
                        success = false;
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){               
                                std::cout<<"Inode #: " <<ctrlnodes.id<<"\n";
                                std::cout<<"Item Size: " <<ctrlnodes.size<<" Bytes\n";
                                std::cout<<"Link Count: " <<ctrlnodes.linkcount<<"\n";
                                if(ctrlnodes.valid == 1){
                                    std::cout<<"Item is valid\n\n";
                                }else{
                                    std::cout<<"Item is invalid\n\n";
                                }
                                success = true; 
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_H && !success && !error){
                ifp.seekg(CurrentDirPTR_H);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_H + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fordname)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "\nDirectory\n";
                        }else{
                            std::cout<< "\nFile\n";
                        }
                        //find inode for desired item
                        ifp.seekg(0);
                        success = false;
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){               
                                std::cout<<"Inode #: " <<ctrlnodes.id<<"\n";
                                std::cout<<"Item Size: " <<ctrlnodes.size<<" Bytes\n";
                                std::cout<<"Link Count: " <<ctrlnodes.linkcount<<"\n";
                                if(ctrlnodes.valid == 1){
                                    std::cout<<"Item is valid\n\n";
                                }else{
                                    std::cout<<"Item is invalid\n\n";
                                }
                                success = true; 
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_I && !success && !error){
                ifp.seekg(CurrentDirPTR_I);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_I + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fordname)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "\nDirectory\n";
                        }else{
                            std::cout<< "\nFile\n";
                        }
                        //find inode for desired item
                        ifp.seekg(0);
                        success = false;
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){               
                                std::cout<<"Inode #: " <<ctrlnodes.id<<"\n";
                                std::cout<<"Item Size: " <<ctrlnodes.size<<" Bytes\n";
                                std::cout<<"Link Count: " <<ctrlnodes.linkcount<<"\n";
                                if(ctrlnodes.valid == 1){
                                    std::cout<<"Item is valid\n\n";
                                }else{
                                    std::cout<<"Item is invalid\n\n";
                                }
                                success = true; 
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                        break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_J && !success && !error){
                ifp.seekg(CurrentDirPTR_J);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_J + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fordname)== 0){
                        if(templatdr.dir == 1){
                            std::cout<< "\nDirectory\n";
                        }else{
                            std::cout<< "\nFile\n";
                        }
                        //find inode for desired item
                        ifp.seekg(0);
                        success = false;
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){               
                                std::cout<<"Inode #: " <<ctrlnodes.id<<"\n";
                                std::cout<<"Item Size: " <<ctrlnodes.size<<" Bytes\n";
                                std::cout<<"Link Count: " <<ctrlnodes.linkcount<<"\n";
                                if(ctrlnodes.valid == 1){
                                    std::cout<<"Item is valid\n\n";
                                }else{
                                    std::cout<<"Item is invalid\n\n";
                                }
                                success = true; 
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                        break;
                    }
                }
            }
        }
    }else{
        std::cout<<"****Error Accessing File System Disk****\n\n";
    }
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
    
    //Search current directory for desired file name
    indexnode ctrlnodes;
    directory templatdr;
    
    bool error = false;
    bool success = false;
    
    //convert inputed name to char array
    char fname[20]; //name of file or directory
    strcpy(fname, filename.c_str());
    
    //open disk
    std::fstream ifp("filesystem.dat", std::ios::in|std::ios::binary);
    
    //ensure "disk" opened correctly
    if (ifp){
        
        //get info of current directory
        int status = GetCurDirInfo();
        if (!status){
            CurrentFileStatus = false;
            std::cout<< "****Error information for current directory not found****\n";
            error = true;
        }
        
        if(!error){
            
            //go to current directory
            ifp.seekg(CurrentDirPTR);

            //Search for desired item
            while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR + DataBlockSize)) && !error)
            {
                if(strcmp(templatdr.dfname, fname)== 0){
                    if(templatdr.dir == 0){
                        //find inode for desired item
                        ifp.seekg(0);
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){               
                                std::cout<< "\nFile: " <<fname<<"\n"; //Name of source file
                                SrcFileID= ctrlnodes.id; //inode id# for source file
                                SrcFileSize= ctrlnodes.size; //Size of source file
                                SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                //Display entire file contents
                                int status = ReadSrcFile();
                                if (!status){
                                    std::cout<< "****Error contents of file not displayed****\n";
                                    error = true;
                                }else{
                                    success = true; 
                                }
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                    }else{
                        std::cout<< "\n*****Error: This is a Directory*******\n";
                        error = true;
                    }
                break;
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_B && !success && !error){
                ifp.seekg(CurrentDirPTR_B);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_B + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){               
                                    std::cout<< "\nFile: " <<fname<<"\n"; //Name of source file
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Display entire file contents
                                    int status = ReadSrcFile();
                                    if (!status){
                                        std::cout<< "****Error contents of file not displayed****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search           
            if(CurrentDirPTR_C && !success && !error){
                ifp.seekg(CurrentDirPTR_C);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_C + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){               
                                    std::cout<< "\nFile: " <<fname<<"\n"; //Name of source file
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Display entire file contents
                                    int status = ReadSrcFile();
                                    if (!status){
                                        std::cout<< "****Error contents of file not displayed****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_D && !success && !error){
                ifp.seekg(CurrentDirPTR_D);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_D + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){               
                                    std::cout<< "\nFile: " <<fname<<"\n"; //Name of source file
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Display entire file contents
                                    int status = ReadSrcFile();
                                    if (!status){
                                        std::cout<< "****Error contents of file not displayed****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_E && !success && !error){
                ifp.seekg(CurrentDirPTR_E);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_E + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){               
                                    std::cout<< "\nFile: " <<fname<<"\n"; //Name of source file
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Display entire file contents
                                    int status = ReadSrcFile();
                                    if (!status){
                                        std::cout<< "****Error contents of file not displayed****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_F && !success && !error){
                ifp.seekg(CurrentDirPTR_F);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_F + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){               
                                    std::cout<< "\nFile: " <<fname<<"\n"; //Name of source file
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Display entire file contents
                                    int status = ReadSrcFile();
                                    if (!status){
                                        std::cout<< "****Error contents of file not displayed****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_G && !success && !error){
                ifp.seekg(CurrentDirPTR_G);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_G + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){               
                                    std::cout<< "\nFile: " <<fname<<"\n"; //Name of source file
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Display entire file contents
                                    int status = ReadSrcFile();
                                    if (!status){
                                        std::cout<< "****Error contents of file not displayed****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_H && !success && !error){
                ifp.seekg(CurrentDirPTR_H);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_H + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){               
                                    std::cout<< "\nFile: " <<fname<<"\n"; //Name of source file
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Display entire file contents
                                    int status = ReadSrcFile();
                                    if (!status){
                                        std::cout<< "****Error contents of file not displayed****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_I && !success && !error){
                ifp.seekg(CurrentDirPTR_I);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_I + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){               
                                    std::cout<< "\nFile: " <<fname<<"\n"; //Name of source file
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Display entire file contents
                                    int status = ReadSrcFile();
                                    if (!status){
                                        std::cout<< "****Error contents of file not displayed****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_J && !success && !error){
                ifp.seekg(CurrentDirPTR_J);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_J + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){               
                                    std::cout<< "\nFile: " <<fname<<"\n"; //Name of source file
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Display entire file contents
                                    int status = ReadSrcFile();
                                    if (!status){
                                        std::cout<< "****Error contents of file not displayed****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
        }
    }else{
        std::cout<<"****Error Accessing File System Disk****\n\n";
    }
}

void Commands::cp(std::string src, std::string dest){
    
    //Search current directory for desired file name
    indexnode ctrlnodes;
    directory templatdr;
    
    bool error = false;
    bool success = false;
    
    //source file name to char array
    char sour[src.length()];
    strcpy(sour, src.c_str());
    //get info of source file
    
    //open filesystem "disk" for reading and writing
    std::fstream ifp ("filesystem.dat", std::ios::in | std::ios::binary);
        
    //ensure "disk" opened correctly
    if (ifp){

        //get info of current directory
        int status = GetCurDirInfo();
        if (!status){
            CurrentFileStatus = false;
            std::cout<< "****Error information for current directory not found****\n";
            error = true;
        }

        if(!error){

            //go to current directory
            ifp.seekg(CurrentDirPTR);

            //Search for desired item
            while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR + DataBlockSize)) && !error)
            {
                if(strcmp(templatdr.dfname, sour)== 0){
                    if(templatdr.dir == 0){
                        //find inode for desired item
                        ifp.seekg(0);
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){
                                SrcFileID= ctrlnodes.id; //inode id# for source file
                                SrcFileSize= ctrlnodes.size; //Size of source file
                                SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                //Export entire file contents
                                int status = SrcToDest(dest);
                                if (!status){
                                    std::cout<< "****Error contents of file not exported****\n";
                                    error = true;
                                }else{
                                    success = true; 
                                }
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                    }else{
                        std::cout<< "\n*****Error: This is a Directory*******\n";
                        error = true;
                    }
                break;
                }

            }

            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_B && !success && !error){
                ifp.seekg(CurrentDirPTR_B);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_B + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, sour)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = SrcToDest(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }

                }
            }

            //go to next block of current directory, if it exists, to continue the search           
            if(CurrentDirPTR_C && !success && !error){
                ifp.seekg(CurrentDirPTR_C);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_C + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, sour)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = SrcToDest(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_D && !success && !error){
                ifp.seekg(CurrentDirPTR_D);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_D + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, sour)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = SrcToDest(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_E && !success && !error){
                ifp.seekg(CurrentDirPTR_E);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_E + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, sour)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = SrcToDest(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_F && !success && !error){
                ifp.seekg(CurrentDirPTR_F);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_F + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, sour)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = SrcToDest(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_G && !success && !error){
                ifp.seekg(CurrentDirPTR_G);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_G + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, sour)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = SrcToDest(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_H && !success && !error){
                ifp.seekg(CurrentDirPTR_H);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_H + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, sour)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = SrcToDest(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_I && !success && !error){
                ifp.seekg(CurrentDirPTR_I);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_I + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, sour)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = SrcToDest(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_J && !success && !error){
                ifp.seekg(CurrentDirPTR_J);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_J + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, sour)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = SrcToDest(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
        }
    }else{
    std::cout<<"****Error Accessing File System Disk****\n\n";
    }
}

void Commands::tree(){
    //add code
}

void Commands::import(std::string src, std::string dest){
    
    //open external file
    char sour[src.length()];
    strcpy(sour, src.c_str());
    std::fstream ifp(sour, std::ios::in);
    
    //create internal file;
    DestFileID = open(dest, "w");
    
    //read in external file to memory
    ifp.seekg(0,ifp.end);
    int len = ifp.tellg();
    char* buffer = new char[len];
    ifp.seekg(0,ifp.beg);
    ifp.read(buffer, len);
    
    //write external file to 
    write(DestFileID, buffer);
    
}

void Commands::fexport(std::string src, std::string dest){
    
    //Search current directory for desired file name
    indexnode ctrlnodes;
    directory templatdr;
    
    bool error = false;
    bool success = false;
    
    //convert inputed name to char array
    char fname[20]; //name of file or directory
    strcpy(fname, src.c_str());
    
    //open disk
    std::fstream ifp("filesystem.dat", std::ios::in|std::ios::binary);
    
    //ensure "disk" opened correctly
    if (ifp){
        
        //get info of current directory
        int status = GetCurDirInfo();
        if (!status){
            CurrentFileStatus = false;
            std::cout<< "****Error information for current directory not found****\n";
            error = true;
        }
        
        if(!error){
            
            //go to current directory
            ifp.seekg(CurrentDirPTR);

            //Search for desired item
            while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR + DataBlockSize)) && !error)
            {
                if(strcmp(templatdr.dfname, fname)== 0){
                    if(templatdr.dir == 0){
                        //find inode for desired item
                        ifp.seekg(0);
                        while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                        {
                            if(ctrlnodes.id == templatdr.dfid){
                                SrcFileID= ctrlnodes.id; //inode id# for source file
                                SrcFileSize= ctrlnodes.size; //Size of source file
                                SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                //Export entire file contents
                                int status = ExportSrcFile(dest);
                                if (!status){
                                    std::cout<< "****Error contents of file not exported****\n";
                                    error = true;
                                }else{
                                    success = true; 
                                }
                                break;
                            }
                        }
                        if (!success){
                            std::cout<< "****Error inode not found for desired item****\n";
                            error = true;
                        }
                    }else{
                        std::cout<< "\n*****Error: This is a Directory*******\n";
                        error = true;
                    }
                break;
                }
                
            }
            
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_B && !success && !error){
                ifp.seekg(CurrentDirPTR_B);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_B + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = ExportSrcFile(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                    
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search           
            if(CurrentDirPTR_C && !success && !error){
                ifp.seekg(CurrentDirPTR_C);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_C + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = ExportSrcFile(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_D && !success && !error){
                ifp.seekg(CurrentDirPTR_D);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_D + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = ExportSrcFile(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_E && !success && !error){
                ifp.seekg(CurrentDirPTR_E);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_E + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = ExportSrcFile(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_F && !success && !error){
                ifp.seekg(CurrentDirPTR_F);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_F + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = ExportSrcFile(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_G && !success && !error){
                ifp.seekg(CurrentDirPTR_G);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_G + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = ExportSrcFile(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_H && !success && !error){
                ifp.seekg(CurrentDirPTR_H);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_H + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = ExportSrcFile(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_I && !success && !error){
                ifp.seekg(CurrentDirPTR_I);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_I + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = ExportSrcFile(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_J && !success && !error){
                ifp.seekg(CurrentDirPTR_J);

                //Search for desired file
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_J + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, fname)== 0){
                        if(templatdr.dir == 0){
                            //find inode for desired item
                            ifp.seekg(0);
                            while(ifp.read((char*)&ctrlnodes, sizeof(ctrlnodes)) && (ifp.tellg() < FileSpacePTR))
                            {
                                if(ctrlnodes.id == templatdr.dfid){
                                    SrcFileID= ctrlnodes.id; //inode id# for source file
                                    SrcFileSize= ctrlnodes.size; //Size of source file
                                    SrcFilePTR= ctrlnodes.fptr_a; //Pointer for source file
                                    SrcFilePTR_B= ctrlnodes.fptr_b; //2nd Pointer for source file
                                    SrcFilePTR_C= ctrlnodes.fptr_c; //3rd Pointer for source file
                                    SrcFilePTR_D= ctrlnodes.fptr_d; //4th Pointer for source file
                                    SrcFilePTR_E= ctrlnodes.fptr_e; //5th Pointer for source file
                                    SrcFilePTR_F= ctrlnodes.fptr_f; //6th Pointer for source file
                                    SrcFilePTR_G= ctrlnodes.fptr_g; //7th Pointer for source file
                                    SrcFilePTR_H= ctrlnodes.fptr_h; //8th Pointer for source file
                                    SrcFilePTR_I= ctrlnodes.fptr_i; //9th Pointer for source file
                                    SrcFilePTR_J= ctrlnodes.fptr_j; //10th Pointer for source file

                                    //Export entire file contents
                                    int status = ExportSrcFile(dest);
                                    if (!status){
                                        std::cout<< "****Error contents of file not exported****\n";
                                        error = true;
                                    }else{
                                        success = true; 
                                    }
                                    break;
                                }
                            }
                            if (!success){
                                std::cout<< "****Error inode not found for desired item****\n";
                                error = true;
                            }
                        }else{
                            std::cout<< "\n*****Error: This is a Directory*******\n";
                            error = true;
                        }
                    break;
                    }
                }
            }
        }
    }else{
        std::cout<<"****Error Accessing File System Disk****\n\n";
    }
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
        return 0;
    }
}

int Commands::GetCurDirInfo(){
    
    indexnode ctrlnodes;
    bool success = false;
    
    //open filesystem "disk" for reading
    std::ifstream ifp ("filesystem.dat", std::ios::in | std::ios::binary);
    
    if(ifp){

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
        if(success){
            return 1;
        }else{
            return 0;
        }
        
    }else{
        std::cout<<"****Error Accessing File System Disk****\n";
        return 0;
    }
}

int Commands::SearchForDirectoryName(char desiredir[20]){
    //Searches directory set by ID # to be current for desired directory name
    indexnode ctrlnodes;
    directory templatdr;
    bool error = false;
    bool success = false;
    
    //open disk
    std::fstream ifp("filesystem.dat", std::ios::in|std::ios::binary);
    
    //ensure "disk" opened correctly
    if (ifp){
        
        //get info of current directory
        int status = GetCurDirInfo();
        if (!status){
            CurrentFileStatus = false;
            std::cout<< "****Error information for current directory not found****\n";
            error = true;
        }
        
        if(!error){
            
            //go to current directory
            ifp.seekg(CurrentDirPTR);

            //Search for desired directory
            while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR + DataBlockSize)) && !error)
            {
                if(strcmp(templatdr.dfname, desiredir)== 0){
                    if(templatdr.dir == 1){
                        CurrentDirID = templatdr.dfid;
                        strcpy(CurrentDir, templatdr.dfname);
                        int status = GetCurDirInfo();
                        if (!status){
                            CurrentFileStatus = false;
                            std::cout<< "****Error information for current directory not found****\n";
                            error = true;
                        }
                        std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                        success = true; 
                        break;
                    }else {
                        std::cout<< "****Error: This is a File****\n";
                        error = true;
                    }
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_B && !success && !error){
                ifp.seekg(CurrentDirPTR_B);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_B + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, desiredir)== 0){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search           
            if(CurrentDirPTR_C && !success && !error){
                ifp.seekg(CurrentDirPTR_C);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_C + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, desiredir)== 0){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_D && !success && !error){
                ifp.seekg(CurrentDirPTR_D);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_D + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, desiredir)== 0){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_E && !success && !error){
                ifp.seekg(CurrentDirPTR_E);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_E + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, desiredir)== 0){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_F && !success && !error){
                ifp.seekg(CurrentDirPTR_F);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_F + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, desiredir)== 0){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_G && !success && !error){
                ifp.seekg(CurrentDirPTR_G);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_G + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, desiredir)== 0){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_H && !success && !error){
                ifp.seekg(CurrentDirPTR_H);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_H + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, desiredir)== 0){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_I && !success && !error){
                ifp.seekg(CurrentDirPTR_I);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_I + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, desiredir)== 0){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_J && !success && !error){
                ifp.seekg(CurrentDirPTR_J);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_J + DataBlockSize)) && !error)
                {
                    if(strcmp(templatdr.dfname, desiredir)== 0){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
        }
        if(success){
            return 1;
        }else{
            return 0;
        }
    }else{
        std::cout<<"****Error Accessing File System Disk****\n\n";
        return 0;
    }
}

int Commands::SearchForDirectoryID(int ID){
    //Searches directory set by ID # to be current for desired directory ID #
    indexnode ctrlnodes;
    directory templatdr;
    bool error = false;
    bool success = false;
    
    //open disk
    std::fstream ifp("filesystem.dat", std::ios::in|std::ios::binary);
    
    //ensure "disk" opened correctly
    if (ifp){
        
        //get info of current directory
        int status = GetCurDirInfo();
        if (!status){
            CurrentFileStatus = false;
            std::cout<< "****Error information for current directory not found****\n";
            error = true;
        }
        
        if(!error){
            
            //go to current directory
            ifp.seekg(CurrentDirPTR);

            //Search for desired directory
            while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR + DataBlockSize)) && !error)
            {
                if(templatdr.dfid == ID){
                    if(templatdr.dir == 1){
                        CurrentDirID = templatdr.dfid;
                        strcpy(CurrentDir, templatdr.dfname);
                        int status = GetCurDirInfo();
                        if (!status){
                            CurrentFileStatus = false;
                            std::cout<< "****Error information for current directory not found****\n";
                            error = true;
                        }
                        std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                        success = true; 
                        break;
                    }else {
                        std::cout<< "****Error: This is a File****\n";
                        error = true;
                    }
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_B && !success && !error){
                ifp.seekg(CurrentDirPTR_B);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_B + DataBlockSize)) && !error)
                {
                    if(templatdr.dfid == ID){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            
            //go to next block of current directory, if it exists, to continue the search           
            if(CurrentDirPTR_C && !success && !error){
                ifp.seekg(CurrentDirPTR_C);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_C + DataBlockSize)) && !error)
                {
                    if(templatdr.dfid == ID){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_D && !success && !error){
                ifp.seekg(CurrentDirPTR_D);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_D + DataBlockSize)) && !error)
                {
                    if(templatdr.dfid == ID){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_E && !success && !error){
                ifp.seekg(CurrentDirPTR_E);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_E + DataBlockSize)) && !error)
                {
                    if(templatdr.dfid == ID){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_F && !success && !error){
                ifp.seekg(CurrentDirPTR_F);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_F + DataBlockSize)) && !error)
                {
                    if(templatdr.dfid == ID){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_G && !success && !error){
                ifp.seekg(CurrentDirPTR_G);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_G + DataBlockSize)) && !error)
                {
                    if(templatdr.dfid == ID){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_H && !success && !error){
                ifp.seekg(CurrentDirPTR_H);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_H + DataBlockSize)) && !error)
                {
                    if(templatdr.dfid == ID){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_I && !success && !error){
                ifp.seekg(CurrentDirPTR_I);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_I + DataBlockSize)) && !error)
                {
                    if(templatdr.dfid == ID){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
            //go to next block of current directory, if it exists, to continue the search
            if(CurrentDirPTR_J && !success && !error){
                ifp.seekg(CurrentDirPTR_J);

                //Search for desired directory
                while(ifp.read((char*)&templatdr, sizeof(templatdr)) && (ifp.tellg()< (CurrentDirPTR_J + DataBlockSize)) && !error)
                {
                    if(templatdr.dfid == ID){
                        if(templatdr.dir == 1){
                            CurrentDirID = templatdr.dfid;
                            strcpy(CurrentDir, templatdr.dfname);
                            int status = GetCurDirInfo();
                            if (!status){
                                CurrentFileStatus = false;
                                std::cout<< "****Error information for current directory not found****\n";
                                error = true;
                            }
                            std::cout<<"The current directory is now "<< CurrentDir << "\n\n";
                            success = true; 
                            break;
                        }else {
                            std::cout<< "****Error: This is a File****\n";
                            error = true;
                        }
                    }
                }
            }
        }
        if(success){
            return 1;
        }else{
            return 0;
        }
    }else{
        std::cout<<"****Error Accessing File System Disk****\n\n";
        return 0;
    }
}

int Commands::ReadSrcFile(){
    bool error = false;
    indexnode ctrlnodes;
    //open filesystem "disk" for reading and writing
    std::fstream ifp ("filesystem.dat", std::ios::in|std::ios::binary);

    //ensure "disk" opened correctly
    if (ifp){

        char buffer;
        if(!error){
            SrcFileOffset = 0;
            if((SrcFileOffset < DataBlockSize) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR);
                while((SrcFileOffset < DataBlockSize) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    std::cout<< buffer;
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > DataBlockSize) && (SrcFileOffset < (2 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_B + SrcFileOffset - DataBlockSize);
                while((SrcFileOffset > DataBlockSize) && (SrcFileOffset < (2 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    std::cout<< buffer;
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (2 * DataBlockSize)) && (SrcFileOffset < (3 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_C + SrcFileOffset - (2 * DataBlockSize));
                while((SrcFileOffset > (2 * DataBlockSize)) && (SrcFileOffset < (3 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    std::cout<< buffer;
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (3 * DataBlockSize)) && (SrcFileOffset < (4 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_D + SrcFileOffset - (3 * DataBlockSize));
                while((SrcFileOffset > (3 * DataBlockSize)) && (SrcFileOffset < (4 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    std::cout<< buffer;
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (4 * DataBlockSize)) && (SrcFileOffset < (5 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_E + SrcFileOffset - (4 * DataBlockSize));
                while((SrcFileOffset > (4 * DataBlockSize)) && (SrcFileOffset < (5 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    std::cout<< buffer;
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (5 * DataBlockSize)) && (SrcFileOffset < (6 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_F + SrcFileOffset - (5 * DataBlockSize));
                while((SrcFileOffset > (5 * DataBlockSize)) && (SrcFileOffset < (6 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    std::cout<< buffer;
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (6 * DataBlockSize)) && (SrcFileOffset < (7 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_G + SrcFileOffset - (6 * DataBlockSize));
                while((SrcFileOffset > (6 * DataBlockSize)) && (SrcFileOffset < (7 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    std::cout<< buffer;
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (7 * DataBlockSize)) && (SrcFileOffset < (8 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_H + SrcFileOffset - (7 * DataBlockSize));
                while((SrcFileOffset > (7 * DataBlockSize)) && (SrcFileOffset < (8 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    std::cout<< buffer;
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (8 * DataBlockSize)) && (SrcFileOffset < (9 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_I + SrcFileOffset - (8 * DataBlockSize));
                while((SrcFileOffset > (8 * DataBlockSize)) && (SrcFileOffset < (9 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    std::cout<< buffer;
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (9 * DataBlockSize)) && (SrcFileOffset < (10 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_J + SrcFileOffset - (9 * DataBlockSize));
                while((SrcFileOffset > (9 * DataBlockSize)) && (SrcFileOffset < (10 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    std::cout<< buffer;
                    SrcFileOffset++;
                }
            }
            std::cout<< "\n\n";
            return 1;
        }else {
            std::cout<<"****Error Accessing File System Disk****\n\n";
            return 0;
        }
    }else{
        std::cout<<"****Error Accessing File System Disk****\n\n";
        return 0;
    }
}

int Commands::ExportSrcFile(std::string dest){
    bool error = false;
    indexnode ctrlnodes;
    char dst[dest.length()];
    //open filesystem "disk" for reading
    std::fstream ifp ("filesystem.dat", std::ios::in|std::ios::binary);
    
    strcpy(dst, dest.c_str());
    //open external file
    std::fstream ofp(dst, std::ios::out);
    
    //ensure "disk" opened correctly
    if (ifp && ofp){

        char buffer;
        
        SrcFileOffset = 0;
        if((SrcFileOffset < DataBlockSize) && (SrcFileOffset < SrcFileSize)){
            ifp.seekg(SrcFilePTR);
            while((SrcFileOffset < DataBlockSize) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                ofp.write((char*)&buffer,sizeof(buffer));
                SrcFileOffset++;
            }
        }
        if((SrcFileOffset > DataBlockSize) && (SrcFileOffset < (2 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
            ifp.seekg(SrcFilePTR_B + SrcFileOffset - DataBlockSize);
            while((SrcFileOffset > DataBlockSize) && (SrcFileOffset < (2 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                ofp.write((char*)&buffer,sizeof(buffer));
                SrcFileOffset++;
            }
        }
        if((SrcFileOffset > (2 * DataBlockSize)) && (SrcFileOffset < (3 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
            ifp.seekg(SrcFilePTR_C + SrcFileOffset - (2 * DataBlockSize));
            while((SrcFileOffset > (2 * DataBlockSize)) && (SrcFileOffset < (3 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                ofp.write((char*)&buffer,sizeof(buffer));
                SrcFileOffset++;
            }
        }
        if((SrcFileOffset > (3 * DataBlockSize)) && (SrcFileOffset < (4 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
            ifp.seekg(SrcFilePTR_D + SrcFileOffset - (3 * DataBlockSize));
            while((SrcFileOffset > (3 * DataBlockSize)) && (SrcFileOffset < (4 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                ofp.write((char*)&buffer,sizeof(buffer));
                SrcFileOffset++;
            }
        }
        if((SrcFileOffset > (4 * DataBlockSize)) && (SrcFileOffset < (5 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
            ifp.seekg(SrcFilePTR_E + SrcFileOffset - (4 * DataBlockSize));
            while((SrcFileOffset > (4 * DataBlockSize)) && (SrcFileOffset < (5 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                ofp.write((char*)&buffer,sizeof(buffer));
                SrcFileOffset++;
            }
        }
        if((SrcFileOffset > (5 * DataBlockSize)) && (SrcFileOffset < (6 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
            ifp.seekg(SrcFilePTR_F + SrcFileOffset - (5 * DataBlockSize));
            while((SrcFileOffset > (5 * DataBlockSize)) && (SrcFileOffset < (6 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                ofp.write((char*)&buffer,sizeof(buffer));
                SrcFileOffset++;
            }
        }
        if((SrcFileOffset > (6 * DataBlockSize)) && (SrcFileOffset < (7 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
            ifp.seekg(SrcFilePTR_G + SrcFileOffset - (6 * DataBlockSize));
            while((SrcFileOffset > (6 * DataBlockSize)) && (SrcFileOffset < (7 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                ofp.write((char*)&buffer,sizeof(buffer));
                SrcFileOffset++;
            }
        }
        if((SrcFileOffset > (7 * DataBlockSize)) && (SrcFileOffset < (8 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
            ifp.seekg(SrcFilePTR_H + SrcFileOffset - (7 * DataBlockSize));
            while((SrcFileOffset > (7 * DataBlockSize)) && (SrcFileOffset < (8 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                ofp.write((char*)&buffer,sizeof(buffer));
                SrcFileOffset++;
            }
        }
        if((SrcFileOffset > (8 * DataBlockSize)) && (SrcFileOffset < (9 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
            ifp.seekg(SrcFilePTR_I + SrcFileOffset - (8 * DataBlockSize));
            while((SrcFileOffset > (8 * DataBlockSize)) && (SrcFileOffset < (9 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                ofp.write((char*)&buffer,sizeof(buffer));
                SrcFileOffset++;
            }
        }
        if((SrcFileOffset > (9 * DataBlockSize)) && (SrcFileOffset < (10 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
            ifp.seekg(SrcFilePTR_J + SrcFileOffset - (9 * DataBlockSize));
            while((SrcFileOffset > (9 * DataBlockSize)) && (SrcFileOffset < (10 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                ofp.write((char*)&buffer,sizeof(buffer));
                SrcFileOffset++;
            }
        }
        return 1;
    }else {
        std::cout<<"****Error File I/O Issue****\n\n";
        return 0;
    }
}

int Commands::SrcToDest(std::string dest){
    bool error = false;
    indexnode ctrlnodes;
    
    //create destination file
    DestFileID = open(dest, "w");
    if(DestFileID== -1){
        error = true;
        std::cout<<"****Error Destination file not created****\n\n";
    }else{

        //open filesystem "disk" for reading and writing
        std::fstream ifp ("filesystem.dat", std::ios::in|std::ios::binary);

        //ensure "disk" opened correctly
        if (ifp){

            char buffer;

            SrcFileOffset = 0;
            if((SrcFileOffset < DataBlockSize) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR);
                while((SrcFileOffset < DataBlockSize) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    write(DestFileID, (char*)&buffer);
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > DataBlockSize) && (SrcFileOffset < (2 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_B + SrcFileOffset - DataBlockSize);
                while((SrcFileOffset > DataBlockSize) && (SrcFileOffset < (2 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    write(DestFileID, (char*)&buffer);
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (2 * DataBlockSize)) && (SrcFileOffset < (3 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_C + SrcFileOffset - (2 * DataBlockSize));
                while((SrcFileOffset > (2 * DataBlockSize)) && (SrcFileOffset < (3 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    write(DestFileID, (char*)&buffer);
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (3 * DataBlockSize)) && (SrcFileOffset < (4 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_D + SrcFileOffset - (3 * DataBlockSize));
                while((SrcFileOffset > (3 * DataBlockSize)) && (SrcFileOffset < (4 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    write(DestFileID, (char*)&buffer);
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (4 * DataBlockSize)) && (SrcFileOffset < (5 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_E + SrcFileOffset - (4 * DataBlockSize));
                while((SrcFileOffset > (4 * DataBlockSize)) && (SrcFileOffset < (5 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    write(DestFileID, (char*)&buffer);
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (5 * DataBlockSize)) && (SrcFileOffset < (6 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_F + SrcFileOffset - (5 * DataBlockSize));
                while((SrcFileOffset > (5 * DataBlockSize)) && (SrcFileOffset < (6 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    write(DestFileID, (char*)&buffer);
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (6 * DataBlockSize)) && (SrcFileOffset < (7 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_G + SrcFileOffset - (6 * DataBlockSize));
                while((SrcFileOffset > (6 * DataBlockSize)) && (SrcFileOffset < (7 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    write(DestFileID, (char*)&buffer);
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (7 * DataBlockSize)) && (SrcFileOffset < (8 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_H + SrcFileOffset - (7 * DataBlockSize));
                while((SrcFileOffset > (7 * DataBlockSize)) && (SrcFileOffset < (8 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    write(DestFileID, (char*)&buffer);
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (8 * DataBlockSize)) && (SrcFileOffset < (9 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_I + SrcFileOffset - (8 * DataBlockSize));
                while((SrcFileOffset > (8 * DataBlockSize)) && (SrcFileOffset < (9 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    write(DestFileID, (char*)&buffer);
                    SrcFileOffset++;
                }
            }
            if((SrcFileOffset > (9 * DataBlockSize)) && (SrcFileOffset < (10 * DataBlockSize)) && (SrcFileOffset < SrcFileSize)){
                ifp.seekg(SrcFilePTR_J + SrcFileOffset - (9 * DataBlockSize));
                while((SrcFileOffset > (9 * DataBlockSize)) && (SrcFileOffset < (10 * DataBlockSize)) && (SrcFileOffset < SrcFileSize) && ifp.read((char*)&buffer,sizeof(buffer))){
                    write(DestFileID, (char*)&buffer);
                    SrcFileOffset++;
                }
            }
            return 1;
        }else {
            std::cout<<"****Error File I/O Issue****\n\n";
            return 0;
        }
    }
}
