# - Try to find the libusb library (v1.0+)
#
#  LIBUSB1_FOUND - system has libusb
#  LIBUSB1_INCLUDE_DIR - the libusb include directory
#  LIBUSB1_LIBRARIES - Link these to use libusb

# Copyright (c) 2006, 2008  Laurent Montel, <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if (LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARIES)

  # in cache already
  set(LIBUSB1_FOUND TRUE)

else (LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARIES)
  # use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  find_package(PkgConfig)
  pkg_check_modules(PC_LIBUSB1 libusb-1.0)

  FIND_PATH(LIBUSB1_INCLUDE_DIR libusb.h
    PATH_SUFFIXES libusb-1.0
    PATHS ${PC_LIBUSB1_INCLUDEDIR} ${PC_LIBUSB1_INCLUDE_DIRS})

  FIND_LIBRARY(LIBUSB1_LIBRARIES NAMES usb-1.0
    PATHS ${PC_LIBUSB1_LIBDIR} ${PC_LIBUSB1_LIBRARY_DIRS})

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBUSB1 DEFAULT_MSG LIBUSB1_LIBRARIES LIBUSB1_INCLUDE_DIR)

  MARK_AS_ADVANCED(LIBUSB1_INCLUDE_DIR LIBUSB1_LIBRARIES)

endif (LIBUSB1_INCLUDE_DIR AND LIBUSB1_LIBRARIES)
