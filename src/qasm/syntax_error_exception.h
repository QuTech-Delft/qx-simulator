/**
 * @file		syntax_error_syntax_error_exception.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		03-04-12
 * @brief		
 */
#ifndef __QASM_SYNTAX_ERROR_EXCEPTION_H__
#define __QASM_SYNTAX_ERROR_EXCEPTION_H__

#include <string>            // for string
#include <cstring>           // for strerror
#include <exception>         // for syntax_error_exception 
#include <cerrno>            // for errno

#include <utils.h>

namespace qx
{
   namespace qasm
   {
      class syntax_error_exception : public std::exception 
      {

	    public:
	      /**
	       *   construct a syntax_error_exception with a explanatory message.
	       *   
	       *   @param message         explanatory message
	       *   @param system_message  true if system message (from strerror(errno))
	       *                          should be postfixed to the user provided message
	       */
	      inline syntax_error_exception(const std::string &message, const int line, const int position) throw ();

	      /**
	       *   provided just to guarantee that no syntax_error_exceptions are thrown.
	       */
	      inline ~syntax_error_exception() throw();

	      /**
	       *   get the syntax_error_exception message
	       *   @return syntax_error_exception message
	       */
	      inline const char *what() const throw();

	    private:
	      
	      std::string m_user_message;  // syntax_error_exception message

      }; // class syntax_error_exception


      #include "syntax_error_exception.cc"
      
   }
} // namespace qx

#endif // __XPU_SYNTAX_ERROR_EXCEPTION_H__



