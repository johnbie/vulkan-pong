#include "AppObject.h"

AppObject::AppObject(bool initialize)
{
    _isAlive = initialize;

    if (initialize)
        this->initialize();
}

bool AppObject::ObjectIsAlive()
{
    return _isAlive;
}

void AppObject::Initialize()
{
    initialize();
    _isAlive = true;
}

void AppObject::Reinitialize()
{
    initialize();
    _isAlive = true;
}

// crashes if not object is not alive; unsafe
void AppObject::Update()
{
    update();
}

void AppObject::UpdateIfAlive()
{
    if (_isAlive)
        update();
}

void AppObject::Clean()
{
    if (_isAlive)
        clean();

    _isAlive = false;
}

AppObject::~AppObject()
{
    if (_isAlive)
        clean();
}

void AppObject::initialize() { }
void AppObject::update() { }
void AppObject::clean() { }