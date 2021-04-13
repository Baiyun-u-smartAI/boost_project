#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <cstring>
#include <opencv2/opencv.hpp>

//using boost::asio::ip::tcp;
//using namespace boost::asio;
//using namespace cv;
int main() {

    std::cout << "客户端已经启动!" << std::endl;
    //(1)通过tcp::socket类定义一个tcp client对象socket
    boost::asio::io_service io;
    boost::asio::ip::tcp::socket socket(io);

    //(2)通过connect函数连接服务器，打开socket连接。
    boost::asio::ip::tcp::endpoint end_point(boost::asio::ip::address::from_string("127.0.0.1"), 1000);
    socket.connect(end_point);

    std::string data;
    for (;;) {
        boost::array<char, 500000> buf;
        boost::system::error_code error;

        //(3)通过read_some函数来读数据
        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        if (error == boost::asio::error::eof) {
            break;    //connection closed cleadly by peer
        } else if (error) {
            throw boost::system::system_error(error);    //some other error
        }


        std::string str;
        std::copy(buf.begin(), buf.begin() + len, std::back_inserter(str));
        cv::Mat mat2(1, str.size(), CV_8U, (char *) str.data());
        cv::Mat dst = cv::imdecode(mat2, cv::IMREAD_COLOR);

        cv::imshow("test", dst);
        cv::waitKey(0);
        std::cout << "已接收长度:" << len << std::endl;
//			std::cout.write(buf.data(), len);
//            std::cout<<std::endl;

    }
    return 0;


}
