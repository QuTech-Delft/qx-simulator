#include "qx/simulator.hpp"

#include <fmt/core.h>
#include <fmt/ostream.h>

#include "qx/version.hpp"

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

int main(int argc, char** argv) {
    std::string file_path;
    size_t iterations = 1;
    print_banner();

    int arg_index = 1;
    bool arg_parsing_failed = false;
    while (arg_index < argc) {
        auto current_arg = argv[arg_index];

        if (std::string(current_arg) == "-c") {
            if (arg_index + 1 >= argc) {
                arg_parsing_failed = true;
            } else {
                iterations = atoi(argv[++arg_index]);
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

    if (file_path.empty() || arg_parsing_failed) {
        fmt::print(std::cerr, "Usage: {} [-c iterations] file.qc\n", argv[0]);
        return -1;
    }
    fmt::print("Will execute {} time{} file '{}'...\n", iterations, (iterations > 1 ? "s" : ""), file_path);

    auto simulation_result = qx::execute_file(file_path, iterations);
    if (auto* error = std::get_if<qx::SimulationError>(&simulation_result)) {
        fmt::print(std::cerr, "{}\n", error->what());
        return 1;
    }

    fmt::print("{}\n", std::get<qx::SimulationResult>(simulation_result));
    return 0;
}
