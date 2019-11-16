#include "MongoDbWrapper.hpp"

using namespace reader;

MongoDbWrapper::MongoDbWrapper(const std::string& address,const unsigned short& port, const std::string& db_name, const std::string& user_name, const std::string& user_pwd) :
    m_client(mongocxx::uri(create_auth_uri(address,port,user_name,user_pwd)))
{
    m_db = m_client[db_name];
}
        
void MongoDbWrapper::insert_one(const std::string& col_name, const std::string& data)
{
    m_db[col_name].insert_one(bsoncxx::from_json(data));
}

void MongoDbWrapper::insert_many(const std::string& col_name, const std::vector<std::string>& data)
{
    std::vector< bsoncxx::document::value > documents;
    for (auto& i : data)
    {
        documents.push_back(bsoncxx::from_json(i));
    }
    m_db[col_name].insert_many(documents);
}

void MongoDbWrapper::update_one(const std::string& col_name, const std::string& data, const fp& filter)
{
     m_db[col_name].update_one(document{} << filter.first << filter.second << finalize, 
                            document{} << "$set" << bsoncxx::from_json(data) << finalize,
                            mongocxx::v_noabi::options::update().upsert(true));
}

void MongoDbWrapper::remove_one(const std::string& col_name, const std::string& data)
{
    m_db[col_name].delete_one(bsoncxx::from_json(data));
}

std::string MongoDbWrapper::create_auth_uri(const std::string& address,const unsigned short& port, const std::string& user_name, const std::string& user_pwd)
{
    std::string result = "mongodb://" + user_name + ":" + user_pwd + "@" + address + ":" + std::to_string(port) + "/admin";
    return result;
} 