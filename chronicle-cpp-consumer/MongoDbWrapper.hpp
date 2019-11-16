//------------------------------------------------------------------------------
// Description: Mongo database wrapper for driver
//------------------------------------------------------------------------------
#pragma once 
#include <string>
#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using fp = std::pair<std::string, std::string>;
 
namespace reader
{
    class MongoDbWrapper
    {
        public:
            MongoDbWrapper(const std::string& address,const unsigned short& port, const std::string& db_name,
                            const std::string& user_name, const std::string& user_pwd);

            void insert_one(const std::string& col_name, const std::string& data);
            void insert_many(const std::string& col_name, const std::vector<std::string>& data);

            void update_one(const std::string& col_name, const std::string& data, const fp& filter);
            void remove_one(const std::string& col_name, const std::string& data);

        private:
            std::string create_auth_uri(const std::string& address,const unsigned short& port,
                                        const std::string& user_name, const std::string& user_pwd);

            mongocxx::instance m_instance{};
            mongocxx::client m_client;
            mongocxx::database m_db;
    };
    typedef std::unique_ptr<MongoDbWrapper> PMongo;
}