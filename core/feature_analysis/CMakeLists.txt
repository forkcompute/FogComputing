cmake_minimum_required(VERSION 2.8.3)
project(feature_analysis)

SET (MCSSLAM_LIB_TYPE SHARED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -O3 -march=native -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3 -march=native -std=c++11")

## Add support for C++11, supported in ROS Kinetic and newer
# add_definitions(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  rospy
  fog_msgs
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES feature_analysis
#  CATKIN_DEPENDS cv_bridge image_transport roscpp rospy
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
find_package(OpenCV REQUIRED)
find_package(Pangolin REQUIRED)

SET(OpenGV_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/ThirdParty/OpenGV/include)
SET(OpenGV_LIBRARIES opengv)
MESSAGE("OpenGV include DIR ${OpenGV_INCLUDE_DIR}")
INCLUDE_DIRECTORIES(${OpenGV_INCLUDE_DIR})

SET(g2o_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/ThirdParty/g2o)
SET(g2o_LIBRARIES g2o)
MESSAGE("g2o include DIR ${g2o_INCLUDE_DIR}")
INCLUDE_DIRECTORIES(${g2o_INCLUDE_DIR})

IF(NOT DEFINED EIGEN_INCLUDE_DIR_HINTS)
  SET(EIGEN_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/ThirdParty/Eigen)
  MESSAGE("Eigen include DIR ${EIGEN_INCLUDE_DIR}")
  SET(MultiCol-SLAM_USE_INTERNAL_EIGEN ON)
  INCLUDE_DIRECTORIES(${EIGEN_INCLUDE_DIR})
ENDIF()

SET(MultiColHeaders
include/cam_model_omni.h
include/cam_system_omni.h
include/cConverter.h
include/cMultiFrame.h
include/cMultiFramePublisher.h
include/cMultiKeyFrame.h
include/cConverter.h
include/cMapPoint.h
include/cMap.h
include/cMapPublisher.h
include/cLocalMapping.h
include/cLoopClosing.h
include/cOptimizer.h
include/cORBVocabulary.h
include/cORBmatcher.h
include/cMultiFrame.h
include/cMultiKeyFrameDatabase.h
include/cSim3Solver.h
include/cSystem.h
include/cTracking.h
include/cMultiInitializer.h
include/cViewer.h
include/mdBRIEFextractorOct.h
include/misc.h
#include/MultiCol_cayley_jacobians.h
include/g2o_MultiCol_vertices_edges.h
include/g2o_MultiCol_sim3_expmap.h  )

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/ThirdParty
    ${PROJECT_SOURCE_DIR}/ThirdParty/g2o
    ${Pangolin_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_library(${PROJECT_NAME} ${MCSSLAM_LIB_TYPE}
src/cam_model_omni.cpp
src/cam_system_omni.cpp
src/cConverter.cpp
src/cMultiFrame.cpp
src/cMultiFramePublisher.cpp
src/cMultiKeyFrame.cpp
src/cConverter.cpp
src/cMapPoint.cpp
src/cMap.cpp
src/cMapPublisher.cpp
src/cOptimizer.cpp
src/cOptimizerLoopStuff.cpp
src/cMultiFrame.cpp
src/cMultiKeyFrameDatabase.cpp
src/cSim3Solver.cpp
src/cSystem.cpp
src/cMultiInitializer.cpp
src/mdBRIEFextractorOct.cpp
src/cTracking.cpp
src/cLocalMapping.cpp
src/cLoopClosing.cpp
src/cViewer.cpp
src/cORBmatcher.cpp
src/g2o_MultiCol_sim3_expmap.cpp
src/g2o_MultiCol_vertices_edges.cpp
src/misc.cpp
${MultiColHeaders}
)

SET (G2OLIBS ${PROJECT_SOURCE_DIR}/ThirdParty/g2o/lib/libg2o.so)
SET (DBOW2LIBS ${PROJECT_SOURCE_DIR}/ThirdParty/DBoW2/lib/libDBoW2.so)
SET (OPENGVLIBS ${PROJECT_SOURCE_DIR}/ThirdParty/OpenGV/build/lib/librandom_generators.a
                ${PROJECT_SOURCE_DIR}/ThirdParty/OpenGV/build/lib/libopengv.a)

add_dependencies(${PROJECT_NAME} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
${Pangolin_LIBRARIES}
${G2OLIBS}
${DBOW2LIBS}
${OPENGVLIBS}
${RANDOMGENLIBS}
${catkin_LIBRARIES}
${OpenCV_INCLUDE_DIRS})

add_executable(FeatureAnalysis src/feature_analysis_main.cpp)
add_dependencies(FeatureAnalysis ${catkin_EXPORTED_TARGETS})
target_link_libraries(FeatureAnalysis ${catkin_LIBRARIES} ${OpenCV_INCLUDE_DIRS} ${PROJECT_NAME})

add_executable(FeatureExtraction src/feature_extraction_main.cpp)
add_dependencies(FeatureExtraction ${catkin_EXPORTED_TARGETS})
target_link_libraries(FeatureExtraction ${catkin_LIBRARIES} ${OpenCV_INCLUDE_DIRS} ${PROJECT_NAME})

add_library(NodeletFeatureExtraction src/feature_extraction.cpp)
add_dependencies(NodeletFeatureExtraction ${catkin_EXPORTED_TARGETS})
target_link_libraries(NodeletFeatureExtraction ${catkin_LIBRARIES} ${OpenCV_INCLUDE_DIRS} ${PROJECT_NAME})

add_library(NodeletFeatureAnalysis src/feature_analysis.cpp)
add_dependencies(NodeletFeatureAnalysis ${catkin_EXPORTED_TARGETS})
target_link_libraries(NodeletFeatureAnalysis ${catkin_LIBRARIES} ${OpenCV_INCLUDE_DIRS} ${PROJECT_NAME})

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/feature_analysis.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/feature_analysis_node.cpp)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_feature_analysis.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
