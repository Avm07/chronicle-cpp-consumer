#include "Config.hpp"

using namespace reader;

Config::Config(const std::string &config_path, const std::string &config_file_name)
    : m_config_path(config_path), m_config_file_name(config_file_name)
{
    read_cfg_file();
}

unsigned short Config::get_server_port()
{
    return m_server_port;
}

unsigned short Config::get_ack_number()
{
    return m_ack_number;
}

bool Config::is_bin_mode()
{
    return m_bin_mode;
}

std::string Config::get_db_adress()
{
    return m_db_adress;
}

unsigned short Config::get_db_port()
{
    return m_db_port;
}

std::string Config::get_db_name()
{
    return m_db_name;
}

std::string Config::get_db_user_name()
{
    return m_db_user_name;
}

std::string Config::get_db_user_pwd()
{
    return m_db_user_pwd;
}

void Config::read_cfg_file()
{
    std::ifstream config_file(get_full_cfg_file_path());

    if (config_file.is_open())
    {
        json j_cfg;
        config_file >> j_cfg;

        m_server_port = j_cfg["server"]["port"].get<unsigned short>();
        m_ack_number = j_cfg["server"]["ack_number"].get<unsigned short>();
        m_bin_mode = j_cfg["server"]["bin_mode"].get<bool>();
        m_db_adress = j_cfg["database"]["address"].get<std::string>();
        m_db_port = j_cfg["database"]["port"].get<unsigned short>();
        m_db_name = j_cfg["database"]["db_name"].get<std::string>();
        m_db_user_name = j_cfg["database"]["user_name"].get<std::string>();
        m_db_user_pwd = j_cfg["database"]["user_pwd"].get<std::string>();
    }
    else
    {
        throw Exception("Failed to open config file " + get_full_cfg_file_path());
    }
}

std::string Config::get_full_cfg_file_path()
{
    if (m_config_path.back() != '/')
        m_config_path += '/';

    return m_config_path + m_config_file_name;
}