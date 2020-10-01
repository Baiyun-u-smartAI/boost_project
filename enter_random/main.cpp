#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/any.hpp>
#include <boost/regex.hpp>
#include <vector>
#include <windows.h>
#define READ_ONLY std::ios::in
#define WRITABLE std::ios::out
#define CLEARFILE std::ios::trunc
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;




int boost_rand_int(int _min=-1000,int _max=1000){
    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> uni_dist(_min, _max);
    return uni_dist(rng);
}
float boost_rand_real(float _min=-999.9999f,float _max=999.9999f){
    boost::random::random_device rng;
    boost::random::uniform_real_distribution<> uni_dist(_min, _max);
    return uni_dist(rng);
}

char boost_rand_char(bool NotCreateNumber= true){
    boost::random::random_device rng;
    std::string characters;
    if (!NotCreateNumber){
        characters="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    }
    else{
        characters="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    boost::random::uniform_int_distribution<> uni_dist(0, characters.size()-1);
    return characters[uni_dist(rng)];
}

template <char T='i'>
boost::any options_generator(string opt){
    const boost::regex __default("^(int)([^:#]{0,})$");
    const boost::regex __uint("int#");
    const boost::regex __exact("int:[^~]+$");
    const boost::regex __personal("int:.*[~].*");
    if(boost::regex_match(opt,__personal)){
        std::stringstream daata;
        string _temp;
        std::vector<int> n;
        n.resize(2);
        daata << boost::regex_replace(opt,boost::regex("(int:)(.*)",boost::regex::icase|boost::regex::perl)
                ,"$2");
        int i=0;
        while(std::getline(daata,_temp,'~')){
            std::stringstream __daata;
            __daata.str(_temp);
            __daata>>n[i++];
        }
        daata.clear();
        daata.str("");
        return boost_rand_int(n[0],n[1]);
    }else if(boost::regex_match(opt,__exact)){
        std::stringstream daata;
        int n;
        daata.str(boost::regex_replace(opt,boost::regex("(int:)(.*)",boost::regex::icase|boost::regex::perl)
                ,"$2"));
        daata>>n;
        daata.clear();
        daata.str();
        return n;
    }else if(boost::regex_match(opt,__uint)){
        return boost_rand_int(0);
    }else {
        return boost_rand_int();
    }
}

template <>
boost::any options_generator<'r'>(string opt){
    const boost::regex __default("^(real)([^:#]{0,})$");
    const boost::regex __ureal("real#");
    const boost::regex __exact("real:[^~]+$");
    const boost::regex __personal("real:.*[~].*");
    if(boost::regex_match(opt,__personal)){
        std::stringstream daata;
        string _temp;
        std::vector<float> n;
        n.resize(2);
        daata << boost::regex_replace(opt,boost::regex("(real:)(.*)",boost::regex::icase|boost::regex::perl)
                ,"$2");
        int i=0;
        while(std::getline(daata,_temp,'~')){
            std::stringstream __daata;
            __daata.str(_temp);
            __daata>>n[i++];
        }
        daata.clear();
        daata.str("");
        return boost_rand_real(n[0],n[1]);
    }else if(boost::regex_match(opt,__exact)){
        std::stringstream daata;
        float n;
        daata.str(boost::regex_replace(opt,boost::regex("(real:)(.*)",boost::regex::icase|boost::regex::perl)
                ,"$2"));
        daata>>n;
        daata.clear();
        daata.str();
        return n;
    }else if(boost::regex_match(opt,__ureal)){
        return boost_rand_real(0);
    }else {
        return boost_rand_real();
    }
}

template <>
boost::any options_generator<'c'>(string opt){
    const boost::regex __default("^(char)([^:#]{0,})$");
    const boost::regex __personal("char:[a-zA-Z]$");
    if (boost::regex_match(opt,__personal)){
        std::stringstream daata;
        char n;
        daata.str(boost::regex_replace(opt,boost::regex("(char:)(.*)",boost::regex::icase|boost::regex::perl)
                ,"$2"));
        daata>>n;
        daata.clear();
        daata.str();
        return n;
    }else {
        return boost_rand_char();
    }
}

template <>
boost::any options_generator<'s'>(string opt){
    const boost::regex __personal("str:.*");
    if (boost::regex_match(opt,__personal)){
        std::stringstream daata;
        string n;
        daata.str(boost::regex_replace(opt,boost::regex("(str:)(.*)",boost::regex::icase|boost::regex::perl)
                ,"$2"));
        daata>>n;
        daata.clear();
        daata.str();
        return n;
    }else {
        return string("STRING IS NOT FOUND");
    }
}

int main() {
    ifstream optionsTXT;
    optionsTXT.open("options.txt",READ_ONLY);
    if (!optionsTXT)
    {
        MessageBox(NULL,"无法确认C程序的输入设置","缺少options.txt",MB_OK+16);
        throw -1;
    }
    std::stringstream tempdata;
    tempdata<<optionsTXT.rdbuf();
    optionsTXT.close();


    ofstream inputTXT;
    inputTXT.open("input.txt",WRITABLE|CLEARFILE);


    boost::regex rgx("^(int|char|str|real).*$");
    string tempstr;
    while(std::getline(tempdata,tempstr,'\n')){
        if (!boost::regex_match(tempstr,rgx)) {
            tempstr.resize(0);
            continue;
        }
        if (boost::regex_match(tempstr,boost::regex("^(int).*$"))){
            int random_Cur=boost::any_cast<int>(options_generator<'i'>(tempstr));
            inputTXT<<std::to_string(random_Cur)<<endl;
        }else if (boost::regex_match(tempstr,boost::regex("^(char).*$"))){
            char random_Cur=boost::any_cast<char>(options_generator<'c'>(tempstr));
            inputTXT<<random_Cur<<endl;
        }else if (boost::regex_match(tempstr,boost::regex("^(str).*$"))){
            string random_Cur=boost::any_cast<string>(options_generator<'s'>(tempstr));
            inputTXT<<random_Cur<<endl;
        }else if (boost::regex_match(tempstr,boost::regex("^(real).*$"))){
            float random_Cur=boost::any_cast<float>(options_generator<'r'>(tempstr));
            inputTXT<<std::to_string(random_Cur)<<endl;
        }
    }
    tempdata.clear();
    tempdata.str("");

    inputTXT.close();

    return 0;
}