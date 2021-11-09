cmake_minimum_required(VERSION 3.5)
project(utils LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC ON)		

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 COMPONENTS Widgets Sql Test Network REQUIRED)
#find_package(Qt5QuickWidgets REQUIRED)
#find_package(Qt5 COMPONENTS Widgets Sql Network Qml Quick Gui QuickControls2  REQUIRED)


add_library(utils
  utils/utils_global.h 
  utils/alignproxymodel.h  utils/alignproxymodel.cpp 
  utils/colorporxymodel.h  utils/colorporxymodel.cpp 
  utils/custumsortfilterproxymodel.h  utils/custumsortfilterproxymodel.cpp
  utils/modifyflagsproxymodel.h utils/modifyflagsproxymodel.cpp 
  utils/mytableview.h  utils/mytableview.cpp 
  utils/numberformatdelegate.h  utils/numberformatdelegate.cpp
  utils/property.h
  utils/betterquery.h
  utils/save_headers.h
  utils/test.h
  utils/horizontalTabStyle.h
  utils/readonly_combobox.h
  utils/immutable_delegate.h
  utils/dirty.h
  utils/tree_sql_relatioanl_delegate.h
  utils/file_model.h   utils/file_model.cpp
  utils/scope_logger.h
  
  utils/model_decorator.h
  utils/table_to_tree_proxy_model.h

  
  )

target_link_libraries(utils PUBLIC Qt5::Widgets Qt5::Sql Qt5::Network Qt5::Test)
target_include_directories(utils SYSTEM PUBLIC ./verdigris)
target_include_directories(utils PUBLIC  .)

target_include_directories(utils
   INTERFACE
   ${CMAKE_CURRENT_BINARY_DIR}/utils_autogen/include/
   )
enable_warning(utils)
target_compile_definitions(utils PRIVATE UTILS_LIBRARY)


add_executable(test_utils tests/main.cpp)
target_link_libraries(test_utils PRIVATE utils)

add_executable(table_to_tree_proxy_model_test tests/table_to_tree_proxy_model_test.cpp)
target_link_libraries(table_to_tree_proxy_model_test PRIVATE utils)

add_executable(file_model_test tests/file_model_test.cpp)
target_link_libraries(file_model_test PRIVATE utils)

add_executable(model_decorator_test tests/model_decorator_test.cpp)
target_link_libraries(model_decorator_test PRIVATE utils)


# add_executable(mytableview_test tests/mytableview_test.cpp)
# target_link_libraries(mytableview_test PRIVATE utils)
