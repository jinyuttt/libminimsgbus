// demo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <msgtopic.h>
#include "blockingconcurrentqueue.h"
#include"Util.h"
#include "BusFactory.h"
#include "PtpFactory.h"
#include "MsgLocalNode.h"
using namespace libminimsgbus;
using namespace std;
BlockingConcurrentQueue<Records> errorRecords;
char Util::guid[32] = {};
void  rev(std::string topic, char* msg, int len)
{
    std::cout << "接收：" + topic << "数据:" << string(msg, len) << std::ends;
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
            pub->publish("jin", ss);
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

//void testnng()
//{
//    NngDataNative nngser;
//    NngDataNative nngclient;
//   /* nngser.receive("tcp://192.168.0.153:52248");
//    thread rec([&]() {
//        while (true)
//        {
//            auto buf = nngser.getData();
//            std::cout << "接收数据："+string(buf.bufdata,buf.size) << std::endl;
//        }
//        });
//    rec.detach();*/
//  
//    int len = 0;
//    Util::guid;
//        auto ss= Util::generate_hex(16);
//        char p[8];
//        int i;
//        for (i = 0;i < ss.length();i++)
//            Util::guid[i] = ss[i];
//    
//        thread sebd([&]() {
//            auto v = new char[4]{ 'j','i','n' ,'y' };
//                while (true)
//                {
//                    try
//                    {
//                        auto buf = Util::Convert("jin", v, 4, '0', 1, len);

//                        auto ret = nngclient.send("tcp://192.168.0.153:52248", buf, &len);
//                        //nngclient.send("tcp://192.168.0.153:52248", v, &len);
//                    }
//                    catch (nng::exception e)
//                    {
//                        std::cout << e.what() << std::endl;
//                    }
//                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//                }
//            
//            });
//        sebd.detach();
//   
//    //msgtopic* pub = new msgtopic();
//    //while (true)
//    //{
//    //    char ss[4] = { '7','d' };
//    //    pub->publish("jin", ss);
//    //    std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // 2 休眠1000ms

//    //}
//}

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
int main()
{
    std::cout << "Hello World!\n";
    //teststream();
    testfactory();
    //TestTopic();
   // testnng();
   // TestQueue();
   // server("tcp://192.168.0.153:52448");
    system("pause");
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
