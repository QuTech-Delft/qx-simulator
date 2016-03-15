/**
 * @file		circuit.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		03-10-15
 * @brief		
 */
#ifndef QX_CIRCUIT_H
#define QX_CIRCUIT_H


#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

#define println(x) std::cout << x << std::endl
#define print(x) std::cout << x 

#include <core/gate.h>

#ifdef XPU_TIMER
#include <xpu/timer.h>
#endif // XPU_TIMER


namespace qx
{
   class circuit
   {
           private:

                   uint32_t            n_qubit;
                   std::vector<gate *> gates;
		   std::string         name;

           public:

                   /**
	    * \brief circuit constructor
            */
	   circuit(uint32_t n_qubit, std::string name = "") : n_qubit(n_qubit), name(name)
	   {
	   }

	   /**
            * \brief destructor
            */
           ~circuit()
	   {
	     for (std::vector<gate*>::iterator it= gates.begin(); it != gates.end(); it++)
	       delete (*it);
	   }

	   /**
	    * \brief add gate <g> at the end of the circuit
	    */
	   void add(gate * g)
	   {
		 // check gate validity before (target/ctrl qubits < n_qubit)
		 gates.push_back(g);
	   }

	   /**
	    * \brief return gate <i>
	    */
	   qx::gate * operator [] (uint32_t i)
	   {
	      assert(i < gates.size());
	      return gates[i];
	   }

           /**
	    * \brief return gate <i>
	    */
	   qx::gate * get(uint32_t i)
	   {
	      assert(i < gates.size());
	      return gates[i];
	   }


	   void execute(qu_register& reg, bool verbose=false, bool only_binary=false)
	   {

	      #ifdef XPU_TIMER
	      println("[+] executing circuit '" << name << "' ...");
	      xpu::timer tmr;
	      tmr.start();
	      #endif

	      if (!verbose) 
		 for (uint32_t i=0; i<gates.size(); ++i)
		    gates[i]->apply(reg);
	      else
	      {
		 for (uint32_t i=0; i<gates.size(); ++i)
		 {
		    println("[-] executing gate " << i << "...");
		    gates[i]->dump();
		    gates[i]->apply(reg);
		    reg.dump(only_binary);
		 }
	      }

	      #ifdef XPU_TIMER
	      tmr.stop();
	      println("[+] circuit execution time: " << tmr.elapsed() << " sec.");
	      #endif // XPU_TIMER

	   }

	   /**
	    * \return gates count
	    */
	   uint32_t size()
	   {
	      return gates.size();
	   }

           /**
	    * insert a gate at the specified 
	    * position
	    */
	   void insert(uint32_t pos, qx::gate * g)
	   {
	      gates.insert(gates.begin()+pos,g);
	   }


	   /**
	    * \return the name of the circuit
	    */
	   std::string id()
	   {
	      return name;
	   }

	   void dump()
	   {
	      println("[+++] circuit '" << name << "' :");
	      for (uint32_t i=0; i<gates.size(); ++i)
	      {
		 print("  |--" << i << "--");
		 gates[i]->dump();
	      }
	   }

	   uint32_t get_qubit_count()
	   {
		 return n_qubit;
	   }
	   
	   void set_qubit_count(uint32_t n)
	   {
		 n_qubit = n;
		 // check gate validity (target/ctrl qubits < n_qubit)
	   }
	   	   
   };
} // namespace qx


#endif // QX_CIRCUIT_H

