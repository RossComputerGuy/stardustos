# Kernel Module CMake Build Script

if(NOT NEWLAND_SOURCE)
  message(FATAL_ERROR "Newland kernel build script requires variable to be defined: NEWLAND_SOURCE")
endif()

if(NOT NEWLAND_ARCH)
  message(FATAL_ERROR "Newland kernel build script requires variable to be defined: NEWLAND_ARCH")
endif()

include("${NEWLAND_SOURCE}/version.cmake")
include("${NEWLAND_SOURCE}/arch/${NEWLAND_ARCH}/build.cmake")

function(add_kmodule name)
  include_directories("${NEWLAND_SOURCE}/include" "${NEWLAND_SOURCE}/arch/${NEWLAND_ARCH}/include")
  add_compile_definitions("NEWLAND_MODULE" "NEWLAND_VERSION=\"${NEWLAND_VERSION}\"")
  add_compile_options("-nostdinc" "-fno-builtin" "-Werror=implicit-function-declaration" "-fno-stack-protector" "-r")
  string(REPLACE "/" "_" "target" "${name}")
  add_library("${target}" OBJECT "${name}")
  set_target_properties("${target}" PROPERTIES PREFIX "")
endfunction()
