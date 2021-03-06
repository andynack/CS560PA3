CS560PA3
========
General:
This system was developed and designed to operate in a linux environment using C++. The file organization structure style is using an inode with 10 direct pointers only. the root directory is intialized at inode 0. The disk that is created is 100MB. It is partitioned so that the first 64KB are reserved for the inodes and the rest of the space is available for text files and directories. In this data space, the block size that has been chosen is 1024 Bytes. This allows for minimal wasted space and a maximum file size of 10KB. For the types of files intended for this system this seemed to be plenty. It includes all of the functions required by the assignment, but has some limitations. One such limitation is that File and Directory names are limited to 19 characters. The other limitations will be discussed within the descriptions of the functions. 

To start the system there are two methods. First if you are wanting to use the interactive shell, simple type the name of the executable:

	anack@VBUbuntu:~/CS560PA3BIN$ ./cs560pa3

Second, if you want to run a script make sure that it is located in the current directory with the executable and then add the script name as an argument:

	anack@VBUbuntu:~/CS560PA3BIN$ ./cs560pa3 AAtestscript.sh

This system does include the ability to run remotely and when you desire to use this feature it can be done like this:

	awachter@hydra15.eecs.utk.edu:~$ sh run.sh <awachter@hydra17.eecs.utk.edu>

	Which displays:
	File System Ready To Receive Commands

	Strike 'q' to exit.
	Enter command:  q
	Connection to hydra17.eecs.utk.edu closed.

This script ssh's into another system and executes the program.  Upon termination with the program, it disconnects. It requires the CS560PA3BIN folder to be in the root directory and the run.sh script must be in the current directory of the remote system. it allows the same functionality as if you were on the local computer.


main.cpp:
The file can be ran with no parameters (where it will continually take input from standard in) or one parameter (where it will take a list of commands and iterate through them).  First it creates a new instance of commands.  With no parameters, it begins a loop that waits for a command from the user.  Or with one it begins a loop getting each line/command from the file.  Once a line is received, main calls the function that parses all commands.  It iterates through the command and splits it by spaces, getting the command parameters.  The function then checks the first token against the list of available commands.  It then verifies there are the appropriate number of parameters for the command.  It then calls the function written in commands.cpp to execute the command.  This was a useful way to develop this since most commands are written the same way and they can all be parsed the same way.  The only special case is 'write' which must separate the line by quotations then by spaces. Program halts either when user enters input q (for quit) or finishes parsing the input text.


Commands.cpp

mkfs
The make file system command creates a blank 100MB file to be used as a "disk" and initializes the root directory.
Ex: Enter command:  mkfs

open
The open command accepts a file name and a flag (write "w", read & write "rw", or read "r"). If the file exists it is opened in the specified mode. If it does not exist then it is created if the flag is "w" or "rw" and then opened in the corresponding mode. If the flag is "r" the open fails. 
When a file is opened the file inode number is identified as "fd"
Ex: Enter command: open test.txt w

read
The read command accepts a "fd" and a number of bytes. If the file corresponding to the "fd" is open for reading then those bytes are displayed in the shell. If the file is not open for reading the read fails.
Ex: Enter command: read 1 10

write
The write command accepts a "fd" and a string in quotes. If the specified file is open for writing the write is performed. if it is not then the write fails.
Ex: Enter command: write 1 "Hello World"

seek
The seek command accepts a "fd" and an offset. If the specified file is open, its pointer is moved accordingly.
Ex: Enter command: seek 1 5

close
Ex: Enter command: close 1

mkdir
Ex: Enter command: mkdir Test

rmdir
Ex: Enter command: rmdir Test

cd
Ex: Enter command: cd Test

link
Ex: Enter command: link Example.txt IceCube

unlink
Ex: Enter command: unlink IceCube

stat
Ex: Enter command: stat Example.txt

ls
Ex: Enter command: ls

cat
Ex: Enter command: cat Example.txt

cp
Ex: Enter command: cp Example.txt Red.txt

tree
Ex: Enter command: tree

import
Ex: Enter command: import External.txt Internal.txt

export
Ex: Enter command: export External.txt Internal.txt


Testing:
Our testing was performed using the included script "AAtestscript.sh" and the output of the test is copied and pasted from the shell interface into an included text file called "ExOutput"


