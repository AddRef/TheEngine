# Contains utils for installation, download, unpack, etc.
macro (launch_process exec_command working_directory)
	message("${${exec_command}}")
	execute_process(COMMAND ${${exec_command}}
					RESULT_VARIABLE var 
					ERROR_VARIABLE err
					WORKING_DIRECTORY ${working_directory})
	if (var) 
		message(${var})
	endif()
	if (err)
		message(FATAL ${err})
	endif()
endmacro()