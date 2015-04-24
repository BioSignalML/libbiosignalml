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

set(HEADERS object.h
            recording.h
            signal.h
            segment.h
            timing.h
            event.h
            annotation.h
            )

set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBRARIES typedobject)

add_subdirectory(formats)

foreach(file ${HEADERS})
  typedobject_generate(GENERATED_FILES ${file})
endforeach()
set(SOURCES ${SOURCES} ${GENERATED_FILES})

add_library(bsml STATIC ${SOURCES})

target_link_libraries(bsml ${LIBRARIES})
target_include_directories(bsml PUBLIC ${INCLUDES})
