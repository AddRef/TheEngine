import os
import datetime
from optparse import OptionParser

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

if __name__ == '__main__':
    pass