/**
 * @file        qx_server.h
 * @author      Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date        25-02-16
 */


#ifndef QX_SERVER_H
#define QX_SERVER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include <map>


#include <xpu/net/tcp_server_socket.h>
#include <qcode/quantum_code_loader.h>



namespace qx
{
   /**
    * \brief qx server
    */
   class qx_server
   {
      public:
        
	/**
	 * ctor
	 */
	qx_server(uint32_t port=5555) : port(port), qubits_count(0), parsed_successfully(false), syntax_error(false), semantic_error(false), error_model(__unknown_error_model__), error_probability(0)
	{
	}

	/**
	 * start
	 */
	void start()
	{
	   char buf[512];
	   xpu::tcp_server_socket server(port);
	   xpu::tcp_socket * sock = server.accept();
	   std::cout << "[+] client connected : " << sock->get_foreign_address() << ":" << sock->get_foreign_port() << std::endl;
	   //sock->send("welcom to server !", 19);
	   bool stop=false;
	   while (!stop)
	   {
	      clear(buf,512);
	      size_t bytes = sock->recv(buf, 512); 
	      buf[bytes] = '\0';
	      std::string cmd = buf;
	      std::cout << cmd << std::endl;
	      if (cmd == "stop")
		 return;
	      process_line(cmd);
	      //sock->send("received.", 10);
	   }
	   return;
	}

      private:

        void clear(char * buf, size_t size)
	{
	   for (uint32_t i=0; i<size; i++)
	      buf[0] = '\0';
	}



#define print_syntax_error(err) \
      {\
	 std::cerr << "[x] syntax error at line " << line_index << " : " << err << std::endl; \
	 std::cerr << "   +--> code: \"" << original_line << "\"" << std::endl; \
	 syntax_error = true;\
	 return 1;\
      }


#define print_semantic_error(err) \
      {\
	 std::cerr << "[x] semantic error at line " << line_index << " : " << err << std::endl; \
	 std::cerr << "   +--> code: \"" << original_line << "\"" << std::endl; \
	 semantic_error = true;\
	 return 1;\
      }

      private:

      /**
       * \brief check if the circuit label is valid
       */
      bool is_label(std::string& str)
      {
	 if (!is_dot(str[0]))
	    return false;
	 for (uint32_t i=1; i<str.size(); ++i)
	 {
	    if ((!is_letter(str[i])) && (!is_digit(str[i])))
	       return false;
	 }
	 return true;
      }

      /**
       * \brief check if <str> is a natural bit
       */
      bool is_bit(std::string& str)
      {
	 return (str[0] == 'b');
      }


      /**
       * \brief retrieve qubit number <N> from a string "qN"
       */
      uint32_t qubit_id(std::string& str)
      {
	 std::string& original_line = str;
	 std::string  qubit;
	 if (str[0] != 'q')
	 {
	    // search in the qubit map first
	    map_t::iterator it = definitions.find(str);
	    if (it != definitions.end())
	    {
	       qubit = it->second;
	       // println(" def[" << str << "] -> " << qubit);
	       if (qubit[0] != 'q')
		  print_syntax_error(" invalid qubit identifier : qubit name not defined, you should use 'map' to name qubit before using it !");
	       str = qubit;
	    }
	    else
	       print_syntax_error(" invalid qubit identifier !");
	 }
	 std::string id = str.substr(1);
	 for (int i=0; i<id.size(); ++i)
	 {
	    if (!is_digit(id[i]))
	       print_syntax_error(" invalid qubit identifier !");
	 }
	 // println(" qubit id -> " << id);
	 return (atoi(id.c_str()));
      }

      uint32_t bit_id(std::string& str)
      {
	 std::string& original_line = str;
	 if (str[0] != 'b')
	    print_syntax_error(" invalid bit identifier !");
	 std::string id = str.substr(1);
	 for (int i=0; i<id.size(); ++i)
	 {
	    if (!is_digit(id[i]))
	       print_syntax_error(" invalid qubit identifier !");
	 }
	 return (atoi(id.c_str()));
      }

      /**
       * \return 
       *    a pointer to the current sub-circuit 
       */
      qx::circuit * current_sub_circuit(uint32_t qubits_count)
      {
	 if (circuits.size())
	 {
	    return circuits.back();
	 }
	 circuits.push_back(new qx::circuit(qubits_count, "default"));
	 return circuits.back();
      }



      /**
       * \brief 
       *   process line
       */
      int32_t process_line(std::string& line)
      {
	 // println("processing line " << line_index << " ...");
	 format_line(line);
	 if (is_empty(line))
	    return 0;
	 if (line[0] == '#') // skip comments
	 {
	    // println("   comment.");
	    return 0;
	 }
	 std::string original_line(line);
	 remove_comment(line,'#');  // remove inline comment
	 format_line(line);
	 if (is_label(line))
	 {
	    if (qubits_count==0) 
	    {
	       print_semantic_error(" qubits number must be defined first !");
	    }
	    else
	    {
	       // println("label : new circuit '" << line << "' created.");
	       circuits.push_back(new qx::circuit(qubits_count, line.substr(1)));
	       return 0;
	    }
	 }

	 strings words = word_list(line, " ");
	 // process display commands
	 if (words.size() == 1)
	 {
	    if (words[0] == "display")
	       current_sub_circuit(qubits_count)->add(new qx::display());
	    else if (words[0] == "display_binary")
	       current_sub_circuit(qubits_count)->add(new qx::display(true));
	    else if (words[0] == "measure")
	       current_sub_circuit(qubits_count)->add(new qx::measure());
	    else
	       print_syntax_error("unknown commad !");
	    return 0;
	 }

	 if (words.size() != 2) 
	    print_syntax_error("malformed code !");


	 if (words[0] == "qubits")    // qubit definition
	 {
	    if (qubits_count) 
	       print_syntax_error("qubits number already defined !");

	    qubits_count = atoi(words[1].c_str());

	    if ((qubits_count > 0) && (qubits_count < MAX_QUBITS))
	    {
	      // println(" => qubits number: " << qubits_count);
	    }
	    else
	       print_syntax_error(" too much qubits (" << qubits_count << ") !");
	 }
	 else if (qubits_count == 0)
	 {
	    print_semantic_error(" qubits number must be defined first !");
	 }
	 else if (words[0] == "map") // definitions
	 {
	    strings params = word_list(words[1],",");
	    uint32_t q     = qubit_id(params[0]);
	    std::string qubit = params[0];
	    std::string name  = params[1];
	    if (q > (qubits_count-1))
	       print_semantic_error(" qubit out of range !");
	    definitions[name] = qubit;
	    // println(" => map qubit " << name << " to " << qubit);
	 }
	 else if (words[0] == "h")    // hadamard gate
	 {
	    uint32_t q = qubit_id(words[1]); // atoi(words[1].c_str());
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => hadamard gate on: " << q);
	    current_sub_circuit(qubits_count)->add(new qx::hadamard(q));
	 } 
	 else if (words[0] == "cnot") // cnot gate
	 {
	    strings params = word_list(words[1],",");
	    uint32_t cq = qubit_id(params[0]);
	    uint32_t tq = qubit_id(params[1]);
	    if (cq > (qubits_count-1))
	       print_semantic_error(" control qubit out of range !");
	    if (tq > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => cnot gate : ctrl_qubit=" << cq << ", target_qubit=" << tq);
	    current_sub_circuit(qubits_count)->add(new qx::cnot(cq,tq));
	 } 
	 else if (words[0] == "swap") // cnot gate
	 {
	    strings params = word_list(words[1],",");
	    uint32_t q1 = qubit_id(params[0]);
	    uint32_t q2 = qubit_id(params[1]);
	    if ((q1 > (qubits_count-1)) || (q1 > (qubits_count-1)))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => swap gate : qubit_1=" << q1 << ", qubit_2=" << q2); 
	    current_sub_circuit(qubits_count)->add(new qx::swap(q1,q2));
	 } 

	 /**
	  * controlled phase shift
	  */
	 else if (words[0] == "cr") 
	 {
	    strings params = word_list(words[1],",");
	    uint32_t q1 = qubit_id(params[0]);
	    uint32_t q2 = qubit_id(params[1]);
	    if ((q1 > (qubits_count-1)) || (q1 > (qubits_count-1)))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => controlled phase shift gate : ctrl_qubit=" << q1 << ", target_qubit=" << q2); 
	    current_sub_circuit(qubits_count)->add(new qx::ctrl_phase_shift(q1,q2));
	 } 

	 /**
	  * pauli gates
	  */
	 else if (words[0] == "x")   // x gate
	 {
	    uint32_t q = qubit_id(words[1]);
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => pauli x gate on: " << q);
	    current_sub_circuit(qubits_count)->add(new qx::pauli_x(q));
	 } 
	 else if (words[0] == "cx")   // x gate
	 {
	    strings params = word_list(words[1],",");
	    bool bit = is_bit(params[0]);
	    uint32_t ctrl   = (bit ? bit_id(params[0]) : qubit_id(params[0]));
	    uint32_t target = qubit_id(params[1]);

	    if (ctrl > (qubits_count-1))
	       print_semantic_error(" ctrl qubit out of range !");
	    if (target > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    if (bit)
	    {
	       // println(" => binary controlled pauli_x gate (ctrl=" << ctrl << ", target=" << target << ")");
	       current_sub_circuit(qubits_count)->add(new qx::bin_ctrl(ctrl,new qx::pauli_x(target)));
	    } 
	    else
	    {
	       // println(" => controlled pauli_x gate (ctrl=" << ctrl << ", target=" << target << ")");
	       current_sub_circuit(qubits_count)->add(new qx::cnot(ctrl,target));
	    }
	 } 
	 else if (words[0] == "y")   // y gate
	 {
	    uint32_t q = qubit_id(words[1]);
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => pauli y gate on: " << atoi(words[1].c_str()));
	    current_sub_circuit(qubits_count)->add(new qx::pauli_y(q));
	 } 
	 else if (words[0] == "z")   // z gate
	 {
	    uint32_t q = qubit_id(words[1]);
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => pauli z gate on: " << atoi(words[1].c_str()));
	    current_sub_circuit(qubits_count)->add(new qx::pauli_z(q));
	 }	
	 else if (words[0] == "cz")   // z gate
	 {
	    strings params = word_list(words[1],",");
	    bool bit = is_bit(params[0]);
	    uint32_t ctrl   = (bit ? bit_id(params[0]) : qubit_id(params[0]));
	    uint32_t target = qubit_id(params[1]);

	    if (ctrl > (qubits_count-1))
	       print_semantic_error(" ctrl qubit out of range !");
	    if (target > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    if (bit)
	    {
	       // println(" => binary controlled pauli_z gate (ctrl=" << ctrl << ", target=" << target << ")");
	       current_sub_circuit(qubits_count)->add(new qx::bin_ctrl(ctrl,new qx::pauli_z(target)));
	    } 
	    else
	    {
	       //println(" => controlled pauli_z gate (ctrl=" << ctrl << ", target=" << target << ")");
	       println("quantum controlled-z not implemented yet !");
	       // current_sub_circuit(qubits_count)->add(new qx::cnot(ctrl,target));
	    }
	 } 	 
	 /**
	  * T gate
	  */
	 else if (words[0] == "t")   // T gate
	 {
	    uint32_t q = qubit_id(words[1]);
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => t gate on: " << q);
	    current_sub_circuit(qubits_count)->add(new qx::t_gate(q));
	 }
	 /**
	  * Tdag gate
	  */
	 else if (words[0] == "tdag")   // Tdag gate
	 {
	    uint32_t q = qubit_id(words[1]);
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => t gate on: " << q);
	    current_sub_circuit(qubits_count)->add(new qx::t_dag_gate(q));
	 }

	 /**
	  * prepz
	  */
	 else if (words[0] == "prepz")   // x gate
	 {
	    uint32_t q = qubit_id(words[1]);
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => t gate on: " << q);
	    current_sub_circuit(qubits_count)->add(new qx::measure(q));
	    current_sub_circuit(qubits_count)->add(new qx::bin_ctrl(q,new qx::pauli_x(q)));
	 }




	 /**
	  * rotations gates
	  */
	 else if (words[0] == "rx")   // rx gate
	 {
	    strings params = word_list(words[1],",");
	    uint32_t q = qubit_id(params[0]);
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => rx gate on " << process_qubit(params[0]) << " (angle=" << params[1] << ")");
	    current_sub_circuit(qubits_count)->add(new qx::rx(q,atof(params[1].c_str())));
	 }
	 else if (words[0] == "ry")   // ry gate
	 {
	    strings params = word_list(words[1],",");
	    uint32_t q = qubit_id(params[0]);
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => ry gate on " << process_qubit(params[0]) << " (angle=" << params[1] << ")");
	    current_sub_circuit(qubits_count)->add(new qx::ry(q,atof(params[1].c_str())));
	 }
	 else if (words[0] == "rz")   // rz gate
	 {
	    strings params = word_list(words[1],",");
	    uint32_t q = qubit_id(params[0]);
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => rz gate on " << process_qubit(params[0]) << " (angle=" << params[1] << ")");
	    current_sub_circuit(qubits_count)->add(new qx::rz(q,atof(params[1].c_str())));
	 }

	 /**
	  * phase 
	  */
	 else if (words[0] == "ph")   // phase shift gate
	 {
	    //strings params = word_list(words[1],",");
	    uint32_t q = qubit_id(words[1]);
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => phase gate on " << process_qubit(words[1]));
	    current_sub_circuit(qubits_count)->add(new qx::phase_shift(q));
	 }

	 /**
	  * measurement 
	  */
	 else if (words[0] == "measure")   // measurement
	 {
	    uint32_t q = qubit_id(words[1]);
	    if (q > (qubits_count-1))
	       print_semantic_error(" target qubit out of range !");
	    // println(" => measure qubit " << atoi(words[1].c_str()));
	    // println(" => measure qubit " << q);
	    current_sub_circuit(qubits_count)->add(new qx::measure(q));
	 } 
	 /**
	  * toffoli 
	  */
	 else if (words[0] == "toffoli")   // rx gate
	 {
	    strings params = word_list(words[1],",");
	    if (params.size() != 3)
	       print_semantic_error(" toffoli gate requires 3 qubits !");
	    uint32_t q0 = qubit_id(params[0]);
	    uint32_t q1 = qubit_id(params[1]);
	    uint32_t q2 = qubit_id(params[2]);
	    if (q0 > (qubits_count-1)) print_semantic_error(" first control qubit out of range !");
	    if (q1 > (qubits_count-1)) print_semantic_error(" scond control qubit out of range !");
	    if (q2 > (qubits_count-1)) print_semantic_error(" target qubit out of range !");
	    // println(" => toffoli gate on " << process_qubit(params[2]) << " (ctrl_q1=" << params[0] << ", ctrl_q2=" << params[1] << ")");
	    current_sub_circuit(qubits_count)->add(new qx::toffoli(q0,q1,q2));
	 }
	 /**
	  * error model
	  */
	 else if (words[0] == "error_model")   // operational errors
	 {
	    strings params = word_list(words[1],",");
	    if (params.size() != 2)
	       print_syntax_error(" error mode should be specified according to the following syntax: 'error_model depolarizing_channel,0.01' ");
	    if (params[0] == "depolarizing_channel")
	    {
	       error_model = __depolarizing_channel__;
	       error_probability = atof(params[1].c_str());
	       println(" => error model:  (name=" << params[0].c_str() << ", error_probability=" << error_probability << ")");
	    }
	    else
	       print_semantic_error(" unknown error model !");
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
	 else
	    print_syntax_error(" unknown gate or command !");

	 return 0;
      }




      private:

        int           line_index;
        bool          parsed_successfully;
        bool          syntax_error;
        bool          semantic_error;

        // definitions
        map_t         definitions;

        uint32_t      qubits_count;

	// error model
	qx::error_model_t          error_model;
	double                     error_probability;

        uint32_t      port;
	circuits_t    circuits;
	qu_register * reg;

   };
}



#endif // QX_SERVER_H



