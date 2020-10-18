//
// Created by pc002 on 2020/10/17.
//

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <bits/stdc++.h>

using std::string;
int main(int argc, char const *argv[]) {
    boost::property_tree::ptree root;
    boost::property_tree::ptree items;
    boost::property_tree::read_json<boost::property_tree::ptree>("data.json",root);


    items=root.get_child("menu");
    for (auto it : items)
    {
//遍历读出数据
        string key=it.first;//key ID
//        string ID=it->second.get<string>("ID");
//        string Name=it->second.get<string>("Name");
        if(!items.get_child(key).data().empty())
        std::cout<<key<<":"<<items.get_child(key).data()<<std::endl;//输出
        else {
            std::cout<<items.get_child(key).begin()->first<<std::endl;
        }
    }




    auto menuitem=items.get_child("popup").get_child("menuitem");
    for (auto &item: menuitem) {
        //std::cout<<item.second.get_value<string>()<<std::endl;// 数组

        for (auto it : item.second){
            string key=it.first;
            std::cout<<key<<" : "<<item.second.get<string>(key)<<",  ";

        }
        std::cout<<"\b\b\b \b"<<std::endl;


    }

    //增加数组+新的值
    boost::property_tree::ptree write_item1,write_item2,write_item3;
    write_item2.put("","0");
    write_item3.put("","1");
    write_item1.push_back(std::make_pair("", write_item2));
    write_item1.push_back(std::make_pair("", write_item3));
    root.get_child("menu").push_back(std::make_pair("test",write_item1));

    write_item1.clear();

    //普通增加值
    write_item1.put("","anything");
    root.get_child("menu").push_back(std::make_pair("test2",write_item1));

    write_item2.clear();

    //插入值
    auto & _menu_item=root.get_child("menu").get_child("popup").get_child("menuitem");
    write_item2.put("value","9999");
    write_item2.put("onclick","6666");
    _menu_item.push_back(std::make_pair("", write_item2));


    write_json("test1.json", root);

    return 0;
}

