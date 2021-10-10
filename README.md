# libminimsgbus
集成订阅发布和点对点通信
## 使用说明

### 订阅发布模式  
    BusFactory::Create(BusType::Inpoc);
    BusFactory::Create(BusType::Ipc);
    BusFactory::Create(BusType::tcp);
1.inpoc：观察着模式  
2.ipc：内存共享  
3.tcp封装zmq组播管理主题,nng传输数据;此模式使用需要设置本地网络地址，使用方法：   
    MsgLocalNode::LocalAddress = "127.0.0.1";  
    MsgLocalNode::LocalPort = 5567;  
	默认地址为*,程序自动获取可用ip,端口默认:0.  
4.mq模式，支持中心节点数据转发。  
1).中心节点  
    BridgeCore *bridge = new BridgeCore();  
    list<string> lstsub;  
	list<string> lstrec;   
    lstsub.push_back("tcp://127.0.0.1:4456");  
	lst.push_back("tcp://127.0.0.1:4457");   
    bridge->pubAddress = lstsub;//通过此地址接收订阅方订阅；  
    bridge->recAddress = lstrec;//通过此地址接收发布方数据；  
    bridge->start();  
	说明：接收数据和订阅数据2类地址不能相同  
2).客户端发布订阅  
 BusFactory::CreateMQ();   
5.如果主播不通，可以直接设置远端地址  
MsgLocalNode::remote
### 点对点通信 
 PtpFactory::Create();
 创建对象，设置其中的地址和端口，即可使用。

## 程序介绍
### 依赖项
1.程序集成了zmq,ipc,nng三个通信组件  
2.依赖放在deloy文件夹中，包括头文件和lib  
3.订阅发布tcp模式使用zmq组播作为寻址，使用nng作为通信组件  
4.增加MiniMsgExport.h文件，导出方法  
### 程序目录
使用cmake直接生成



### 程序实现

c++11编写，支持Windows和Linux


	
	
