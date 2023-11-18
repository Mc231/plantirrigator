// LittleFSImpl.h
#ifndef LittleFSImpl_h
#define LittleFSImpl_h

#include "AbstractFS.h"
#include <LittleFS.h>

class LittleFSImpl : public AbstractFS {
public:
    bool begin() override;
    void end() override;
    bool exists(const String& path) override;
    File open(const String& path, const String& mode) override;
};

#endif
