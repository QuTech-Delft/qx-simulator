/**
 * @file
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date		10-12-15
 * @brief
 */
{
    uint32_t n = 4;
    qx::qu_register reg(n);

    println("[T]======== Precision Loss Experiment =========[T]");

    println("[+] performance :");

    n = 8;
    qx::qu_register r(n);
    qx::qu_register ref(n);

    qx::circuit p(n, "precision_loss_experiment");

    uint32_t ng = 2048 * 64;
    /*
       for (uint32_t i=0; i<ng; i++)
       {
          //p.add(std::make_shared<qx::rx>(0,0.12345));
          //p.add(std::make_shared<qx::rz>(0,0.12345));
          //p.add(std::make_shared<qx::rz>(0,-0.12345));
          //p.add(std::make_shared<qx::rx>(0,-0.12345));
          //p.add(std::make_shared<qx::phase_shift>(i%n)); // fidelity 1
          //p.add(std::make_shared<qx::pauli_x>(i%n));     // fidelity 1
          //p.add(std::make_shared<qx::pauli_y>(i%n));     // fidelity 1
          p.add(std::make_shared<qx::hadamard>(0));   // fidelity f(n_gates)
          //p.add(std::make_shared<qx::hadamard>(i%n));   // fidelity f(n_gates)
       }
       //   for (uint32_t j=0; j<n; j++)
            // p.add(std::make_shared<qx::hadamard>(j));
    */

    for (uint32_t i = 0; i < 200; i++) {
        for (uint32_t j = 0; j < 5000; j++) {
            p.add(std::make_shared<qx::hadamard>(0)); // fidelity f(n_gates)
            p.add(std::make_shared<qx::pauli_x>(0));  // fidelity 1
            // p.add(std::make_shared<qx::pauli_y>(i%n));     // fidelity 1
        }

        // double   predicted_fidelity = 1-0.000035*(ng/2048);
        // println("[+] predicted fidelity    : " << predicted_fidelity);
        println("[+] circuit depth (gates) : " << (i * 5000));
        p.execute(r);
        println("[>] fidelity : " << qx::fidelity(r, ref));
        println("-------------------------------------------");
    }

    println("[T]============================================[T]");
}
