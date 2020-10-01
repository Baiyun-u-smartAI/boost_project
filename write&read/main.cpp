#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/regex.hpp>
#include <windows.h>

void StandardlizeEndLine(const char* fname){
    std::fstream testFile;
    testFile.open(fname,std::fstream::in | std::fstream::out);
    /*boost::filesystem::fstream boostfs(boost::filesystem::path("test.txt"));*/
    if (testFile.fail()){
        std::cout<<"缺少标准答案文件"<<std::endl;
        throw ;
    }

    std::stringstream buffer;
    buffer << testFile.rdbuf();
    testFile.close();
    std::string contents(buffer.str());
    if (boost::regex_match(contents,boost::regex("[\\r]"))){
        std::cout<<"get"<<std::endl;
    }
    contents = boost::regex_replace(contents,boost::regex("[\\r]"),std::string("\\n"));
    buffer.clear();
    buffer.str("");
    buffer.str(contents);
    testFile.open(fname, std::fstream::out | std::fstream::trunc);

    testFile << buffer.rdbuf();
    testFile.close();
}
std::string LastMatch(std::string src){
    std::string split;
    std::stringstream ssrc;
    ssrc.str(src);
    while (std::getline(ssrc,split,'%')){
        ;
    }
    ssrc.str("");
    std::string dst;
    dst.append("%").append(split);
    dst = boost::regex_replace(dst,boost::regex("%(-?[0-9](.[0-9])?){0,1}[a-z]{1,2}"),"");
    dst = boost::regex_replace(dst,boost::regex("\".*\\).*"),"");
    dst = boost::regex_replace(dst,boost::regex("\\\\n"),"\\\\\\\\n");
    return dst;
}
void StandardlizePrintf(const char* fname){
    std::fstream testFile;
    testFile.open(fname,std::fstream::in | std::fstream::out);
    /*boost::filesystem::fstream boostfs(boost::filesystem::path("test.txt"));*/
    if (testFile.fail()){
        std::cout<<"缺少标准答案文件"<<std::endl;
        throw -1;
    }
    std::stringstream buffer;
    buffer << testFile.rdbuf();
    testFile.close();
    testFile.open(fname, std::fstream::out| std::fstream::trunc);

    std::string linedata;
    bool isMain=true;
    bool isInclude = true;
    while(std::getline(buffer,linedata,'\n')){
        std::string temp;
        if (boost::regex_match(linedata,boost::regex("^[ \t]{0,}(//).*"))) continue;
        if (boost::regex_match(linedata,boost::regex("^[ \t]{0,}(/\\*).*"))) continue;
        if (isInclude&&boost::regex_match(linedata,boost::regex("#include.*"))){
            temp = boost::regex_replace(linedata,boost::regex("#include.*"),"$&\n#include \"correct.hpp\"");
            testFile << temp<<std::endl;
            isInclude = false;
            continue;
        }
        if (isMain&&boost::regex_match(linedata,boost::regex(".*main.*"))) {
            temp = boost::regex_replace(linedata,boost::regex("(int|void)[ ]{1,}main"),"int correct");
            testFile << temp<<std::endl;
            isMain = false;
            continue;
        }
        if (boost::regex_match(linedata,boost::regex("^[ ]{0,}printf.*[%].*"))) {

            // 处理 \n 或没有\n
            temp = boost::regex_replace(linedata,boost::regex("\\\\n"),"@\\\\n");
            temp = boost::regex_replace(temp,boost::regex("(^[ ]{0,})(printf[ ]{0,}\\([^0-9%]{0,})([^\"@]+)(\")",
                                                        boost::regex::icase|boost::regex::perl),"$1$2$3@$4");
            temp = boost::regex_replace(temp,boost::regex("(%(-?[0-9](.[0-9])?){0,1}(lf|d|f|p|E|e|s|c|lld))([^0-9%@]{0,}@)",
                                                        boost::regex::icase|boost::regex::perl),"$1@[^0-9]{0,}");

            // 处理%与%间
            temp = boost::regex_replace(temp,boost::regex("([ ]{0,})(%(-?[0-9](.[0-9])?){0,1}(lf|d|f|p|E|e|s|c|lld))([^0-9%@]+)",
                                                          boost::regex::icase|boost::regex::perl),"$2[^0-9]+");
            // 处理开头
            temp = boost::regex_replace(temp,boost::regex("(^[ ]{0,})(printf[ ]{0,}\\([^0-9%]{0,})(%(-?[0-9](.[0-9])?){0,1}(lf|d|f|p|E|e|s|c|lld))",
                                                          boost::regex::icase|boost::regex::perl),"$1printf(\"[^0-9]{0,}$3");

            // 修正处理 \n
            temp = boost::regex_replace(temp,boost::regex("@\\[\\^"),"[^");

            testFile << temp<<std::endl;

        }else if (!boost::regex_match(linedata,boost::regex(".*printf.*"))){
            testFile << linedata << std::endl;
        }else{
            testFile << "printf(\"\\n\");"<<std::endl;
        }

    }
    testFile.close();
}

int main(int argc, const char* argv[])
{
#if 1
    StandardlizeEndLine(argv[1]);
    StandardlizePrintf(argv[1]);

    std::ofstream headFile;
    headFile.open("correct.hpp",std::ios::out | std::fstream::trunc);
    headFile << "#ifndef CCHHH"<<std::endl;
    headFile << "#define CCHHH"<<std::endl;
    headFile << "int correct();"<<std::endl;
    headFile << "#endif"<<std::endl;
    headFile.close();

    std::ofstream correctCFile;
    correctCFile.open("correct.c",std::ios::out | std::fstream::trunc);
    correctCFile << "#include \"correct.hpp\""<<std::endl;
    correctCFile << "int main(){"<<std::endl;
    correctCFile << "      correct();"<<std::endl;
    correctCFile << "      return 0;"<<std::endl;
    correctCFile << "}"<<std::endl;
    correctCFile.close();

    std::ofstream batFile;
    batFile.open("generate.bat",std::ios::out|std::ios::trunc);


    std::string DllCommand("gcc ");
    DllCommand.append(argv[1]).append(" correct.hpp -shared -fPIC -o libcorrect.dll");

    /*system(DllCommand.c_str());*/
    batFile<<"@echo off"<<std::endl;
    batFile<<DllCommand<<std::endl;
    /*system("gcc correct.c correct.hpp libcorrect.dll -o correct.exe");*/
    batFile<<"gcc correct.c correct.hpp libcorrect.dll -o correct.exe"<<std::endl;
    std::string delCommand("del ");
    delCommand.append(argv[1]).append(" correct.c correct.hpp");
    /*system(delCommand.c_str());*/
    batFile<<delCommand<<std::endl;
    batFile<<"exit"<<std::endl;
    batFile.close();
    system("start /MIN generate.bat");

#else
    std::string src("        printf(\"%d \", arr[i]);");
    src = boost::regex_replace(src,boost::regex("\\\\n"),"@\\\\n");
    src = boost::regex_replace(src,boost::regex("(^[ ]{0,})(printf[ ]{0,}\\([^0-9%]{0,})([^\"@]+)(\")",
                                                      boost::regex::icase|boost::regex::perl),"$1$2$3@$4");
    src = boost::regex_replace(src,boost::regex("(%(-?[0-9](.[0-9])?){0,1}(lf|d|f|p|E|e|s|c|lld))([^0-9%@]{0,}@)",
                                                boost::regex::icase|boost::regex::perl),"$1@[^0-9]{0,}");
    std::cout<<src<<std::endl;

#endif
    return 0;
}