#include <iostream>
#include <stdexcept>
#include <winsock.h>
#include <minwindef.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")
class WSAInitializer {
public:
    WSAInitializer(){
        WSADATA wsadata;
        if(WSAStartup(MAKEWORD(2, 2), &wsadata) != 0){
            throw std::runtime_error("初始化失败");
        }
    }
    ~WSAInitializer(){
        WSACleanup();
        std::cout << "WSA已清理" << std::endl;
    }
};

class TCP{
private:
    SOCKET socket_;
public:
    TCP(){
        socket_=socket(AF_INET,SOCK_STREAM,0);
    }
    ~TCP(){
        if(socket_!=INVALID_SOCKET){
            closesocket(socket_);
            std::cout<<"套接字已关闭"<<std::endl;
        }
    }
    bool listen(std::string&ip,int&port){
        SOCKADDR_IN add;
        add.sin_family=AF_INET;
        add.sin_addr.s_addr=inet_addr(ip.c_str());
        add.sin_port=htons(port);
        if(bind(socket_,(sockaddr*)&add,sizeof(add))==SOCKET_ERROR){
            closesocket(socket_);
            return 0;
        }
        if(::listen(socket_, SOMAXCONN) == SOCKET_ERROR){
            closesocket(socket_);
            return 0;
        }   
        std::cout << "监听成功，等待连接..." << std::endl;
        return 1;
    }
    SOCKET accept(){
        SOCKADDR_IN add;
        int len=sizeof(add);
        SOCKET client_socket = ::accept(socket_, (sockaddr*)&add, &len);
        if(client_socket == INVALID_SOCKET){
            std::cout << "连接失败" << std::endl;
            return INVALID_SOCKET;
        }
        std::cout << "连接成功,客户端IP: " << inet_ntoa(add.sin_addr) << std::endl;
        return client_socket;
    }

    bool get(SOCKET client_socket,std::string&message){
        message.resize(1024);
        int result=::recv(client_socket,&message[0],message.size(),0);
        if(result==SOCKET_ERROR){
            std::cout<<"接收数据失败，错误码为："+std::to_string(WSAGetLastError())<<std::endl;
            return 0;
        }
        else if(result==0){
            std::cout<<"请求关闭"<<std::endl;
            return 0;
        }
        message.resize(result);
        std::cout<<"接收到数据: "<<message<<std::endl;
        return 1;
    }
};
int main(){
    SetConsoleOutputCP(CP_UTF8);
    WSAInitializer wsaInitializer;
    TCP tcp;
    std::string ip = "192.168.205.1";
    int port = 8080;
    if(!tcp.listen(ip, port)){
        std::cerr << "监听失败" << std::endl;
        return 1;
    }
    SOCKET client_socket = tcp.accept();
    if(client_socket == INVALID_SOCKET){
        std::cerr << "接受连接失败" << std::endl;
        return 1;
    }
    std::string message;
    while(true){
        if(!tcp.get(client_socket, message)){
            break;
        }
    }
    return 0;
}