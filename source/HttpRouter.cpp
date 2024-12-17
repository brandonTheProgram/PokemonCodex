#include "HttpRouter.h"

#include <string>

HttpRouter::HttpRouter(httplib::Server &server) : pokedex(), logger(Logger::getInstance())
{
    this->initializeRoutes(server);
}

void HttpRouter::initializeRoutes(httplib::Server &server)
{
    server.Get("/getRegionData",
               [this](const httplib::Request &req, httplib::Response &res)
               {
                   Json::StreamWriterBuilder writer;

                   this->logger.debug("HttpRouter::initializeRoutes getRegionData envoked");

                   std::string output = Json::writeString(writer, this->pokedex.getRegionData());

                   if (output.empty())
                   {
                       logger.warning(
                           "HttpRouter::initializeRoutes Received no Pokemon information from the "
                           "database");
                   }

                   res.set_content(output, "application/json");
               });

    server.Get("/getLatestsPokemon",
               [this](const httplib::Request &req, httplib::Response &res)
               {
                   Json::StreamWriterBuilder writer;

                   this->logger.debug("HttpRouter::initializeRoutes getLatestsPokemon envoked");

                   std::string output =
                       Json::writeString(writer, this->pokedex.getLatestsPokemon());

                   if (output.empty())
                   {
                       logger.warning(
                           "HttpRouter::initializeRoutes Received no Pokemon information from the "
                           "database");
                   }

                   res.set_content(output, "application/json");
               });

    server.Get(
        R"(/region/([a-zA-Z]+))",
        [this](const httplib::Request &req, httplib::Response &res)
        {
            Json::StreamWriterBuilder writer;

            this->logger.debug("HttpRouter::initializeRoutes /region/ envoked");

            std::string region = req.matches[1].str();

            if (region.empty())
            {
                logger.warning("HttpRouter::initializeRoutes Received no region from the page");
            }
            else
            {
                logger.info("HttpRouter::initializeRoutes Received the region: " + region +
                            " from the page");
            }

            std::string output =
                Json::writeString(writer, this->pokedex.getRegionPokemonData(region));

            if (output.empty())
            {
                logger.warning(
                    "HttpRouter::initializeRoutes Received no Pokemon information from the "
                    "database");
            }

            res.set_content(output, "application/json");
        });

    server.Get(
        R"(/getStarting/([a-zA-Z]+))",
        [this](const httplib::Request &req, httplib::Response &res)
        {
            Json::StreamWriterBuilder writer;

            this->logger.debug("HttpRouter::initializeRoutes getStarting envoked");

            std::string region = req.matches[1].str();

            if (region.empty())
            {
                logger.warning("HttpRouter::initializeRoutes Received no region from the page");
            }
            else
            {
                logger.info("HttpRouter::initializeRoutes Received the region: " + region +
                            " from the page");
            }

            std::string output =
                Json::writeString(writer, this->pokedex.getRegionPokemonData(region, true));

            if (output.empty())
            {
                logger.warning(
                    "HttpRouter::initializeRoutes Received no Pokemon information from the "
                    "database");
            }

            res.set_content(output, "application/json");
        });
}