import utils

from zipfile import ZipFile
import tarfile
import os.path

def is_supported_archive(input_file):
    name, ext = os.path.splitext(input_file)
    if ext == '.zip' or ext == '.bz2':
        return True
    return False

def get_output_path(input_file, output_dir):
    input_file = os.path.abspath(input_file)
    input_file_name, __ = utils.splitext(os.path.basename(input_file))
    output_dir = os.path.abspath(output_dir)
    output_dir = os.path.join(output_dir, input_file_name)
    return output_dir

def unpack(input_file, output_dir):
    name, ext = utils.splitext(input_file)
    if ext == '.zip':
        _unzip(input_file, output_dir)
    elif ext == '.tar.bz2':
        _untar(input_file, output_dir)
    else:
        raise Exception('unsupported archive type')

def _unzip(input_file, output_dir):
    zfile = ZipFile(input_file)
    # Add zip name to output path so every zip is extracted into
    # separate folder having the same name as an archive
    zip_name, _ = os.path.splitext(os.path.basename(input_file))
    output_dir = os.path.join(output_dir, zip_name)
    for name in zfile.namelist():
        dir_name, file_name = os.path.split(name)
        # create directory for current file in archive
        new_dir = os.path.join(output_dir, dir_name)
        if not os.path.exists(new_dir):
            os.makedirs(new_dir)
        # and finaly write a file
        if file_name:
            fd = open(os.path.join(output_dir, name), 'wb')
            fd.write(zfile.read(name))
            fd.close()
    zfile.close()

def _untar(input_file, output_dir):
    tfile = tarfile.open(input_file, 'r:bz2')
    tfile.extractall(output_dir)