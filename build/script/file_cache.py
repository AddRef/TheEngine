import debug

import os
import datetime
from optparse import OptionParser

g_log = debug.Log()

class Cache:
    """Provices check if particular file or folder has changed since last usage"""
    def __init__(self, cache_file):
        # Read file with timestamps of file modifications to check with
        self._cache_file = cache_file
        self._cache = self._load_cache(self._cache_file)

    def entry_has_changed(self, path):
        """Checks if file has been modified and updates cache otherwise"""
        timestamp = self._get_entry_timestamp(path)
        # Do not process files/folders that havn't been modified
        if path in self._cache:
            if self._cache[path] == timestamp or self._cache[path] == 'disabled':
                return False        
        return True

    def update_entry(self, path, disable_timestamp = False):
        """Updates entry about specific file or folder"""
        if not os.path.exists(path):
            g_log.error("Path %s doesn't exist" % path)
        if disable_timestamp:
            timestamp = 'disabled'
        else:
            timestamp = self._get_entry_timestamp(path)
        self._cache[path] = timestamp

    def discard_entry(self, path):
        """Removes file from cache"""
        del self._cache[path]

    def flush(self):
        """Stores accumulated cache"""
        self._store_cache(self._cache, self._cache_file)

    def _get_entry_timestamp(self, path):
        t = os.path.getmtime(path)
        timestamp = ('%s') % datetime.datetime.fromtimestamp(t)
        timestamp = timestamp.strip()
        return timestamp

    def _load_cache(self, cache_path):
        filters = {}
        if os.path.exists(self._cache_file):
            f = open(self._cache_file, 'r')
            for line in f.readlines():
                (path, file_creation_date) = line.split(',')
                path = path.strip()
                file_creation_date = file_creation_date.strip()
                filters[path] = file_creation_date
            f.close()
        return filters

    def _store_cache(self, filters, cache_path):
        f = open(self._cache_file, 'w+')
        for (path, file_creation_date) in filters.iteritems():
            str = "%s,%s\n" % (path, file_creation_date)
            f.write(str)
        f.close()

if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option('-u', '--update', dest='update_target', help='updates entry in cache')
    parser.add_option('-c', '--cache', dest='cache_file', help='cache location')
    (options, args) = parser.parse_args()
    g_log.debug("unpack.py options: %s" % options)
    if options.update_target:
        if not options.cache_file:
            g_log.error("Cache file is not specified")
        if not os.path.exists(options.cache_file):
            g_log.error("Cache file %s doesn't exist" % options.cache_file)
        cache = Cache(options.cache_file)
        cache.update_entry(options.update_target)