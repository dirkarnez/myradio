cmake_minimum_required(VERSION 3.14)

project(curl-playground)

find_package(CURL REQUIRED) 
if (CURL_FOUND)
    message("CURL found")
else (CURL_FOUND)
    message("CURL not found")
endif()

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")














add_library(clip STATIC IMPORTED)

set_property(
  TARGET clip APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE
)

set_target_properties(clip PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${CLIP_ROOT}/build/Release/clip.lib"
)
  
set_target_properties(clip PROPERTIES
  MAP_IMPORTED_CONFIG_MINSIZEREL Release
  MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
)

target_include_directories(clip INTERFACE "${CLIP_ROOT}")













add_executable(${PROJECT_NAME}
    "src/main.cpp"
)

target_link_libraries(${PROJECT_NAME} CURL::libcurl)