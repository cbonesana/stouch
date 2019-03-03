#include "Module.h"

Module::Module(QString config, QString componentName, QObject *parent) : QObject(parent), componentName(componentName)
{
    fileParser = new FileParser(config);
}

Module::~Module()
{
    delete(fileParser);
}
