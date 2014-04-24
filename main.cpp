#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
<<<<<<< HEAD
#include <stdio.h>
#include <cstring>
#include "index.h"
#include "Commands.h"
=======
>>>>>>> 4870414fd5b9b67dbb6448e5657183a62dbae28c

#include "Commands.h"
//#include "Commands.cpp"
using namespace std;

<<<<<<< HEAD
int main(int argc, char** argv) {
    
    Commands customfilesys; //test
    customfilesys.mkfs(); //test
    char file[20];
    char flg[5];
    strcpy(file, "test.txt");
    strcpy(flg, "rw");
    customfilesys.open(file, flg);
    
=======
void callCommand(Commands, string);

int main(int argc, char** argv)
{
	Commands coms;
	cout << "Strike 'q' to exit." << endl;

	if (argc==1) {
		string input;
		while (input != "q") {
			cout << "Enter command:  ";
	
			getline(cin, input);
			if (input == "q")	break;
			callCommand(coms, input);
	
			//cout << "Name: " << input << endl;
		}
		
	}
	else if (argc==2) {
		ifstream infile(argv[1]);
		string line;
		while (getline(infile, line))
		{
			callCommand(coms, line);
		}
	}
>>>>>>> 4870414fd5b9b67dbb6448e5657183a62dbae28c
    return 0;
}

void callCommand(Commands coms, string command)
{
	string buf;
	stringstream ss(command);
	vector<string> tokens;

	while(ss >> buf)	tokens.push_back(buf);
	//cout << tokens[0] << endl;
	//for(int i=0; i<tokens.size(); i++)	cout << tokens[i] << endl;

	if (tokens[0]=="mkfs") {
		if (tokens.size() != 1)	cout << "Usage error.  No parameters." << endl;
		else coms.mkfs();
	} else if (tokens[0]=="open"){
		if (tokens.size() != 3)	cout << "Usage error.  open <filename <flag>" << endl;
		else coms.open(tokens[1], tokens[2]);
	} else if (tokens[0]=="read"){
		if (tokens.size() != 3)	cout << "Usage error.  read <fd> <size>" << endl;
		else coms.read(atoi(tokens[1].c_str()), atoi(tokens[2].c_str()));
	} else if (tokens[0]=="write"){ //special case
		stringstream ss1(command);
		vector<string> tokens1;
		while(getline(ss1, buf, '"'))	tokens1.push_back(buf);
		//cout << tokens1[0] << " " << tokens1[1] << endl;
		stringstream ss2(tokens1[0]);
		vector<string> tokens2;
		while(ss2 >> buf)	tokens2.push_back(buf);
		tokens2.push_back(tokens1[1]);
		if (tokens2.size() != 3)	cout << "Usage error.  write <fd> <string>" << endl;
		else coms.write(atoi(tokens[1].c_str()), tokens[2]);
	} else if (tokens[0]=="seek"){
		if (tokens.size() != 3)	cout << "Usage error.  seek <fd> <offset>" << endl;
		else coms.seek(atoi(tokens[1].c_str()), atoi(tokens[2].c_str()));
	} else if (tokens[0]=="close"){
		if (tokens.size() != 2)	cout << "Usage error.  close <fd>" << endl;
		else coms.close(atoi(tokens[1].c_str()));
	} else if (tokens[0]=="mkdir"){
		if (tokens.size() != 2)	cout << "Usage error.  mkdir <dirname>" << endl;
		else coms.mkdir(tokens[1]);
	} else if (tokens[0]=="rmdir"){
		if (tokens.size() != 2)	cout << "Usage error.  rmdir <dirname>" << endl;
		else coms.rmdir(tokens[1]);
	} else if (tokens[0]=="cd"){
		if (tokens.size() != 2)	cout << "Usage error.  cd <dirname>" << endl;
		else coms.cd(tokens[1]);
	} else if (tokens[0]=="link"){
		if (tokens.size() != 3)	cout << "Usage error.  link <src> <dest>" << endl;
		else coms.link(tokens[1], tokens[2]);
	} else if (tokens[0]=="unlink"){
		if (tokens.size() != 2)	cout << "Usage error.  unlink <name>" << endl;
		else coms.unlink(tokens[1]);
	} else if (tokens[0]=="stat"){
		if (tokens.size() != 2)	cout << "Usage error.  stat <name>" << endl;
		else coms.stat(tokens[1]);
	} else if (tokens[0]=="ls"){
		if (tokens.size() != 1)	cout << "Usage error.  No paramaters." << endl;
		else coms.ls();
	} else if (tokens[0]=="cat"){
		if (tokens.size() != 2)	cout << "Usage error.  cat <filename>" << endl;
		else coms.cat(tokens[1]);
	} else if (tokens[0]=="cp"){
		if (tokens.size() != 3)	cout << "Usage error.  cp <src> <dest>" << endl;
		else coms.cp(tokens[1], tokens[2]);
	} else if (tokens[0]=="tree"){
		if (tokens.size() != 1)	cout << "Usage error.  No parameters." << endl;
		else coms.tree();
	} else if (tokens[0]=="import"){
		if (tokens.size() != 3)	cout << "Usage error.  import <srcname> <destname>" << endl;
		else coms.import(tokens[1], tokens[2]);
	} else if (tokens[0]=="export"){
		if (tokens.size() != 3)	cout << "Usage error.  export <srcname> <destname>" << endl;
		else coms.fexport(tokens[1], tokens[2]);
	} else if (tokens[0]=="exit"){
		cout << "Exiting." << endl;
	} else {
		cout << "Command not found.  " << command << endl;
	}

}