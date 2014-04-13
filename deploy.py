#!/usr/bin/python

from os import getcwd, chdir, path
from subprocess import call

def deploy():
	print " ### Deploy qkprogram"
	rootdir = getcwd()

	print " === Generate documentation"
	chdir(path.join(rootdir,"doc"))
	call(["doxygen","Doxyfile"])

if __name__ == "__main__":
	deploy();
