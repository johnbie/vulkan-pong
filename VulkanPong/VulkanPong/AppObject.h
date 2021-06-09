#pragma once
#include "Configs.h"
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

    virtual void initialize();
    virtual void update();
    virtual void clean();
    static int beep(int freq, int ms);
private:
    bool _isAlive;
};