add_executable(out main.cpp)

get_filename_component(DayName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
FILE(COPY input DESTINATION "${CMAKE_BINARY_DIR}/${DayName}")

set(DEBUG_FLAGS "-ggdb" "-march=native" "-Wall" "-Wextra" "-Wpedantic" "-Werror"
	"-fsanitize=address" "-fsanitize=leak")
set(RELEASE_FLAGS "-march=native" "-O2")

target_compile_options(out PUBLIC
	$<$<CONFIG:Debug>:${DEBUG_FLAGS}>
	$<$<CONFIG:Release>:${RELEASE_FLAGS}>
	$<$<CONFIG:Bench>:${RELEASE_FLAGS}>
)

target_compile_definitions(out PUBLIC
	$<$<CONFIG:Bench>:"_BENCH">
)

target_link_libraries(out
	$<$<CONFIG:Debug>:${LIBASAN}>
)
