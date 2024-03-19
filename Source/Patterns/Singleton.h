#pragma once

#include <memory>

template <typename T>
class Singleton
{
protected:
    static std::unique_ptr<T> _instance;

    Singleton()  = default;
    ~Singleton() = default;

public:
    Singleton(const Singleton&)     = delete;
    Singleton& operator=(Singleton) = delete;
    static T* getInstance();
};

template <typename T>
std::unique_ptr<T> Singleton<T>::_instance;

template <typename T>
T* Singleton<T>::getInstance()
{
    if(!_instance.get())
    {
        _instance = std::unique_ptr<T>(new T());
    }

    return _instance.get();
}
