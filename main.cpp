#include "rom.h"

int main(int argc, char** argv) {
    rom new_rom;

    std::cout << "Please enter NES rom location: " << std::endl;

    std::string filename;
    std::cin >> filename;

    new_rom.load(filename.c_str());
    new_rom.exportCHR(filename.replace(filename.length() - 4, filename.length(), ".chr").c_str());

    std::cin.get();
    return 0;
}