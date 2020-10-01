#include <bits/stdc++.h>
#include <boost/asio.hpp>
#ifdef IS_SERVER
// BoostTcpServer.cpp : �������̨Ӧ�ó������ڵ㡣
//



using namespace boost::asio;

int main(int argc, char* argv[])
{
#if 1
    // ����asio�඼��Ҫio_service����
    io_service iosev;
    ip::tcp::acceptor acceptor(iosev,
                               ip::tcp::endpoint(ip::tcp::v4(), 1000));

    for(;;)
    {
        // socket����
        ip::tcp::socket socket(iosev);

        // �ȴ�ֱ���ͻ������ӽ���
        acceptor.accept(socket);
        // ��ʾ���ӽ����Ŀͻ���

        std::cout << socket.remote_endpoint().address() << std::endl;


        // ��ͻ��˷���message
        boost::system::error_code ec;
        std::string message;
        while (std::cin>>message) {
            socket.write_some(buffer(message), ec);
            if (*(message.end()-1)=='@') break;
        }

        // ���������ӡ������Ϣ
        if(ec)
        {
            std::cout <<
                      boost::system::system_error(ec).what() << std::endl;
            break;
        }
        // �뵱ǰ�ͻ�������ɺ�ѭ�������ȴ���һ�ͻ�����
    }
#endif

    return 0;
}
#endif

#ifdef IS_CLIENT
//code of client
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        //(1)ͨ��tcp::socket�ඨ��һ��tcp client����socket
        boost::asio::io_service io;
        tcp::socket socket(io);

        //(2)ͨ��connect�������ӷ���������socket���ӡ�
        tcp::endpoint end_point(boost::asio::ip::address::from_string("127.0.0.1"), 1000);
        socket.connect(end_point);

        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            //(3)ͨ��read_some������������
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



#endif
