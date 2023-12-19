cmake_minimum_required(VERSION 3.22)

project(Levels VERSION 1.0.0 LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

find_package(JUCE CONFIG REQUIRED)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment target")
set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(MSVC)
add_compile_options("/W4")
else()
add_compile_options(
    -Wall
    -Wbool-conversion
    -Wcast-align
    -Wconditional-uninitialized
    -Wconstant-conversion
    -Wconversion
    -Wdeprecated
    -Wextra-semi
    #-Wfloat-equal
    -Winconsistent-missing-destructor-override
    -Wint-conversion
    -Wmissing-field-initializers
    -Wmissing-prototypes
    -Wno-ignored-qualifiers
    -Wnullable-to-nonnull-conversion
    -Woverloaded-virtual
    -Wpedantic
    -Wreorder
    -Wshadow-all
    -Wshift-sign-overflow
    -Wshorten-64-to-32
    -Wsign-compare
    -Wsign-conversion
    -Wstrict-aliasing
    -Wswitch-enum
    -Wuninitialized
    -Wunreachable-code
    -Wunused-parameter
    -Wunused-private-field
    -Wzero-as-null-pointer-constant
)
endif()

juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME "audiodev.blog"
    PLUGIN_MANUFACTURER_CODE Dvbl
    PLUGIN_CODE Levl
    BUNDLE_ID "blog.audiodev.levels"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "Levels"
)

juce_generate_juce_header(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    src/AnalysisData.h
    src/PluginEditor.cpp
    src/PluginEditor.h
    src/PluginProcessor.cpp
    src/PluginProcessor.h
    src/VUMeter.cpp
    src/VUMeter.h
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
    JUCE_MODAL_LOOPS_PERMITTED=0
    DONT_SET_USING_JUCE_NAMESPACE=1
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
)
