/**
 * @file
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date		14-10-15
 * @brief		Precision loss across circuit
 */
{
    /**
     * program 2 : precision loss across circuit
     */

    qx::qu_register reg(2);
    qx::qu_register ref(2);

    cmatrix_t h = build_matrix(hadamard_c, 2);
    cmatrix_t hh = tensor(h, h);

    println("[+] Precision loss across long circuits: ");
    println(" |- state after 1000 hadamard gates: ");
    for (int j = 0; j < 1000; ++j)
        reg = mxv(hh, reg.get_data());
    reg.dump();
    println("[+] Fidelity : ", fidelity(reg, ref));

    println(" |- state after 10000 hadamard gates: ");
    for (int j = 0; j < 9900; ++j)
        reg = mxv(hh, reg.get_data());
    reg.dump();
    println("[+] Fidelity : ", fidelity(reg, ref));

    println(" |- state after 100000 hadamard gates: ");
    for (int j = 0; j < 90000; ++j)
        reg = mxv(hh, reg.get_data());
    reg.dump();
    println("[+] Fidelity : ", fidelity(reg, ref));

    println(" |- state after 1000000 hadamard gates: ");
    for (int j = 0; j < 900000; ++j)
        reg = mxv(hh, reg.get_data());
    reg.dump();
    println("[+] Fidelity : ", fidelity(reg, ref));

    println(" |- state after 10000000 hadamard gates: ");
    for (int j = 0; j < 9000000; ++j)
        reg = mxv(hh, reg.get_data());
    reg.dump();
    println("[+] Fidelity : ", fidelity(reg, ref));

    println(" |- state after 100000000 hadamard gates: ");
    for (int j = 0; j < 90000000; ++j)
        reg = mxv(hh, reg.get_data());
    reg.dump();
    println("[+] Fidelity : ", fidelity(reg, ref));
}
