#! /usr/bin/python2

import os
import sys
import subprocess

# prebuild script called by bjam before build is started

# notes:
# if you use os.chdir() don't foget to return back to gitroot


#_______________________FIND_GIT_ROOT_-_NEXT_TO_JAMROOT_________________________

gitroot = os.getcwd()

while True:
	files = os.listdir(gitroot)
	files = [f for f in files if os.path.isfile(os.path.join(gitroot,f))] 
	if "Jamroot" in files:
		del files
		break
	if gitroot == os.path.dirname(gitroot): raise Exception("no Jamroot found in current directory or its parents")
	gitroot = os.path.dirname(gitroot)

os.chdir(gitroot)


#_______________________VERSION_HEADERS_GENERATION______________________________

def makeVersionBaseHeader():
	prog = os.path.join(".", "gitversion.sh")
	targ = os.path.join("TSeries_Utils", "include", "TSeries", "git_version.h")
	command = "{prog} {targ}".format(**locals())
	print command
	rc = os.system(command)
	if rc != 0: sys.exit(rc)


def makeVersionProjectHeader(project):
	prog = os.path.join(".", "version.sh")
	base = os.path.join(project, "baseversion")
	targ = os.path.join(project, "src", "gen_version.h")
	command = "{prog} {base} {targ}".format(**locals())
	print command
	rc = os.system(command)
	if rc != 0: sys.exit(rc)

makeVersionBaseHeader()
makeVersionProjectHeader("TSeries")
makeVersionProjectHeader("TSeries_Utils")
makeVersionProjectHeader(os.path.join("TSDevices_Services","RemoteControl"))


#_______________________UI_FILES_GENERATION____________________________________

if "clean" in sys.argv:
	subprocess.call(["TSDevices_Apps/tbs-uic.py", "clean"])
else:
	subprocess.call(["TSDevices_Apps/tbs-uic.py"])

