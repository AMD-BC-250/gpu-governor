#
# Project
#

cmake_minimum_required(VERSION 3.20.0)

project(amd-bc-250-gpu-governor
    VERSION 0.2.0
    LANGUAGES CXX
)



#
# Dependencies
#

find_package(yaml-cpp REQUIRED)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBDRM REQUIRED libdrm)
pkg_check_modules(LIBDRM_AMDGPU REQUIRED libdrm_amdgpu)



#
# Executable
#

add_executable(amd-bc-250-gpu-governor
	src/governor.cpp
	src/governor.hpp
	src/oberon.cpp
	src/oberon.hpp
	src/main.cpp
)

set_target_properties(amd-bc-250-gpu-governor PROPERTIES CXX_STANDARD 20)
set_target_properties(amd-bc-250-gpu-governor PROPERTIES CXX_STANDARD_REQUIRD ON)

target_compile_definitions(amd-bc-250-gpu-governor PRIVATE GOVERNOR_VERSION="${PROJECT_VERSION}")

target_include_directories(amd-bc-250-gpu-governor PRIVATE ${LIBDRM_INCLUDE_DIRS} ${LIBDRM_AMDGPU_INCLUDE_DIRS})
target_link_libraries(amd-bc-250-gpu-governor PRIVATE ${LIBDRM_LIBRARIES} ${LIBDRM_AMDGPU_LIBRARIES})

target_link_libraries(amd-bc-250-gpu-governor PRIVATE yaml-cpp::yaml-cpp)
install(
    TARGETS amd-bc-250-gpu-governor
    DESTINATION "bin"
)



#
# Installation
#

# systemd service
configure_file(data/amd-bc-250-gpu-governor.service.in amd-bc-250-gpu-governor.service)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/amd-bc-250-gpu-governor.service"
    DESTINATION "/usr/lib/systemd/system"
)

# config file
configure_file(data/amd-bc-250-gpu-governor.yaml.in amd-bc-250-gpu-governor.yaml)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/amd-bc-250-gpu-governor.yaml"
    DESTINATION "/etc/"
)
