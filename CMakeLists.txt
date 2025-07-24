cmake_minimum_required(VERSION 3.28)

# Setup compiler requirements
set(CMAKE_C_COMPILER gcc-14)

# Setup projects
project(plzip C)
project(run_tests C)

# Setup standard directories
cmake_path(APPEND include_dir ${CMAKE_CURRENT_SOURCE_DIR} "include")
cmake_path(APPEND plzip_source_dir ${CMAKE_CURRENT_SOURCE_DIR} "src")
cmake_path(APPEND test_source_dir ${CMAKE_CURRENT_SOURCE_DIR} "test")

# Setup sources
set(plzip_source_files
        "bitstream.c"
        "file.c"
        "hashmap.c"
        "huffman.c"
        "list.c"
        "main.c"
        "prio_queue.c"
)

set(plzip_source_paths)
foreach (filename IN ITEMS ${plzip_source_files})
    cmake_path(APPEND filepath ${plzip_source_dir} ${filename})
    list(APPEND plzip_source_paths ${filepath})
endforeach()

set(test_source_files
        "test_bitstream.c"
        "test_huffman.c"
        "test.c"
)

set(test_source_paths)
list(APPEND test_source_paths ${plzip_source_paths})
foreach(filename IN ITEMS)
    cmake_path(APPEND filepath ${test_source_dir} ${filename})
    list(APPEND test_source_paths ${filepath})
endforeach()


# Create targets
add_executable(plzip ${plzip_source_paths})
set_property(TARGET plzip PROPERTY C_STANDARD 23)
set_property(TARGET plzip PROPERTY C_STANDARD_REQUIRED true)

add_executable(run_tests ${test_source_paths})
set_property(TARGET run_tests PROPERTY C_STANDARD 23)
set_property(TARGET run_tests PROPERTY C_STANDARD_REQUIRED true)

# Setup includes
target_include_directories(plzip PUBLIC ${include_dir})
target_include_directories(run_tests PUBLIC ${include_dir})














