# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\cartes_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\cartes_autogen.dir\\ParseCache.txt"
  "cartes_autogen"
  )
endif()
