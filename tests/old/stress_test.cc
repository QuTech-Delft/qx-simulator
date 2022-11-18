/**
 * @file		stress_test.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date		14-10-15
 * @brief
 */

{
    uint32_t n = 8;

    if (argc == 2)
        n = std::abs(atoi(argv[1]));
    n = ((n > 1 && n < 40) ? n : 8);

    println("[+] creating ", n, " qubits ...");

    qx::qu_register reg(n);

    /*
    // hadamard
    cmatrix_t h  = build_matrix(hadamard_c,2);
    // nth kronecker
    cmatrix_t hn = tensor(h, 1 << n);

    println("reg    dim : " , reg.states());
    println("matrix sz1 : " , hn.size1());
    println("matrix sz2 : " , hn.size2());

    reg = mxv(hn,reg.get_data());
    */

    qx::circuit c(n);

    println("[+] building quantum circuit...");

    // for (uint32_t i=0; i<n; i++)
    //    c.add(std::make_shared<hadamard>(i));
    for (uint32_t i = 0; i < n - 1; i++) {
        c.add(std::make_shared<pauli_x>(i + 1));
        c.add(std::make_shared<hadamard>(i));
        c.add(std::make_shared<cnot>(i, i + 1));
        c.add(std::make_shared<pauli_y>(i));
    }

    println("[+] executing quantum  circuit...");

    c.execute(reg, true);

    println("[+] execution done.");

    // reg.dump();

    // reg.check();
}
