import debug

import os
import sys
import subprocess
from optparse import OptionParser

g_log = debug.Log()
g_log.enable(debug.LogType.Info, True)
g_log.enable(debug.LogType.Error, True)
g_log.enable(debug.LogType.Debug, True)

class Process:
    def __init__(self, executable, working_directory, arguments=''):
        self._executable = os.path.abspath(executable)
        self._arguments = arguments
        self._output = ''
        self._print_output = True
        if working_directory:
            self._working_directory = working_directory
        else:
            self._working_directory = os.getcwd()

    def enable_output_print(self, enable):
        self._print_output = enable

    def set_working_dir(self, working_directory):
        g_log.debug("Working directory: %s" % working_directory)
        self._working_directory = working_directory

    def launch(self, arguments=None):
        g_log.debug("Launching: %s %s" % (self._executable, self._arguments))
        # Use arguments passed into 'launch' function at a first place 
        if not arguments:
            arguments = self._arguments
        # Launch the process
        prev_work_dir = os.getcwd()
        os.chdir(self._working_directory)
        process = subprocess.Popen(self._executable + " " + arguments, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        os.chdir(prev_work_dir)
        # Poll process for new output until finished
        if self._print_output:
            while True:
                nextline = process.stdout.readline()
                if nextline == '' and process.poll() != None:
                    break
                sys.stdout.write(nextline)
                sys.stdout.flush()

        self._output = process.communicate()[0]
        return  process.returncode

    def get_output(self):
        return self._output


if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option('-f', '--file', dest='file', help='executable file to launch')
    parser.add_option('-a', '--arguments', dest='arguments', help='executable arguments', default='')
    parser.add_option('-w', '--working_dir', dest='working_dir', help='working directory', default='.')
    (options, args) = parser.parse_args()
    g_log.debug("process.py options: %s" % options)
    process = Process(options.file, options.arguments)
    process.launch2(options.working_dir)
