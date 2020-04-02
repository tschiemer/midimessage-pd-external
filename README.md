# midimessage-pd-external

https://github.com/tschiemer/midimessage-pd-external

MidiMessage external for PureData (PD) offering conversion from a raw MIDI stream to a human readable, command-like format.


## Building

```shell
git clone --recursive https://github.com/tschiemer/midimessage-pd-external
cd midimessage-pd-external/
make
```

The external object files are generated in the respective object folders, ie `src/midimessage_gen/midimessage_gen.pd_darwin` and `src/midimessage_parse/midimessage_parse.pd_darwin` which have to be placed in a library search path for PD.

## Objects

### midimessage_gen

Generates MIDI byte sequences from given command; for command format also see https://github.com/tschiemer/midimessage

*runningstatus [0,1]* turns on or off running status, ie messages are generating conforming to the MIDI running status feature.


### midimessage_parse

Attempts to parse incoming byte sequences (single or lists of integers) and output the corresponding command (as above).

*runningstatus [0,1]* turns on or off running status, ie messages conforming to the MIDI running status feature are accepted.

*outputdiscarded [0,1]* turns on or off the output of discarded bytes (through the second output).
