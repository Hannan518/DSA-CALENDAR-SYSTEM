# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\CalendarSystem_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CalendarSystem_autogen.dir\\ParseCache.txt"
  "CalendarSystem_autogen"
  )
endif()
