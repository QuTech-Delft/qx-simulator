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
    std::string file_path = "";
    size_t navg = 0;
    std::string json_filename = "";
    print_banner();

    int arg_index = 1;
    bool arg_parsing_failed = false;
    while (arg_index < argc) {
        auto current_arg = argv[arg_index];

        if (std::string(current_arg) == "-j") {
            if (arg_index + 1 >= argc) {
                arg_parsing_failed = true;
            } else {
                json_filename = std::string(argv[++arg_index]);
            }
        } else if (std::string(current_arg) == "-c") {
            if (arg_index + 1 >= argc) {
                arg_parsing_failed = true;
            } else {
                navg = atoi(argv[++arg_index]);
            }
        } else {
            if (arg_index + 1 < argc) {
                arg_parsing_failed = true;
            } else {
                file_path = std::string(current_arg);
            }
        }

        if (arg_parsing_failed) {
            break;
        }

        ++arg_index;
    }
    
    if (file_path == "" || arg_parsing_failed) {
        println("usage: \n   ", argv[0], " [-j filename.json] [-c iterations] file.qc");
        return -1;
    }

    if (json_filename != "") {
        println("Will output JSON simulation result to file ", json_filename);
    }
    
    if (navg >= 1) {
        println("Will execute circuit ", navg, " times and aggregate measurement registers");
    } else {
        println("Will simulate circuit once and output the final quantum state");
    }

    std::unique_ptr<QX> qx;
    try {
        qx = std::make_unique<QX>();
    } catch (std::exception const& e) {
        return 1;
    }

    if (!qx->set(file_path)) {
        println("Failed to load cqasm file.");

        return 1;
    }
    qx->set_json_output_path(json_filename);
    qx->execute(navg);

    return 0;
}
