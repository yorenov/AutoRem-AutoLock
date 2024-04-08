cmake_minimum_required (VERSION 3.7)

project ("AutoRem"
	VERSION 0.0
)

execute_process(COMMAND git submodule update --init kthook WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
execute_process(COMMAND git submodule update --init ktsignal WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/kthook)
execute_process(COMMAND git submodule update --init xbyak WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/kthook)

add_subdirectory(kthook)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src ${PROJECT_NAME}_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/SAMP ${PROJECT_NAME}_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/kthook/hde ${PROJECT_NAME}_LIST)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/kthook)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/kthook/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SAMP)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/kthook/ktsignal/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/kthook/xbyak)

add_library(${PROJECT_NAME} SHARED)
target_sources(${PROJECT_NAME} PRIVATE
    ${${PROJECT_NAME}_LIST}
)
target_link_libraries(${PROJECT_NAME} PUBLIC kthook)

set_target_properties(${PROJECT_NAME}
	PROPERTIES
	OUTPUT_NAME ${PROJECT_NAME}
	SUFFIX ".asi"
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)