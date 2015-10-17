# Check if we compile for iphone
macro(check_apple_platform)
  if(APPLE)
    include(CheckCSourceCompiles)
    check_c_source_compiles("
      #include \"TargetConditionals.h\"
      int main(int argc, char *argv[]) {
      #if !TARGET_OS_MAC
        -1
      #endif
      }
    " OS_OSX)
    check_c_source_compiles("
      #include \"TargetConditionals.h\"
      int main(int argc, char *argv[]) {
      #if !TARGET_OS_IPHONE
        -1
      #endif
      }
    " OS_IPHONE)
  endif()
endmacro()

function(bin2cpp in_file out_file)
    add_custom_command(
        OUTPUT ${out_file}
        COMMAND ${BIN2CPP} ${in_file} ${out_file}
        DEPENDS ${in_file}
    )
endfunction()
