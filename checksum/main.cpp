#include <boost/crc.hpp>      // for boost::crc_basic, boost::crc_optimal
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <cstddef>    // for std::size_t
#include <iostream>   // for std::cout
#include <fstream>
std::string CRC64(const std::string &str)
{
    boost::crc_optimal<64, 0x42F0E1EBA9EA3693,
            0xffffffffffffffff, 0xffffffffffffffff,
            false, false> crc;

    crc.process_bytes(str.c_str(), str.size());

    char crcStr[128] = {0};
    sprintf_s(crcStr, 128, "%016llx", crc.checksum());
    return crcStr;
}
std::string PrintTime(boost::filesystem::path _file) {
    using boost::posix_time::ptime;
    using adj = boost::date_time::c_local_adjustor<ptime>;

    time_t const sys_time = last_write_time(_file);
    ptime const utc       = boost::posix_time::from_time_t(sys_time);
    ptime const local     = adj::utc_to_local(utc);

//    std::cout << "utc: " << utc << "\n";
//    std::cout << "local: " << local << "\n";

    return boost::posix_time::to_iso_extended_string(local);
}
int main(int argc, const char* argv[])
{
    std::fstream testFile;
    testFile.open("main.cpp",std::fstream::in | std::fstream::out);
    /*boost::filesystem::fstream boostfs(boost::filesystem::path("test.txt"));*/
    if (testFile.fail()){
        std::cout<<"缺少标准答案文件"<<std::endl;
        throw -1;
    }

    std::string data;
    int i=0;
    while(std::getline(testFile,data)){
        std::cout<<data<<std::endl;
        testFile.seekg(-data.size()-5,testFile.cur);
        testFile << std::to_string(++i).append(": ").append(data).append("\r");
    }
    testFile.close();

    return 0;
}