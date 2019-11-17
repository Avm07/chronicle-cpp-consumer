cmake_minimum_required(VERSION 3.13.4)

project(chronicle-cpp-consumer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

set(CMAKE_CXX_FLAGS "-Wall -Wextra -DBOOST_ALL_DYN_LINK")

set(JSON_BuildTests OFF CACHE BOOL "turn off bulding nlohman::json unit tests" FORCE)

find_package(Boost 1.70 COMPONENTS log REQUIRED)
find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)

add_subdirectory(externals/json)

add_executable(
chronicle-consumer
chronicle-cpp-consumer/main.cpp
chronicle-cpp-consumer/TCPSyncServer.cpp
chronicle-cpp-consumer/Config.cpp
chronicle-cpp-consumer/MongoDbWrapper.cpp
chronicle-cpp-consumer/Exception.cpp
chronicle-cpp-consumer/EosTokenFilter.cpp
chronicle-cpp-consumer/DataHandler.cpp
)

include_directories(
externals/json
chronicle-cpp-consumer
${LIBMONGOCXX_INCLUDE_DIRS}
${LIBBSONCXX_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS} ) 

target_link_libraries(
chronicle-consumer 
PRIVATE nlohmann_json::nlohmann_json
PUBLIC ${Boost_LIBRARIES} ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES} 
pthread)




