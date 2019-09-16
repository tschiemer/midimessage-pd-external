/*
 * HOWTO write an External for Pure data
 * (c) 2001-2006 IOhannes m zm�lnig zmoelnig[AT]iem.at
 *
 * this is the source-code for the second example in the HOWTO
 * it creates an object that increments and outputs a midimessage_gen
 * whenever it gets banged.
 *
 * for legal issues please see the file LICENSE.txt
 */



/**
 * include the interface to Pd
 */
#include "m_pd.h"
#include <midimessage.h>
#include <midimessage/stringifier.h>
#include <string.h>

/**
 * define a new "class"
 */
static t_class *midimessage_gen_class;


/**
 * this is the dataspace of our new object
 * the first (mandatory) "t_object"
 * and a variable that holds the current midimessage_gen value
 */
typedef struct _midimessage_gen {
    t_object  x_obj;
    t_inlet *in;
    t_outlet *byte_out;
//    t_outlet *dbg_out;
    bool runningStatusEnabled;
    uint8_t runningStatusState;
} t_midimessage_gen;

void midimessage_gen_setup(void);
void *midimessage_gen_new();
void midimessage_gen_runningstatus(t_midimessage_gen *self, t_floatarg f);
void midimessage_gen_anything(t_midimessage_gen *self, t_symbol *s, int argc, t_atom *argv);
void midimessage_gen_generatorError(int code, uint8_t argc, uint8_t ** argv);


/**
 * define the function-space of the class
 */
void midimessage_gen_setup(void) {
    midimessage_gen_class = class_new(gensym("midimessage_gen"),
                                      (t_newmethod)midimessage_gen_new,
                                      0,
                                      sizeof(t_midimessage_gen),
                                      CLASS_DEFAULT,
                                      0);

    /* call a function when object gets banged */
//    class_addbang(midimessage_gen_class, midimessage_gen_bang);


    class_addmethod(midimessage_gen_class,
                    (t_method)midimessage_gen_runningstatus, gensym("runningstatus"), A_DEFFLOAT, 0);

//    class_addmethod(midi)
    class_addlist(midimessage_gen_class, midimessage_gen_anything);
    class_addanything(midimessage_gen_class, midimessage_gen_anything);

}

/**
 * this is the "constructor" of the class
 * we have one argument of type floating-point (as specified below in the midimessage_gen_setup() routine)
 */
void *midimessage_gen_new()
{
    t_midimessage_gen *self = (t_midimessage_gen *)pd_new(midimessage_gen_class);


//    self->in = inlet_new(&self->x_obj, &self->x_obj.ob_pd,
//              gensym(""), gensym("list"));

    /* create a new outlet for floating-point values */
    self->byte_out = outlet_new(&self->x_obj, &s_float);
//    self->dbg_out = outlet_new(&self->x_obj, &s_symbol);

    self->runningStatusEnabled = false;
    self->runningStatusState = MidiMessage_RunningStatusNotSet;

    return (void *)self;
}

void midimessage_gen_runningstatus(t_midimessage_gen *self, t_floatarg f)
{
    self->runningStatusEnabled = (bool)f;
    self->runningStatusState = MidiMessage_RunningStatusNotSet;

}


void midimessage_gen_anything(t_midimessage_gen *self, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 32){
        post("Error: too many arguments (max 32, or recompile with larger number...");
        return;
    }

//    post("argc = %d", argc);

    // turn into usable form
    uint8_t * argvStr[32];
    uint8_t line[256];

    // get first argument from passed symbol
    argvStr[0] = line;
    uint8_t pos = 0;

    for(;s->s_name[pos] != '\0'; pos++){
        line[pos] = s->s_name[pos];
    }
    line[pos++] = '\0';

//    post("argv[0] = %s", argvStr[0]);

    for(int i = 0; i < argc; i++){
        atom_string( &argv[i], (char*)&line[pos], 256 - pos);
        argvStr[i+1] = &line[pos];
        pos += strlen((char*)argvStr[i+1]) + 1;
//        post("argv[%d] = %s", i+1, argvStr[i+1]);
    }

    //  try to turn into message
    uint8_t sysexBuffer[128];
    Message_t msg;
    msg.Data.SysEx.ByteData = sysexBuffer;

    int result = MessagefromArgs( &msg, argc+1, argvStr );

    if (StringifierResultOk != result){
        midimessage_gen_generatorError(result, argc+1, argvStr);
        return;
    }


    // turn message into byte sequence
    uint8_t bytes[128];
    uint8_t length = pack( bytes, &msg );

    if (length == 0){
        return;
    }

    // prepare output
    uint8_t * output = bytes;
    if (self->runningStatusEnabled && updateRunningStatus( &self->runningStatusState, bytes[0] )){
        output = &bytes[1];
        length--;
    }

    // output byte by byte as number
    for (uint8_t i = 0; i < length; i++){
        t_float f = output[i];
        outlet_float( self->byte_out, f );
    }
}


void midimessage_gen_generatorError(int code, uint8_t argc, uint8_t ** argv){

    for(uint8_t i = 1; i < argc; i++){
        argv[i][-1] = ' ';
//        post("%d = [%s] of [%s]", i-1, argv[i-1], argv[0]);
    }

    switch (code) {
        case StringifierResultGenericError:
            post("Generic error: %s", argv[0]);
            break;
        case StringifierResultInvalidValue:
            post("Invalid value: %s", argv[0]);
            break;
        case StringifierResultWrongArgCount:
            post( "Wrong arg count: %s", argv[0]);
            break;
        case StringifierResultNoInput:
            post( "No input: %s", argv[0]);
            break;
        case StringifierResultInvalidU4:
            post( "Invalid U4/Nibble Value: %s", argv[0]);
            break;
        case StringifierResultInvalidU7:
            post( "Invalid U7 Value: %s", argv[0]);
            break;
        case StringifierResultInvalidU14:
            post( "Invalid U14 Value: %s", argv[0]);
            break;
        case StringifierResultInvalidU21:
            post( "Invalid U21 Value: %s", argv[0]);
            break;
        case StringifierResultInvalidU28:
            post( "Invalid U28 Value: %s", argv[0]);
            break;
        case StringifierResultInvalidU35:
            post( "Invalid U35 Value: %s", argv[0]);
            break;
        case StringifierResultInvalidHex:
            post( "Invalid Hex Value: %s", argv[0]);
            break;
        default:
            post("unknown error??? %s", argv[0]);
    }
}


