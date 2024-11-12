# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\simpson_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\simpson_autogen.dir\\ParseCache.txt"
  "simpson_autogen"
  )
endif()
