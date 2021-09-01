# pragma once

#include "beatsaber-hook/shared/utils/logging.hpp"

struct ModInfo;
namespace Log{

    extern Logger& GetLogger();
    extern void LoadLogger(ModInfo& modInfo);
    extern Logger* myLogger;

    #define INFO(value) myLogger->info("%s", value)
    #define ERROR(value) myLogger->error("%s", value)
    #define WARNING(value) myLogger->warning("%s", value)

    #define DETAILED(mlLvl, value) myLogger->mlLvl("Line: %d, File: %s, Function: %s \n Message: %s", __LINE__, __FILE__, __func__, value)
    #define D_INFO(value) DETAILED(info, value)
    #define D_ERROR(value) DETAILED(error, value)
    #define D_WARNING(value) DETAILED(warning, value)
}