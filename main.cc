#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
namespace asio = boost::asio;

#include <discordpp/bot.hh>
#include <discordpp/rest-curlpp.hh>
#include <discordpp/websocket-websocketpp.hh>
#include <discordpp/framework.hh>

//#include <lib/nlohmannjson/src/json.hpp>
//#include <nlohmann/json.hpp>

std::string readTokenFile(std::string tokenFilePath);

using json = nlohmann::json;
using aios_ptr = std::shared_ptr<asio::io_service>;

int main() {
    std::cout << "Starting bot...\n\n";

    /*/
     * Read token from token file.
     * Tokens are required to communicate with Discord, and hardcoding tokens is a bad idea.
     * If your bot is open source, make sure it's ignore by git in your .gitignore file.
    /*/
    std::string token;
    if(boost::filesystem::exists("token.dat")){
        token = readTokenFile("token.dat");
    } else {
        std::cerr << "CRITICAL: There is no valid way for Discord++ to obtain a token! Copy the example login.dat or token.dat to make one.\n";
        exit(1);
    }

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
    framework.setCommand("hi", [&](std::string hi, json msg){
        framework.messageReply(msg, "Hello!");
    });

    bot.addHandler("PRESENCE_UPDATE", [](discordpp::Bot*, json) {
        // ignore
    });
    bot.addHandler("TYPING_START", [](discordpp::Bot*, json) {
        // ignore
    });

    aios->run();

    return 0;
}

std::string readTokenFile(std::string tokenFilePath){
    std::ifstream tokenFile;
    tokenFile.open(tokenFilePath);

    std::string token;

    if (tokenFile.is_open()) {
        std::getline(tokenFile, token);
    } else {
        std::cerr << "CRITICAL: There is no such file as " + tokenFilePath + "! Copy the example login.dat to make one.\n";
        exit(1);
    }
    tokenFile.close();
    std::cout << "Retrieved token.\n\n";

    return token;
}