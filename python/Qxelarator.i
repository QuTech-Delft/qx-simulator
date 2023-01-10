%define DOCSTRING
"`qxelarator` Python interface to QX simulator as an accelerator."
%enddef

%module(docstring=DOCSTRING) qxelarator

%include "std_string.i"

%{
#include "qx/Qxelarator.h"
%}

// Include the header file with above prototypes
%include "qx/Qxelarator.h"
