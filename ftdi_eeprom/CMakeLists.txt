
option(FTDI_EEPROM "Build ftdi_eeprom" ON)

if ( FTDI_EEPROM )
  find_package ( Confuse )
else(FTDI_EEPROM)
  message(STATUS "ftdi_eeprom build is disabled")
endif ()


if ( CONFUSE_FOUND )
  message(STATUS "Building ftdi_eeprom")

  include_directories ( ${CONFUSE_INCLUDE_DIRS} )

  list ( APPEND libs ${CONFUSE_LIBRARIES} )

  # Version defines
  set ( EEPROM_MAJOR_VERSION 0 )
  set ( EEPROM_MINOR_VERSION 17 )
  set ( EEPROM_VERSION_STRING ${EEPROM_MAJOR_VERSION}.${EEPROM_MINOR_VERSION} )

  include_directories ( BEFORE ${CMAKE_SOURCE_DIR}/src )
  include_directories ( BEFORE ${CMAKE_CURRENT_BINARY_DIR} )

  configure_file(
    ftdi_eeprom_version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/ftdi_eeprom_version.h
  )

  add_executable ( ftdi_eeprom main.c )
  target_link_libraries ( ftdi_eeprom ftdi1 )
  target_link_libraries ( ftdi_eeprom ${CONFUSE_LIBRARIES} )

  install ( TARGETS ftdi_eeprom DESTINATION bin )
else ()
  message ( STATUS "libConfuse not found, won't build ftdi_eeprom" )
endif ()

