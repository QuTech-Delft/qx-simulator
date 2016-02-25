/**
 * @file    exception.h
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @date    15/03/2010
 * @brief   xpu exception
 *
 * @copyright
 *
 * XPU - C++ Parallel  Programming Library for Multicore Architectures
 *
 * Copyright (C) 2014 Nader Khammassi, All Rights Reserved.
 *
 * This file is part of XPU and has been downloaded from 
 * http://www.xpu-project.net/.
 *
 * XPU is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * XPU is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */


#ifndef __XPU_EXCEPTION_H__
#define __XPU_EXCEPTION_H__

#include <string>            // for string
#include <cstring>           // for strerror
#include <exception>         // for exception 
#include <cerrno>            // for errno

namespace xpu
{
   class exception : public std::exception 
   {

	 public:
	   /**
	    *   construct a exception with a explanatory message.
	    *   
	    *   @param message         explanatory message
	    *   @param system_message  true if system message (from strerror(errno))
	    *                          should be postfixed to the user provided message
	    */
	   inline exception(const std::string &message, bool system_message = false) throw();

	   /**
	    *   provided just to guarantee that no exceptions are thrown.
	    */
	   inline ~exception() throw();

	   /**
	    *   get the exception message
	    *   @return exception message
	    */
	   inline const char *what() const throw();

	 private:
	   
	   std::string user_message;  // exception message

   }; // class exception

   #include "exception.cc"

} // namespace xpu

#endif // __XPU_EXCEPTION_H__


