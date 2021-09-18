#include <boost/program_options.hpp>
#include <iostream>


using namespace std;

void on_age(int age) {
    cout << "On Age " << age << "\n";
}

int main(int argc, char * argv[]) {
    try {
        boost::program_options::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Help screen")
                ("pi", boost::program_options::value<float>()->default_value(3.14f)->value_name("constant"), "Pi")
                ("age", boost::program_options::value<int>()/*->notifier(on_age)*/, "Age");
        boost::program_options::variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);
        if (vm.count("help")) {
            cout << desc << endl;
        } else if (vm.count("age")) {
            cout << "Age is :" << vm["age"].as<int>() << endl;
        } else if (vm.count("pi")) {
            cout << "Pi:" << vm["pi"].as<float>() << endl;
        }
        else {
            std::cerr << "arguments error, please press [-h] for help"<< std::endl;
        }
    } catch (const boost::program_options::error &ex) {
        std::cerr << "catch an error"<<std::endl;
        std::cerr << ex.what() << "\n";
    }

}
