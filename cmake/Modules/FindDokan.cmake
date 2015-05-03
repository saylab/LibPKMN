####################################################################################
#  Find Dokan
#
#  DOKAN_ROOT        - Input variable: Set to 
#  DOKAN_FOUND       - Output variable: Dokan was found.
#  DOKAN_INCLUDE_DIR - Output variable: Directory which contains the dokan.h file.
#  DOKAN_LIBRARIES   - Output variable: Will be set to dokan.lib location.
####################################################################################
FIND_PATH(DOKAN_INCLUDE_DIR dokan.h PATHS $ENV{ProgramFiles}/Dokan/DokanLibrary ${DOKAN_ROOT})
FIND_LIBRARY(DOKAN_LIBRARY dokan $ENV{ProgramFiles}/Dokan/DokanLibrary ${DOKAN_ROOT})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DOKAN DEFAULT_MSG DOKAN_LIBRARY DOKAN_INCLUDE_DIR)

IF(DOKAN_FOUND)
    SET(DOKAN_LIBRARIES ${DOKAN_LIBRARY})
    INCLUDE_DIRECTORIES(${DOKAN_INCLUDE_DIR})
ENDIF(DOKAN_FOUND)

MARK_AS_ADVANCED(DOKAN_LIBRARY)