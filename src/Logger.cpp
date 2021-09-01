#include "modloader/shared/modloader.hpp"
#include "Logger.hpp"

namespace Log{

    Logger* myLogger;

    Logger& GetLogger()
    {
        return *myLogger;
    }

    void LoadLogger(ModInfo& modInfo)
    {
        if(myLogger != nullptr) return;
        static Logger* logger = new Logger(modInfo);
        myLogger = logger;
    }
}