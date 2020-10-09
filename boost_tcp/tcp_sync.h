//
// Created by pc002 on 2020/10/2.
//

#ifndef TCP_IP_CLIENT_TCP_SYNC_H
#define TCP_IP_CLIENT_TCP_SYNC_H
#include <bits/stdc++.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>
using namespace std;
using boost::asio::ip::tcp;
using namespace boost::asio;

int server(int argc, char* argv[])
{
#if 1
    // 所有asio类都需要io_service对象
    io_service iosev;
    ip::tcp::acceptor acceptor(iosev,
                               ip::tcp::endpoint(ip::tcp::v4(), 1000));

    for(;;)
    {
        // socket对象
        ip::tcp::socket socket(iosev);

        // 等待直到客户端连接进来
        acceptor.accept(socket);
        // 显示连接进来的客户端
        std::cout << "已连接上地址：";
        std::cout << socket.remote_endpoint().address() << std::endl;
        std::cout << "请输入发送内容，如果以@结尾则终止本次通讯："<< std::endl;

        // 向客户端发送message
        boost::system::error_code ec;
        std::string message;
        while (std::cin>>message) {
            socket.write_some(buffer(message), ec);
            if (*(message.end()-1)=='@') break;
        }

        // 如果出错，打印出错信息
        if(ec)
        {
            std::cout <<
                      boost::system::system_error(ec).what() << std::endl;
            break;
        }
        // 与当前客户交互完成后循环继续等待下一客户连接
    }
#endif

    return 0;
}

int client(int argc, char* argv[])
{
    try
    {
        //(1)通过tcp::socket类定义一个tcp client对象socket
        boost::asio::io_service io;
        tcp::socket socket(io);

        //(2)通过connect函数连接服务器，打开socket连接。
        tcp::endpoint end_point(boost::asio::ip::address::from_string("39.108.140.215"), 32123);
        socket.connect(end_point);

        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            //(3)通过read_some函数来读数据
            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
            {
                break;    //connection closed cleadly by peer
            }
            else if (error)
            {
                throw boost::system::system_error(error);    //some other error
            }

            cout.write(buf.data(), len);
            cout<<"\n";
        }
    }
    catch (std::exception& e)
    {
        cout<<e.what()<<endl;
    }
    system("pause");
}




#endif //TCP_IP_CLIENT_TCP_SYNC_H
