#include <fstream>
#include "Aesset.hpp"

using namespace Labyrinth;

int main(int argc, char* argv[]) {

    std::ifstream File;
    File.open("Test.lab");
    if(!File.is_open()) {
        errorln("Failed to open file!");
        return 1;
    }
    LabAsset Test{File};
    File.close();

    successln(Test["Test"]["Nested"]["Nested"].String());

    return 0;
}