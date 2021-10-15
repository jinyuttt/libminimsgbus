// demo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//#ifdef VCZH_CHECK_MEMORY_LEAKS
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#define VCZH_CHECK_MEMORY_LEAKS_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new VCZH_CHECK_MEMORY_LEAKS_NEW
//#endif

#include <iostream>
#include <msgtopic.h>
#include<fstream>
#include "blockingconcurrentqueue.h"
#include"Util.h"
#include "BusFactory.h"
#include "PtpFactory.h"
#include "MsgLocalNode.h"
#include <NngDataNative.h>
#include <BridgeCore.h>
#include "NetMsgBus.h"
//#include "vld.h"


using namespace libminimsgbus;
using namespace std;
BlockingConcurrentQueue<Records> errorRecords;
char Util::guid[32] = {};


void  rev(std::string topic, char* msg, int len)
{
    //std::cout << std::endl;
    std::cout << "接收：" << topic << " 数据:" << string(msg, len) << std::endl;
}
void TestTopic()
{
    thread suber([]() {
        msgtopic* sub = new msgtopic();
        sub->rectopic = rev;
        sub->subscribe("jin");
        });
    suber.detach();
    thread puber([]() {
        msgtopic* pub = new msgtopic();
        while (true)
        {
            char ss[4] = { '7','d' };
            pub->publish("jin", ss,4);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // 2 休眠1000ms

        }

        });
    puber.detach();

}
void TestQueue()
{

    thread check([&]()
        {

            while (true)
            {
                Records p;
                errorRecords.wait_dequeue(p);
            }
        });
    check.detach();
}

void testnng()
{
    NngDataNative nngser;
    NngDataNative nngclient;
    auto localaddress=nngser.receive("tcp://192.168.0.115:5556");
    thread rec([&]() {
        while (true)
        {
            auto buf = nngser.getData();
            std::cout << "接收数据："+string(buf.bufdata,buf.size) << std::endl;
        }
        });
    rec.detach();
  
    int len = 0;
    Util::guid;
        auto ss= Util::generate_hex(16);
     
        int i;
        for (i = 0;i < ss.length();i++)
            Util::guid[i] = ss[i];
    
        thread sebd([&]() {
            auto v = new char[4]{ 'j','i','n' ,'y' };
            len = 4;
                while (true)
                {
                    try
                    {
                       // auto buf = Util::Convert("jin", v, 4, '0', 1, len);

                       // auto ret = nngclient.send("tcp://192.168.0.115:5556", buf, &len);
                        nngclient.send("tcp://192.168.0.115:5556", v, len);
                    }
                    catch (nng::exception e)
                    {
                        std::cout << e.what() << std::endl;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                }
            
            });
        sebd.detach();
   
   
        system("pause");
}

void testFilenng()
{
    NngDataNative nngser;
    NngDataNative nngclient;
    auto localaddress = nngser.receive("tcp://192.168.0.124:5556");
    thread rec([&]() {
        while (true)
        {
            auto buf = nngser.getData();
            //std::cout << "接收数据：" + string(buf.bufdata, buf.size) << std::endl;
            // write file
            ofstream outfile;
            outfile.open("E:\\study\\oh.mp4", ios::out | ios::binary| ios::trunc);
            assert(outfile.is_open());
            outfile.write(buf.bufdata, buf.size);
            outfile.close();
           
        }
        });
    rec.detach();

    int len = 0;
    Util::guid;
    auto ss = Util::generate_hex(16);

    int i;
    for (i = 0; i < ss.length(); i++)
        Util::guid[i] = ss[i];
    string filename = "E:\\study\\12.MP4";
    thread sebd([&]() {
      
        while (true)
        {
            try
            {
                ifstream in(filename, ios::in | ios::binary);
                int l = in.tellg();
                in.seekg(0, ios::end);
                int m = in.tellg();
                int size = m - l;
                in.seekg(0, ios::beg);
               char* buffer = new char[size];
                in.read(buffer, size);
                in.close();
                nngclient.send("tcp://192.168.0.124:5556", buffer, size);
                break;
            }
            catch (nng::exception e)
            {
                std::cout << e.what() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }

        });
    sebd.detach();
    system("pause");

   
}

void teststream()
{
    /* char* m = new char[7]{ 'j','i','n','\0','s','o','n' };
     istringstream memory(m, 7);
     istrstream ins(m);
     char p[3];
     memory.get(p, 2);
     std::cout <<string(p)<<std::endl;
     memory.get(p, 4);
     std::cout << string(p) << std::endl;
     ins.read(p, 2);
     std::cout << string(p) << std::endl;
     ins.read(p, 4);
     std::cout << string(p) << std::endl;*/

     //
    char* num = new char[4]{ '4' };
    intToByte(4, num);
    auto s = bytesToInt(num);
    std::cout << s << std::endl;
    char* lg = new char[8]{ '4' };
    int8to_bytes(23, lg);
    auto lf = bytesto_int8(lg);
    std::cout << lf << std::endl;
}

void testfactory()
{
    char ss[4]{ 'j','i','n' ,'y' };
    auto& f = BusFactory::Create(BusType::Inpoc);
    BusFactory::Create(BusType::Ipc);
    BusFactory::Create(BusType::tcp);
    f.publish("ss", ss, 4);
   // MsgLocalNode::LocalAddress = "127.0.0.1";
    //MsgLocalNode::LocalPort = 5567;

    //auto &sss = PtpFactory::Create();

}


void testNetTopic()
{
  auto pub=  BusFactory::CreatePoint(BusType::tcp);
  pub->revmsg = rev;
  pub->subscribe("jin");
  pub->subscribe("yu");
  auto sub = BusFactory::CreatePoint(BusType::tcp);
  int num = 0;
  while (true)
  {
      char ss[4]{ 'j','i','n' ,'y' };
      num++;
      auto sss = "mmmddd" + to_string(num);
      auto ddd = const_cast<char*>(sss.data());
      int size = sss.length();
      sub->publish("jin", ddd, size);

      //
      auto kkk = "sssddd" + to_string(num);
      auto mmm = const_cast<char*>(kkk.data());
      sub->publish("yu", mmm, size);
      if(num%100==0)
      std::this_thread::sleep_for(std::chrono::milliseconds(60000));  // 2 休眠1000ms
      if (num > 100)
          break;
  }
  
}



void testIpcTopic()
{
    auto pub = BusFactory::CreatePoint(BusType::Ipc);
    pub->revmsg = rev;
    pub->subscribe("jin");
    pub->subscribe("yu");
    auto sub = BusFactory::CreatePoint(BusType::Ipc);
    int num = 0;
    while (true)
    {
        char ss[4]{ 'j','i','n' ,'y' };
        num++;
        auto sss = "mmmddd" + to_string(num);
        auto ddd = const_cast<char*>(sss.data());
        int size = sss.length();
        sub->publish("jin", ddd, size);

        //
        auto kkk = "sssddd" + to_string(num);
        auto mmm = const_cast<char*>(kkk.data());
        sub->publish("yu", mmm, size);
        if (num % 100 == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(60000));  // 2 休眠1000ms
        if (num > 100)
            break;
    }

}

void testNetunsubscribeTopic()
{
    auto pub = BusFactory::CreatePoint(BusType::tcp);
    pub->revmsg = rev;
    pub->subscribe("jin");
    pub->subscribe("yu");
    auto sub = BusFactory::CreatePoint(BusType::tcp);
    int num = 0;
    while (true)
    {
        char ss[4]{ 'j','i','n' ,'y' };
        num++;
        auto sss = "mmmddd" + to_string(num);
        auto ddd = const_cast<char*>(sss.data());
        int size = sss.length();
        sub->publish("jin", ddd, size);

        //
        auto kkk = "sssddd" + to_string(num);
        auto mmm = const_cast<char*>(kkk.data());
        sub->publish("yu", mmm, size);
        if (num % 100 == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(6000));  // 2 休眠1000ms
         
        if (num > 500)
            break;
        if (num > 200)
        {
            pub->unsubscribe("yu");
        }
    }

}

void testNetunTopic()
{
    auto pub = BusFactory::CreatePoint(BusType::tcp);
    auto pub1 = BusFactory::CreatePoint(BusType::tcp);
    pub->revmsg = rev;
    pub1->revmsg = rev;
    pub->subscribe("jin");
    pub1->subscribe("yu");
    auto sub = BusFactory::CreatePoint(BusType::tcp);
    int num = 0;
    while (true)
    {
        char ss[4]{ 'j','i','n' ,'y' };
        num++;
        auto sss = "mmmddd" + to_string(num);
        auto ddd = const_cast<char*>(sss.data());
        int size = sss.length();
        sub->publish("jin", ddd, size);
       
        //
        auto kkk = "sssddd" + to_string(num);
        auto mmm = const_cast<char*>(kkk.data());
        sub->publish("yu", mmm, size);
       
        if (num % 100 == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // 2 休眠1000ms

       
        if (num == 201)
        {
           // pub->unsubscribe("yu");
           
            delete pub1;
        }
        if(num>10000)
            break;
    }

}

void testMQTopic()
{
    BridgeCore* bridge = new BridgeCore();
    list<string> lstpub;
    list<string> lstrec;
    lstpub.push_back("tcp://127.0.0.1:4456");
    lstrec.push_back("tcp://127.0.0.1:4457");
    bridge->pubAddress = lstpub;//通过此地址接收订阅方订阅；
    bridge->recAddress = lstrec;//通过此地址接收发布方数据；
    bridge->start();

    auto mq = BusFactory::CreateMQ();
    mq->url = "tcp://127.0.0.1:4456";
    mq->subscribe("jin");
    mq->revmsg = rev;
    auto mqpub = BusFactory::CreateMQ();
    mqpub->url = "tcp://127.0.0.1:4457";
  
    int num = 0;
    while (true)
    {
       
        num++;
        auto sss = "mmmddd" + to_string(num);
        auto ddd = const_cast<char*>(sss.data());
        int size = sss.length();
        mqpub->publish("jin", ddd, size);

        //
       /* auto kkk = "sssddd" + to_string(num);
        auto mmm = const_cast<char*>(kkk.data());
        mq->publish("yu", mmm, size);*/

        if (num % 100 == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // 2 休眠1000ms


        if (num == 201)
        {
            // pub->unsubscribe("yu");
        }
        if (num > 10000)
            break;
    }
   
}

int main()
{

    cout << "Memory leak test!" << endl;
    //testFilenng();
    //std::cout << "Hello World!\n";
    //testnng();
    //teststream();
   // testfactory();
    //TestTopic();
   //testNetTopic();
   //testNetunsubscribeTopic();
   // testNetunTopic();
    //cout << "Memory leak test!" << endl;
   // std::this_thread::sleep_for(std::chrono::milliseconds(30000));  // 2 休眠1000ms
    //testNetunTopic();
    testMQTopic();
    cout << "end test!" << endl;
    //testIpcTopic();
   // TestQueue();
   // server("tcp://192.168.0.153:52448");
    
   // system("pause");
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
