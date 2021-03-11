cmake_minimum_required(VERSION 3.5)

project(rqt_cascade_hfsm)

find_package(ament_cmake REQUIRED)

if(WIN32)
  message(STATUS "rqt_cascade_hfsm is not yet supported on Windows. Package will not be built.")
  ament_package()
  return()
endif()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(rclcpp REQUIRED)
find_package(qt_gui_cpp REQUIRED)
find_package(rqt_gui_cpp REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(std_msgs REQUIRED)

set(rqt_cascade_hfsm_SRCS
  src/rqt_cascade_hfsm/CascadeHFSM.cpp
  src/rqt_cascade_hfsm/HFSMScene.cpp
  src/rqt_cascade_hfsm/State.cpp
  src/rqt_cascade_hfsm/Transition.cpp
  src/rqt_cascade_hfsm/Component.cpp
  src/rqt_cascade_hfsm/Dependency.cpp
)

set(rqt_cascade_hfsm_HDRS
  include/rqt_cascade_hfsm/CascadeHFSM.hpp
  include/rqt_cascade_hfsm/HFSMScene.hpp
  include/rqt_cascade_hfsm/State.hpp
  include/rqt_cascade_hfsm/Transition.hpp
  include/rqt_cascade_hfsm/Component.hpp
  include/rqt_cascade_hfsm/Dependency.hpp
)

set(rqt_cascade_hfsm_UIS
  src/rqt_cascade_hfsm/cascade_hfsm.ui
)

set(rqt_cascade_hfsm_INCLUDE_DIRECTORIES
  include
  ${rclcpp_INCLUDE_DIRS}
  ${qt_gui_cpp_INCLUDE_DIRS}
  ${rqt_gui_cpp_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Xml_INCLUDE_DIRS}
  ${std_msgs_INCLUDE_DIRS}
)

qt5_wrap_cpp(rqt_cascade_hfsm_MOCS ${rqt_cascade_hfsm_HDRS})

qt5_wrap_ui(rqt_cascade_hfsm_UIS_H ${rqt_cascade_hfsm_UIS})

include_directories(
  ${rqt_cascade_hfsm_INCLUDE_DIRECTORIES}
)

add_library(${PROJECT_NAME} SHARED
  ${rqt_cascade_hfsm_SRCS}
  ${rqt_cascade_hfsm_MOCS}
  ${rqt_cascade_hfsm_UIS_H}
)

target_link_libraries(
  ${PROJECT_NAME}
  ${rclcpp_LIBRARIES}
  ${qt_gui_cpp_LIBRARIES}
  ${rqt_gui_cpp_LIBRARIES}
  ${std_msgs_LIBRARIES}
  Qt5::Widgets
  Qt5::Xml
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib/${PROJECT_NAME}
  LIBRARY DESTINATION lib/${PROJECT_NAME}
  RUNTIME DESTINATION bin/${PROJECT_NAME}
  INCLUDES DESTINATION include)

install(PROGRAMS scripts/rqt_cascade_hfsm
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(FILES plugin.xml
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY resource
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

pluginlib_export_plugin_description_file(rqt_gui "plugin.xml")

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

ament_package()
