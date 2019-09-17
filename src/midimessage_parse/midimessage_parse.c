/*
 * @file
 * @copyright 2019 Philip Tschiemer, https://filou.se
 * @license MIT
 */



/**
 * include the interface to Pd
 */
#include "m_pd.h"
#include <midimessage.h>
#include <midimessage/stringifier.h>
#include <midimessage/parser.h>
#include <string.h>



/**
 * define a new "class"
 */
static t_class *midimessage_parse_class;



/**
 * this is the dataspace of our new object
 * the first (mandatory) "t_object"
 * and a variable that holds the current midimessage_parse value
 */
typedef struct _midimessage_parse {
    t_object  x_obj;
    t_outlet *list_out;
    t_outlet *discarded_out;
    bool outputDiscardedEnabled;
    Parser_t parser;
    uint8_t buffer[128];
    Message_t message;
    uint8_t msgBuffer[128];
} t_midimessage_parse;


void midimessage_parse_setup(void);
void *midimessage_parse_new();
void midimessage_parse_outputdiscarded(t_midimessage_parse *self, t_floatarg f);
void midimessage_parse_runningstatus(t_midimessage_parse *self, t_floatarg f);
void midimessage_parse_float(t_midimessage_parse *self, t_floatarg f);
void messageHandler(Message_t *msg, void * context);
void discardedHandler(uint8_t * bytes, uint8_t len, void * context);

/**
 * define the function-space of the class
 */
void midimessage_parse_setup(void) {
    midimessage_parse_class = class_new(gensym("midimessage_parse"),
                                      (t_newmethod)midimessage_parse_new,
                                      0,
                                      sizeof(t_midimessage_parse),
                                      CLASS_DEFAULT,
                                      0);

    /* call a function when object gets banged */
//    class_addbang(midimessage_parse_class, midimessage_parse_bang);


    class_addmethod(midimessage_parse_class,
                    (t_method)midimessage_parse_outputdiscarded, gensym("outputdiscarded"), A_DEFFLOAT, 0);
    class_addmethod(midimessage_parse_class,
                    (t_method)midimessage_parse_runningstatus, gensym("runningstatus"), A_DEFFLOAT, 0);

//    class_addmethod(midi)
    class_doaddfloat(midimessage_parse_class, (t_method)midimessage_parse_float);

}

/**
 * this is the "constructor" of the class
 * we have one argument of type floating-point (as specified below in the midimessage_parse_setup() routine)
 */
void *midimessage_parse_new()
{
    t_midimessage_parse *self = (t_midimessage_parse *)pd_new(midimessage_parse_class);



    self->list_out = outlet_new(&self->x_obj, gensym(""));
    self->discarded_out = outlet_new(&self->x_obj, &s_list);

    self->outputDiscardedEnabled = false;

    self->message.Data.SysEx.ByteData = self->msgBuffer;

    parser_init( &self->parser, false, self->buffer, 128, &self->message, messageHandler, discardedHandler, self);

    return (void *)self;
}

void midimessage_parse_outputdiscarded(t_midimessage_parse *self, t_floatarg f)
{
    self->outputDiscardedEnabled = (bool)f;
}

void midimessage_parse_runningstatus(t_midimessage_parse *self, t_floatarg f)
{
    self->parser.RunningStatusEnabled = (bool)f;
}



void midimessage_parse_float(t_midimessage_parse *self, t_floatarg f)
{
    uint8_t byte = f;

    parser_receivedData( &self->parser, &byte, 1 );
}

void messageHandler(Message_t *msg, void * context){
    t_midimessage_parse * self = (t_midimessage_parse*)context;

    uint8_t bytes[256];

    int length = MessagetoString(  bytes, msg );

    if (length <= 0){
        return;
    }

    bytes[length] = '\0';

//    post("rx %s", bytes);

    uint8_t * argv[32];
    uint8_t argc = stringToArgs( argv, 32, bytes, length);

    t_atom atoms[32];
    for(uint8_t i = 1; i < argc; i++){
        SETSYMBOL(&atoms[i-1], gensym((char*)argv[i]));
    }


    outlet_anything( self->list_out, gensym((char*)argv[0]), argc - 1, atoms);
}

void discardedHandler(uint8_t * bytes, uint8_t len, void * context){
    t_midimessage_parse * self = (t_midimessage_parse*)context;

    if ( ! self->outputDiscardedEnabled ){
        return;
    }

    for(uint8_t i = 0; i< len; i++){
        outlet_float( self->discarded_out, bytes[i] );
    }
}