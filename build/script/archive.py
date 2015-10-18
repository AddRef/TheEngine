import utils

from zipfile import ZipFile
import tarfile
import os.path

def is_supported_archive(source_file_path):
    name, ext = os.path.splitext(source_file_path)
    if ext == '.zip' or ext == '.bz2':
        return True
    return False

def unpack(source_file_path, destination):
    print "Unpack"
    (name, ext) = utils.splitext(source_file_path)
    if ext == '.zip':
        _unzip(source_file_path, destination)
    elif ext == '.tar.bz2':
        _untar(source_file_path, destination)
    else:
        raise Exception('unsupported archive type')

def _unzip(source_file_path, destination):
    zfile = ZipFile(source_file_path)
    # Add zip name to output path so every zip is extracted into
    # separate folder having the same name as an archive
    zip_name, _ = os.path.splitext(os.path.basename(source_file_path))
    destination = os.path.join(destination, zip_name)
    for name in zfile.namelist():
        dir_name, file_name = os.path.split(name)
        # create directory for current file in archive
        new_dir = os.path.join(destination, dir_name)
        if not os.path.exists(new_dir):
            os.makedirs(new_dir)
        # and finaly write a file
        if file_name:
            fd = open(os.path.join(destination, name), 'wb')
            fd.write(zfile.read(name))
            fd.close()
    zfile.close()

def _untar(source_file_path, destination):
    print "_untar"
    tfile = tarfile.open(source_file_path, 'r:bz2')
    tfile.extractall(destination)