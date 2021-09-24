# libminimsgbus
集成订阅发布和点对点通信
## 使用说明

### 订阅发布模式  
    BusFactory::Create(BusType::Inpoc);
    BusFactory::Create(BusType::Ipc);
    BusFactory::Create(BusType::tcp);
1.inpoc采集观察着模式  
2.ipc采集内存共享  
3.tcp封装zmq组播,nng传输;此模式使用需要设置本地网络地址，使用方法：   
    MsgLocalNode::LocalAddress = "127.0.0.1";  
    MsgLocalNode::LocalPort = 5567;  
	默认地址为*,程序自动获取可用ip,端口默认:5556.  
### 点对点通信 
 PtpFactory::Create();
 创建对象，设置其中的地址和端口，即可使用。

## 程序介绍
### 依赖项
1.程序集成了zmq,ipc,nng三个通信组件  
2.依赖放在deloy文件夹中，包括头文件和lib  
3.订阅发布tcp模式使用zmq组播作为寻址，使用nng作为通信组件

### 程序目录
使用cmake直接生成



### 程序实现

c++11编写，支持Windows和Linux

### 升级

新增mq模式，支持中心节点数据转发。  
1.中心节点  
    BridgeCore *bridge = new BridgeCore();  
    list<string> lst;  
    lst.push_back("tcp://127.0.0.1:4456");  
    bridge->pubAddress = lst;//通过此地址接收订阅方订阅；  
    bridge->recAddress = lst;//通过此地址接收发布方数据；  
    bridge->start();  
2.客户端发布订阅  
 BusFactory::CreateMQ();
	
	
