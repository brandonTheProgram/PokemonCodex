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
                       res.status = 404;  // Not Found
                       logger.warning(
                           "HttpRouter::initializeRoutes Received no region information from the "
                           "database");
                       res.set_content("{\"error\": \"Region Data not found\"}",
                                       "application/json");
                       return;
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
                       res.status = 404;  // Not Found
                       logger.warning(
                           "HttpRouter::initializeRoutes Received no Pokemon information from the "
                           "database");
                       res.set_content("{\"error\": \"Pokemon not found\"}", "application/json");
                       return;
                   }

                   res.set_content(output, "application/json");
               });

    server.Get(
        R"(/region/([a-zA-Z]+))",
        [this](const httplib::Request &req, httplib::Response &res)
        {
            Json::StreamWriterBuilder writer;
            std::string output = "";

            this->logger.debug("HttpRouter::initializeRoutes /region/ envoked");

            std::string region = req.matches[1].str();

            if (region.empty())
            {
                res.status = 404;  // Not Found
                logger.warning("HttpRouter::initializeRoutes Received no region from the page");
                res.set_content("{\"error\": \"No region received\"}", "application/json");
                return;
            }
            else
            {
                logger.info("HttpRouter::initializeRoutes Received the region: " + region +
                            " from the page");

                output = Json::writeString(writer, this->pokedex.getRegionPokemonData(region));

                if (output.empty())
                {
                    res.status = 404;  // Not Found
                    logger.warning(
                        "HttpRouter::initializeRoutes Received no Pokemon information from the "
                        "database");
                    res.set_content("{\"error\": \"Pokemon not found\"}", "application/json");
                    return;
                }
            }

            res.set_content(output, "application/json");
        });

    server.Get(
        R"(/getStarting/([a-zA-Z]+))",
        [this](const httplib::Request &req, httplib::Response &res)
        {
            Json::StreamWriterBuilder writer;
            std::string output = "";

            this->logger.debug("HttpRouter::initializeRoutes getStarting envoked");

            std::string region = req.matches[1].str();

            if (region.empty())
            {
                res.status = 404;  // Not Found
                logger.warning("HttpRouter::initializeRoutes Received no region from the page");
                res.set_content("{\"error\": \"No region received\"}", "application/json");
                return;
            }
            else
            {
                logger.info("HttpRouter::initializeRoutes Received the region: " + region +
                            " from the page");

                output =
                    Json::writeString(writer, this->pokedex.getRegionPokemonData(region, true));

                if (output.empty())
                {
                    res.status = 404;  // Not Found
                    logger.warning(
                        "HttpRouter::initializeRoutes Received no Pokemon information from the "
                        "database");
                    res.set_content("{\"error\": \"Pokemon not found\"}", "application/json");
                    return;
                }
            }

            res.set_content(output, "application/json");
        });

    server.Get(
        R"(/pokemon/(\d+)(?:/(\d+))?)",
        [this](const httplib::Request &req, httplib::Response &res)
        {
            Json::StreamWriterBuilder writer;
            std::string output;

            this->logger.debug("HttpRouter::initializeRoutes /pokemon/ envoked");

            std::string pokedexNumber  = req.matches[1].str();
            std::string regionalFormId = req.matches.size() > 2 ? req.matches[2].str() : "";

            if (pokedexNumber.empty())
            {
                res.status = 404;  // Not Found
                logger.warning(
                    "HttpRouter::initializeRoutes Received no pokdex number from the page");
                res.set_content("{\"error\": \"No pokdex number received\"}", "application/json");
                return;
            }
            else
            {
                logger.info("HttpRouter::initializeRoutes Received the Pokemon: " + pokedexNumber +
                            " from the page");

                output = Json::writeString(
                    writer, this->pokedex.getPokemonData(pokedexNumber, regionalFormId));

                if (output.empty())
                {
                    res.status = 404;  // Not Found
                    logger.warning(
                        "HttpRouter::initializeRoutes Received no Pokemon information from the "
                        "database");
                    res.set_content("{\"error\": \"Pokemon not found\"}", "application/json");
                    return;
                }
            }

            res.set_content(output, "application/json");
        });

    server.Get("/getPokemonTypes",
               [this](const httplib::Request &req, httplib::Response &res)
               {
                   Json::StreamWriterBuilder writer;

                   this->logger.debug("HttpRouter::initializeRoutes getPokemonTypes envoked");

                   std::string output = Json::writeString(writer, this->pokedex.getPokemonTypes());

                   if (output.empty())
                   {
                       res.status = 404;  // Not Found
                       logger.warning(
                           "HttpRouter::initializeRoutes Received no Pokemon information from the "
                           "database");
                       res.set_content("{\"error\": \"Pokemon types not found\"}",
                                       "application/json");
                       return;
                   }

                   res.set_content(output, "application/json");
               });

    server.Get(
        R"(/search/([a-zA-Z]+))",
        [this](const httplib::Request &req, httplib::Response &res)
        {
            Json::StreamWriterBuilder writer;
            std::string output = "";

            this->logger.debug("HttpRouter::initializeRoutes search envoked");

            std::string searchTerm = req.matches[1].str();

            if (searchTerm.empty())
            {
                res.status = 404;  // Not Found
                logger.warning(
                    "HttpRouter::initializeRoutes Received no search term from the page");
                res.set_content("{\"error\": \"No search term received\"}", "application/json");
                return;
            }
            else
            {
                logger.info("HttpRouter::initializeRoutes Received the search term: " + searchTerm +
                            " from the page");

                output = Json::writeString(writer, this->pokedex.getSearchPokemon(searchTerm));
            }

            res.set_content(output, "application/json");
        });
}