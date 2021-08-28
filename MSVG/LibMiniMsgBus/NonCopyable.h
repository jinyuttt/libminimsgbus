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

    //ע��۲���, ֧����ֵ����
    int Connect(Func&& f) {
        return Assign(f);
    }

    //ע��۲���
    int Connect(Func& f) {
        return Assign(f);
    }

    //�Ƴ��۲���
    void Disconnect(int key) {
        m_connections.erase(key);
    }

    //֪ͨ���еĹ۲���
    template <typename... Args>
    void Notify(Args&& ... args) {
        for (auto& it : m_connections) {
            it.second(std::forward<Args>(args)...);
        }
    }

private:
    //����۲��߲�����۲��ߵı��
    template <typename F>
    int Assign(F&& f) {
        int k = m_observerId++;
        m_connections.emplace(k, std::forward<F>(f));
        return k;
    }

private:
    int m_observerId = 0;   //�۲��߱��

    std::map<int, Func> m_connections;      //�۲����б�

};

