set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ontology.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/object.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/recording.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/signal.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/segment.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/timing.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/event.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/annotation.cpp
            )

set(HEADERS object.h
            recording.h
            signal.h
            segment.h
            timing.h
            event.h
            annotation.h
            )
foreach(file ${HEADERS})
  typedobject_generate(GENERATED_FILES ${file})
endforeach()

set(SOURCES ${SOURCES} ${GENERATED_FILES})
set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})

add_library(bsml SHARED ${SOURCES})
target_link_libraries(bsml typedobject)
target_include_directories(bsml PUBLIC ${INCLUDES})
