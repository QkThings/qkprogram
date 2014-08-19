#!/usr/bin/python

from os import getcwd, chdir, path
from distutils.dir_util import copy_tree
from subprocess import call
import shutil

def cp(root_src,root_dest,rel_path):
	print "Copying %s from %s to %s" % (rel_path, root_src, root_dest)
	copy_tree(path.join(root_src, rel_path), path.join(root_dest, rel_path))

def deploy():
	print " ### Deploy qkprogram"
	rootdir = getcwd()


	print " === Generate documentation"
	chdir(path.join(rootdir,"doc"))
	if path.exists("html"):
		shutil.rmtree("html")
	call(["/usr/local/bin/doxygen"])
#	cp(".", "html", "css")
	cp(".", "html", "fonts")

if __name__ == "__main__":
	deploy()
