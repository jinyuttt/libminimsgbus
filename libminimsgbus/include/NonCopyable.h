#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <map>
#include <algorithm>
using namespace std;
class NonCopyable
{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator= (const NonCopyable&) = default;
};
template <typename Func>
class Events : public NonCopyable
{
public:
    Events() {}
    ~Events() {}

    //注册观察者, 支持右值引用
    int Connect(Func&& f) {
        return Assign(f);
    }

    //注册观察者
    int Connect(Func& f) {
        return Assign(f);
    }

    //移除观察者
    void Disconnect(int key) {
        m_connections.erase(key);
    }

    //通知所有的观察者
    template <typename... Args>
    void Notify(Args&& ... args) {
        for (auto& it : m_connections) {
            it.second(std::forward<Args>(args)...);
        }
    }

private:
    //保存观察者并分配观察者的编号
    template <typename F>
    int Assign(F&& f) {
        int k = m_observerId++;
        m_connections.emplace(k, std::forward<F>(f));
        return k;
    }

private:
    int m_observerId = 0;   //观察者编号

    std::map<int, Func> m_connections;      //观察者列表

};

