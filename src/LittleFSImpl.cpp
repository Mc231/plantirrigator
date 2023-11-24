// LittleFSImpl.cpp
#include "LittleFSImpl.h"
#include <FS.h>  // Ensure this is included

bool LittleFSImpl::begin() {
    return LittleFS.begin();
}

void LittleFSImpl::end() {
    LittleFS.end();
}

bool LittleFSImpl::exists(const String& path) {
    return LittleFS.exists(path);
}

File LittleFSImpl::open(const String& path, const String& mode) {
    return LittleFS.open(path.c_str(), mode.c_str()); 
}

bool LittleFSImpl::remove(const String& path) {
    return LittleFS.remove(path);  
}
