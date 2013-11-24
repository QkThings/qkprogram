# QkProgram

The embedded framework of the QkThings platform.

## Building

	make clean
	make TARGET=<targetname>
	make upload PORT=<portname>

See _build/target_ folder to get the available targets.

#### Example:
	make clean
	make TARGET=arduino
	make upload PORT=COM1

## Contribute

When cloning this repository, make sure you clone the **develop** branch so you get the latest changes. Do whatever changes you want and if you can improve the code or add new features don't hesitate to make a **pull request**. Need some ideas? See the following TODO list.

### TODO

* arduino port is still incomplete (some issues regarding the timer ISR, clean up and improve the code)
* currently the makefiles are tested only on windows, linux may require some changes
* specify a test in the makefile (e.g. TEST=test/main.c), otherwise a static library is created
