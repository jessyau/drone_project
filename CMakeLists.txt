cmake_minimum_required(VERSION 2.8.3)
project(drone_project)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Make it automatically compiles Qt project
set(CMAKE_AUTOMOC ON)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
  std_msgs
)
find_package(OpenCV)

include_directories(  ${catkin_INCLUDE_DIRS})

include_directories(  ${OpenCV_INCLUDE_DIRS})

# Find the QtWidgets library
set(CMAKE_PREFIX_PATH /home/arianna/qt5/qt/5.4/gcc_64)

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)



catkin_package()



add_executable(image_converter src/image_converter.cpp)
target_link_libraries(image_converter ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})


# The UI Frame
#add_executable(UI src/qt/mainwindow.h src/qt/main.cpp src/qt/mainwindow.cpp src/qt/mainwindow.ui)
#target_link_libraries(UI Qt5::Widgets Qt5::Gui Qt5::Core ${OpenCV_LIBRARIES} ${catkin_LIBRARIES})

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

