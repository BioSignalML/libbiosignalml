## This function needs to come from ${TYPEDOBJECT}/cmake
function(make_tobj output filename)
  get_filename_component(base ${filename} NAME_WE)
  set(tobj_file ${CMAKE_CURRENT_BINARY_DIR}/tobj_${base}.cpp)
  set(${output} ${${output}} ${tobj_file} PARENT_SCOPE)
  add_custom_command(
    OUTPUT ${tobj_file}
    COMMAND python ${TYPEDOBJECT}/src/clang/generate.py
                   -I${TYPEDOBJECT}/src
                   ${CMAKE_CURRENT_SOURCE_DIR}/${filename}
                   ${tobj_file}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${filename}
    )
  set_source_files_properties(${tobj_file} PROPERTIES GENERATED TRUE)
endfunction()

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ontology.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/object.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/recording.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/signal.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/segment.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/timing.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/event.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/annotation.cpp
            )
source_group(cpp FILES ${SOURCES})

set(HEADERS object.h
            recording.h
            signal.h
            segment.h
            timing.h
            event.h
            annotation.h
            )
foreach(file ${HEADERS})
  make_tobj(GENERATED ${file})
endforeach()
source_group(gen FILES ${GENERATED})

message(${GENERATED})

set(SOURCES ${SOURCES} ${GENERATED})

set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${TYPEDOBJECT}/src)

add_library(bsml SHARED ${SOURCES})
target_link_libraries(bsml typedobject)
target_include_directories(bsml PUBLIC ${INCLUDES})
