# StackCraft: x86-64 Compiler Adventure

## Project Overview

This project, named StackCraft, is a compiler development adventure where the goal is to transform a stack-based calculator language into x86-64 assembly code. The provided compiler (`calc3i.c`) serves as the foundation, the assembly code generated compatible with the x86-64 architecture on Ubuntu Linux.


### Project Structure

└─$ tree
.
├── bin
├── lexyacc-code
│   ├── build
│   ├── calc3a.c
│   ├── calc3g.c
│   ├── calc3.h
│   ├── calc3i.c
│   ├── calc3.l
│   └── calc3.y
├── makefile
├── testprogs
│   ├── fact.calc
│   ├── gcd.calc
│   ├── harmonic.calc
│   ├── looptest.calc
│   ├── pi.calc
│   ├── readme.txt
│   └── temp.c
└── x86-64-driver.sh

Run this command with the input .calc file which is a calc program. This tool generates assembly language for the respective input code but only some operators and functions mentioned in the calc3i.b file can execute. Check the `calc3i.c` before you input.


First make sure you run the following command to create all the executible files neccessary for the shell script to run

```bash
make
```


Run the following command to generate the assembly language for the calc file and also run the output of the calc file. I have taken `fact.calc`.

``` bash
./x86-64-driver.sh ./testprogs/fact.calc
```

Fact.calc looks something like this 

```input calc
print fact 0;
print fact 1;
print fact 2;
print fact 3 ;
print fact 4;
print fact 5;
print fact 6;
print fact 7;
print fact 8;
print fact 9;
print fact 10;
print fact 11;
``` 

Assembly code generated for this `.calc` file will be stored in `bin/fact.s`


## Referebces Used

- [Programming from the Ground Up Book](http://savannah.nongnu.org/projects/pgubook) by Jonathan Bartlett
- [x86-64 Assembly Language Programming with Ubuntu](http://www.egr.unlv.edu/~ed/x86.html) by Ed Jorgensen
- [Intel R© 64 and IA-32 Architectures Software Developer Manuals](https://software.intel.com/en-us/articles/intel-sdm)









