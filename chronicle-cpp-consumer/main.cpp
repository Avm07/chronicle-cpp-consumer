#include "TCPSyncServer.hpp"
#include "Exception.hpp"

using namespace reader;

int main(const int argc, const char *argv[])
{
  try
  {
    if(argc != 3)
    {
      throw Exception("Missing config file, provide config path and config file name");
    }

    TCPSyncServer server(argv[1], argv[2]);
    server.do_accept();
  }
  catch (std::exception& e)
  {
    BOOST_LOG_TRIVIAL(fatal) << e.what() << std::endl;
  }
  catch (Exception& e)
  {
    BOOST_LOG_TRIVIAL(fatal) << e.what() << std::endl;
  }
  catch (...)
  {
    BOOST_LOG_TRIVIAL(fatal) << "Fatal exception" << std::endl;
  }
  return 0;
}