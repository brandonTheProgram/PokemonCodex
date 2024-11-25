#include <httplib.h>
#include "HttpRouter.h"

int main()
{
    Logger::getInstance().info("Starting the application");
    
    httplib::Server server;
    HttpRouter router(server); // Initialize routes

    server.listen("localhost", 8080);
    return 0;
}