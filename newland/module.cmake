# Kernel Module CMake Build Script

if(NOT NEWLAND_SOURCE)
  message(FATAL_ERROR "Newland kernel build script requires variable to be defined: NEWLAND_SOURCE")
endif()

if(NOT NEWLAND_ARCH)
  message(FATAL_ERROR "Newland kernel build script requires variable to be defined: NEWLAND_ARCH")
endif()

function(add_kmodule name sources)
  include_directories("${NEWLAND_SOURCE}/include" "${NEWLAND_SOURCE}/arch/${NEWLAND_ARCH}/include")
  add_compile_definitions("NEWLAND_MODULE")
  add_compile_options("-nostdinc" "-fno-builtin" "-Werror=implicit-function-declaration" "-fno-stack-protector")
  set(CMAKE_SHARED_LINKER_FLAGS "-T${NEWLAND_SOURCE}/arch/${NEWLAND_ARCH}/module.ld -nostdlib")
  add_library("${name}" SHARED ${sources})
  set_target_properties("${name}" PROPERTIES PREFIX "")
endfunction()
