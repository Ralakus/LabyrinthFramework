#include <fstream>
#include <Utilities/LabAsset.hpp>

#include <Event/EventHandler.hpp>

using namespace Labyrinth;

int main(int argc, char* argv[]) {

    EventHandler EH;
    EH.RegisterEvent("I am an event", std::make_shared<Event>());
    
    std::shared_ptr<EventCallback> EC = std::make_shared<EventCallback>([](){ successln("I am an event callback"); });
    EH.RegisterCallback("I am an event", EC);

    EH.TriggerEvent("I am an event");

    std::ifstream File;
    File.open("Test.lab");
    if(!File.is_open()) {
        errorln("Failed to open file!");
        return 1;
    }
    LabAsset Test{File};
    File.close();

    successln(Test["Test"]["Nested"]["Nested"].String());

    noticeln("DUMPING BUFFER");
    for( auto& str : GStreams::StringBuffer() ) {
        GStreams::PrintOut() << str;
    }
    GStreams::PrintOut() << std::endl;
    
    return 0;
}