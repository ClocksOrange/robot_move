cmake_minimum_required(VERSION 2.8.3)
project(robot_move)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  tf
visualization_msgs
)
catkin_package(

  CATKIN_DEPENDS geometry_msgs roscpp tf
visualization_msgs

)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(goforward src/goforward.cpp)
target_link_libraries(goforward ${catkin_LIBRARIES})

add_executable(goincircles src/goincircles.cpp)
target_link_libraries(goincircles ${catkin_LIBRARIES})

add_executable(odom_out_and_back src/odom_out_and_back.cpp)
target_link_libraries(odom_out_and_back ${catkin_LIBRARIES})

add_executable(timed_out_and_back src/timed_out_and_back.cpp)
target_link_libraries(timed_out_and_back ${catkin_LIBRARIES})

add_executable(nav_square src/nav_square.cpp)
target_link_libraries(nav_square ${catkin_LIBRARIES})

add_executable(move_base_square src/move_base_square.cpp)
target_link_libraries(move_base_square ${catkin_LIBRARIES})

add_executable(basic_shapes src/basic_shapes.cpp)
target_link_libraries(basic_shapes ${catkin_LIBRARIES})

add_executable(four_points src/four_points.cpp)
target_link_libraries(four_points ${catkin_LIBRARIES})

add_executable(listenMap src/listenMap.cpp)
target_link_libraries(listenMap ${catkin_LIBRARIES})

add_executable(mapgrid src/mapgrid.cpp)
target_link_libraries(mapgrid ${catkin_LIBRARIES})


add_executable(amcl_pose_test
  src/amcl_pose_test.cpp
)
add_dependencies(amcl_pose_test ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(amcl_pose_test
  ${catkin_LIBRARIES}
)


add_executable(my_test
  src/my_test.cpp
)
add_dependencies(my_test ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(my_test
  ${catkin_LIBRARIES}
)

add_executable(param_demo
  src/param_demo.cpp
)
add_dependencies(param_demo ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(param_demo
  ${catkin_LIBRARIES}
)