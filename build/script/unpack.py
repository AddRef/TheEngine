import archive
import utils
import file_cache
import debug
import build_config

import os
import shutil
from optparse import OptionParser

g_log = debug.Log()
g_log.enable(debug.LogType.Info, True)
g_log.enable(debug.LogType.Warning, True)
g_log.enable(debug.LogType.Error, True)
g_log.enable(debug.LogType.Debug, True)

class Unpacker:
    def __init__(self):
        pass

    def unpack_dir(self, input_dir, output_dir, cache=None, config=None):
        config_path = config.get_config_path()
        if cache and config and cache.entry_has_changed(config_path):
            # if config has changes from last repack then we need to fully rerun unpack
            cache.invalidate()
        cache.update_entry(config_path)
        
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        for input_file in os.listdir(input_dir):
            name, __ = utils.splitext(input_file)
            input_file = os.path.join(input_dir, input_file)
            module = config.get_module(name)
            if not config or module != None:
                self.unpack_file(input_file, output_dir, cache, module)

    def unpack_file(self, input_file, output_dir, cache=None, config_entry=None):
        # if upack location is available for a specific file override it
        unpack_destination = self._get_unpack_destination(config_entry)
        if config_entry and unpack_destination:
            g_log.info("Unpack destination for module %s has been overridden on %s for %s" % (input_file, unpack_destination.value, input_file))
            output_dir = unpack_destination.value
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
            if not config_entry or self._is_destination_cache_enabled(config_entry):
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
            if not config_entry or self._is_destination_cache_enabled(config_entry): 
                cache.update_entry(archive.get_output_path(input_file, output_dir))
            cache.update_entry(input_file)
            cache.flush()

    def _remove_file(self, path):
        if (os.path.exists(path)):
            shutil.rmtree(path, ignore_errors=True)

    def _get_unpack_destination(self, config_entry):
        unpack_destination_attr = config_entry.get_attribute('unpack_destination') if config_entry else None
        unpack_destination = unpack_destination_attr.value if unpack_destination_attr else None
        return unpack_destination

    def _is_destination_cache_enabled(self, config_entry):
        enable_destination_cache_attr = config_entry.get_attribute('enable_destination_cache') if config_entry else None
        enable_destination_cache = enable_destination_cache_attr.value if enable_destination_cache_attr else 'enabled'
        return enable_destination_cache != 'disabled'



if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option('-i', '--input', dest='input', help='input directory or file', default='.')
    parser.add_option('-o', '--output', dest='output', help='output directory', default='./_unpack')
    parser.add_option('-c', '--config', dest='config', help='configuration file containing packages to unpack', default='../build_config.xml')
    (options, args) = parser.parse_args()
    g_log.debug("unpack.py options: %s" % options)
    cache = file_cache.Cache(os.path.join(options.output, 'file_cache'))
    unpacker = Unpacker()
    if os.path.isdir(options.input):
        config = build_config.BuildConfig(options.config)
        unpacker.unpack_dir(options.input, options.output, cache, config)
    elif os.path.isfile(options.input):
        unpacker.unpack_file(options.input, options.output, cache)
