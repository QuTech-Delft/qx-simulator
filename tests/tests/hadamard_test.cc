/**
 * @file
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date		20-10-15
 * @brief
 */
{
    uint32_t n = 4;
    qx::qu_register reg(n);

    println("[T]======== Hadamard Gate Test =========[T]");

    println("[+] initial state:");
    reg.dump();

    println("[+] hadamard on all :");

    qx::circuit c(n);

    c.add(new qx::hadamard(0));
    c.add(new qx::hadamard(0));
    c.add(new qx::hadamard(1));
    c.add(new qx::hadamard(1));
    c.add(new qx::hadamard(2));
    c.add(new qx::hadamard(2));
    c.add(new qx::hadamard(3));
    c.add(new qx::hadamard(3));

    c.execute(reg, true);

    println("[+] final state:");
    reg.dump();
    /*

       println("[+] performance :");

       n=8;
       qx::qu_register r(n);
       qx::qu_register ref(n);

       qx::circuit p(n);

       uint32_t ng = 2048*64;
       double   predicted_fidelity = 1-0.000035*(ng/2048);

       println("[+] circuit depth (gates) : " << ng);
       println("[+] predicted fidelity    : " << predicted_fidelity);

       for (uint32_t i=0; i<ng; i++)
       {
          //p.add(new qx::rx(0,0.12345));
          //p.add(new qx::rz(0,0.12345));
          //p.add(new qx::rz(0,-0.12345));
          //p.add(new qx::rx(0,-0.12345));
          //p.add(new qx::phase_shift(i%n)); // fidelity 1
          //p.add(new qx::pauli_x(i%n));     // fidelity 1
          //p.add(new qx::pauli_y(i%n));     // fidelity 1
          p.add(new qx::hadamard(0));   // fidelity f(n_gates)
          //p.add(new qx::hadamard(i%n));   // fidelity f(n_gates)
       }
       //   for (uint32_t j=0; j<n; j++)
            // p.add(new qx::hadamard(j));

       p.execute(r);

       // r.dump();

       println("[>] fidelity : " << qx::fidelity(r,ref));
    */
    println("[T]======== Hadamard Gate Test =========[T]");
}
