# ------ Set the basic options for the project ------- #
message(STATUS "# Set the basic options for the project")

function(project_options)
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)

	message(STATUS "- Compiler : ${CMAKE_CXX_COMPILER_ID}")

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