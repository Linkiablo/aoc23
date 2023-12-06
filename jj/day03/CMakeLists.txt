
get_filename_component(DayName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
FILE(COPY input DESTINATION "${CMAKE_BINARY_DIR}/${DayName}")
FILE(COPY test DESTINATION "${CMAKE_BINARY_DIR}/${DayName}")

add_executable(${DayName} main.cpp)

set(DEBUG_FLAGS "-ggdb" "-march=native" "-Wall" "-Wextra" "-Wpedantic" "-Werror"
	"-fsanitize=address" "-fsanitize=leak")
set(RELEASE_FLAGS "-march=native" "-O2")

target_compile_options(${DayName} PUBLIC
	$<$<CONFIG:Debug>:${DEBUG_FLAGS}>
	$<$<CONFIG:Release>:${RELEASE_FLAGS}>
	$<$<CONFIG:Bench>:${RELEASE_FLAGS}>
)

target_compile_definitions(${DayName} PUBLIC
	$<$<CONFIG:Bench>:"_BENCH">
)

target_link_libraries(${DayName}
	$<$<CONFIG:Debug>:${LIBASAN}>
)
