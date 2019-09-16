
# PDDIR=${CURDIR}/deps/pure-data

MIDIMESSAGE_DIR=${CURDIR}/deps/midimessage
CUTIL_DIR=${MIDIMESSAGE_DIR}/deps/c-utils/lib
PDLIBBUILDER_DIR=${CURDIR}/deps/pd-lib-builder

cflags += -I${MIDIMESSAGE_DIR}/include -I ${CUTIL_DIR}
ldflags += -L${MIDIMESSAGE_DIR}/lib -lmidimessage
# ldflags += -stdlib=libc++

export PDDIR
export MIDIMESSAGE_DIR
export CUTIL_DIR
export PDLIBBUILDER_DIR
export cflags
export ldflags


all: midimessage
	$(MAKE) -C src/midimessage_gen
	$(MAKE) -C src/midimessage_parse
	
midimessage: ${MIDIMESSAGE_DIR}/Makefile
	$(MAKE) -C ${MIDIMESSAGE_DIR} midimessage
	
${MIDIMESSAGE_DIR}/Makefile:
	cd $(MIDIMESSAGE_DIR) ; cmake .
	
install:
	$(MAKE) -C src/midimessage_gen install
	$(MAKE) -C src/midimessage_parse install
	
clean:
	$(MAKE) -C src/midimessage_gen clean
	$(MAKE) -C src/midimessage_parse clean
	cd ${MIDIMESSAGE_DIR}; $(MAKE) clean;
	$(RM) ${MIDIMESSAGE_DIR}/Makefile

