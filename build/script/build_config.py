import debug
import utils

import os
import xml.etree.ElementTree as etree
import re

g_log = debug.Log()
g_log.enable(debug.LogType.Info, True)
g_log.enable(debug.LogType.Error, True)
g_log.enable(debug.LogType.Debug, True)

class ConfigBase:
    def _process_env_variable(self, input_str):
        input_str = input_str.lower()
        if input_str.startswith('{env:'):
            m = re.match('\{env:(\w+)\}(.*)', input_str)
            env_variable_name = m.group(1)
            remaining_path = m.group(2)
            env_variable_value = os.environ[env_variable_name]
            return env_variable_value + remaining_path
        else:
            # not an enviroment variables
            output = input

########################################################################
## Unpack config
########################################################################
class UnpackConfigEntry:
    enable_destination_cache = True
    unpack_destination = ''

class UnpackConfig(ConfigBase):
    """Filters list of files based on unpack_config.xml"""
    def __init__(self, unpack_config_path):
        self._filters = {}
        self._unpack_config_path = os.path.abspath(unpack_config_path)
        # Build list of files to unpack
        tree = etree.parse(self._unpack_config_path)
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

    def get_config_path(self):
        return self._unpack_config_path

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
            if 'enable_destination_cache' in module.attrib:
                if module.attrib['enable_destination_cache'] == 'enabled':
                    filter_entry.enable_destination_cache = True
                if module.attrib['enable_destination_cache'] == 'disabled':
                    filter_entry.enable_destination_cache = False
            if 'unpack_destination' in module.attrib:
                filter_entry.unpack_destination
                # process enviroment alias as an unpack destination
                filter_entry.unpack_destination = self._process_env_variable(module.attrib['unpack_destination'])
            self._filters[module.tag] = filter_entry

########################################################################
## Boost config
########################################################################
class BoostConfigEntry:
    build_directory = ''
    remove_after_build = False

class BoostConfig(ConfigBase):
    """Filters list of files based on unpack_config.xml"""
    def __init__(self, unpack_config_name):
        tree = etree.parse(unpack_config_name)
        components_config = tree.find('components_config')
        boost_config = components_config.find('boost')
        self._process_config(boost_config)

    def get_modules_list(self):
        return self._modules

    def get_config(self):
        return self._boost_config

    def _process_config(self, config):
        for module in config:
            #build a list of modules to build
            self._modules.append(module.tag)
        if 'build_directory' in config.attrib:
            self._boost_config.build_directory = self._process_env_variable(config.attrib['build_directory'])
        if 'remove_after_build' in config.attrib:
            if config.attrib['remove_after_build'] == 'enabled':
                self._boost_config.remove_after_build = True
            if config.attrib['remove_after_build'] == 'disabled':
                self._boost_config.remove_after_build = False

    _modules = []
    _boost_config = BoostConfigEntry()