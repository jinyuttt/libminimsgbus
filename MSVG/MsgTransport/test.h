#pragma once
#include <iostream>
#include<thread>
#include "NngDataNative.h"
using namespace msgtransport;
using namespace std;


int main()
{
    NngDataNative nngser;
    NngDataNative nngclient;
    nngser.receive("tcp://192.168.0.153:5555");
    thread rec([&]() {
        while (true)
        {
          auto buf= nngser.getData();
          std::cout << "接收数据：" << std::endl;
        }
        });
    auto v = new char[3]{ 'j','i','n' };
    int len = 0;
    while (true)
    {
        nngclient.send("tcp://192.168.0.153:5555", v, &len);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
   
    std::cout << "Hello World!\n";
  
    system("pause");
}
