#!/usr/bin/python

from os import getcwd, chdir, path
from subprocess import call

def deploy():
	rootdir = getcwd()

	targets = []
	targets.append("arduino.uno")
	targets.append("efm32.gecko")

	chdir(path.join(rootdir,"build"))

	for target in targets:
		print " === Build target", target
		call(["make","clean","TARGET=%s" % target])
		call(["make","TARGET=%s" % target])

	print " === Generate documentation"
	chdir(path.join(rootdir,"doc"))
	call(["doxygen","Doxyfile"])

if __name__ == "__main__":
	deploy();
