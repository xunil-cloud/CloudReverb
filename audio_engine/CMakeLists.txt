# CloudReverb audio_engine CMake target
# Required by root CMakeLists.txt: add_subdirectory(audio_engine)

cmake_minimum_required(VERSION 3.15)

option(CLOUDREVERB_ENABLE_AVX2 "Compile audio_engine with /arch:AVX2 on MSVC" OFF)

add_library(audio_engine STATIC)

file(GLOB AUDIO_ENGINE_CPP CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/AudioLib/*.cpp"
)

file(GLOB AUDIO_ENGINE_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/AudioLib/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Utils/*.h"
)

target_sources(audio_engine PRIVATE ${AUDIO_ENGINE_CPP} ${AUDIO_ENGINE_HEADERS})

target_include_directories(audio_engine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/AudioLib"
    "${CMAKE_CURRENT_SOURCE_DIR}/Utils"
)

target_compile_features(audio_engine PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(audio_engine PRIVATE
        /O2
        /permissive-
        /wd4702 /wd4244 /wd4305 /wd4458 /wd4100 /wd4996
    )

    if(CLOUDREVERB_ENABLE_AVX2)
        target_compile_options(audio_engine PRIVATE /arch:AVX2)
    endif()
endif()

set_target_properties(audio_engine PROPERTIES POSITION_INDEPENDENT_CODE ON)

include(CheckIPOSupported)
check_ipo_supported(RESULT CLOUDREVERB_IPO_SUPPORTED OUTPUT CLOUDREVERB_IPO_ERROR)
if(CLOUDREVERB_IPO_SUPPORTED)
    set_property(TARGET audio_engine PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()
