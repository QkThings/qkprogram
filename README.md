 QkProgram

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

## TODO

* arduino port is still not complete (some issues with the timer ISR)
* currently the makefiles are tested only on windows, linux may require some changes
* specify a test in the makefile (e.g. TEST=test/main.c), otherwise a static library is created

