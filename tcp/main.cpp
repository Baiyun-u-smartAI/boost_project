#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <cstring>
#include <opencv2/opencv.hpp>
using namespace std;
using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace cv;
int main(int argc,char** argv){
	
	
	
	
	if (argc==2 && !strcmp(argv[1],"server")){
		std::cout<<"服务器已经启动!"<<std::endl;
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
			

			// 向客户端发送message
			boost::system::error_code ec;
			Mat src = imread(R"(G:\mingw64\opencv\build\cpp_code_src\java\test\common_test\res\drawable\lena.png)");
			string message;
			std::vector<unsigned char> buff;
			cv::imencode(".png", src, buff);
			message.resize(buff.size());
			memcpy(&message[0], buff.data(), buff.size());
			socket.write_some(buffer(message), ec);

			// 如果出错，打印出错信息
			if(ec)
			{
				std::cout <<
						  boost::system::system_error(ec).what() << std::endl;
				break;
			}
			// 与当前客户交互完成后循环继续等待下一客户连接
		}	
	}
	else if (argc==2 && !strcmp(argv[1],"client")){
		std::cout<<"客户端已经启动!"<<std::endl;
		//(1)通过tcp::socket类定义一个tcp client对象socket
        boost::asio::io_service io;
        tcp::socket socket(io);

        //(2)通过connect函数连接服务器，打开socket连接。
        tcp::endpoint end_point(boost::asio::ip::address::from_string("127.0.0.1"), 1000);
        socket.connect(end_point);

        for (;;)
        {
            boost::array<char, 512*512*999> buf;
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
			
            
			string str;
			std::copy(buf.begin(), buf.begin()+len, std::back_inserter(str));
			cv::Mat mat2(1, str.size(), CV_8U, (char*)str.data());
			cv::Mat dst = cv::imdecode(mat2, IMREAD_COLOR);
	
			imshow("test",dst);
			waitKey(0);
			//cout.write(buf.data(), len);
            //cout<<endl;
        }
	}
	return 0;
	
	
	
	
	
	
}