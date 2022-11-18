/**
 * @file
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date		14-10-15
 * @copyright  none - confidential
 * @brief
 */

{
    /**
     * noisy hadamard
     */

    qx::qu_register ref(2); // reference register

    qx::gaussian::random r;

    // cmatrix_t nh = noisy_hadamard(r.next()*0.0001f, r.next()*0.00001f);
    cmatrix_t nh = noisy_hadamard(0.01f, 0.01f);
    cmatrix_t h = build_matrix(hadamard_c, 2);
    cmatrix_t hh = tensor(h, h);

    println("[+] perfect hadamard gate                 : " << h);
    println("[+] noisy hadamard gate (epsilon=10e-5 rd): " << nh);

    qu_register r1 = ref;
    qu_register r2 = ref;

    r1.dump();
    r2.dump();
    r1 = mxv(hh, r1.get_data());             // perfect hadamard
    r2 = mxv(tensor(nh, nh), r2.get_data()); // noisy hadamard
    r1.dump();
    r2.dump();

    println("[+] Fidelity : " << std::setprecision(7) << fidelity(r1, r2));

    /*
       reg.set_data(mxv(tensor(i,x),reg.get_data()));
       reg.dump();
       reg.set_data(mxv(tensor(h,h),reg.get_data()));
       reg.dump();
       reg.set_data(mxv(cnot,reg.get_data()));
       reg.dump();
    */
}
