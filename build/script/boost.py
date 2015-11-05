import debug
import utils
import process

import os
import sys
from optparse import OptionParser
import xml.etree.ElementTree as etree

g_log = debug.Log()
g_log.enable(debug.LogType.Info, True)
g_log.enable(debug.LogType.Error, True)
g_log.enable(debug.LogType.Debug, True)

class BoostConfig:
    """Filters list of files based on unpack_config.xml"""
    def __init__(self, unpack_config_name):
        self._filters = {}
        tree = etree.parse(unpack_config_name)
        components_config = tree.find('components_config')
        boost_config = components_config.find('boost')
        self._process_config(boost_config)

    def get_modules_list(self):
        return self._modules

    def _process_config(self, config):
        for module in config:
            #build a list of modules to build
            self._modules.append(module.tag)

    _modules = []

class Boost:
    def __init__(self, boost_path):
        boost_path = os.path.abspath(boost_path)
        if not os.path.exists(boost_path):
            g_log.error("Boost location %s doesn't exist" % boost_path)
            sys.exit(-1)
        self._platform = utils.get_platform()
        self._boost_path = boost_path

    def build(self, config=None):
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
        print config
        if config:
            for module in config.get_modules_list():
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


if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option('-i', '--input', dest='input', help='input boost directory')
    parser.add_option('-c', '--config', dest='config', help='config that contains information about what and how modules should be built')
    parser.add_option('-b', '--build', dest='build', action='store_true', help='input boost directory')
    parser.add_option('-t', '--output_headers', dest='output_headers', help='destination for headers')
    parser.add_option('-l', '--output_libs', dest='output_libs', help='destination for built libraries')
    (options, args) = parser.parse_args()
    g_log.debug("boost.py options: %s" % options)
    boost = Boost(options.input)
    if (options.config):
        config = BoostConfig(options.config)
    if (options.build):
        boost.build(config)
    if options.output_headers:
        boost.copy_headers(options.output_headers)
    if options.output_libs:
        boost.copy_libs(options.output_libs)

