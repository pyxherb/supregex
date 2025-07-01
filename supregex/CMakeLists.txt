find_package(peff REQUIRED)

file(GLOB HEADERS *.h *.hh)
file(GLOB SRC *.c *.cc)

set(IOCAPE_BUILD_SHARED TRUE CACHE BOOL "Build shared library")

add_library(supregex STATIC)
target_compile_definitions(supregex PRIVATE IOCAPE_IS_BUILDING=1)
set_property(TARGET supregex PROPERTY CXX_STANDARD 17)
target_include_directories(supregex PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/> $<INSTALL_INTERFACE:include/>)
target_link_libraries(supregex PUBLIC peff_base_static peff_containers_static)
set_target_properties(supregex PROPERTIES
    VERSION ${PROJECT_VERSION}
)

add_library(supregexd SHARED)
target_compile_definitions(supregexd PRIVATE IOCAPE_IS_BUILDING=1)
target_compile_definitions(supregexd PUBLIC IOCAPE_BUILD_SHARED=1)
set_property(TARGET supregexd PROPERTY CXX_STANDARD 17)
set_property(TARGET supregexd PROPERTY PUBLIC_HEADER "")
target_include_directories(supregexd PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/> $<INSTALL_INTERFACE:include/>)
target_link_libraries(supregexd PUBLIC peff_base peff_containers)
set_target_properties(supregexd PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    ARCHIVE FALSE
)

target_sources(supregex PRIVATE ${HEADERS} ${SRC})

if(IOCAPE_BUILD_SHARED)
    target_sources(supregexd PRIVATE ${HEADERS} ${SRC})
endif()

configure_file(
    config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/config.h
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/generated/config.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/supregex/generated)

if(${IOCAPE_ENABLE_DEBUGGER})
endif()

install(TARGETS supregex supregexd
    EXPORT supregexTargets
    LIBRARY DESTINATION lib/
    ARCHIVE DESTINATION lib/
    RUNTIME DESTINATION bin
)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/supregex)

if(WIN32)
    add_subdirectory("win32")
endif()
