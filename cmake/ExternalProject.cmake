# ------ Set the options for the external project ------- #
message(STATUS "# Set the options for the external project")

include(ExternalProject)

if (WIN32)
    message(STATUS "- Platform : Windows")

    set(STATIC_LIBRARY lib)
    set(SHARED_LIBRARY dll)
else()
    message(STATUS "- Platform : Others")

    set(STATIC_LIBRARY a)
    set(SHARED_LIBRARY so)
endif()

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build Type")
set(EXE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

set(EXT_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)
set(EXT_LIB_DIR ${CMAKE_BINARY_DIR}/lib)
set(EXT_BIN_DIR ${EXE_DIR})
set(EXT_LIST "")
set(EXT_LIBRARIES "")

target_include_directories(${PROJECT_NAME} PUBLIC ${EXT_INCLUDE_DIR})

# ------ Add the external project ------- #
function(add_external_library)
	cmake_parse_arguments(
		ARG # Options / Single Value / Multi Value
		""	
		"PROJECT_NAME;LIBRARY_NAME;GIT_REPOSITORY;GIT_TAG;UPDATE_COMMAND;PATCH_COMMAND;TEST_COMMAND;BUILD_SHARED_LIBRARY;BUILD_DEMO_FILE"
		""
		${ARGN})

	if (NOT DEFINED ARG_PROJECT_NAME)
		message(FATAL_ERROR "# Error : The external project must have a name")
	endif()

	if (NOT DEFINED ARG_LIBRARY_NAME)
		set(ARG_LIBRARY_NAME ${ARG_PROJECT_NAME})
	endif()

	if (NOT DEFINED ARG_GIT_REPOSITORY)
		message(FATAL_ERROR "# Error : The external project must have a git repository")
	endif()

	if (NOT DEFINED ARG_GIT_TAG)
		set(ARG_GIT_TAG "")
	endif()

	if (NOT DEFINED ARG_UPDATE_COMMAND)
		set(ARG_UPDATE_COMMAND "")
	endif()

	if (NOT DEFINED ARG_PATCH_COMMAND)
		set(ARG_PATCH_COMMAND "")
	endif()

	if (NOT DEFINED ARG_TEST_COMMAND)
		set(ARG_TEST_COMMAND "")
	endif()

	if (NOT DEFINED ARG_BUILD_SHARED_LIBRARY)
		set(ARG_BUILD_SHARED_LIBRARY ON)
	endif()

	if (NOT DEFINED ARG_BUILD_DEDMO_FILE)
		set(ARG_BUILD_DEMO_FILE OFF)
	endif()

	message(STATUS "# Download the external project : ${ARG_PROJECT_NAME}")
	message(STATUS "- The repository : ${ARG_GIT_REPOSITORY} | the tag : ${ARG_GIT_TAG}")
	message(STATUS "- The External Build Type : ${CMAKE_BUILD_TYPE}")
	message(STATUS "- The External Include Directory : ${EXT_INCLUDE_DIR}")
	message(STATUS "- The External Library Directory : ${EXT_LIB_DIR}")
	message(STATUS "- The External Binary Directory : ${EXT_BIN_DIR}")
	message(STATUS "- The External Library Type : ${ARG_BUILD_SHARED_LIBRARY}")

	ExternalProject_Add(
		${ARG_PROJECT_NAME}
		GIT_REPOSITORY ${ARG_GIT_REPOSITORY}
		GIT_TAG ${ARG_GIT_TAG}

		PREFIX ${CMAKE_BINARY_DIR}/Prefix/${ARG_LIBRARY_NAME}

		UPDATE_COMMAND ${ARG_UPDATE_COMMAND} 
		PATCH_COMMAND ${ARG_PATCH_COMMAND} 
		TEST_COMMAND ${ARG_TEST_COMMAND}
		CMAKE_ARGS 
			-DINCLUDE_DIR=${EXT_INCLUDE_DIR}
			-DLIB_DIR=${EXT_LIB_DIR}
			-DBIN_DIR=${EXT_BIN_DIR}
			-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
			-DBUILD_SHARED_LIBRARY=${ARG_BUILD_SHARED_LIBRARY}
 			-DBUILD_DEMO_FILE=${ARG_BUILD_DEMO_FILE}
	)

	message(STATUS "# Link the external library")

	target_link_libraries(${PROJECT_NAME} PUBLIC ${EXT_LIB_DIR}/${ARG_LIBRARY_NAME}.${STATIC_LIBRARY})
	add_dependencies(${PROJECT_NAME} ${ARG_PROJECT_NAME})

endfunction()