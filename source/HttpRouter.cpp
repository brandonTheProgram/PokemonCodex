
#include "HttpRouter.h"

HttpRouter::HttpRouter(httplib::Server &server) : pokedex() {
    this->initializeRoutes(server);
}

void HttpRouter::initializeRoutes(httplib::Server &server)
{
    server.Get("/pokedex", [this](const httplib::Request &, httplib::Response &res) {
        Json::StreamWriterBuilder writer;
        std::string output = Json::writeString(writer, this->pokedex.getAllPokedexImages());
        res.set_content(output, "application/json");
    });

    server.Get("/", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("<html><body><h1>Welcome to the Pokédex!</h1></body></html>", "text/html");
    });
}