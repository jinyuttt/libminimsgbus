#pragma once
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>
#include <Tools.h>
#include"MsgDef.h"
#include <random>
#include <strstream>
using namespace std;
class Util
{
 
public:
    static char guid[32] ;
    static char* Convert(string topic, char bytes[],int size, char flage, int64_t msgid,int &dlen)
    {
        int  msglen = 1 + 32 + 8 + 4 + (int)topic.size() + size;
        auto buf = new char[msglen];
        auto tp = topic.c_str();
        char* len = new char[4];
        intToByte(topic.size(),len);
        char* msgflage = new char[8];
        int8to_bytes(msgid, msgflage);
      
        dlen = msglen;
        
        ostringstream oss(msglen);
        char* tmp = new char[1]{ flage };
        oss.write(tmp, 1);
        oss.write(guid,32);
        oss.write(msgflage,8);
        oss.write(len,4);
        oss.write(tp, topic.size());
        oss.write(bytes,size);
        
        string ss = oss.str();
        const char* buffer = ss.c_str();
        //buf = const_cast<char*>(buffer);
        memcpy_s(buf, msglen, buffer, msglen);
        return buf;
    }

        
    static TopicStruct Convert(char bytes[],int dlen)
    {
        //标记（1）+guid+msgid+主题长度（4）+主题（m）+数据
        char flage = bytes[0];
        //数据类型
        
        int index = 1;
        //节点guid
        char id[32] ;
        for (size_t i = 0; i < 32; i++)
        {
            id[i] = bytes[index++];
        }
        string guid{id,32};
        //消息ID
        char msgno[8];
        for (size_t i = 0; i < 8; i++)
        {
            msgno[i] = bytes[index++];
        }
        auto msgid = bytesto_int8(msgno);
        char len[4]{};
        for (size_t i = 0; i < 4; i++)
        {
            len[i] = bytes[index++];
        }
        auto  curLen = bytesToInt(len);

        //主题
        char* topic = new char[curLen];
        for (size_t i = 0; i < curLen; i++)
        {
            topic[i] = bytes[index++];
        }
        string strTopic(topic, curLen);
          //数据
        int size = dlen - curLen - 45;
        char* buffer = new char[size];
        for (size_t i = 0; i < size; i++)
        {
           buffer[i] = bytes[index++];
        }
        delete[] topic;
        return  TopicStruct{ msgid, guid,strTopic,flage,  buffer,size };

    }
    static vector<string> StringSplit(const string& in, const string& delim)
    {
      
        try
        {
            regex re{ delim };
            return vector<string>{
                sregex_token_iterator(in.begin(), in.end(), re, -1),
                    sregex_token_iterator()
            };
        }
        catch (const std::exception& e)
        {
            cout << "error:" << e.what() << std::endl;
        }
        
    }

    static char* ConvertString(string str,int &len)
    {
        int size = str.size();
        char* buf = new  char[size];
        str.copy(buf, size);
        len = size;
        return buf;

    }
  static  unsigned int random_char() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        return dis(gen);
    }

   /// <summary>
   /// 输出uuid
   /// </summary>
   /// <param name="len">32</param>
   /// <returns></returns>
   static std::string generate_hex(const unsigned int len) {
        std::stringstream ss;
        for (auto i = 0; i < len; i++) {
            const auto rc = random_char();
            std::stringstream hexstream;
            hexstream << std::hex << rc;
            auto hex = hexstream.str();
            ss << (hex.length() < 2 ? '0' + hex : hex);
        }
        return ss.str();
    }
};

