# Linux builder

if(NOT EXISTS "${PROJECT_BINARY_DIR}/linux.src")
	execute_process(COMMAND "git clone https://github.com/torvalds/linux ${PROJECT_BINARY_DIR}/linux.src"
		COMMENT "Downloading Linux")
endif()