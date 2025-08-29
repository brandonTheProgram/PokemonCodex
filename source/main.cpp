#include <httplib.h>

#include "Config.h"
#include "HttpRouter.h"

int main()
{
    try
    {
        Config::getInstance().load("config.json");

        Logger::getInstance().info("Starting the application");

        httplib::Server server;
        HttpRouter router(server);  // Initialize routes

        server.listen("localhost", 8080);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}