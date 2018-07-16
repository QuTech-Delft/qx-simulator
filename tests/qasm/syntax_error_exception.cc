/**
 * @file		syntax_error_syntax_error_exception.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		03-04-12
 * @brief		
 */

/**
 * syntax_error_exception implementation
 */

syntax_error_exception::syntax_error_exception(const std::string &message, 
                                               const int line, 
									  const int position) 
                                               //const std::string& expected, 
									  //const std::string& found)
                                               throw()  
{
   m_user_message = "[x] syntax error exception : at line ";
   m_user_message.append(int_to_str(line));
   m_user_message.append(":");
   m_user_message.append(int_to_str(position));
   /*
   m_user_message.append(" : expected '");
   m_user_message.append(expected);
   m_user_message.append("', found '");
   m_user_message.append(found);
   */
   m_user_message.append("' : ");
   m_user_message.append(message);
}

/**
 * dtor
 */

syntax_error_exception::~syntax_error_exception() throw() 
{
}

/**
 * explainatory message
 */

const char *syntax_error_exception::what() const throw() 
{
  return m_user_message.c_str();
}

