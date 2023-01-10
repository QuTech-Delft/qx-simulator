#include "qx/Simulator.h"

#include "qasm_semantic.hpp"
#include "qx/LibqasmInterface.h"
#include "qx/Qxelarator.h"
#include "qx/Version.h"

void print_banner() {
    // clang-format off
    std::cout << std::endl;
    std::cout << "  =================================================================================================== " << std::endl; 
    std::cout << "        _______                                                                                       " << std::endl;
    std::cout << "       /  ___   \\   _  __      ____   ____   __  ___  __  __   __    ___  ______  ____    ___         " << std::endl;
    std::cout << "      /  /   /  |  | |/ /     / __/  /  _/  /  |/  / / / / /  / /   / _ |/_  __/ / __ \\  / _ \\        " << std::endl;
    std::cout << "     /  /___/  /   >   <     _\\ \\   _/ /   / /|_/ / / /_/ /  / /__ / __ | / /   / /_/ / / , _/        " << std::endl;
    std::cout << "     \\______/\\__\\ /_/|_|    /___/  /___/  /_/  /_/  \\____/  /____//_/ |_|/_/    \\____/ /_/|_|         " << std::endl;
    std::cout << "                                                                                                      " << std::endl;
    std::cout << "       Version " << QX_VERSION << " - QuTech - " << QX_RELEASE_YEAR << " - report bugs and suggestions to: p.lehenaff@tudelft.nl" << std::endl;
    std::cout << "  =================================================================================================== " << std::endl;
    std::cout << "" << std::endl;
    // clang-format on
}

int main(int argc, char **argv) {
    std::string filePath = "";
    size_t numberOfRuns = 1;
    std::string jsonFilename = "";
    print_banner();

    int argIndex = 1;
    bool argParsingFailed = false;
    while (argIndex < argc) {
        auto currentArg = argv[argIndex];

        if (std::string(currentArg) == "-j") {
            if (argIndex + 1 >= argc) {
                argParsingFailed = true;
            } else {
                jsonFilename = std::string(argv[++argIndex]);
            }
        } else if (std::string(currentArg) == "-c") {
            if (argIndex + 1 >= argc) {
                argParsingFailed = true;
            } else {
                numberOfRuns = atoi(argv[++argIndex]);
            }
        } else {
            if (argIndex + 1 < argc) {
                argParsingFailed = true;
            } else {
                filePath = std::string(currentArg);
            }
        }

        if (argParsingFailed) {
            break;
        }

        ++argIndex;
    }

    if (filePath == "" || argParsingFailed) {
        std::cerr << "Usage: \n   " << argv[0]
                  << " [-j filename.json] [-c iterations] file.qc" << std::endl;
        return -1;
    }

    if (jsonFilename != "") {
        std::cout << "Will output JSON simulation result to file "
                  << jsonFilename << std::endl;
    }

    std::cout << "Will execute circuit " << numberOfRuns << " time"
              << (numberOfRuns > 1 ? "s" : "") << std::endl;

    qx::QX qx;

    if (!qx.set(filePath)) {
        std::cerr << "Failed to load cqasm file" << std::endl;
        return 1;
    }
    qx.set_json_output_path(jsonFilename);
    qx.execute(numberOfRuns);

    return 0;
}
