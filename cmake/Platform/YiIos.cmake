if (__yi_ios_project_included)
    return()
endif()
set(__yi_ios_project_included 1)

# This sample has restrictions on the orientation for the devices on iOS,
# so the project uses a custom Platform/YiIos.cmake module to first set
# the orientation restrictions then the shipped YiIos.cmake module will
# be included for the rest of the default platform configuration.

option(YI_IPHONE_ORIENTATION_PORTRAIT "" ON)
option(YI_IPHONE_ORIENTATION_PORTRAIT_UPSIDEDOWN "" ON)
option(YI_IPAD_ORIENTATION_PORTRAIT "" OFF)
option(YI_IPAD_ORIENTATION_PORTRAIT_UPSIDEDOWN "" OFF)

include(Modules/YiCapitalizeString)
yi_capitalize_string(SRC_STRING "${YI_PLATFORM}" 
    RESULT_VARIABLE _PLATFORM_CAPITALIZED
    DELIMITERS "-;_"
)
include("${YouiEngine_DIR}/cmake/Platform/Yi${_PLATFORM_CAPITALIZED}.cmake")
