
MIDIMESSAGE_DIR=${CURDIR}/deps/midimessage
PDLIBBUILDER_DIR=${CURDIR}/deps/pd-lib-builder/

export MIDIMESSAGE_DIR
export PDLIBBUILDER_DIR


all: 
	$(MAKE) -C src/midimessage_gen
	$(MAKE) -C src/midimessage_parse
	
midimessage:
	cd $(MIDIMESSAGE_DIR) ; cmake . ;
	$(MAKE) -C $(MIDIMESSAGE_DIR)
	
install:
	$(MAKE) -C src/midimessage_gen install
	$(MAKE) -C src/midimessage_parse install
	
clean:
	$(MAKE) -C src/midimessage_gen clean
	$(MAKE) -C src/midimessage_parse clean

