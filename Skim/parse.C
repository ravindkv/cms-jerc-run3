#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

int main() {
    try {
        // Create an empty property tree
        boost::property_tree::ptree pt;

        // Load the INI file into the property tree
        boost::property_tree::ini_parser::read_ini("config.ini", pt);

        // Access values in the property tree
        std::string server = pt.get<std::string>("Server.Address");
        int port = pt.get<int>("Server.Port");
        std::string username = pt.get<std::string>("User.Username");
        std::string password = pt.get<std::string>("User.Password");

        // Display the parsed values
        std::cout << "Server Address: " << server << std::endl;
        std::cout << "Server Port: " << port << std::endl;
        std::cout << "Username: " << username << std::endl;
        std::cout << "Password: " << password << std::endl;
    } catch (const boost::property_tree::ptree_error& e) {
        std::cerr << "Error while parsing the configuration file: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

