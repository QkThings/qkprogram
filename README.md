# QkProgram

QkProgram is the embedded framework of QkThings. 

## Building Instructions

* Building as a static library:

	make TARGET=<targetname>

See _build/target_ folder to get the available targets.

* Compiling the library with a test file that provides the main() function:

	make test MAIN=<testname>.c TARGET=<targetname>
	make upload PORT=<portname>

In this case, if the library doesn't exists it will be automatically created.
This feature is meant to be used to test the library in different situations (unit testing) and for different hardware (useful when porting the library to other targets).
Some tests are available under the _test_ folder.

#### Additional notes:

Under linux you may have to change permissions of your /dev directory in order to be able to upload the program to the board. 

	sudo chown -R <username> /dev/

## Porting Code

Most of the code is written in ANSI C resulting in a high level of portability. 
Hardware-dependend code is part of the _Hardware Abstraction Layer_ (HAL) and can be found on _src/hal/<targetname>_ folder.
That's the only code that needs to be changed/created when porting QkProgram to another target. 
See the files under that folder to understand how they are organized and which functions need to be implemented. Actually, if you really want to know which functions must be implemented you shoud take a look at the corresponding header files on _src/hal_ folder (one header file for each peripheral).

## Contribute

When cloning this repository, make sure you clone the **develop** branch so you get the latest changes. Do whatever changes you want and if you can improve the code or add new features don't hesitate to make a **pull request**. Need some ideas? See the following TODO list.

