/**
 * @file   qxelarator.i
 * @author Imran Ashraf
 * @brief  swig interface file
 */
%define DOCSTRING
"`qxelarator` Python interface to QX simulator as an accelerator."
%enddef

%module(docstring=DOCSTRING) qxelarator

%include "std_string.i"

%{
#include "qx/qxelarator.h"
%}

// Include the header file with above prototypes
%include "qx/qxelarator.h"
