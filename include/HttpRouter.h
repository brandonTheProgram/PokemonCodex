#ifndef HTTPROUTER_H_
#define HTTPROUTER_H_

#include <httplib.h>
#include "Pokedex.h"

class HttpRouter
{
public:
    HttpRouter(httplib::Server &server);
    ~HttpRouter() = default;

    void initializeRoutes(httplib::Server &server);

private:
    Pokedex pokedex;
    Logger& logger;
};

#endif