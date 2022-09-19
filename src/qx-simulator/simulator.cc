/**
 * @file	simulator.cc
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date	01-01-16
 * @brief
 */

#include "qx/libqasm_interface.h"
#include "qx/representation.h"
#include "qx/qxelarator.h"
#include <qasm_semantic.hpp>
#ifdef USE_GPERFTOOLS
#include <gperftools/profiler.h>
#endif

#include <iostream>
#include <memory>

#include "qx/version.h"

void print_banner() {
    // clang-format off
    println("");
    println("  =================================================================================================== "); 
    println("        _______                                                                                       ");
    println("       /  ___   \\   _  __      ____   ____   __  ___  __  __   __    ___  ______  ____    ___         ");
    println("      /  /   /  |  | |/_/     / __/  /  _/  /  |/  / / / / /  / /   / _ |/_  __/ / __ \\  / _ \\        ");
    println("     /  /___/  /  _>  <      _\\ \\   _/ /   / /|_/ / / /_/ /  / /__ / __ | / /   / /_/ / / , _/        ");
    println("     \\______/\\__\\ /_/|_|    /___/  /___/  /_/  /_/  \\____/  /____//_/ |_|/_/    \\____/ /_/|_|         ");
    println("                                                                                                      ");
    println("     version " , QX_VERSION , " - QuTech - " , QX_RELEASE_YEAR , " - report bugs and suggestions to: nader.khammassi@gmail.com");
    println("  =================================================================================================== ");
    println("");
    // clang-format on
}

/**
 * simulator
 */
int main(int argc, char **argv) {
    std::string file_path;
    size_t navg = 0;
    print_banner();

    if (!(argc == 2 || argc == 3 || argc == 4)) {
        println("error : you must specify a circuit file !");
        println("usage: \n   ", argv[0], " file.qc [iterations] [num_cpu]");
        return -1;
    }

    // parse arguments and initialise xpu cores
    file_path = argv[1];
    if (argc > 2) {
        navg = (atoi(argv[2]));
    }

    std::unique_ptr<QX> qx;
    try {
        qx = std::make_unique<QX>();
    } catch (std::exception const& e) {
        return 1;
    }

    qx->set(file_path);

    qx->execute(navg);

    // FIXME: when averaging measurements are ON, this will result in always "flat" state even though the circuit doesn't contain measure gates.
    println("Final quantum state after completion of the circuit: ", qx->get_state());

    return 0;
}
