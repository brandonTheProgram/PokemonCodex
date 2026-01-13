#ifndef HTTPROUTER_H_
#define HTTPROUTER_H_

#include <httplib.h>

#include "Pokedex.h"

/**
 * @class HttpRouter
 * @brief Manage HTTP routes for the webpages.
 *
 * This class manages the HTTP routes for the webpages.
 */
class HttpRouter
{
    public:
        /**
         * @brief Constructs a new HttpRouter object.
         * @param server The server instance from HTTP Library.
         */
        HttpRouter(httplib::Server &server);

        /**
         * @brief Destroys the HttpRouter object.
         */
        ~HttpRouter() = default;

        /**
         * @brief Initialize the routes for the webpages.
         * @param server The server instance from HTTP Library.
         */
        void initializeRoutes(httplib::Server &server);

    private:
        Pokedex pokedex_;
        Logger &logger_;
};

#endif