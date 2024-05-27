#pragma once

#include <core/circuit.h>

namespace qx {
namespace qecc {

/**
 * \brief 17q ninja star
 */
class ninja_star_17q {
private:
#define frame_size 8    // number of syndrome qubits
#define history_depth 3 // number of history frames

    typedef qx::state_t frame_t[frame_size]; // single frame

    frame_t history[history_depth]; // 0 is the oldest frame, 2 the latest

public:
#define __X44__ (reg->get_binary(0) == qx::__state_1__)
#define __D1__ (reg->get_binary(1) == qx::__state_1__)
#define __D3__ (reg->get_binary(2) == qx::__state_1__)
#define __D5__ (reg->get_binary(3) == qx::__state_1__)
#define __Z10__ (reg->get_binary(4) == qx::__state_1__)
#define __X2__ (reg->get_binary(5) == qx::__state_1__)
#define __Z4__ (reg->get_binary(6) == qx::__state_1__)
#define __D11__ (reg->get_binary(7) == qx::__state_1__)
#define __D13__ (reg->get_binary(8) == qx::__state_1__)
#define __D15__ (reg->get_binary(9) == qx::__state_1__)
#define __Z12__ (reg->get_binary(10) == qx::__state_1__)
#define __X14__ (reg->get_binary(11) == qx::__state_1__)
#define __Z16__ (reg->get_binary(12) == qx::__state_1__)
#define __D21__ (reg->get_binary(13) == qx::__state_1__)
#define __D23__ (reg->get_binary(14) == qx::__state_1__)
#define __D25__ (reg->get_binary(15) == qx::__state_1__)
#define __X22__ (reg->get_binary(16) == qx::__state_1__)

#define __state(x) (x == 0 ? '0' : (x == 1 ? '1' : 'X'))

    /**
     * \brief ctor
     */
    ninja_star_17q(qx::qu_register *reg) : reg(reg) {}

    /**
     * \brief history update
     */
    void update_history() {
        // slides window
        for (uint32_t h = 1; h < history_depth; ++h)
            for (uint32_t i = 0; i < frame_size; ++i)
                history[h - 1][i] = history[h][i];
        // update last frame
        history[history_depth - 1][0] = reg->get_binary(0);
        history[history_depth - 1][1] = reg->get_binary(4);
        history[history_depth - 1][2] = reg->get_binary(5);
        history[history_depth - 1][3] = reg->get_binary(6);
        history[history_depth - 1][4] = reg->get_binary(10);
        history[history_depth - 1][5] = reg->get_binary(11);
        history[history_depth - 1][6] = reg->get_binary(12);
        history[history_depth - 1][7] = reg->get_binary(16);
    }

    /**
     * \brief print history frames
     */
    void print_history() {
        for (uint32_t h = 0; h < history_depth; ++h) {
            print("[frame " << h << "] [ ");
            for (uint32_t i = 0; i < frame_size; ++i)
                print(history[0][i] << " ");
            println("]");
        }
    }

    /**
     * \brief print ns17 state (text)
     */
    void print_state() {
        println("[+]--- 17q ninja star state ---");
        println(" X44 = ", reg->get_binary(0));
        println(" D1  = ", reg->get_binary(1));
        println(" D3  = ", reg->get_binary(2));
        println(" D5  = ", reg->get_binary(3));
        println(" Z10 = ", reg->get_binary(4));
        println(" X2  = ", reg->get_binary(5));
        println(" Z4  = ", reg->get_binary(6));
        println(" D11 = ", reg->get_binary(7));
        println(" D13 = ", reg->get_binary(8));
        println(" D15 = ", reg->get_binary(9));
        println(" Z12 = ", reg->get_binary(10));
        println(" X14 = ", reg->get_binary(11));
        println(" Z16 = ", reg->get_binary(12));
        println(" D21 = ", reg->get_binary(13));
        println(" D23 = ", reg->get_binary(14));
        println(" D25 = ", reg->get_binary(15));
        println(" X22 = ", reg->get_binary(16));
        println("[+]----------------------------");
    }

    /**
     * \brief correct bitflip on data qubits
     */
    void correct_bitflips(
        uint32_t round) // correct on multiple round using different correction
                        // when some syndromes are still there
    {
        qx::qu_register &r = *reg;
        if (round == 1) // correct for 2 ancillas syndrom
        {
            if (__Z10__ && __Z12__)
                qx::pauli_x(7).apply(r); // correct D11
            if (__Z12__ && __Z4__)
                qx::pauli_x(8).apply(r); // correct D13
            if (__Z4__ && __Z16__)
                qx::pauli_x(9).apply(r); // correct D15
        } else if (round == 2) // correct for single ancilla syndrom (round 2)
        {
            // correct back if measurement erro on one the 2 surrounding
            // syndroms
            /*
            if (__Z10__ && __Z12__)
               qx::pauli_x(7).apply(r); // correct back D11
            if (__Z12__ && __Z4__)
               qx::pauli_x(8).apply(r); // correct back D13
            if (__Z4__ && __Z16__)
               qx::pauli_x(9).apply(r); // correct back D15
            */
            if (__Z10__)
                qx::pauli_x(1).apply(r); // correct D1
            if (__Z4__)
                qx::pauli_x(2).apply(r); // correct D3
            if (__Z12__)
                qx::pauli_x(13).apply(r); // correct D21
            if (__Z16__)
                qx::pauli_x(15).apply(r); // correct D25
        } else if (round == 3) // correct for single ancilla syndrom (round 3)
        {
            if (__Z4__) {
                qx::pauli_x(3).apply(r); // correct D5
                qx::pauli_x(2).apply(
                    r); // flip back D3 (corrected on the previous round)
            }
            if (__Z12__) {
                qx::pauli_x(14).apply(r); // correct D23
                qx::pauli_x(13).apply(
                    r); // flip back D21 (corrected in the previous round)
            }
        }
    }

    /**
     * \brief correct phaseflip on data qubits
     */
    void correct_phaseflips(
        uint32_t round) // correct on multiple round using different correction
                        // when some syndromes are still there
    {
        qx::qu_register &r = *reg;
        if (round == 1) // correct for 2 ancillas syndrom
        {
            if (__X44__ && __X2__)
                qx::pauli_z(2).apply(r); // correct D3
            if (__X2__ && __X14__)
                qx::pauli_z(8).apply(r); // correct D13
            if (__X14__ && __X22__)
                qx::pauli_z(14).apply(r); // correct D23
        } else if (round == 2) // correct for single ancilla syndrom (round 2)
        {
            // !!!!!! correct back if X44 and X2 still up (might be a
            // measurement error on one of the 2 syndroms)
            /*
            if (__X44__ && __X2__)
               qx::pauli_z(2).apply(r); // correct back D3
            if (__X2__ && __X14__)
               qx::pauli_z(8).apply(r); // correct back D13
            if (__X14__ && __X22__)
               qx::pauli_z(14).apply(r); // correct back D23
            */
            if (__X44__)
                qx::pauli_z(3).apply(r); // correct D5
            if (__X2__)
                qx::pauli_z(1).apply(r); // correct D1
            if (__X14__)
                qx::pauli_z(9).apply(r); // correct D15
            if (__X22__)
                qx::pauli_z(13).apply(r); // correct D21
        } else if (round == 3) // correct for single ancilla syndrom (round 3)
        {
            if (__X2__) {
                qx::pauli_z(7).apply(r); // correct D11
                qx::pauli_z(1).apply(
                    r); // flip back D1 (corrected on the previous round)
            }
            if (__X14__) {
                qx::pauli_z(15).apply(r); // correct D25
                qx::pauli_z(9).apply(
                    r); // flip back D15 (corrected in the previous round)
            }
        }
    }

    /**
     * \brief correct phase flips and bit flips
     */
    void correct(uint32_t round) {
        correct_bitflips(round);
        correct_phaseflips(round);
    }

#define z_basis(x) (x == 0 ? 1 : -1)

    // measure
    // return 0 or 1
    bool is_0_logical_state(qx::qu_register &reg) {
        qx::measure(1).apply(reg);
        qx::measure(8).apply(reg);
        qx::measure(15).apply(reg);
        int32_t m = z_basis(reg.get_binary(1)) * z_basis(reg.get_binary(8)) *
                    z_basis(reg.get_binary(15));
        println("[>] logical state: ", m);
        return (m == 1);
    }

    /**
     * \brief draw ns17 (text)
     */
    void draw() {
        uint32_t X44 = reg->get_binary(0);
        uint32_t D1 = reg->get_binary(1);
        uint32_t D3 = reg->get_binary(2);
        uint32_t D5 = reg->get_binary(3);
        uint32_t Z10 = reg->get_binary(4);
        uint32_t X2 = reg->get_binary(5);
        uint32_t Z4 = reg->get_binary(6);
        uint32_t D11 = reg->get_binary(7);
        uint32_t D13 = reg->get_binary(8);
        uint32_t D15 = reg->get_binary(9);
        uint32_t Z12 = reg->get_binary(10);
        uint32_t X14 = reg->get_binary(11);
        uint32_t Z16 = reg->get_binary(12);
        uint32_t D21 = reg->get_binary(13);
        uint32_t D23 = reg->get_binary(14);
        uint32_t D25 = reg->get_binary(15);
        uint32_t X22 = reg->get_binary(16);

        println("[+]------- Ninja Star State -------");
        println("                 ", __state(X44));
        println("     ", __state(D1), "       ", __state(D3), "       ",
                __state(D5));
        println(" ", __state(Z10), "       ", __state(X2), "       ",
                __state(Z4));
        println("     ", __state(D11), "       ", __state(D13), "       ",
                __state(D15));
        println("         ", __state(Z12), "       ", __state(X14), "       ",
                __state(Z16));
        println("     ", __state(D21), "       ", __state(D23), "       ",
                __state(D25));
        println("         ", __state(X22));
    }

    /**
     * report as svg image
     */
    void report(std::ofstream &file, std::string title) {
        std::string color[3] = {"white", "red", "yellow"};
        std::string data[3] = {"black", "red", "yellow"};

        // file.open (filename, ios::out | ios::app);
        file << "<h1>" << title << "</h1>";

        uint32_t X44 = reg->get_binary(0);
        uint32_t D1 = reg->get_binary(1);
        uint32_t D3 = reg->get_binary(2);
        uint32_t D5 = reg->get_binary(3);
        uint32_t Z10 = reg->get_binary(4);
        uint32_t X2 = reg->get_binary(5);
        uint32_t Z4 = reg->get_binary(6);
        uint32_t D11 = reg->get_binary(7);
        uint32_t D13 = reg->get_binary(8);
        uint32_t D15 = reg->get_binary(9);
        uint32_t Z12 = reg->get_binary(10);
        uint32_t X14 = reg->get_binary(11);
        uint32_t Z16 = reg->get_binary(12);
        uint32_t D21 = reg->get_binary(13);
        uint32_t D23 = reg->get_binary(14);
        uint32_t D25 = reg->get_binary(15);
        uint32_t X22 = reg->get_binary(16);

        // println("	<!DOCTYPE html>");
        // println("	   <html>");
        // println("	   <body>");
        // println("");
        // println("	   <h1>17-Qubits Ninja Star</h1>");
        file << "\n";
        file << "	   <svg width=\"400\" height=\"400\">\n";
        file << "\n";
        file << "	   <!-- grids - counter-diag -->\n";
        file << "\n";
        file << "	   <line x1=\"250\" y1=\"50\" x2=\"300\" y2=\"100\"\n";
        file << "	   style=\"stroke: #808080; fill:none;\n";
        file << "	stroke-width: 3px;\n";
        file << "	stroke-dasharray: 5 3\"  />\n";
        file << "\n";
        file << "	   <line x1=\"200\" y1=\"100\" x2=\"350\" y2=\"250\"\n";
        file << "	   style=\"stroke: #808080; fill:none;\n";
        file << "	stroke-width: 3px;\n";
        file << "	stroke-dasharray: 5 3\"  />\n";
        file << "\n";
        file << "	   <line x1=\"100\" y1=\"100\" x2=\"300\" y2=\"300\"\n";
        file << "	   style=\"stroke: #808080; fill:none;\n";
        file << "	stroke-width: 3px;\n";
        file << "	stroke-dasharray: 5 3\"  />\n";
        file << "\n";
        file << "	   <line x1=\"50\" y1=\"150\" x2=\"200\" y2=\"300\"\n";
        file << "	   style=\"stroke: #808080; fill:none;\n";
        file << "	stroke-width: 3px;\n";
        file << "	stroke-dasharray: 5 3\"  />\n";
        file << "\n";
        file << "	   <line x1=\"100\" y1=\"300\" x2=\"150\" y2=\"350\"\n";
        file << "	   style=\"stroke: #808080; fill:none;\n";
        file << "	stroke-width: 3px;\n";
        file << "	stroke-dasharray: 5 3\"  />\n";
        file << "\n";
        file << "	   <!-- gird diag -->\n";
        file << "\n";
        file << "	   <line x1=\"100\" y1=\"100\" x2=\"50\" y2=\"150\"\n";
        file << "	   style=\"stroke: #808080; fill:none;\n";
        file << "	stroke-width: 3px;\n";
        file << "	stroke-dasharray: 5 3\"  />\n";
        file << "\n";
        file << "	   <line x1=\"250\" y1=\"50\" x2=\"100\" y2=\"200\"\n";
        file << "	   style=\"stroke: #808080; fill:none;\n";
        file << "	stroke-width: 3px;\n";
        file << "	stroke-dasharray: 5 3\"  />\n";
        file << "\n";
        file << "	   <line x1=\"300\" y1=\"100\" x2=\"100\" y2=\"300\"\n";
        file << "	   style=\"stroke: #808080; fill:none;\n";
        file << "	stroke-width: 3px;\n";
        file << "	stroke-dasharray: 5 3\"  />\n";
        file << "\n";
        file << "	   <line x1=\"300\" y1=\"200\" x2=\"150\" y2=\"350\"\n";
        file << "	   style=\"stroke: #808080; fill:none;\n";
        file << "	stroke-width: 3px;\n";
        file << "	stroke-dasharray: 5 3\"  />\n";
        file << "\n";
        file << "	   <line x1=\"350\" y1=\"250\" x2=\"300\" y2=\"300\"\n";
        file << "	   style=\"stroke: #808080; fill:none;\n";
        file << "	stroke-width: 3px;\n";
        file << "	stroke-dasharray: 5 3\"  />\n";
        file << "\n";
        file << "	   <!-- qubits -->\n";
        file << "\n";
        file << "	   <circle cx=\"250\" cy=\"50\" r=\"20\" "
                "stroke=\"black\" stroke-width=\"1\" fill=\""
             << color[X44] << "\" />\n";
        file << "	   <text x=\"235\" y=\"55\" "
                "fill=\"black\">X44</text>\n";
        file << "\n";
        file << "	   <circle cx=\"100\" cy=\"100\" r=\"20\" stroke=\""
             << data[D1] << "\" stroke-width=\"" << (1 + D1)
             << "\" fill=\"gray\" /> <text x=\"85\" y=\"105\" "
                "fill=\"black\">D1</text>\n";
        file << "	   <circle cx=\"200\" cy=\"100\" r=\"20\" stroke=\""
             << data[D3] << "\" stroke-width=\"" << (1 + D3)
             << "\" fill=\"gray\" /> <text x=\"185\" y=\"105\" "
                "fill=\"black\">D3</text>\n";
        file << "	   <circle cx=\"300\" cy=\"100\" r=\"20\" stroke=\""
             << data[D5] << "\" stroke-width=\"" << (1 + D5)
             << "\" fill=\"gray\" /> <text x=\"285\" y=\"105\" "
                "fill=\"black\">D5</text>\n";
        file << "\n";
        file << "	   <circle cx=\"50\" cy=\"150\" r=\"20\" "
                "stroke=\"black\" stroke-width=\"1\" fill=\""
             << color[Z10]
             << "\" /> <text x=\"35\" y=\"155\" fill=\"black\">Z10</text>\n";
        file << "	   <circle cx=\"150\" cy=\"150\" r=\"20\" "
                "stroke=\"black\" stroke-width=\"1\" fill=\""
             << color[X2]
             << "\" /> <text x=\"135\" y=\"155\" fill=\"black\">X2</text>\n";
        file << "	   <circle cx=\"250\" cy=\"150\" r=\"20\" "
                "stroke=\"black\" stroke-width=\"1\" fill=\""
             << color[Z4]
             << "\" /> <text x=\"235\" y=\"155\" fill=\"black\">Z4</text>\n";
        file << "\n";
        file << "	   <circle cx=\"100\" cy=\"200\" r=\"20\" stroke=\""
             << data[D11] << "\" stroke-width=\"" << (1 + D11)
             << "\" fill=\"gray\" /> <text x=\"85\" y=\"205\" "
                "fill=\"black\">D11</text>\n";
        file << "	   <circle cx=\"200\" cy=\"200\" r=\"20\" stroke=\""
             << data[D13] << "\" stroke-width=\"" << (1 + D13)
             << "\" fill=\"gray\" /> <text x=\"185\" y=\"205\" "
                "fill=\"black\">D13</text>\n";
        file << "	   <circle cx=\"300\" cy=\"200\" r=\"20\" stroke=\""
             << data[D15] << "\" stroke-width=\"" << (1 + D15)
             << "\" fill=\"gray\" /> <text x=\"285\" y=\"205\" "
                "fill=\"black\">D15</text>\n";
        file << "\n";
        file << "	   <circle cx=\"150\" cy=\"250\" r=\"20\" "
                "stroke=\"black\" stroke-width=\"1\" fill=\""
             << color[Z12]
             << "\" /> <text x=\"135\" y=\"255\" fill=\"black\">Z12</text>\n";
        file << "	   <circle cx=\"250\" cy=\"250\" r=\"20\" "
                "stroke=\"black\" stroke-width=\"1\" fill=\""
             << color[X14]
             << "\" /> <text x=\"235\" y=\"255\" fill=\"black\">X14</text>\n";
        file << "	   <circle cx=\"350\" cy=\"250\" r=\"20\" "
                "stroke=\"black\" stroke-width=\"1\" fill=\""
             << color[Z16]
             << "\" /> <text x=\"335\" y=\"255\" fill=\"black\">Z16</text>\n";
        file << "\n";
        file << "	   <circle cx=\"100\" cy=\"300\" r=\"20\" stroke=\""
             << data[D21] << "\" stroke-width=\"" << (1 + D21)
             << "\" fill=\"gray\" /> <text x=\"85\" y=\"305\" "
                "fill=\"black\">D21</text>\n";
        file << "	   <circle cx=\"200\" cy=\"300\" r=\"20\" stroke=\""
             << data[D23] << "\" stroke-width=\"" << (1 + D23)
             << "\" fill=\"gray\" /> <text x=\"185\" y=\"305\" "
                "fill=\"black\">D23</text>\n";
        file << "	   <circle cx=\"300\" cy=\"300\" r=\"20\" stroke=\""
             << data[D25] << "\" stroke-width=\"" << (1 + D25)
             << "\" fill=\"gray\" /> <text x=\"285\" y=\"305\" "
                "fill=\"black\">D25</text>\n";
        file << "\n";
        file << "	   <circle cx=\"150\" cy=\"350\" r=\"20\" "
                "stroke=\"black\" stroke-width=\"1\" fill=\""
             << color[X22]
             << "\" /> <text x=\"135\" y=\"355\" fill=\"black\">X22</text>\n";
        file << "\n";
        file << "	   </svg> \n";
        file << "\n";
    }

private:
    std::string report_file;
    qx::qu_register *reg;

}; // ninja_star_17q

} // namespace qecc

} // namespace qx