set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ontology.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/object.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/recording.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/signal.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/segment.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/timing.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/event.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/annotation.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/formats.cpp
            )

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/object.h
            ${CMAKE_CURRENT_SOURCE_DIR}/recording.h
            ${CMAKE_CURRENT_SOURCE_DIR}/signal.h
            ${CMAKE_CURRENT_SOURCE_DIR}/segment.h
            ${CMAKE_CURRENT_SOURCE_DIR}/timing.h
            ${CMAKE_CURRENT_SOURCE_DIR}/event.h
            ${CMAKE_CURRENT_SOURCE_DIR}/annotation.h
            )

set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBRARIES typedobject)

add_subdirectory(data)

foreach(file ${HEADERS})
  typedobject_generate(GENERATED_FILES ${file})
endforeach()
set(SOURCES ${SOURCES} ${GENERATED_FILES})

add_library(bsml STATIC ${SOURCES})

target_link_libraries(bsml ${LIBRARIES})
target_include_directories(bsml PUBLIC ${INCLUDES})
