# =============================================================================
# © You i Labs Inc. 2000-2017. All rights reserved.
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# Pre-defined options
option(YI_TREAT_WARNINGS_AS_ERRORS "Warnings will result in failure to compile when set to ON." ON)

set(YI_PROJECT_NAME "ListView" CACHE STRING "The name of the project. When building for Android, this is passed from Android Studio.")
set(YI_PACKAGE_NAME "tv.youi.listview" CACHE STRING "The package name used for the project.")
set(YI_DISPLAY_NAME "List View" CACHE STRING "The name of the application displayed on the device.")
set(YI_VERSION_NUMBER 1.0.0.0 CACHE STRING "Build version of the application. This is the version that will be displayed to the end user in the store.")
set(YI_BUILD_NUMBER "ENG-unversioned" CACHE STRING "Release version number that needs to be incremented for each store submission. For Apple platforms, this is the CFBundleVersion. For Android, this is the Version Code.")
set(YI_YOUI_ENGINE_VERSION 4.5.0 CACHE STRING "Version required for the You.i Engine.")
set(YI_EXCLUDED_ASSET_FILE_EXTENSIONS ".log,.aep" CACHE STRING "Comma-delimited list of file extensions whose files should be omitted during asset copying.")

set(_STAGING_DIR "${CMAKE_CURRENT_BINARY_DIR}/Staging")
set(_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Default the CMAKE_BUILD_TYPE if not using a multi-configuration generator
if (NOT DEFINED CMAKE_CONFIGURATION_TYPES AND NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "")
endif()

if (DEFINED ENV{YOUIENGINE_HOME})
    get_filename_component(YouiEngine_DIR "$ENV{YOUIENGINE_HOME}" ABSOLUTE)
else()    
    get_filename_component(YouiEngine_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
endif()

# The ordering of the paths within the CMAKE_MODULE_PATH variable is imporant, since having the project's 'cmake'
# path first will tell CMake to first look under that folder when the short form 'include(Modules/<module>)' command is used
# instead of the long form 'include(<path>/Modules/<filename>.cmake)'.
#
# If CMake finds a module under the project directory structure, it will load that file and stop looking in the CMAKE_MODULE_PATH.
# This allows projects to customize modules as they need to, without requiring changes at the engine level.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${YouiEngine_DIR}/cmake)

if(NOT DEFINED YI_PLATFORM)
    message(FATAL_ERROR "The YI_PLATFORM variable was not given to the generator.")
endif()

include(Modules/YiCapitalizeString)
yi_capitalize_string(SRC_STRING "${YI_PLATFORM}" 
    RESULT_VARIABLE _PLATFORM_CAPITALIZED
    DELIMITERS "-;_"
)

string(TOUPPER "${YI_PLATFORM}" _PLATFORM_UPPER)
string(TOLOWER "${YI_PLATFORM}" _PLATFORM_LOWER)
include(Platform/Yi${_PLATFORM_CAPITALIZED} OPTIONAL RESULT_VARIABLE HAVE_YIPLATFORM_INCLUDE)
if (NOT HAVE_YIPLATFORM_INCLUDE)
    message( FATAL_ERROR "Unsupported platform specified: " ${_PLATFORM_CAPITALIZED})
endif()

# The call to project() needs to happen after the Platform file has been included. This is because some
# platforms set CMake specific variables that affect the choices made when a language is configured.
#
# If you would like to have the project() call happen at the start of the CMakeLists.txt file, then
# you will need to make sure that no languages are set when the project() command is called. You can do
# this by passing NONE to the LANGUAGES property. If you do so, however, you will need to add calls to
# enable_language() for each language that your project supports.
#
# For example:
#
# cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
# project(MyProject LANGUAGES NONE)
#
# ...
# include(Platform/Yi${_PLATFORM_CAPITALIZED})
# ...
# enable_language(CXX)
# enable_language(C)
project(${YI_PROJECT_NAME} LANGUAGES C CXX)

# By having the list of source files for the project outside of the CMakeLists.txt file a project can
# use tools to auto-generate the contents of that file without having a lot of commits on the CMakeLists.txt
# file. This also makes the CMakeLists.txt and SourceList.cmake files more portable to move between projects with
# minimal effort.
include("${CMAKE_CURRENT_SOURCE_DIR}/SourceList.cmake")

# Locating the You.i Engine is now done via the 'find_package' command. We first set the version of the engine that
# we want to look for, using the PACKAGE_FIND_VERSION variable that is used within the 'find_package' flow.
#
# Next, the call to 'find_package' passing the 'YouiEngine_DIR' path to the HINTS argument is made. This tells
# CMake to look within the 'YouiEngine_DIR' path for the You.i Engine. If found, the 'find_package' command will
# give back a 'youiengine' target that the project can link against. This target contains all of the third party
# libraries and their include directories, as well as the engine library, its include directories and any compiler
# definitions or options.
set(PACKAGE_FIND_VERSION ${YI_YOUI_ENGINE_VERSION})

find_package(YouiEngine 
    HINTS
        ${YouiEngine_DIR}
    REQUIRED
)

include(Modules/YiPlatformSourceFiles)
set(YI_PROJECT_CODE_FILES
    ${YI_PROJECT_SOURCE}
    ${YI_PROJECT_HEADERS}
    ${YI_APP_SOURCE_FILES}
    ${YI_APP_SOURCE_HEADERS}
    ${PLATFORM_SOURCE_FILES_${_PLATFORM_UPPER}}
    ${PLATFORM_SOURCE_FILES_${_PLATFORM_UPPER}_${YI_RENDER_TYPE}}
    ${PLATFORM_SOURCE_HEADERS_${_PLATFORM_UPPER}}
    ${PLATFORM_SOURCE_HEADERS_${_PLATFORM_UPPER}_${YI_RENDER_TYPE}}
)

# Not all platforms will need to perform some kind of initialization. Because this is optional,
# we first check to see if the command actually exists. If not, we can safely ignore it.
#
# Initialization is performed before the target for the project() is created. At this time,
# PROJECT_NAME is not available and thus no use of it should be done. If further 
# configuration is required, that can be done within the optional yi_configure_platform method.
include(Modules/${_PLATFORM_LOWER}/YiInitializePlatform OPTIONAL)
if(COMMAND yi_initialize_platform)
    yi_initialize_platform()
endif()

# Some platforms, such as IOS and TVOS have resources that are used for setting up the application
# icons. By making this include OPTIONAL, if no YiPlatformResources.cmake file exists, the build
# will continue without failing.
include(Modules/${_PLATFORM_LOWER}/YiPlatformResources OPTIONAL)

# The yi_add_executable function can be swapped with yi_add_library, if your project needs to
# output a library instead of an application executable.
include(Modules/YiAddExecutable)
yi_add_executable(TARGET ${PROJECT_NAME} 
    SRC_FILES ${YI_PROJECT_CODE_FILES} 
    EXTRA_FILES ${YI_PLATFORM_RESOURCES_${_PLATFORM_UPPER}}
)

# NOTE: This is only valid for Visual Studio-based generators
set_directory_properties(PROPERTIES
    VS_STARTUP_PROJECT ${PROJECT_NAME}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${_SRC_DIR}
    PRIVATE ${YouiEngine_DIR}/templates/mains/src
)

# Linking with the You.i Engine library is now as simple as adding the 'youiengine' target
# to the 'target_link_libraries' command. Thanks to CMake's feature called Transitive Dependencies,
# by linking with the 'youiengine' library, the project will inherit all of the compiler settings,
# include directories and libraries that the 'youiengine' target makes public.
#
# This makes linking to the You.i Engine and all of the third party libraries much easier, as there
# is only one target that has to be linked against.
target_link_libraries(${PROJECT_NAME}
    PRIVATE youiengine
)

include(Modules/${_PLATFORM_LOWER}/YiLinkExternalLibraries OPTIONAL RESULT_VARIABLE _EXTERNAL_LIBS_PATH)
if(NOT _EXTERNAL_LIBS_PATH MATCHES NOTFOUND)
    yi_link_external_libraries(TARGET ${PROJECT_NAME})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RESOURCE "${YI_PLATFORM_RESOURCES_${_PLATFORM_UPPER}}"
)

source_group("Header Files" FILES ${YI_PROJECT_HEADERS})
source_group("Source Files" FILES ${YI_PROJECT_SOURCE})
source_group("Header Files\\App" FILES 
    ${YI_APP_SOURCE_HEADERS}
    ${PLATFORM_SOURCE_HEADERS_${_PLATFORM_UPPER}}
    ${PLATFORM_SOURCE_HEADERS_${_PLATFORM_UPPER}_${YI_RENDER_TYPE}}
)
source_group("Source Files\\App" FILES 
    ${YI_APP_SOURCE_FILES}
    ${PLATFORM_SOURCE_FILES_${_PLATFORM_UPPER}}
    ${PLATFORM_SOURCE_FILES_${_PLATFORM_UPPER}_${YI_RENDER_TYPE}}
)
source_group("Resources" FILES ${YI_PLATFORM_RESOURCES_${_PLATFORM_UPPER}})

# === START OF OPTION HANDLING ================================================

# Instrumented builds can help with determining code coverage.
# By using this module, the project can set up Debug builds to be able to output
# coverage data.
include(Modules/YiConfigureInstrumentedBuild)
yi_configure_instrumented_build(TARGET ${PROJECT_NAME})

# === END OF OPTION HANDLING ==================================================

# === START OF COMPILER SETUP =================================================

if(MSVC)
    target_compile_options(${PROJECT_NAME}
        PRIVATE $<$<BOOL:${YI_TREAT_WARNINGS_AS_ERRORS}>:/WX>
    )
else()
    target_compile_options(${PROJECT_NAME}
        PRIVATE -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wwrite-strings -Wunused-variable
        PRIVATE -Wno-unused-result -Wno-unused-function
        PRIVATE $<$<BOOL:${YI_TREAT_WARNINGS_AS_ERRORS}>:-Werror>
    )
endif()

# The configuration method is made available for a platform to set up any specific
# functionality that requires use of the target variables, such as PROJECT_NAME, etc.
if(COMMAND yi_configure_platform)
    yi_configure_platform()
endif()

# === END OF COMPILER SETUP ===================================================

# === START OF ASSET COPYING ==================================================

include(Modules/YiConfigureAssetCopying)
yi_configure_asset_copying(PROJECT_TARGET ${PROJECT_NAME}
    OUTPUT_DIRECTORY ${YI_BASE_OUTPUT_DIRECTORY}
    EXCLUDED_EXTENSIONS ${YI_EXCLUDED_ASSET_FILE_EXTENSIONS}
)

# === END OF ASSET COPYING ====================================================

# === START OF PROJECT PACKAGING ==============================================

# Because not every platform requires custom packaging functionality, having a Package target
# is optional only if the platform's CMake file implements the yi_configure_packaging_for_platform
# function.
include(Modules/${_PLATFORM_LOWER}/YiConfigurePackagingForPlatform OPTIONAL)

if (COMMAND yi_configure_packaging_for_platform)
    add_custom_target(Package ${YI_PACKAGE_IN_ALL})
    yi_configure_packaging_for_platform(PROJECT_TARGET ${PROJECT_NAME} 
        PACKAGE_TARGET Package
    )
endif()

# === END OF PROJECT PACKAGING ================================================
