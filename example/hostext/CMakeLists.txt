add_executable(hostext "main.cc")
target_link_libraries(hostext iocape)

set_property(TARGET hostext PROPERTY CXX_STANDARD 17)
