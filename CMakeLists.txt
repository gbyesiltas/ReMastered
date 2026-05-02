cmake_minimum_required(VERSION 3.22)

project(ReMastered VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.4
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(ReMastered
    COMPANY_NAME "GBY"
    BUNDLE_ID com.gby.remastered
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE GbY1
    PLUGIN_CODE RmS1
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "ReMastered"
    MICROPHONE_PERMISSION_ENABLED TRUE
    MICROPHONE_PERMISSION_TEXT "ReMastered needs microphone access to detect pitch and generate harmonies."
)

juce_generate_juce_header(ReMastered)

file(GLOB_RECURSE AUBIO_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Library/aubio/*.c"
)

file(GLOB SOUNDTOUCH_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Library/SoundTouch Library/*.cpp"
)

# Exclude generator/project artifacts.
list(FILTER SOUNDTOUCH_SOURCES EXCLUDE REGEX ".*SoundTouch\\.vcxproj$")
list(FILTER SOUNDTOUCH_SOURCES EXCLUDE REGEX ".*SoundTouch\\.sln$")

target_sources(ReMastered
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        ${AUBIO_SOURCES}
        ${SOUNDTOUCH_SOURCES}
)

target_include_directories(ReMastered
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Source"
        "${CMAKE_CURRENT_SOURCE_DIR}/Library"
        "${CMAKE_CURRENT_SOURCE_DIR}/Library/aubio"
        "${CMAKE_CURRENT_SOURCE_DIR}/Library/SoundTouch Library"
)

# Minimal aubio config for macOS build.
target_compile_definitions(ReMastered
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        HAVE_STDLIB_H=1
        HAVE_STDIO_H=1
        HAVE_COMPLEX_H=1
        HAVE_MATH_H=1
        HAVE_STRING_H=1
        HAVE_ERRNO_H=1
        HAVE_LIMITS_H=1
        HAVE_STDARG_H=1
        HAVE_ACCELERATE=1
)

target_link_libraries(ReMastered
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        "-framework Accelerate"
)
