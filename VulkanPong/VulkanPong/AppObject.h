#pragma once
#include "pch.h"

class AppObject
{
public:
    AppObject(bool initialize = false);
    bool ObjectIsAlive();
    void Initialize();
    void Reinitialize();
    void Update();
    void UpdateIfAlive();
    void Clean();
    ~AppObject();
protected:
    const uint32_t WIDTH = 1200;
    const uint32_t HEIGHT = 1000;
    const uint32_t PIXEL_SIZE = 5;

    virtual void initialize();
    virtual void update();
    virtual void clean();
private:
    bool _isAlive;
};