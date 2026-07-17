add_library(catch2 STATIC
    "catch_amalgamated.cpp"
    "catch_amalgamated.hpp"
)

macro(epcs2_add_test _NAME _SOURCE)
    add_executable(${_NAME} ${_SOURCE})
    target_link_libraries(${_NAME} PRIVATE epcs2-emu-lib catch2)
    set_target_properties(${_NAME} PROPERTIES CXX_STANDARD 20)
    add_test(NAME ${_NAME} COMMAND ${_NAME})
endmacro()

epcs2_add_test("epcs2_r5900_isa_ialu_test" "epcs2_r5900_isa_ialu_test.cpp")
