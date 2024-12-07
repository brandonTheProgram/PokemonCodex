#include <string>
#include "HttpRouter.h"

HttpRouter::HttpRouter(httplib::Server &server) : pokedex(), logger(Logger::getInstance()) {
    this->initializeRoutes(server);
}

void HttpRouter::initializeRoutes(httplib::Server &server)
{
    server.Get("/GetRegionNames", [this](const httplib::Request &req, httplib::Response &res) {
        Json::StreamWriterBuilder writer;

        std::string output = Json::writeString(writer, this->pokedex.getRegionNames());

        if(output.empty()) {
            logger.warning("HttpRouter::initializeRoutes Received no Pokemon information from the database");
        }

        res.set_content(output, "application/json");
    });

    server.Get(R"(/region/([a-zA-Z]+))", [this](const httplib::Request &req, httplib::Response &res) {
        Json::StreamWriterBuilder writer;
                
        std::string region = req.matches[1].str();

        if(region.empty()) {
            logger.warning("HttpRouter::initializeRoutes Received no region from the page");
        }
        else {
            logger.info("HttpRouter::initializeRoutes Received the region: " + region + " from the page");
        }

        std::string output = Json::writeString(writer, this->pokedex.getRegionData(region));

        if(output.empty()) {
            logger.warning("HttpRouter::initializeRoutes Received no Pokemon information from the database");
        }

        res.set_content(output, "application/json");
    });
}