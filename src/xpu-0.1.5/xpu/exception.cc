/**
 * @file    exception.cc
 * @author  Nader KHAMMASSI
 * @contact nader.khammassi@gmail.com
 * @date    15/03/2010
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




/**
 * exception implementation
 */

exception::exception(const std::string &message, 
                     bool system_message)
                     throw() : user_message(message) 
{
  if (system_message) 
  {
    user_message.append(": ");
    user_message.append(strerror(errno));
  }
}

/**
 * dtor
 */

exception::~exception() throw() 
{
}

/**
 * explainatory message
 */

const char *exception::what() const throw() 
{
  return user_message.c_str();
}

