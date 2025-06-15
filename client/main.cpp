#include <stdexcept>
#include <string>
#include <winsock2.h>
#include <minwindef.h>
#include <iostream>
#include <string>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
class WSAInitializer {
public:
    WSAInitializer(){
        WSADATA wsadata;
        if(WSAStartup(MAKEWORD(2, 2), &wsadata)!=0){
            throw std::runtime_error("WSAStartup failed,exit code="+std::to_string(WSAGetLastError()));
        }
    }
    ~WSAInitializer(){
        WSACleanup();
        std::cout<<"释放"<<std::endl;
    }
};

class TCP{
private:
    SOCKET socket_;
public:
    TCP():socket_(INVALID_SOCKET){
        socket_=socket(AF_INET,SOCK_STREAM,0);
        std::cout<<"socket创建成功"<<std::endl;
    }
    ~TCP(){
        if(socket_!=INVALID_SOCKET){
            closesocket(socket_);
            std::cout<<"socket关闭成功"<<std::endl;
        }
    }
    //连接服务器
    bool connect(std::string ip, int port){
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        
        if(::connect(socket_, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR){
            throw std::runtime_error("连接服务器失败，错误码="+std::to_string(WSAGetLastError()));
        }
        std::cout<<"连接服务器成功"<<std::endl;
        return true;
    }
    int send(std::string message){
        if(socket_==INVALID_SOCKET){
        }
        int result=::send(socket_,message.c_str(), message.size(), 0);
        if(result==SOCKET_ERROR){
            throw std::runtime_error("发送失败，错误码="+std::to_string(WSAGetLastError()));
        }
        return result;
    }
};
int main(){
    WSAInitializer wsa;
    TCP tcp;
    try{
        tcp.connect("192.168.50.128", 8888);
        std::string message = "Hello, Server!";
        int bytesSent = tcp.send(message);
    }catch(const std::runtime_error& e){
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}