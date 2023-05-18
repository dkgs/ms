cmake_minimum_required(VERSION 3.15)
project(ms CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_SOURCE_DIR}/src)

add_library(ms_lib STATIC
        ${CMAKE_SOURCE_DIR}/src/service/booking_service_impl.cpp
        ${CMAKE_SOURCE_DIR}/src/service/show_manager_impl.cpp
        ${CMAKE_SOURCE_DIR}/src/service/theater_manager_impl.cpp
)
target_link_libraries(ms_lib ${CONAN_LIBS})

add_executable(ms src/main.cpp)
target_link_libraries(ms ms_lib ${CONAN_LIBS})

add_executable(ms_tu
        ${CMAKE_SOURCE_DIR}/test/main.cpp
        ${CMAKE_SOURCE_DIR}/test/service/booking_service_impl_test.cpp
        ${CMAKE_SOURCE_DIR}/test/service/show_manager_impl_test.cpp
        ${CMAKE_SOURCE_DIR}/test/service/theater_manager_impl_test.cpp
        ${CMAKE_SOURCE_DIR}/test/api/connection_manager_test.cpp
        ${CMAKE_SOURCE_DIR}/test/api/request_dispatcher_test.cpp
        ${CMAKE_SOURCE_DIR}/test/api/handler/book_seat_handler_test.cpp
        ${CMAKE_SOURCE_DIR}/test/api/handler/choose_seat_handler_test.cpp
        ${CMAKE_SOURCE_DIR}/test/api/handler/health_handler_test.cpp
        ${CMAKE_SOURCE_DIR}/test/api/handler/root_handler_test.cpp
)
target_include_directories(ms_tu PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(ms_tu ms_lib ${CONAN_LIBS})

enable_testing()
add_test(NAME UnitTests
        COMMAND ${CMAKE_BINARY_DIR}/bin/ms_tu
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
