import debug
import utils
import process
import build_config
import shutil

import os
import sys
from optparse import OptionParser

g_log = debug.Log()
g_log.enable(debug.LogType.Info, True)
g_log.enable(debug.LogType.Warning, True)
g_log.enable(debug.LogType.Error, True)
g_log.enable(debug.LogType.Debug, True)

class Boost:
    def __init__(self, boost_path, config=None, remove_boost = False):
        print 0
        boost_path = os.path.abspath(boost_path) if boost_path else None
        self._platform = utils.get_platform()
        self._boost_path = boost_path
        self._remove_boost = remove_boost
        self._boost_config = config.get_module(self._boost_name)
        self._process_config(self._boost_config)
        if not self._boost_path or not os.path.exists(self._boost_path):
            g_log.warning("Boost location %s doesn't exist" % self._boost_path)
            sys.exit(0)

    def __del__(self):
        if self._remove_boost:
            g_log.debug("Removing location: self._boost_path")
            shutil.rmtree(self._boost_path, ignore_errors=True)

    def build(self, build_arguments):
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
        bjam_command_line = (build_arguments + " ") if build_arguments else ""
        bjam_command_line += 'link=static runtime-link=static' + self._form_bjam_module_list(self._boost_config)
        print bjam_command_line
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

    def _process_config(self, module):
        if not config:
            return
        build_directory = module.get_attribute('unpack_destination')
        remove_after_build = module.get_attribute('remove_after_build')
        if build_directory:
            g_log.info("Boost build directory has been overridden by config file on: %s" % build_directory.value)
            self._boost_path = os.path.join(build_directory.value, self._boost_name)
        if remove_after_build:
            g_log.info("Boost will be removed at the end if this script based on config file")
            self._remove_boost = remove_after_build

    def _form_bjam_module_list(self, module):
        if not module:
            return ""
        if not module.get_attribute('build_components'):
            return ""
        components_string = module.get_attribute('build_components').value
        component_strings = utils.split_string(components_string)
        command_line = ''
        for component in component_strings:
            command_line = command_line + " --with-" + component
        return command_line


    _boost_config = None
    _boost_path = None
    _remove_boost = False
    _config = None
    _boost_name = 'boost_1_59_0'



if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option('-i', '--input', dest='input', help='input boost directory')
    parser.add_option('-c', '--config', dest='config', help='config that contains information about what and how modules should be built')
    parser.add_option('-b', '--build', dest='build', action='store_true', help='input boost directory')
    parser.add_option('-t', '--output_headers', dest='output_headers', help='destination for headers')
    parser.add_option('-l', '--output_libs', dest='output_libs', help='destination for built libraries')
    parser.add_option('-r', '--remove_boost_on_exit', dest='remove_boost_on_exit', action='store_true', help='removes boost directory after this script finishes to work')
    parser.add_option('-a', '--build_arguments', dest='build_arguments', help='list of additional boost build arguments')
    (options, args) = parser.parse_args()
    g_log.debug("boost.py options: %s" % options)
    if (options.config):
        config = build_config.BuildConfig(options.config)
    print options.input
    boost = Boost(options.input, config, options.remove_boost_on_exit)
    if (options.build):
        boost.build(options.build_arguments)
    if options.output_headers:
        boost.copy_headers(options.output_headers)
    if options.output_libs:
        boost.copy_libs(options.output_libs)

