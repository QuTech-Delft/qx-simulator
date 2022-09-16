/**
 * @file
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date		14-10-15
 * @brief		Program 1 : example 1 from personal notes
 */

{
    qx::qu_register reg(2);

    println("[+] initial state:");
    reg.dump();

    // identity
    cmatrix_t i = build_matrix(identity_c, 2);
    // pauli-x
    cmatrix_t x = build_matrix(pauli_x_c, 2);
    // pauli-y
    cmatrix_t y = build_matrix(pauli_y_c, 2);
    // pauli-z
    cmatrix_t z = build_matrix(pauli_z_c, 2);
    // phase
    cmatrix_t s = build_matrix(phase_c, 2);
    // hadamard
    cmatrix_t h = build_matrix(hadamard_c, 2);
    // cnot
    cmatrix_t cnot = build_matrix(cnot_c, 4);
    // swap
    cmatrix_t swap = build_matrix(swap_c, 4);

    // -------- print gates ---------
    println("hadamard : ", h);
    println("pauli-x  : ", x);
    println("pauli-y  : ", y);
    println("pauli-z  : ", z);
    println("phase    : ", s);
    println("identity : ", i);
    println("cnot     : ", cnot);
    println("swap     : ", swap);

    // ---------- operations ----------
    reg = mxv(tensor(i, x), reg.get_data());
    reg.dump();
    reg = mxv(tensor(h, h), reg.get_data());
    reg.dump();
    reg = mxv(cnot, reg.get_data());
    reg.dump();
    reg = mxv(tensor(h, i), reg.get_data());
    reg.dump();
    reg.measure();
    reg.dump();
}
