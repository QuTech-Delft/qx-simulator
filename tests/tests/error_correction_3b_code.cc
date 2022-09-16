/**
 * @file
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date	01-12-15
 * @brief
 */
{

#define B_00 0
#define B_01 1
#define B_10 2
#define B_11 3

    uint32_t n = 5; // 1 logical qubit = 3 physical qubit and 2 ancillas
    qx::qu_register reg(n);

    uint32_t q0 = 0; // physic. qubit 1
    uint32_t q1 = 1; // physic. qubit 2
    uint32_t q2 = 2; // physic. qubit 3
    uint32_t a0 = 3; // Ancilla 1
    uint32_t a1 = 4; // Ancilla 2

    println("[T]======== 3 QBits Error Correction Code Test =========[T]");

    println("[>>>>>>] initial state:");
    reg.dump();

    // initialization circuit
    qx::circuit init(n);
    init.add(std::make_shared<qx::pauli_x>(q0)); // set logical qubit to 1
    // init.add(std::make_shared<qx::hadamard>(q0)); // set logical qubit to 1

    // encoding circuit
    qx::circuit encoding(n);
    encoding.add(std::make_shared<qx::cnot>(q0, q1));
    encoding.add(std::make_shared<qx::cnot>(q0, q2));

    // error injection
    qx::circuit error(n);
    error.add(std::make_shared<qx::pauli_x>(q2));

    // parity check
    qx::circuit parity_check(n);
    parity_check.add(std::make_shared<qx::cnot>(q0, a0));
    parity_check.add(std::make_shared<qx::cnot>(q1, a0));
    parity_check.add(std::make_shared<qx::cnot>(q0, a1));
    parity_check.add(std::make_shared<qx::cnot>(q2, a1));
    parity_check.add(std::make_shared<qx::measure>(a0));
    parity_check.add(std::make_shared<qx::measure>(a1));

    // decoding circuit
    qx::circuit decoding(n);
    decoding.add(std::make_shared<qx::cnot>(q0, q2));
    decoding.add(std::make_shared<qx::cnot>(q0, q1));

    // correction circuit
    qx::circuit correction(n);
    qx::lookup_gate_table *lt = std::make_shared<qx::lookup_gate_table>(a0, a1);

    lt->add_gate(B_11, std::make_shared<qx::pauli_x>(q0));
    lt->add_gate(B_10, std::make_shared<qx::pauli_x>(q1));
    lt->add_gate(B_01, std::make_shared<qx::pauli_x>(q2));

    correction.add(lt);

    println("[+++++++++++++++++ initializing logical qubit +++++++++++++++]");
    init.execute(reg, true, true);
    println("[+++++++++++++++++++++ encoding circuit +++++++++++++++++++++]");
    encoding.execute(reg, true, true);
    println("[++++++++++++++++++ error injection circuit +++++++++++++++++]");
    error.execute(reg, true, true);
    println("[++++++++++++++++++++++ parity circuit ++++++++++++++++++++++]");
    parity_check.execute(reg, true, true);
    println(
        "[++++++++++++++++++++++ correction circuit ++++++++++++++++++++++]");
    correction.execute(reg, true, true);
    println("[++++++++++++++++++++ decoding circuit ++++++++++++++++++++++]");
    decoding.execute(reg, true, true);

    println("[>>>>>>] final state:");
    reg.dump();

    println("[T]==================== END =====================[T]");
}
