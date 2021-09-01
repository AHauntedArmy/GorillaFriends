# pragma once

#include "beatsaber-hook/shared/utils/logging.hpp"

struct ModInfo;
namespace Log{

    extern Logger& GetLogger();
    extern void LoadLogger(ModInfo& modInfo);
    extern Logger* myLogger;

    #define INFO(value...) myLogger->info(value)
    #define ERROR(value...) myLogger->error(value)
    #define WARNING(value...) myLogger->warning(value)
}