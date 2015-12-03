import debug
import utils

import os
import xml.etree.ElementTree as etree
import re

g_log = debug.Log()
g_log.enable(debug.LogType.Info, True)
g_log.enable(debug.LogType.Error, True)
g_log.enable(debug.LogType.Debug, True)

class ModuleAttribute:
    name = ''
    value = ''

class Module:
    def __init__(self):
        self.clear()

    def add_attribute(self, attribute):
        self._attributes.append(attribute)

    def get_attribute(self, attribute_name):
        for attribute in self._attributes:
            if attribute.name == attribute_name:
                return attribute
        return None

    def get_attributes(self):
        return self._attributes

    def clear(self):
        self._attributes = []

    _attributes = []


class BuildConfig:
    def __init__(self, unpack_config_path):
        self._path = unpack_config_path
        tree = etree.parse(self._path)
        unpack_config = tree.find('unpack_config')
        for module in unpack_config:
            m = Module()
            for attribute in module:
                if not self._is_attribute_available_for_platform(attribute):
                    continue
                a = ModuleAttribute()
                a.name = attribute.tag
                if 'value' in attribute.attrib:
                    a.value = attribute.attrib['value']
                # adjust attributes
                if a.name == 'unpack_destination':
                    a.value = self._process_env_variable(a.value)
                # store
                m.add_attribute(a)
            self._modules[module.tag] = m

    def get_module(self, module_name):
        if module_name in self._modules:
            return self._modules[module_name]
        return None

    def get_config_path(self):
        return self._path

    def print_out(self):
        for name, attributes in self._modules.iteritems():
            print "module %s has the following attributes:" % name
            for attribute in attributes.get_attributes():
                print "\t%s: %s" % (attribute.name, attribute.value)

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
            return input_str

    def _is_attribute_available_for_platform(self, attribute):
        current_platform_listed = False;
        if 'platforms' in attribute.attrib:
            platforms = self._process_platforms(attribute.attrib['platforms'])
            if utils.get_platform() in platforms:
                current_platform_listed = True;
        else:
            # 'platform' specification is missing so this attribute is available for all platforms
            current_platform_listed = True
        return current_platform_listed

    def _process_platforms(self, platforms_string):
        platform_strings = utils.split_string(platforms_string)
        platforms = []
        if 'windows' in platform_strings:
            platforms.append(utils.Platform.Windows)
        if 'linux' in platform_strings:
            platforms.append(utils.Platform.Linux)
        if 'osx' in platform_strings:
            platforms.append(utils.Platform.OSX)
        return platforms

    _modules = {}
    _path = ''
