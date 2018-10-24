
cmake_minimum_required(VERSION 3.7)
set(CMAKE_TOOLCHAIN_FILE "/data/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(us)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)

find_package(Sqlite3 REQUIRED)
file(GLOB SOURCE_FILES "src/*.cc" "src/*.cpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} sqlite3)

# below are to find libs and link it
find_package(Boost COMPONENTS date_time filesystem regex atomic chrono log_setup thread log system context REQUIRED)
include_directories(${BOOST_INCLUDEDIR})
target_link_libraries(${PROJECT_NAME} 
    Boost::date_time 
    Boost::filesystem 
    Boost::regex 
    Boost::atomic 
    Boost::chrono 
    Boost::log_setup 
    Boost::thread 
    Boost::log 
    Boost::system 
    Boost::context 
)