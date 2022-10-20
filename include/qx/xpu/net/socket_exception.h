#pragma once

#include <cerrno>    // for errno
#include <cstring>   // for strerror
#include <exception> // for exception
#include <string>    // for string

namespace xpu {

class socket_exception : public std::exception {

public:
    /**
     *   construct a socket_exception with a explanatory message.
     *
     *   @param message         explanatory message
     *   @param system_message  true if system message (from strerror(errno))
     *                          should be postfixed to the user provided message
     */
    socket_exception(const std::string &message,
                     bool system_message = false) throw();

    /**
     *   provided just to guarantee that no exceptions are thrown.
     */
    ~socket_exception() throw();

    /**
     *   get the exception message
     *   @return exception message
     */
    const char *what() const throw();

private:
    std::string user_message; // exception message

}; // class socket_exception
} // namespace xpu