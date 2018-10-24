#include "pmgr.h"

int main(int argc, char *argv[])
{
    try
    {
        int port = 7900;
        if (argc != 2)
        {
            std::cerr << "Usage: us <port>\n";
            std::cerr << "not specify port number, use default port: 7900\n";
        } 
        else 
        {
            port = std::atoi(argv[1]);
        }        
        boost::asio::io_context io_context;
        PMgr s(io_context, port);
        std::cout<< "us listening on: " << port << std::endl;
        io_context.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
