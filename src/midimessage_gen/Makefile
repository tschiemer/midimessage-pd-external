# Makefile to build class 'counter' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.


# library name
lib.name = midimessage_gen

# input source file (class name == source file basename)
class.sources = midimessage_gen.c 
#${MIDIMESSAGE_DIR}/src/midimessage.cpp ${MIDIMESSAGE_DIR}/src/parser.cpp ${MIDIMESSAGE_DIR}/src/stringifier.cpp ${CUTIL_DIR}/util-hex.c

# all extra files to be included in binary distribution of the library
datafiles =

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder

