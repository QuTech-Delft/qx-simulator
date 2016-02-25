/**
 * @file		qasm_scanner.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		19-10-15
 * @brief		
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
//#include <ctype>

#include <qasm/token.h>
#include <qasm/syntax_error_exception.h>


namespace qx
{
   namespace qasm
   {
	 friend class parser;

	 public:
	   
	   scanner(const char * str) : m_str(str), m_current(0), m_current_line(0), m_current_pos(0)
	   {
	   }
	   
	   scanner(std::string str) : m_str(str), m_current(0), m_current_line(0), m_current_pos(0)
	   {
	   }

	   /**
	    * return next token. if the token is a number (float), its 
	    *        value is returned in x
	    */
	   token_t next_token(float * x, std::string &var_name) throw (syntax_error_exception)
	   {
		 const char * s  = m_str.c_str();
		 int len         = m_str.length();
		 // skip spaces
		 skip_spaces();
		 if (m_current >= len) return __end_token__;
		 // is a var name : expression must start with var_name = ...
		 if (is_letter(s[m_current]))
		 {
		    std::string str = "";
		    str += s[m_current];
		    m_current++; m_current_pos++;
		    while ((is_letter(s[m_current]) || is_digit(s[m_current])) && (m_current<len))
		    {
			  str += s[m_current];
			  m_current++; m_current_pos++;
		    }
		    token_t t = string_token(str);
		    if (t == __var_name_token__)
		    {
			  var_name = str;
			  return t;
		    }
		    else return t;
		 } 
		  
		 skip_spaces();
		 if (m_current >= len) return __end_token__;
		 
		 // is equal sign   
		 if (s[m_current] == '=')
		 {
		    m_current++; m_current_pos++;
		    return __equal_token__;
		 }
 
		 // is float :  x.yyyy
		 if (is_digit(s[m_current]))
		 {
		    std::string num = "";
		    num += s[m_current];
		    m_current++; m_current_pos++;
		    while (is_digit(s[m_current])  && (m_current<len))
		    {
			  num += s[m_current];
			  m_current++; m_current_pos++;
		    }
		    if (is_dot(s[m_current])) 
		    {
			  num += '.';
			  m_current++; m_current_pos++;
		    }
		    while (is_digit(s[m_current])  && (m_current<len))
		    {
			  num += s[m_current];
			  m_current++; m_current_pos++;
		    }
		    *x = atof(num.c_str());
		    return __num_token__;
		 }
		 if (m_current >= len) return __end_token__;
		 // skip spaces
		 skip_spaces();
		 //while (is_space(s[m_current]) && (m_current<len))
		 //{
		 //   m_current++; m_current_pos++;
		 //}
		 if (m_current >= len) return __end_token__;
		 // is string : var or function
		 if (is_letter(s[m_current]))
		 {
		    std::string str = "";
		    str += s[m_current];
		    m_current++; m_current_pos++;
		    while ((is_letter(s[m_current]) || is_digit(s[m_current])) && (m_current<len))
		    {
			  str += s[m_current];
			  m_current++; m_current_pos++;
		    }
		    token_t t = string_token(str);
		    if (t == __var_name_token__)
		    {
			  var_name = str;
			  return t;
		    }
		    else return t;
		 }
		 // skip spaces
		 skip_spaces();
		 //while (is_space(s[m_current]) && (m_current<len))
		 //{
		 //   m_current++; m_current_pos++;
		 //}
		 if (m_current >= len) return __end_token__;
		 // is period
		 if (is_period(s[m_current]))
		 {
		    m_current++; m_current_pos++;
		    return __period_token__;
		 }
		 // skip spaces
		 skip_spaces();
		 //while (is_space(s[m_current]) && (m_current<len))
		 //{
		 //   m_current++; m_current_pos++;
		 //}
		 if (m_current >= len) return __end_token__;
		 // operators and paren ...
		 switch (s[m_current])
		 {
		      case '+': m_current++; m_current_pos++; return __plus_token__;
                case '-': m_current++; m_current_pos++; return __minus_token__;
                case '*': m_current++; m_current_pos++; return __times_token__;
                case '/': m_current++; m_current_pos++; return __divide_token__;
                case '(': m_current++; m_current_pos++; return __left_paren_token__;
                case ')': m_current++; m_current_pos++; return __right_paren_token__;
                case '.': m_current++; m_current_pos++; return __dot_token__;
                // default: std::cerr << " error: illegal character " << s[m_current] << std::endl; return __error_token__;   // throw exception instead
                default:
			    std::string err = " illegal character '"; err += s[m_current]; // err += "' at line "; err += int_to_str(m_current_line+1); err += " : pos "; err +=  int_to_str(m_current_pos+1); 
			    throw syntax_error_exception(err,m_current_line+1,m_current_pos+1); return __error_token__;   // throw exception instead
		 }
	   }

	   void init()
	   {
		 m_current      = 0;
		 m_current_line = 0;
		 m_current_pos  = 0;
	   }
	   
	   void set_string(std::string s)
	   {
		 m_str = s;
		 init();
	   }

	 private: 

	   inline bool is_space(char x)
	   {
		 switch (x)
		 {
		    case '\f' : return true; 
		    case '\n' : m_current_line++; m_current_pos=-1; return true; 
		    case '\r' : return true; 
		    case '\t' : return true; 
		    case '\v' : return true; 
		    case ' '  : return true; 
		    default   : return false; 
		 }
		 // return ((x == '\f') || (x == '\n') || (x == '\r') || 
		 //	  (x == '\t') || (x == '\v') || (x == ' '));
	   }

	   inline bool is_digit(char x)
	   {
		 return ((x >= '0') && (x<='9'));
	   }

	   inline bool is_letter(char x)
	   {
		 return (((x >= 'a') && (x<='z')) || ((x >= 'A') && (x<='Z')) || (x == '_'));
	   }
	   
	   inline bool is_dot(char x)
	   {
		 return (x=='.');
	   }

	   inline bool is_period(char x)
	   {
		 return (x==';');
	   }

	   inline void skip_spaces()
	   {
           // skip spaces
		 while (is_space(m_str[m_current]) && (m_current<m_str.length()))
		 {
		    m_current++; m_current_pos++;
		 }
	   }

	   token_t string_token(std::string& s)
	   {
		 //std::cout << __func__ << ": " << s << std::endl;
		 if (s == "sin")
		    return __sin_token__;
		 if (s == "cos")
		    return __cos_token__;
		 if (s == "tan")
		    return __tan_token__;
		 if (s == "sinh")
		    return __sinh_token__;
		 if (s == "cosh")
		    return __cosh_token__;
		 if (s == "tanh")
		    return __tanh_token__;
		 if (s == "asin")
		    return __asin_token__;
		 if (s == "acos")
		    return __acos_token__;
		 if (s == "atan")
		    return __atan_token__;
		 if (s == "ln")
		    return __ln_token__;
		 if (s == "log")
		    return __log_token__;
		 if (s == "exp")
		    return __exp_token__;
		 if (s == "sqrt")
		    return __sqrt_token__;
		 return __var_name_token__;
	   }

	   std::string                        m_str;
	   size_t                             m_current;
	   size_t                             m_current_line;
	   size_t                             m_current_pos;
	   //std::vector<token_t>               m_tokens;
	   //std::vector<token_t>::iterator     m_current;
   };
          
	 class scanner
	 {
	    public:

	    private:

	 };

	 /**
	  * int to str
	  */
	 std::string int_to_str(int i)
	 {
	    std::stringstream strm;
	    std::string num;
	    strm << i;
	    strm >> num;
	    return num;
	 }


   } // qasm
} // qx
