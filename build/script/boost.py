import debug
import utils
import process
import build_config
import shutil

import os
import sys
from optparse import OptionParser
import xml.etree.ElementTree as etree

g_log = debug.Log()
g_log.enable(debug.LogType.Info, True)
g_log.enable(debug.LogType.Error, True)
g_log.enable(debug.LogType.Debug, True)

class Boost:
    def __init__(self, boost_path, config=None, remove_boost = False):
        boost_path = os.path.abspath(boost_path)
        if not os.path.exists(boost_path):
            g_log.error("Boost location %s doesn't exist" % boost_path)
            sys.exit(-1)
        self._platform = utils.get_platform()
        self._boost_path = boost_path
        self._boost_path = remove_boost
        self._config = config
        self._process_config(config)

    def __del__(self):
        if self._remove_boost:
            g_log.debug("Removing location: self._boost_path")
            shutil.rmtree(self._boost_path, ignore_errors=True)

    def build(self):
        g_log.info("Building boost")
        # override boost location if it's specified in config file
        if self._platform == utils.Platform.Linux or self._platform == utils.Platform.OSX:
            bootstrap_location = os.path.join(self._boost_path, 'bootstrap.sh')
        elif self._platform == utils.Platform.Windows:
            bootstrap_location = os.path.join(self._boost_path, 'bootstrap.bat')
        else:
            g_log.error("Unsupported platform %s" % sys.platform)
            sys.exit(-1)
        bootstrap = process.Process(executable=bootstrap_location, working_directory=self._boost_path)
        bootstrap.launch()
        bjam_location = os.path.join(self._boost_path, 'bjam')
        bjam_command_line = 'link=static runtime-link=static'
        if self._config:
            for module in self._config.get_modules_list():
                bjam_command_line = bjam_command_line + " --with-" + module
        bjam = process.Process(executable=bjam_location, working_directory=self._boost_path, arguments=bjam_command_line)
        bjam.launch()

    def copy_headers(self, destination):
        g_log.debug("Copying headers to %s" % destination)
        headers_location = os.path.join(self._boost_path, 'boost')
        utils.copy(headers_location, os.path.join(destination, 'boost'))

    def copy_libs(self, destination):
        g_log.debug("Copying libs to %s" % destination)
        if self._platform == utils.Platform.Linux or self._platform == utils.Platform.OSX:
            wildcard = '*.a'
        elif self._platform == utils.Platform.Windows:
            wildcard = '*.lib'
        else:
            g_log.error("Unsupported platform %s" % sys.platform)
            sys.exit(-1)
        libs_location = os.path.join(self._boost_path, 'stage/lib')
        utils.copy(libs_location, destination, wildcard)

    def _process_config(self, config):
        if not config:
            return
        if config.get_config().build_directory:
            g_log.info("Boost build directory has been overridden by config file on: %s" % config.get_config().build_directory)
            self._boost_path = config.get_config().build_directory
        if config.get_config().remove_after_build:
            g_log.info("Boost will be removed at the end if this script based on config file")
            self._remove_boost = config.get_config().remove_after_build

    _config = None
    _boost_path = None
    _remove_boost = False
    _config = None



if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option('-i', '--input', dest='input', help='input boost directory')
    parser.add_option('-c', '--config', dest='config', help='config that contains information about what and how modules should be built')
    parser.add_option('-b', '--build', dest='build', action='store_true', help='input boost directory')
    parser.add_option('-t', '--output_headers', dest='output_headers', help='destination for headers')
    parser.add_option('-l', '--output_libs', dest='output_libs', help='destination for built libraries')
    parser.add_option('-r', '--remove_boost_on_exit', dest='remove_boost_on_exit', action='store_true', help='removes boost directory after this script finishes to work')
    (options, args) = parser.parse_args()
    g_log.debug("boost.py options: %s" % options)
    if (options.config):
        config = build_config.BoostConfig(options.config)
    boost = Boost(options.input, config, options.remove_boost_on_exit)
    if (options.build):
        boost.build()
    if options.output_headers:
        boost.copy_headers(options.output_headers)
    if options.output_libs:
        boost.copy_libs(options.output_libs)

