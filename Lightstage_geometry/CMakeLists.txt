project(Lightstage_geometry)
cmake_minimum_required(VERSION 2.8)

find_package( OpenCV REQUIRED )

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

include_directories("/home/als31/trimesh2/include")
link_directories("/home/als31/trimesh2/lib.Linux64")

target_link_libraries( Lightstage_geometry ${OpenCV_LIBS} trimesh )
