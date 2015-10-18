import archive
import utils

from os import listdir
import os.path
import shutil
import datetime
import xml.etree.ElementTree as etree
from sys import platform
from optparse import OptionParser
from collections import namedtuple

class Config:
    """Filters list of files based on unpack_config.xml"""
    def __init__(self, unpack_config_name):
        self._filters = {}
        # Build list of files to unpack
        tree = etree.parse('unpack_config.xml')
        # Find common section of filter file
        common_config = tree.find('common')
        # Find OS specific part of filter file
        config_name = ""
        if platform == 'linux' or platform == 'linux2':
            config_name = 'linux'
        elif platform == 'darwin':
            config_name = 'mac'
        elif platform == 'win32':
            config_name = 'windows'
        else:
            print("Unsupported platform %s. Terminaring.") % platform
            sys.exit(-1)
        os_specific_config = tree.find(config_name)
        # Process configs
        self._process_config(common_config)
        self._process_config(os_specific_config)

    def needs_process(self, file_name):
        # Do not process files that are missing in filter list
        if not file_name in self._filters:
            return False
        return True

    def needs_dest_caching(self, file_name):
        if not file_name in self._filters or self._filters[file_name].dest_cache == 'disabled':
            return False
        return True

    def _process_config(self, config):
        for module in config:
            # Read attributes
            filter_entry = namedtuple('FilterEntry', ['dest_cache'])
            if 'dest_cache' in module.attrib:
                filter_entry.dest_cache = module.attrib['dest_cache']
            self._filters[module.tag] = filter_entry


class Cache:
    """Provices check if particular file has changed since last usage"""
    def __init__(self, cache_file):
        # Read file with timestamps of file modifications to check with
        self._cache_file = cache_file
        self._cache = self._load_cache(self._cache_file)

    def has_changed(self, file_name):
        """Checks if file has been modified and updates cache otherwise"""
        timestamp = self._get_file_timestamp(file_name)
        # Do not process files/folders that havn't been modified
        if file_name in self._cache:
            if self._cache[file_name] == timestamp or self._cache[file_name] == 'disabled':
                return False        
        return True

    def update_file(self, file_name, disable_timestamp = False):
        """Updates cache for specific file"""
        if disable_timestamp:
            timestamp = 'disabled'
        else:
            timestamp = self._get_file_timestamp(file_name)
        self._cache[file_name] = timestamp

    def discard_file(self, file_name):
        """Removes file from cache"""
        del self._cache[file_name]

    def flush(self):
        """Stores accumulated cache"""
        self._store_cache(self._cache, self._cache_file)

    def _get_file_timestamp(self, file_name):
        t = os.path.getmtime(file_name)
        timestamp = ('%s') % datetime.datetime.fromtimestamp(t)
        timestamp = timestamp.strip()
        return timestamp

    def _load_cache(self, cache_file_name):
        filters = {}
        if os.path.exists(self._cache_file):
            f = open(self._cache_file, 'r')
            for line in f.readlines():
                (file_name, file_creation_date) = line.split(',')
                file_name = file_name.strip()
                file_creation_date = file_creation_date.strip()
                filters[file_name] = file_creation_date
            f.close()
        return filters

    def _store_cache(self, filters, cache_file_name):
        f = open(self._cache_file, 'w+')
        for (file_name, file_creation_date) in filters.iteritems():
            str = "%s,%s\n" % (file_name, file_creation_date)
            f.write(str)
        f.close()


class Unpacker:
    """Unpacks zip archives located in current folder"""
    def __init__(self, input, output, config, cache):
        self._path = input
        # process destination folder
        self._unpack_folder = output
        if not os.path.exists(self._unpack_folder):
            os.makedirs(self._unpack_folder)
        self._unpack_config = config
        self._files_cache = cache

    def unpack(self):
        """unpacks path set in class constructor"""
        for f in listdir(self._path):
            self._process_file(f)
        self._files_cache.flush()

    def _process_file(self, file_name):
        full_file_path = os.path.abspath(os.path.join(self._path, file_name))
        if (os.path.isfile(full_file_path)):
            (name, ext) = utils.splitext(file_name)
            if (archive.is_supported_archive(file_name) and self._unpack_config.needs_process(name)):
                # check if zip has modified recently
                zip_path = os.path.abspath(os.path.join(self._unpack_folder, name))
                zip_exists = os.path.exists(zip_path)
                # Check if either source zip or destination folder has changed
                zip_changed = self._files_cache.has_changed(full_file_path)
                unpack_zip_changed = zip_exists and self._files_cache.has_changed(zip_path)
                if zip_changed or not zip_exists or unpack_zip_changed:
                    print ("Extracting %s to %s...") % (full_file_path, self._unpack_folder)
                    self._remove_file(zip_path)
                    archive.unpack(full_file_path, self._unpack_folder)
                    print ("Extracting %s... DONE") % (full_file_path)
                else:
                    print("[INFO]: Skipping file %s: it was already unpacked") % (file_name)
                # update cache
                if not self._unpack_config.needs_dest_caching(name):
                    print "[WARNING]: Destination cache is disabled for file %s" % (file_name)
                disable_timestamp = not self._unpack_config.needs_dest_caching(name)
                self._files_cache.update_file(zip_path, disable_timestamp)
                self._files_cache.update_file(full_file_path)

    def _remove_file(self, path):
        """removes file or folder recursively"""
        if (os.path.exists(path)):
            shutil.rmtree(path, ignore_errors=True)


if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option('-i', '--input', dest='input', help='input directory', default='.')
    parser.add_option('-o', '--output', dest='output', help='output directory', default='./_unpack')
    parser.add_option('-c', '--config', dest='config', help='configuration file containing packages to unpack', default='./unpack_config.xml')
    (options, args) = parser.parse_args()
    config = Config(options.config)
    cache = Cache(os.path.join(options.output, 'file_cache'))
    unpacker = Unpacker(options.input, options.output, config, cache)
    unpacker.unpack()
