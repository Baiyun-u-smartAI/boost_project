//
// Created by pc002 on 2020/2/17.
//
#ifndef BOOST_ADAGRAD_CPP
#define BOOST_ADAGRAD_CPP
#include "adagrad.hpp"

boost::compute::double_* AdaGrad::Grad(boost::compute::double_* x0){
    // 直接传值给父类对象，现在父类成员dim代表的是Parameter维度，保证父类函数能正确调用成员函数
    PDF::dim=PDim;

    return PDF::Grad(x0);// 调用父类的Grad()函数
}

#define Yi datai.first
#define Xi datai.second
boost::compute::double_ AdaGrad::loss(boost::compute::double_ *_parameter) {
    boost::compute::double_ out = 0;
#ifdef THREAD4BOOST
    boost::thread_group threads;
#endif
    size_t data_index=0;
    for (auto datai:this->data) {
        //this->FuncModel(Xi,this->parameter): PredictY
#ifndef THREAD4BOOST
        boost::compute::double_ temp = Yi - this->FuncModel(Xi, _parameter);
        out += temp * temp;
#endif
#ifdef THREAD4BOOST
        if(data_index++%AdaGrad::threadsNum==0) threads.join_all();
            threads.create_thread(boost::bind(&(AdaGrad::updateLoss),this->FuncModel,&out,Xi,Yi,_parameter));
#endif
    }
#ifdef THREAD4BOOST
    threads.join_all();
#endif
    return out;
}

#undef Yi
#undef Xi

#ifdef THREAD4BOOST
void AdaGrad::updateLoss(boost::compute::double_ (*FuncModel)(boost::compute::double_*,boost::compute::double_*), boost::compute::double_* out,boost::compute::double_* _xi,boost::compute::double_ _yi,boost::compute::double_ *_parameter){
    boost::compute::double_ temp = _yi - FuncModel(_xi, _parameter);
    mutex.lock();
    *out += temp * temp;
    mutex.unlock();
}
boost::mutex AdaGrad::mutex;
size_t AdaGrad::threadsNum=4;
#endif

void AdaGrad::start(boost::compute::double_* p0,boost::compute::double_ learningspeed){

    if (this->data.empty()) throw "data cannot be empty";

    boost::compute::double_* offset = new boost::compute::double_[this->PDim];// Δp
    boost::compute::double_* r=new boost::compute::double_[PDim]{0};//累计平方密度
    boost::compute::double_ B4Learn,AftLearn;
    do {
        B4Learn = loss(p0);// 单步训练前损失函数
        boost::compute::double_* g=Grad(p0);
        for (int i = 0; i < PDim; ++i) {
            r[i] = r[i] + g[i] / data.size() * g[i] / data.size();
            offset[i] = -learningspeed / (this->Delta + sqrt(r[i])) * g[i];
            p0[i] = p0[i] + offset[i];
#ifdef DEBUG_ENABLE
            cout<<p0[i]<<" ";
#endif
        }
#ifdef DEBUG_ENABLE
        cout<<endl;
#endif
        AftLearn = loss(p0);// 单步训练后损失函数
    } while (abs(AftLearn - B4Learn) > 1e-10);// 是否损失函数收敛

    std::memcpy(this->parameter, p0, sizeof(boost::compute::double_) * this->PDim);// 保存结果

    delete[] offset;
    delete[] r;

}





#endif



