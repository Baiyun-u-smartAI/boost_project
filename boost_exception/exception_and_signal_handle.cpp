#include <iostream>
#include <csignal> /*需要重新定义系统的异常处理函数*/
#include <boost/exception/diagnostic_information.hpp> /*处理boost 异常的头文件*/


// 用于算术异常的处理
void handle_div_0(int code)
{
    std::cout<<"error code:"<<code<<std::endl;
    // 除以0异常的错误码是8
    if(code==8) std::cerr << "attempt to divide integer by 0." << std::endl;
    exit(code);
}
// 用于处理内存段错误的异常
void pointer_not_init(int code)
{
    std::cout<<"error code:"<<code<<std::endl;
    // 给野指针赋值的错误码是11
    if(code==11) std::cerr << "attempt to assign a non-inited pointer." << std::endl;
    exit(code);
}
int main() {
    /*重新定义算术异常接入函数*/
    if (SIG_ERR == signal(SIGFPE, handle_div_0)) {
        std::cerr << "fail to setup handler." << std::endl;
        return 1;
    }
    /*重新定义段错误异常接入函数*/
    if (SIG_ERR == signal(SIGSEGV, pointer_not_init)) {
        std::cerr << "fail to setup handler." << std::endl;
        return 3;
    }
    try {
        int* a=new int(6);
        *a=4;
        std::cout << *a/0 << std::endl;
    }
    catch (std::exception &e){
        std::cerr << "exception:" <<e.what();
        return 1;
    }
    catch (...) {
        // 用于输出boost库定义的异常
        std::cerr << "Unhandled exception!" << std::endl <<
                  boost::current_exception_diagnostic_information();
        return 66;
    }
    return 0;
}
