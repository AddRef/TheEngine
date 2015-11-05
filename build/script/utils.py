import debug

import os
import sys
import shutil
import fnmatch

g_log = debug.Log()
g_log.enable(debug.LogType.Info, True)
g_log.enable(debug.LogType.Error, True)
g_log.enable(debug.LogType.Debug, True)

def splitext(file):
    name, ext = os.path.splitext(file)
    # special case for tar.bz2 and tar.gz archives
    if ext == '.bz2' or ext == '.gz':
        name, subext = os.path.splitext(name)
        ext = subext + ext
    return (name, ext)

class Platform:
	Unknown = 0
	Linux = 1
	OSX = 2
	Windows = 3

def get_platform():
    if sys.platform == 'linux' or sys.platform == 'linux2':
    	return Platform.Linux
    elif sys.platform == 'darwin':
    	return Platform.OSX
    elif sys.platform == 'win32':
        return Platform.Windows
    else:
    	return Platform.Unknown

def copy(source, destination, wildcard='*', recursive=True):
    g_log.debug("copy is called with source: %s, destination: %s, wildcard: %s, recursive: %s" 
                % (source, destination, wildcard, recursive), debug.Verbose.Two)
    if not os.path.exists(source):
        g_log.error("Source path doesn't exist: %s", source)
        return False
    # Create destination directory in case of it's abscence when source is directory as well
    if not os.path.exists(destination) and os.path.isdir(source):
        os.makedirs(destination)
    # Allow copy only of file-to-file or dir-to-dir
    both_dirs = os.path.isdir(source) and os.path.isdir(destination) 
    both_files = os.path.isfile(source) and os.path.isfile(destination)
    if not both_dirs and not both_files:
       g_log.error("Source and destination have different types: %s %s" % (source, destination))
    # Perform actual copy
    if os.path.isdir(source):
        files = os.listdir(source)
        for f in files:
            g_log.debug("Preparing to coping file: %s, wildcard: %s" % (f, wildcard), debug.Verbose.Two)
            source_f = os.path.join(source, f)
            destination_f = os.path.join(destination, f)
            # Copy files
            if os.path.isfile(source_f) and fnmatch.fnmatch(source_f, wildcard):
                g_log.debug("Coping file: %s" % f, debug.Verbose.One)
                shutil.copyfile(source_f, destination_f)
            # Copy dirs
            if os.path.isdir(source_f) and recursive:
                g_log.debug("Coping directory: %s" % f, debug.Verbose.One)
                copy(source_f, destination_f, wildcard, recursive)
    else:
        g_log.debug("Coping file: %s" % source)
        shutil.copyfile(source, destination)
    return True