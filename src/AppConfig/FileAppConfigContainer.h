#ifndef _FILE_APP_CONFIG_CONTAINER_H
#define _FILE_APP_CONFIG_CONTAINER_H

#include <string>

// This class represents container of application configuration constants
// defined in external file(s) and loaded from them to RAM memory.
class FileAppConfigContainer {
public:
#define DEFINE_CONSTANT(type, name) type name;
    #include "AppConfigDefinitions.h"
#undef DEFINE_CONSTANT
};

// Declare global object in order to static access from whole project.
extern FileAppConfigContainer fileAppConfigContainer;

#endif // _FILE_APP_CONFIG_CONTAINER_H
