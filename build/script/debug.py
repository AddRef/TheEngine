class LogType:
	Debug = 0
	Info = 1
	Error = 2
	Warning = 3
	All = 256

class Verbose:
	Zero = 0
	One = 1
	Two = 2

class Log:
	def enable(self, log_type, enable):
		if log_type == LogType.Debug:
			self._debug = enable
		elif log_type == LogType.Info:
			self._info = enable
		elif log_type == LogType.Error:
			self._error = enable
		elif log_type == LogType.Warning:
			self._warning = enable
		elif log_type == LogType.All:
			self.enable(LogType.Debug, enable)
			self.enable(LogType.Info, enable)
			self.enable(LogType.Error, enable)
			self.enable(LogType.Warning, enable)

	def set_debug_verbose_level(self, verbose_level):
		self._verbose_level = verbose_level

	def debug(self, str, verbose=Verbose.Zero):
		if self._debug and self._verbose_level >= verbose:
			print "[DEBUG]: " + str

	def info(self, str):
		if self._info:
			print "[INFO]: " + str

	def error(self, str):
		if self._error:
			print "[ERROR]: " + str
			raise

	def warning(self, str):
		if self._warning:
			print "[WARNING]: " + str

	_debug = False
	_info = False
	_error = False
	_warning = False
	_verbose_level = Verbose.Zero
