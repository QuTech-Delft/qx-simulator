/**
 * @file        qx_server.cc
 * @author      Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date        25-02-16
 */

#include "server.h"
#include "qx/version.h"

int main(int argc, char **argv) {
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

    size_t port = 5555;

    if (argc == 2) {
        port = atoi(argv[1]);
    }

    qx::qx_server server(port);
    server.start();

    return 0;
}
