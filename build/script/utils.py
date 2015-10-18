import os

def splitext(file):
    name, ext = os.path.splitext(file)
    # special case for tar.bz2 and tar.gz archives
    if ext == '.bz2' or ext == '.gz':
        name, subext = os.path.splitext(name)
        ext = subext + ext
    return (name, ext)