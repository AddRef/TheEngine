import archive
import utils
import file_cache
import debug

import sys
import os
import shutil
import xml.etree.ElementTree as etree
from optparse import OptionParser

g_log = debug.Log()
g_log.enable(debug.LogType.Info, True)
g_log.enable(debug.LogType.Error, True)
g_log.enable(debug.LogType.Debug, True)

class UnpackConfigEntry:
    cache_destination = True

class UnpackConfig:
    """Filters list of files based on unpack_config.xml"""
    def __init__(self, unpack_config_name):
        self._filters = {}
        # Build list of files to unpack
        tree = etree.parse(unpack_config_name)
        # Find common section of filter file
        unpack_config = tree.find('unpack_config')
        common_config = unpack_config.find('common')
        # Find OS specific part of filter file
        config_name = ""
        if utils.get_platform() == utils.Platform.Linux:
            config_name = 'linux'
        elif utils.get_platform() == utils.Platform.OSX:
            config_name = 'mac'
        elif utils.get_platform() == utils.Platform.Windows:
            config_name = 'windows'
        else:
            g_log.error("Unsupported platform %s. Terminaring." % sys.platform)
            sys.exit(-1)
        os_specific_config = unpack_config.find(config_name)
        # Process configs
        self._process_config(common_config)
        self._process_config(os_specific_config)

    def needs_process(self, file_name):
        # Do not process files that are missing in filter list
        if not file_name in self._filters:
            return False
        return True

    def get_entry_config(self, file_name):
        if file_name in self._filters:
            return self._filters[file_name]
        return None

    def _process_config(self, config):
        for module in config:
            # Read attributes
            filter_entry = UnpackConfigEntry()
            if 'cache_destination' in module.attrib and module.attrib['cache_destination'] == 'disabled':
                filter_entry.cache_destination = False
            self._filters[module.tag] = filter_entry

class Unpacker:
    def __init__(self):
        pass

    def unpack_dir(self, input_dir, output_dir, cache=None, config=None):
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        for input_file in os.listdir(input_dir):
            name, __ = utils.splitext(input_file)
            input_file = os.path.join(input_dir, input_file)
            if not config or config.needs_process(name):
                self.unpack_file(input_file, output_dir, cache, config.get_entry_config(name))

    def unpack_file(self, input_file, output_dir, cache=None, config_entry=None):
        input_file = os.path.abspath(input_file)
        output_dir = os.path.abspath(output_dir)
        if not os.path.isfile(input_file):
            g_log.debug("%s is not a file and has been skipped" % input_file)
            return
        if not archive.is_supported_archive(input_file):
            g_log.error("Unsupported archive %s" % input_file)
            return
        if not self._file_requires_unpack(input_file, output_dir, cache, config_entry):
            g_log.info("Skipping file %s: it was already unpacked" % input_file)
            return
        self._unpack_file(input_file, output_dir)
        self._update_cache(input_file, output_dir, cache, config_entry)

        
    def _file_requires_unpack(self, input_file, output_dir, cache, config_entry):
        if cache:
            output_dir = archive.get_output_path(input_file, output_dir)
            input_file_changed = cache.entry_has_changed(input_file)
            output_dir_changed = False
            if not config_entry or config_entry.cache_destination:
                output_dir_changed = not os.path.exists(output_dir) or cache.entry_has_changed(output_dir)
            return input_file_changed or output_dir_changed
        return True

    def _unpack_file(self, input_file, output_dir):
        g_log.info("Extracting %s to %s..." % (input_file, output_dir))
        self._remove_file(archive.get_output_path(input_file, output_dir))
        archive.unpack(input_file, output_dir)
        g_log.info("Extracting %s... DONE" % (input_file))                    

    def _update_cache(self, input_file, output_dir, cache, config_entry):
        if cache:
            if not config_entry or config_entry.cache_destination: 
                cache.update_entry(archive.get_output_path(input_file, output_dir))
            cache.update_entry(input_file)
            cache.flush()

    def _remove_file(self, path):
        if (os.path.exists(path)):
            shutil.rmtree(path, ignore_errors=True)


if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option('-i', '--input', dest='input', help='input directory or file', default='.')
    parser.add_option('-o', '--output', dest='output', help='output directory', default='./_unpack')
    parser.add_option('-c', '--config', dest='config', help='configuration file containing packages to unpack', default='./unpack_config.xml')
    (options, args) = parser.parse_args()
    g_log.debug("unpack.py options: %s" % options)
    cache = file_cache.Cache(os.path.join(options.output, 'file_cache'))
    unpacker = Unpacker()
    if os.path.isdir(options.input):
        config = UnpackConfig(options.config)
        unpacker.unpack_dir(options.input, options.output, cache, config)
    elif os.path.isfile(options.input):
        unpacker.unpack_file(options.input, options.output, cache)
