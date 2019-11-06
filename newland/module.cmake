# Kernel Module CMake Build Script

if(NOT NEWLAND_SOURCE)
  message(FATAL_ERROR "Newland kernel build script requires variable to be defined: NEWLAND_SOURCE")
endif()

if(NOT NEWLAND_ARCH)
  message(FATAL_ERROR "Newland kernel build script requires variable to be defined: NEWLAND_ARCH")
endif()

include("${NEWLAND_SOURCE}/version.cmake")
include("${NEWLAND_SOURCE}/arch/${NEWLAND_ARCH}/build.cmake")

function(add_kmodule name sources)
  # TODO: fixme
endfunction()
