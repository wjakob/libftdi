include(FindSWIG)
include(FindPythonLibs)
include(FindPythonInterp)
include(UseSWIG)

option(PYTHON_BINDINGS "Build python bindings via swig" ON)

if(PYTHON_BINDINGS AND SWIG_FOUND AND PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
    INCLUDE_DIRECTORIES(BEFORE ${CMAKE_SOURCE_DIR}/src)
    INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
    LINK_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/../src)

    swig_add_module ( ftdi1 python ftdi1.i )
    swig_link_libraries ( ftdi1 ftdi1 )

    # do not link python modules on debian
    # http://www.debian.org/doc/packaging-manuals/python-policy/ch-module_packages.html
    if ( NOT EXISTS "/etc/debian_version" )
        swig_link_libraries ( ftdi1 ${PYTHON_LIBRARIES} )
    endif ()

    execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "import sys; print( '%d.%d' % ( sys.version_info[0], sys.version_info[1] ) )"
                      OUTPUT_VARIABLE PYTHON_VERSION
                      OUTPUT_STRIP_TRAILING_WHITESPACE )

    set ( SITEPACKAGE lib${LIB_SUFFIX}/python${PYTHON_VERSION}/site-packages )

    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/_ftdi1.so DESTINATION ${SITEPACKAGE})
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/ftdi1.py DESTINATION ${SITEPACKAGE})

    message(STATUS "Building python bindings via swig. Will be installed under ${CMAKE_INSTALL_PREFIX}/${SITEPACKAGE}")
else(PYTHON_BINDINGS AND SWIG_FOUND AND PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
    message(STATUS "Not building python bindings")
endif(PYTHON_BINDINGS AND SWIG_FOUND AND PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
