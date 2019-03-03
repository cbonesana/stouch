#include <QCoreApplication>

#include "../Prototipo1b/Sound/SoundModule.h"
#include "../Prototipo1b/FileParser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    SoundModule *sounds = new SoundModule("/home/dna/Development/stouch/data/sounds.config", "/home/dna/Development/stouch/data/sounds/");

    sounds->play(SoundModule::playCreativo,1);

    return 0;
}
