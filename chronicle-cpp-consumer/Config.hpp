//------------------------------------------------------------------------------
// Description: Config file reader for server and db
//------------------------------------------------------------------------------
#pragma once 
#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp> 
#include "Exception.hpp"    

using json = nlohmann::json;

namespace reader
{
    class Config
    {
    public:
        Config(const std::string &config_path, const std::string &config_file_name);
        
        unsigned short get_server_port();
        unsigned short get_ack_number();
        bool is_bin_mode();

        std::string get_db_adress();
        unsigned short get_db_port();
        std::string get_db_name();
        std::string get_db_user_name();
        std::string get_db_user_pwd();

    private:
        void read_cfg_file();
        std::string get_full_cfg_file_path();

    private:
        std::string m_config_path;
        std::string m_config_file_name;

        unsigned short m_server_port;
        unsigned short m_ack_number;
        bool m_bin_mode;
        
        std::string m_db_adress;
        unsigned short m_db_port;
        std::string m_db_name;
        std::string m_db_user_name;
        std::string m_db_user_pwd;
    };
    typedef std::unique_ptr<Config> PConfig;
}