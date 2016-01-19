set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resource.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/recording.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/signal.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/segment.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/timing.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/event.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/annotation.cpp
            )

add_subdirectory(data)

set(INCLUDES  ${INCLUDES}  PARENT_SCOPE)
set(SOURCES   ${SOURCES}   PARENT_SCOPE)
set(LIBRARIES ${LIBRARIES} PARENT_SCOPE)

