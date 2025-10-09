# ------ Set the basic options for the project ------- #
message(STATUS "# Set the basic options for the project")

function(project_options)
	message(STATUS "- Compiler : ${CMAKE_CXX_COMPILER_ID}")

	target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		add_compile_options(/Zi /MP)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_compile_options(-g -Wall -Wextra)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		add_compile_options(-g -Wall -Wextra)
	else()
		message(FATAL_ERROR "The Compiler ${CMAKE_CXX_COMPILER_ID} is not supported")
	endif()
endfunction()