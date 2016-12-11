CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT ( roboy_legs_stand_control )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

SET(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_CXX_FLAGS "--std=gnu++11 -g")
#==============================================================================
# Dependencies
#==============================================================================
find_package(catkin REQUIRED COMPONENTS
        common_utilities
	flexrayusbinterface
        rosconsole
        roscpp)

find_library( LEAP_LIB Leap PATH /home/letrend/Downloads/LeapDeveloperKit_2.2.7+30199_linux/LeapSDK/lib/x64 )
include_directories ( /home/letrend/Downloads/LeapDeveloperKit_2.2.7+30199_linux/LeapSDK/include )

# pthread
find_package (Threads)

catkin_package(DEPENDS Boost ncurses
        CATKIN_DEPENDS common_utilities roscpp rosconsole
        LIBRARIES libflexrayusbinterface
        DEPENDS libftd2xx)

INCLUDE_DIRECTORIES(    
        ${libftd2xx_INCLUDE_DIRS}
        ${catkin_INCLUDE_DIRS}
        )

#==============================================================================
# Application
#==============================================================================
add_executable( roboy_legs_stand_control src/roboy_legs_stand_control.cpp )
add_dependencies( roboy_legs_stand_control std_msgs_generate_messages_cpp common_utilities_generate_messages_cpp)
target_link_libraries( roboy_legs_stand_control
        ${CMAKE_THREAD_LIBS_INIT}
        ${catkin_LIBRARIES}
	${LEAP_LIB}
       -ldl
        )
