#!/usr/bin/python

from os import getcwd, chdir, path
from subprocess import call

def deploy():
	rootdir = getcwd()

	chdir(path.join(rootdir,"build"))
	call(["make","clean"])
	call(["make"])

	chdir(path.join(rootdir,"doc"))
	call(["doxygen","Doxyfile"])

if __name__ == "__main__":
	deploy();
