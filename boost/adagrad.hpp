//
// Created by pc002 on 2020/2/15.
//

#ifndef BOOST_ADAGRAD_HPP
#define BOOST_ADAGRAD_HPP

#include "PDF.hpp"
#include <iostream>
#include <utility>
#include <cstring>
#include <vector>
#include <boost/thread.hpp>
#include <boost/compute.hpp>
using namespace std;




//boost::compute::double_ PDF::precision=1e-10;

// 测试偏导的函数


// AdaGrad

#define PDim dims.second
#define DDim dims.first
#define DataY boost::compute::double_
#define DataXGroup boost::compute::double_*
class AdaGrad:public PDF{
private:
#ifdef THREAD4BOOST
    static boost::mutex mutex;
    static size_t threadsNum;
#endif
    pair<size_t, size_t> dims;//<DataDim,ParameterDim>
    boost::compute::double_* parameter=NULL;

    boost::compute::double_* Grad(boost::compute::double_* x0);
public:
    boost::compute::double_ Delta=1e-7;// 维持数值稳定性而添加的常数

    //e.i. p[0]*data_x[0]+p[1]*data_x[1]+p[2] _1:DataX _2:Parameter return:DataY
    boost::compute::double_ (*FuncModel)(boost::compute::double_*,boost::compute::double_*) =NULL;// 模型函数

    vector<pair<DataY, DataXGroup> > data;// 用于学习的数据

    AdaGrad(boost::compute::double_ (*func)(boost::compute::double_ *dataX, boost::compute::double_ *parameter),
            size_t DataDim, size_t ParameterDim) : FuncModel(func),
            dims(pair<size_t, size_t>(DataDim, ParameterDim)) {
        /*this->FuncModel=func;
        this->dims=pair<size_t, size_t>(DataDim, ParameterDim);*/
        this->parameter = new boost::compute::double_[PDim];
    }

    // 导入训练数据
    void append_data(pair<DataY, DataXGroup> input_data){
        data.push_back(input_data);
    }

#undef DataY
#undef DataXGroup


#ifdef THREAD4BOOST
    static void updateLoss(boost::compute::double_ (*FuncModel)(boost::compute::double_*,boost::compute::double_*), boost::compute::double_* out,boost::compute::double_* _xi,boost::compute::double_ _yi,boost::compute::double_ *_parameter);
#endif

    boost::compute::double_ loss(boost::compute::double_ *_parameter) ;

    void start(boost::compute::double_* p0,boost::compute::double_ learningspeed=0.5);

    inline boost::compute::double_ GetValue(boost::compute::double_* p){
        return this->loss(p);
    }

    inline boost::compute::double_* ShowParameter()const{
        return this->parameter;
    }

};


#endif //BOOST_ADAGRAD_HPP
