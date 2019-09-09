/*
 * HOWTO write an External for Pure data
 * (c) 2001-2006 IOhannes m zm�lnig zmoelnig[AT]iem.at
 *
 * this is the source-code for the second example in the HOWTO
 * it creates an object that increments and outputs a midimessage_parse
 * whenever it gets banged.
 *
 * for legal issues please see the file LICENSE.txt
 */



/**
 * include the interface to Pd
 */
#include "m_pd.h"

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
    t_int i_count;
} t_midimessage_parse;


/**
 * this method is called whenever a "bang" is sent to the object
 * a reference to the class-dataspace is given as argument
 * this enables us to do something with the data (e.g. increment the midimessage_parse)
 */
void midimessage_parse_bang(t_midimessage_parse *x)
{
    /*
     * convert the current midimessage_parse value to floating-point to output it later
     */
    t_float f=x->i_count;
    /* increment the midimessage_parse */
    x->i_count++;
    /* send the old midimessage_parse-value to the 1st outlet of the object */
    outlet_float(x->x_obj.ob_outlet, f);
}


/**
 * this is the "constructor" of the class
 * we have one argument of type floating-point (as specified below in the midimessage_parse_setup() routine)
 */
void *midimessage_parse_new(t_floatarg f)
{
    t_midimessage_parse *x = (t_midimessage_parse *)pd_new(midimessage_parse_class);

    /* set the midimessage_parse value to the given argument */
    x->i_count=f;

    /* create a new outlet for floating-point values */
    outlet_new(&x->x_obj, &s_float);

    return (void *)x;
}


/**
 * define the function-space of the class
 */
void midimessage_parse_setup(void) {
    midimessage_parse_class = class_new(gensym("midimessage_parse"),
                              (t_newmethod)midimessage_parse_new,
                              0,
                              sizeof(t_midimessage_parse),
                              CLASS_DEFAULT,
                              A_DEFFLOAT, 0); /* the object takes one argument which is a floating-point and defaults to 0 */

    /* call a function when object gets banged */
    class_addbang(midimessage_parse_class, midimessage_parse_bang);
}