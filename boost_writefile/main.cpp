#include <iostream>
#include <fstream>
#include <boost/random.hpp>
#include <boost/regex.hpp>
#include <windows.h>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <sstream>
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int search(std::string inputStr, const char* expressRegex){

    boost::regex e1(expressRegex);

    std::string::const_iterator start, end;
    start = inputStr.begin();
    end = inputStr.end();

    boost::match_results<std::string::const_iterator> what;
    boost::match_flag_type flags = boost::match_default;

    while(regex_search(start, end, what, e1, flags))
    {
        return 1;
        start = what[0].second;
    }
    return 0;
}

void runtest_bat(unsigned long long* tt){
    const boost::posix_time::ptime beginT = boost::posix_time::microsec_clock::local_time();//��¼���еĿ�ʼʱ��

    system("test.bat 2>&1 > output.txt");
    const boost::posix_time::ptime endT = boost::posix_time::microsec_clock::local_time();//��¼���еĽ���ʱ��
    boost::posix_time::time_duration TT = endT-beginT;
    *tt = TT.total_milliseconds();
}
void timeout(unsigned long long* tt){
    boost::this_thread::sleep(boost::chrono::milliseconds(2000));
    system("taskkill /f /im test1.exe");

    if (*tt>1800) {
        MessageBox(NULL,"��ʱ","��������ѭ��...",MB_OK+16);
        system("del output.txt input.txt test.bat test1.exe generate.bat");
    }


}

void getRgx(std::vector<std::string>& expRegex){
    std::ofstream RgxBatFile;
    expRegex.clear();
    expRegex.resize(0);
    RgxBatFile.open("ans.bat", std::ios::out|std::ios::trunc);
    RgxBatFile << "@echo off" << std::endl;
    RgxBatFile << "if exist correct.exe (" << std::endl;
    RgxBatFile << "correct.exe < input.txt" << std::endl;
    RgxBatFile << ") else (" << std::endl;
    RgxBatFile << "echo x=msgbox^(\"û�б������\" ,16, \"���ش���\"^) > msgbox.vbs" << std::endl;
    RgxBatFile << "start msgbox.vbs" << std::endl;
    RgxBatFile << ")" << std::endl;
    RgxBatFile.close();
    system("ans.bat 2>&1 > test.rgx");
    system("del ans.bat");
    std::ifstream RgxFile;
    RgxFile.open("test.rgx", std::ios::in);
    std::string linedata;
    RgxFile>>linedata;
    expRegex.push_back(linedata);
    while(RgxFile>>linedata){
        expRegex.push_back(linedata);
    }

    RgxFile.close();


}



int main(int argc,char *argv[]) {
    /*boost::minstd_rand generator(time(0));
    boost::uniform_int<> uni_dist(0, 40);
    boost::variate_generator<boost::minstd_rand, boost::uniform_int<> > uni(generator, uni_dist);*/
//    int x = uni();
//    int y = uni();
    std::string c2exe=std::string("gcc \"").append(argv[1]).append("\" -o test1.exe");
//    system(c2exe.c_str());// ����m.c
//    std::string in1=std::string("echo.").append(std::to_string(x)).append("> input.txt ");
//    std::string in2=std::string("echo.").append(std::to_string(y)).append(">> input.txt");
    std::ofstream batFile;
    batFile.open("test.bat", std::ios::out|std::ios::trunc);
    if (!batFile)
    {
        throw -1;
    }
    batFile << "@echo off" << std::endl;
    batFile << "echo by ����ʦstudio" << std::endl;
    batFile << c2exe << std::endl;// ����m.c


    batFile << "scanf_random.exe" << std::endl;// ���ô����һ����x
//    batFile << in2 << std::endl;// ���ô���ڶ�����y
    batFile << "echo. >> input.txt" << std::endl;//����ɵ��������ĵȴ�
    batFile << "echo. >> input.txt" << std::endl;//����ɵ��������ĵȴ�


    batFile << "if exist test1.exe (" << std::endl;
    batFile << "test1.exe < input.txt)" << std::endl;// ����
//    batFile <<"pause"<<std::endl;
    batFile << "exit";// �������input.txt�ͱ������ļ�test1.exe
    batFile.close();

    unsigned long long tt=0;
    // ��ʼ�������m.c�ļ���
    /*const boost::posix_time::ptime beginT = boost::posix_time::microsec_clock::local_time();//��¼���еĿ�ʼʱ��
    system("test.bat 2>&1 > output.txt");
    const boost::posix_time::ptime endT = boost::posix_time::microsec_clock::local_time();//��¼���еĽ���ʱ��

    boost::posix_time::time_duration TT = endT-beginT;
    auto tt = TT.total_milliseconds();*/
    boost::thread_group TG;
    TG.create_thread(boost::bind(&runtest_bat,&tt));
    TG.create_thread(boost::bind(&timeout,&tt));
    TG.join_all();



    std::ifstream outFile;
    outFile.open("output.txt", std::ios::in);
    if (!outFile)
    {
        throw -1;
    }
    std::string data;

    // �𰸵�������ʽ��
    std::vector<std::string> expRegex;
    getRgx(expRegex);
    if (expRegex.empty()) {
        boost::this_thread::sleep(boost::chrono::milliseconds(3000));
        system("del msgbox.vbs test.rgx output.txt test.bat input.txt test1.exe generate.bat");
        throw -3;
    }
    boost::this_thread::sleep(boost::chrono::milliseconds(200));
    //����ʱ����ʾ�ַ���
    std::string time=std::string("����&����ʱ�䣺").append(std::to_string(tt)).append("ms");


    while (std::getline(outFile,data)) {
        if (boost::regex_match(data,boost::regex(*(expRegex.begin())))) {
            expRegex.erase(expRegex.begin());
            if (!expRegex.empty()) continue;
            MessageBox(NULL,"���Գɹ�",time.c_str(),MB_OK+48);
            std::cout << "success!" << std::endl;
            outFile.close();
            system("del output.txt test.bat input.txt test1.exe test.rgx generate.bat");
            return 0;
        }
        std::cout <<"fail: " << data << std::endl;
    }


    MessageBox(NULL,"����ʧ��","���C��������",MB_OK+16);
    outFile.close();

    std::ifstream inputFile;
    inputFile.open("input.txt",std::fstream::in);
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    inputFile.close();

    std::ofstream logFile;
    logFile.open("fail.log",std::ios::out|std::ios::trunc);
    logFile<<"�������룺"<<std::endl;
    logFile<<buffer.rdbuf();
    buffer.str("");

    logFile<<"���ԵĹ���(������ʽ)��"<<std::endl;
    for (auto Regex:expRegex){
        logFile<<Regex<<std::endl;
    }

    logFile<<"���������"<<std::endl;
    outFile.open("output.txt", std::ios::in);
    buffer << outFile.rdbuf();
    logFile<<buffer.rdbuf();
    buffer.str("");


    outFile.close();
    logFile.close();





    system("del output.txt test.bat input.txt test1.exe test.rgx generate.bat");
    return 1;
}
/*   m.c ����
 * #include <stdio.h>
 * int main(){
 *      int n,m;
 *      scanf("%d%d",&n,&m);
 *      printf("The Num is:%d,%d\n",n,m);
 *      return 0;
 *      }
 */