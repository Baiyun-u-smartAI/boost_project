#include <boost/thread.hpp>
#include <bits/stdc++.h>

using namespace std;
boost::thread thd;
boost::thread thd_intrp;
boost::mutex mtx;
void worker2();

void worker1() {
    try {
        std::cout << "begin!" << std::endl;

        string str;
        boost::this_thread::interruption_point();
        boost::this_thread::sleep(boost::posix_time::seconds(10));
//        cin >> str;
//        cout << str << endl;

        //如果2没有给1中断信号就重新启动2
        boost::this_thread::disable_interruption d1;
        thd_intrp.interrupt();
        thd_intrp.join();
        thd_intrp = boost::thread(&worker2);
        boost::this_thread::restore_interruption r1(d1);


    } catch (boost::thread_interrupted &) {
        std::cout << "1 interrupted!" << std::endl;
        ///也让2中断结束
        thd_intrp.interrupt();
        thd_intrp.join();
    }
}

int main() {
    thd = boost::thread(&worker1);
    thd_intrp = boost::thread(&worker2);

    if (thd_intrp.joinable()) {
        thd_intrp.join();
    }
    thd.join();

}

void worker2() {
    try {
        for (;;) {
            boost::this_thread::sleep(boost::posix_time::seconds(5));

            boost::this_thread::disable_interruption d1;

            thd.interrupt(); ///不能结束另一个线程访问I/O设备时的阻塞
            cout << "!\n";
            boost::this_thread::restore_interruption r1(d1);
        }

    } catch (boost::thread_interrupted &) {
        std::cout << "2 interrupted!" << std::endl;
    }

}