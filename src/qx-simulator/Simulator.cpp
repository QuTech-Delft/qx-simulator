#include "qx/Simulator.hpp"
#include "qx/Version.hpp"

#include <fmt/core.h>
#include <fmt/ostream.h>


static constexpr char const* banner = R"(
===============================================================================================
      _______
     /  ___   \   _  __      ____   ____   __  ___  __  __   __    ___  ______  ____    ___
    /  /   /  |  | |/ /     / __/  /  _/  /  |/  / / / / /  / /   / _ |/_  __/ / __ \  / _ \
   /  /___/  /   >   <     _\ \   _/ /   / /|_/ / / /_/ /  / /__ / __ | / /   / /_/ / / , _/
   \______/\__\ /_/|_|    /___/  /___/  /_/  /_/  \____/  /____//_/ |_|/_/    \____/ /_/|_|

                     <   QuTech - TU Delft   |   Version {} ({})   >
===============================================================================================)";


void print_banner() {
    fmt::print(banner, QX_VERSION, QX_RELEASE_YEAR);
    fmt::print("\n\n");
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
        fmt::print(std::cerr, "Usage: {} [-c iterations] file.qc\n", argv[0]);
        return -1;
    }
    fmt::print("Will execute {} time{} file '{}'...\n", iterations, (iterations > 1 ? "s" : ""), filePath);

    auto simulationResult = qx::executeFile(filePath, iterations);
    if (auto* error = std::get_if<qx::SimulationError>(&simulationResult)) {
        fmt::print(std::cerr, "{}\n", error->message);
        return 1;
    }

    fmt::print("{}\n", std::get<qx::SimulationResult>(simulationResult));
    return 0;
}
