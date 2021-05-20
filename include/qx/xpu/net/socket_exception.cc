
/**
 * socket_exception implementation
 */

socket_exception::socket_exception(const std::string &message, 
                                   bool system_message)
                                   throw() : user_message(message) 
{
  if (system_message) 
  {
    user_message.append(": ");
    user_message.append(strerror(errno));
  }
}


socket_exception::~socket_exception() throw() 
{
}

const char *socket_exception::what() const throw() 
{
  return user_message.c_str();
}

