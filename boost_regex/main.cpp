#include <iostream>
#include <string>
#include <fstream>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/locale.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/compute.hpp>
#include <boost/type_index.hpp>
#include <windows.h>
#include <boost/regex.hpp>

// 随机产生器
boost::random::mt11213b gen(GetTickCount());
using std::cout,std::endl,std::string;
int generator(){
    boost::uniform_int<> Ui_ran(0x0,0xFFFF);
    return Ui_ran(gen);
}

// 获取变量名
template <class T>
void GetType(T) {
    std::cout << "\n Short name: " << boost::typeindex::type_id<T>().raw_name();
    std::cout << "\n Readable name: " << boost::typeindex::type_id<T>().pretty_name();
}

// 定义二元函数compute
/*struct VarGroup{
    boost::compute::int_ x;
    boost::compute::int_ y;
};
BOOST_COMPUTE_ADAPT_STRUCT(VarGroup, VarGroup, (x, y));*/

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



auto main(int argc,char* argv[]) -> int {
    // 检测鼠标位置 延时
    /*POINT P0,P1;
    while(1){
        do{
            GetCursorPos(&P0);
            boost::this_thread::sleep(boost::chrono::milliseconds(60000));//延时1000ms
            GetCursorPos(&P1);
        }while( (P0.x!=P1.x) || (P0.y!=P1.y));

        system("randomutf.exe");
    }*/

    // 随机汉字
    /*
    system("color 0a");
    system("mode con cols=60 lines=20");
    for (int j = 0; j < 600; ++j) {
        std::string temp;
        while (temp.empty()){
            unsigned int point = generator();
            std::string utf8 = boost::locale::conv::utf_to_utf<char>(&point, &point + 1);
            temp=boost::locale::conv::from_utf(utf8,"GB2312");
        }
        boost::this_thread::sleep(boost::chrono::nanoseconds(80));
        cout<<temp;
        temp.resize(0);
    }
    boost::this_thread::sleep(boost::chrono::milliseconds(3000));
    system("taskkill /f /im cmd.exe");
    boost::this_thread::sleep(boost::chrono::milliseconds(10));
    system("start timing.bat");*/

    // 回溯检索文件（夹）
    /* #define IS_RECURSIVE 0
    boost::filesystem::path root =boost::filesystem::current_path();
#if IS_RECURSIVE
    boost::filesystem::recursive_directory_iterator endIter;
#else
    boost::filesystem::directory_iterator endIter;
#endif
#if IS_RECURSIVE
    for (boost::filesystem::recursive_directory_iterator iter(root);
    iter!=endIter; iter++){
#else
    for (boost::filesystem::directory_iterator iter(root);
    iter!=endIter; iter++){
#endif
        if(boost::filesystem::is_directory(*iter)){
            cout<< "dir: "<< iter->path().string()<<endl;
            std::time_t const dir_last_time = boost::filesystem::last_write_time(iter->path());
            boost::posix_time::ptime const dir_utc = boost::posix_time::from_time_t(dir_last_time);
            boost::posix_time::ptime const dir_local
            = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(dir_utc);
            //comparable ptime.time_of_day() ptime.date()
            cout<<boost::posix_time::to_iso_extended_string(dir_local)<<endl;

        }else {
            cout<< "file: "<< iter->path().string()<<endl;
            std::time_t const file_last_time = boost::filesystem::last_write_time(iter->path());
            boost::posix_time::ptime const file_utc = boost::posix_time::from_time_t(file_last_time);
            boost::posix_time::ptime const file_local
                    = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(file_utc);
            cout<<boost::posix_time::to_iso_extended_string(file_local)<<endl;
        }
    }*/


    // 定义二元函数compute
    /*using namespace boost;

    using compute::int_;
    // get default device and setup context
    compute::device device = compute::system::default_device();
    compute::context context(device);
    compute::command_queue queue(context, device);

    // point coordinates


    VarGroup a[]={{2,8},{1,5}};

    // create vector for five points
    compute::vector<VarGroup> vector(2, context);

    // copy point data to the device
    compute::copy(
            reinterpret_cast<VarGroup *>(a),
            reinterpret_cast<VarGroup *>(a) + 2,
            vector.begin(),
            queue
    );

    compute::vector<boost::compute::double_> out(2, context);
    boost::compute::function<double (VarGroup)> add_four =
            boost::compute::make_function_from_source<double (VarGroup)>(
                    "add_four",
                    "double add_four(VarGroup v) { return v.x+v.y + 4.2; }"
            );

    boost::compute::transform(vector.begin(), vector.end(), out.begin(), add_four, queue);

    std::vector<boost::compute::double_> cpu_area(2);
    boost::compute::copy(out.begin(),out.end(),cpu_area.begin(),queue);
    cout<<cpu_area[0]<<','<<cpu_area[1];*/


//    if (argc<2){
//        throw 1;
//    }
//    boost::filesystem::path input(argv[1]);
    boost::regex x2old("(..*$)",boost::regex::icase|boost::regex::perl);
    if (argc<2){
        throw 1;
    }
    for (int i=1;i<argc;i++){
        string s=argv[i];
        std::time_t const dir_last_time = boost::filesystem::last_write_time(s);
        boost::posix_time::ptime const dir_utc = boost::posix_time::from_time_t(dir_last_time);
        boost::posix_time::ptime const dir_local
                = boost::date_time::c_local_adjustor<boost::posix_time::ptime>::utc_to_local(dir_utc);
        //comparable ptime.time_of_day() ptime.date()

        string Fdate = boost::posix_time::to_iso_extended_string(dir_local);
        Fdate =boost::regex_replace(Fdate,boost::regex("-|T|:"),"_");
        Fdate.resize(Fdate.size()-3);
        string FileDir=boost::regex_replace(s,boost::regex("\\\\[^\\\\]+$"),"\\\\");
        string FileName=boost::regex_replace(s,boost::regex("^(.*\\\\)([^\\\\]+)$",
                        boost::regex::icase|boost::regex::perl),"$2");
        boost::filesystem::create_directory(FileDir.append(Fdate));
        boost::filesystem::copy_file(s , FileDir.append("\\").append(FileName),
                                     boost::filesystem::copy_option::overwrite_if_exists);


        string Tips=string("已将").append(s).append("复制到\n").append(FileDir).append("\n").append("将保留原文件？");
        int value = MessageBoxEx(NULL, Tips.c_str(),"提示", MB_ICONWARNING | MB_YESNO, MAKELANGID(LANG_CHINESE_SIMPLIFIED, SUBLANG_CHINESE_TRADITIONAL));
        if (value==7){
            Fdate=boost::regex_replace(Fdate,boost::regex("_"),"@");
            string d=boost::regex_replace(s,x2old,string("$1.").append(Fdate));
            boost::filesystem::rename(s , d);
            boost::filesystem::ofstream( s,std::ios::trunc);
        }
    }


    return 0;
}