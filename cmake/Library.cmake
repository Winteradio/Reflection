# ------ Set the variables for the library options ------- #
message(STATUS "# Set the options for the library")

set(INCLUDE_DIR ${CMAKE_BINARY_DIR}/include CACHE PATH "Include files Path")
set(LIB_DIR ${CMAKE_BINARY_DIR}/lib CACHE PATH "Library files Path")
set(BIN_DIR ${CMAKE_BINARY_DIR}/bin CACHE PATH "Execute files Path")

# ------ Set the options for the library ------- #
function(create_library)
	cmake_parse_arguments(
		ARG # Options / Single Value / Multi Value
		""	
		"SHARED_LIBRARY;TARGET_NAME;"
		"PUBLIC_HEADERS;PRIVATE_SOURCES"
		${ARGN})

	if (NOT DEFINED ARG_TARGET_NAME)
		set(ARG_TARGET_NAME ${PROJECT_NAME})
		message(WARNING "# Warning : The library will use the project name as the target name")
	endif()

	if (${ARG_SHARED_LIBRARY})
		set(LIBRARY_TYPE SHARED)
	else()
		set(LIBRARY_TYPE STATIC)
	endif()

	message(STATUS "# Create the library : ${ARG_TARGET_NAME}")
	message(STATUS "- The Library Type : ${LIBRARY_TYPE}")
	message(STATUS "- The Binary Directory : ${BIN_DIR}")
	message(STATUS "- The Library Directory : ${LIB_DIR}")
	message(STATUS "- The Include Directory : ${INCLUDE_DIR}")

	add_library(${ARG_TARGET_NAME} ${LIBRARY_TYPE})

	target_sources(${ARG_TARGET_NAME}
		PUBLIC		${ARG_PUBLIC_HEADERS}
		PRIVATE		${ARG_PRIVATE_SOURCES}
	)

	if (WIN32 AND ${ARG_SHARED_LIBRARY})
		set_target_properties(${ARG_TARGET_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
	endif()

endfunction()

# ------ Copy the public header files for the library ------- #
function(install_library)
	cmake_parse_arguments(
		ARG # Options / Single Value / Multi Value
		""	
		"TARGET_NAME;"
		"PUBLIC_HEADERS;PUBLIC_INCLUDE_DIRS"
		${ARGN})

	if (NOT DEFINED ARG_TARGET_NAME)
		set(ARG_TARGET_NAME ${PROJECT_NAME})
		message(WARNING "# Warning : The library will use the project name as the target name")
	endif()

	if (NOT DEFINED ARG_PUBLIC_INCLUDE_DIRS)
		message(WARNING "# Warning : The include directories list is empty")
		set(ARG_PUBLIC_INCLUDE_DIRS "")
	endif()

	message(STATUS "# Install the library")
	message(STATUS "- The binary Path : ${BIN_DIR}")
	message(STATUS "- The library Path : ${LIB_DIR}")
	message(STATUS "- The include Path : ${INCLUDE_DIR}")

	message(${ARG_PUBLIC_INCLUDE_DIRS})

	target_include_directories(${ARG_TARGET_NAME} PUBLIC ${ARG_PUBLIC_INCLUDE_DIRS} ${INCLUDE_DIR})

	install(TARGETS ${ARG_TARGET_NAME}
		RUNTIME DESTINATION ${BIN_DIR}
		ARCHIVE DESTINATION ${LIB_DIR}
		LIBRARY DESTINATION ${LIB_DIR}
	)

	install(FILES ${ARG_PUBLIC_HEADERS} DESTINATION ${INCLUDE_DIR}/${ARG_TARGET_NAME})

endfunction()


