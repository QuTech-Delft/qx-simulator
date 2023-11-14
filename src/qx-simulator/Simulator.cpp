#include "qx/Simulator.hpp"
#include "qx/Version.hpp"
#include "v10/qasm_semantic.hpp"


void print_banner() {
    // clang-format off
    std::cout << std::endl;
    std::cout << R"( =============================================================================================== )" << std::endl;
    std::cout << R"(       _______                                                                                   )" << std::endl;
    std::cout << R"(      /  ___   \   _  __      ____   ____   __  ___  __  __   __    ___  ______  ____    ___     )" << std::endl;
    std::cout << R"(     /  /   /  |  | |/ /     / __/  /  _/  /  |/  / / / / /  / /   / _ |/_  __/ / __ \  / _ \    )" << std::endl;
    std::cout << R"(    /  /___/  /   >   <     _\ \   _/ /   / /|_/ / / /_/ /  / /__ / __ | / /   / /_/ / / , _/    )" << std::endl;
    std::cout << R"(    \______/\__\ /_/|_|    /___/  /___/  /_/  /_/  \____/  /____//_/ |_|/_/    \____/ /_/|_|     )" << std::endl;
    std::cout << R"(                                                                                                 )" << std::endl;
    std::cout <<  "      Version " << QX_VERSION << " - QuTech - " << QX_RELEASE_YEAR << " - report bugs and suggestions to: p.lehenaff@tudelft.nl" << std::endl;
    std::cout << R"( =============================================================================================== )" << std::endl;
    std::cout << "" << std::endl;
    // clang-format on
}

int main(int argc, char **argv) {
    std::string filePath;
    size_t iterations = 1;
    print_banner();

    int argIndex = 1;
    bool argParsingFailed = false;
    while (argIndex < argc) {
        auto currentArg = argv[argIndex];

        if (std::string(currentArg) == "-c") {
            if (argIndex + 1 >= argc) {
                argParsingFailed = true;
            } else {
                iterations = atoi(argv[++argIndex]);
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

    if (filePath.empty() || argParsingFailed) {
        std::cerr << "Usage: \n   " << argv[0]
                  << " [-c iterations] file.qc" << std::endl;
        return -1;
    }

    std::cout << "Will execute " << iterations << " time"
              << (iterations > 1 ? "s" : "") << " file '" << filePath << "'..."
              << std::endl;

    auto simulationResult = qx::executeFile(filePath, iterations);

    if (auto* error = std::get_if<qx::SimulationError>(&simulationResult)) {
        std::cerr << error->message << std::endl;
        return 1;
    }

    std::cout << std::get<qx::SimulationResult>(simulationResult) << std::endl;
    return 0;
}
