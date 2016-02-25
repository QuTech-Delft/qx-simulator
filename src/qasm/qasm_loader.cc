/**
 * @file		qasm_loader.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		19-10-15
 * @brief		
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdint.h>

#include <strings.h>
#include <core/circuit.h>


// #include <qasm/token.h>
// #include <qasm/syntax_error_exception.h>

using namespace str;

#define println(x) std::cout << x << std::endl;
#define error(x) std::cout << "[x] error : " << x << std::endl;

typedef enum __gate_type
{
   __hadamard_gate__,
   __cnot_gate__,
   __swap_gate__,
   __phase_gate__,
   __cphase_gate__,
   __pauli_x_gate__,
   __pauli_y_gate__,
   __pauli_z_gate__,
   __toffoli_gate__
} gate_type_t;


/**
 * qasm parser
 */
class qasm_parser
{

   private:

     std::string   file_name;

     int           line_index;
     bool          syntax_error;

     qx::circuit * circuit;


   public:
     
     qasm_parser(std::string file_name) : file_name(file_name)
     {
     }

     uint32_t parse()
     {
	line_index   = 0;
	syntax_error = false;
	println("[-] loading qasm file '" << file_name << "'...");
	std::ifstream stream(file_name.c_str());
	if (stream)
	{
	   char buf[2048];
	   while(stream.getline(buf, 2048))
	   {
	      line_index++;
	      std::string line = buf;
	      if (line.length()>0)
		 process_line(line);
	      if (syntax_error)
		 break;
	   }
	   stream.close();
	   if (syntax_error)
	   {
	      exit(-1);
	   }
	}
	else error("cannot open file " << file_name);
	
	// check semantic
	/*
	if (sampling == 0)
	{
	   std::cerr << "[x] error: sampling rate is not defined or missing !" << std::endl;
	   clean();
	   exit(-1);
	}
	if (database.length() == 0)
	{
	   std::cerr << "[x] error: source database is not defined or missing !" << std::endl;
	   clean();
	   exit(-1);
	}
	if (registrations.size() == 0)
	{
	   std::cerr << "[x] error: there is no class definition : nothing to do !" << std::endl;
	   clean();
	   exit(-1);
	}
	*/
     }

     /**
      * destructor
      */
     virtual ~qasm_parser()
     {
	// clean();
     }


   private:

     /**
      * @brief process line
      */
     void process_line(std::string& line)
     {
	println("processing line " << line_index << " ...");
	if (line[0] == '#') // skip comments
	{
	   println("   comment.");
	   return;
	}
	remove_comment(line,'#');  // remove inline comment
	format_line(line);
	strings words = word_list(line, " ");
	if (words.size() != 2) 
	{
	   std::cerr << "[x] syntax error at line " << line_index << ": \n\t" <<  line << std::endl;
	   syntax_error = true;
	}

	for (int i=0; i<words.size(); i++)
	   println("  > " << words[i]);
	
	if (words[0] == "qubits")    // qubit definition
	{
	   println(" => qubits number: " << atoi(words[1].c_str()));
	}
	else if (words[0] == "h")    // hadamard gate
	{
	   println(" => hadamard gate on: " << atoi(words[1].c_str()));
	} 
	else if (words[0] == "cnot") // cnot gate
	{
	   strings params = word_list(words[1],",");
	   println(" => cnot gate : ctrl_qubit=" << params[0] << ", target_qubit=" << params[1]);
	} 
	else if (words[0] == "swap") // cnot gate
	{
	   strings params = word_list(words[1],",");
	   println(" => swap gate : qubit_1=" << params[0] << ", qubit_2=" << params[1]);
	} 
	/**
	 * pauli gates
	 */
	else if (words[0] == "x")   // x gate
	{
	   println(" => pauli x gate on: " << atoi(words[1].c_str()));
	} 
	else if (words[0] == "y")   // y gate
	{
	   println(" => pauli y gate on: " << atoi(words[1].c_str()));
	} 
	else if (words[0] == "z")   // z gate
	{
	   println(" => pauli z gate on: " << atoi(words[1].c_str()));
	} 
	/**
	 * rotations gates
	 */
	else if (words[0] == "rx")   // rx gate
	{
	   strings params = word_list(words[1],",");
	   println(" => rx gate on " << process_qubit(params[0]) << " (angle=" << params[1] << ")");
	}
	else if (words[0] == "ry")   // ry gate
	{
	   strings params = word_list(words[1],",");
	   println(" => ry gate on " << process_qubit(params[0]) << " (angle=" << params[1] << ")");
	}
	else if (words[0] == "rz")   // rz gate
	{
	   strings params = word_list(words[1],",");
	   println(" => rz gate on " << process_qubit(params[0]) << " (angle=" << params[1] << ")");
	}
	/**
	 * phase 
	 */
	else if (words[0] == "ph")   // rx gate
	{
	   strings params = word_list(words[1],",");
	   println(" => phase gate on " << process_qubit(params[0]) << " (angle=" << params[1] << ")");
	}
	/**
	 * measurement 
	 */
	else if (words[0] == "measure")   // measurement
	{
	   println(" => measure qubit " << atoi(words[1].c_str()));
	} 
	/**
	 * noise 
	 */
	else if (words[0] == "noise")   // operational errors
	{
	   strings params = word_list(words[1],",");
	   println(" => noise (theta=" << params[0].c_str() << ", phi=" << params[1].c_str() << ")");
	} 
	else if (words[0] == "decoherence")   // decoherence
	{
	   println(" => decoherence (dt=" << words[1] << ")");
	}
	else if (words[0] == "qec")   // decoherence
	{
	   println(" => quantum error correction scheme = " << words[1]);
	}
	
	/*
	if (words.size() == 2) // database or sampling line
	{
	   if (words[0] == "qubit") // database
	   {
	      process_database(words);
	   }
	   else if (words[0] == "h")
	   {
	      process_gate(words);
	   } else
	   {
	      std::cerr << "[x] syntax error at line " << line_index << ": \n\t" <<  line << std::endl;
	      syntax_error = true;
	   }
	}
	else
	{
	   std::cerr << "[x] syntax error at line " << line_index << ": \n\t" <<  line << std::endl;
	   syntax_error = true;
	}
	*/

     }

     uint32_t qubit_id(std::string& str)
     {
	std::string id = str.substr(1);
	println("id=" << id);
	return atoi(id.c_str());
     }

     bool is_bit(std::string& str)
     {
	return (str[0] == 'b');
     }

     std::string process_qubit(std::string& str)
     {
	// check validity
	if ((str[0] != 'q') && (str[0] != 'b'))
	{
	   println("[x] syntax error: invalid qubit/bit identifier !");
	   return "";
	}
	for (int i=1; i<(str.size()-1); i++)
	{
	   if (!is_digit(str[i]))
	   {
	      println("[x] syntax error: invalid qubit/bit identifier !");
	      return "";
	   }
	}
	// valid qubit/bit identifier, process it ...
	std::string r="";
	r+=(is_bit(str) ? "bit " : "qubit "); 
	r+= int_to_str(qubit_id(str));
	return r;
     }

     /**
      * @brief set source database name
      */
     void process_gate(strings &words)
     {
	std::cout << "gate name: " << words[1] << std::endl;
	//database = words[1];
     }



     
};


int main(int ragc, char ** argv)
{
   qasm_parser qp("test.qc");

   qp.parse();

   return 0;
}
