# Contains utils for installation, download, unpack, etc.
function (unpack file output_dir)
      message(STATUS "Extracting file ${file}")
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${file}
                      WORKING_DIRECTORY ${output_dir}
                      RESULT_VARIABLE Result
                      )
      if(NOT Result EQUAL "0")
        message(FATAL_ERROR "Failed extracting file ${file}")
      endif()
endfunction()