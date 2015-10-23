import archive
import utils
import file_cache
import debug

import sys
import os
import shutil
import xml.etree.ElementTree as etree
from sys import platform
from optparse import OptionParser
from collections import namedtuple

g_log = debug.Log()

class Config:
    """Filters list of files based on unpack_config.xml"""
    def __init__(self, unpack_config_name):
        self._filters = {}
        # Build list of files to unpack
        tree = etree.parse(unpack_config_name)
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
            g_log.error("Unsupported platform %s. Terminaring." % platform)
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

class Unpacker:
    def __init__(self):
        pass

    def unpack_dir(self, input_dir, output_dir, cache = None, config = None):
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        for input_file in os.listdir(input_dir):
            name, __ = utils.splitext(input_file)
            input_file = os.path.join(input_dir, input_file)
            if not config or config.needs_process(name):
                self.unpack_file(input_file, output_dir, cache)

    def unpack_file(self, input_file, output_dir, cache = None):
        input_file = os.path.abspath(input_file)
        output_dir = os.path.abspath(output_dir)
        if not os.path.isfile(input_file):
            g_log.debug("%s is not a file and has been skipped" % input_file)
            return
        if not archive.is_supported_archive(input_file):
            g_log.error("Unsupported archive %s" % input_file)
            return
        if not self._file_requires_unpack(input_file, output_dir, cache):
            g_log.info("Skipping file %s: it was already unpacked" % input_file)
            return
        self._unpack_file(input_file, output_dir)
        self._update_cache(input_file, output_dir, cache)

        
    def _file_requires_unpack(self, input_file, output_dir, cache):
        if cache:
            output_dir = archive.get_output_path(input_file, output_dir)
            input_file_changed = cache.has_changed(input_file)
            output_dir_changed = os.path.exists(output_dir) and cache.has_changed(output_dir)
            return input_file_changed or output_dir_changed
        return True

    def _unpack_file(self, input_file, output_dir):
        g_log.info("Extracting %s to %s..." % (input_file, output_dir))
        self._remove_file(archive.get_output_path(input_file, output_dir))
        archive.unpack(input_file, output_dir)
        g_log.info("Extracting %s... DONE" % (input_file))                    

    def _update_cache(self, input_file, output_dir, cache):
        if cache:
            cache.update_file(archive.get_output_path(input_file, output_dir))
            cache.update_file(input_file)
            cache.flush()

    def _remove_file(self, path):
        if (os.path.exists(path)):
            shutil.rmtree(path, ignore_errors=True)


if __name__ == '__main__':
    g_log.enable(debug.LogType.Info, True)
    g_log.enable(debug.LogType.Error, True)
    g_log.enable(debug.LogType.Debug, True)

    parser = OptionParser()
    parser.add_option('-i', '--input', dest='input', help='input directory', default='.')
    parser.add_option('-o', '--output', dest='output', help='output directory', default='./_unpack')
    parser.add_option('-c', '--config', dest='config', help='configuration file containing packages to unpack', default='./unpack_config.xml')
    (options, args) = parser.parse_args()
    g_log.debug("unpack.py options: %s" % options)
    config = Config(options.config)
    cache = file_cache.Cache(os.path.join(options.output, 'file_cache'))
    unpacker = Unpacker()
    unpacker.unpack_dir(options.input, options.output, cache, config)
