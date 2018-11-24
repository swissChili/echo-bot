#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
namespace asio = boost::asio;

#include <discordpp/bot.hh>
#include <discordpp/rest-curlpp.hh>
#include <discordpp/websocket-websocketpp.hh>
#include <discordpp/framework.hh>


using json = nlohmann::json;
using aios_ptr = std::shared_ptr<asio::io_service>;

int main() {
    std::cout << "Starting bot...\n\n";
    std::string token = discordpp::tokenFromFile("token.dat");

    aios_ptr aios = std::make_shared<asio::io_service>();
    discordpp::Bot bot(
            aios,
            token,
            std::make_shared<discordpp::RestCurlPPModule>(aios, token),
            std::make_shared<discordpp::WebsocketWebsocketPPModule>(aios, token)
    );

    discordpp::Framework framework(&bot);

    framework.prompt = "--";
    // will reply to --hi with "Hello!"
    framework.setCommand("hi", [&](std::string, json msg){
        framework.messageReply(msg, "Hello!");
    });

    framework.setCommand("hola", [&](std::string, json msg){
        framework.messageReply(msg, "Hola!");
    });

    aios->run();
}
