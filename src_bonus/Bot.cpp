#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <csignal>
#include "Bot.hpp"

#define BUFFER_SIZE 4096

void sendCommand(int sock, const std::string& command) {
    std::string message = command + "\r\n";
    send(sock, message.c_str(), message.length(), 0);
}

void sighandler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Caught SIGINT, exiting..." << std::endl;
        exit(0);
    }
}

void handlePing(const std::string& line, int server_fd) {
    if (line.find("PING") == 0) {
        std::string token = line.substr(5);
        std::string response = "PONG " + token + "\r\n";
        send(server_fd, response.c_str(), response.length(), 0);
        std::cout << "Sent: " << response << std::endl;
    }
}

void handlePrivmsg(const std::string& line, int server_fd) {
    size_t prefix_end = line.find(' ');
    if (prefix_end == std::string::npos) return;
    std::string prefix = line.substr(0, prefix_end);

    size_t cmd_start = prefix_end + 1;
    size_t cmd_end = line.find(' ', cmd_start);
    if (cmd_end == std::string::npos) return;

    size_t params_start = cmd_end + 1;
    size_t text_start = line.find(":", params_start);
    if (text_start == std::string::npos) return;

    std::string recipient = line.substr(params_start, text_start - params_start - 1);
    std::string text = line.substr(text_start + 1);
    text.erase(text.find_last_not_of("\r\n") + 1);

    if (text == "PING") {
        std::string sender_nick;
        if (prefix[0] == ':') {
            size_t nick_end = prefix.find('!');
            if (nick_end != std::string::npos)
                sender_nick = prefix.substr(1, nick_end - 1);
            else
                sender_nick = prefix.substr(1);
        }

        if (sender_nick != "BotGPT") {
            std::string response = "PRIVMSG " + recipient + " :PONG\r\n";
            send(server_fd, response.c_str(), response.length(), 0);
            std::cout << "Sent: " << response << std::endl;
        }
    }
}

void handleInvite(const std::string& line, int server_fd) {
    size_t channel_pos = line.find_last_of(':');
    if (channel_pos != std::string::npos) {
        std::string channel = line.substr(channel_pos + 1);
        channel.erase(channel.find_last_not_of("\r\n") + 1);
        std::string join_command = "JOIN " + channel + "\r\n";
        send(server_fd, join_command.c_str(), join_command.length(), 0);
        std::cout << "Sent: " << join_command << std::endl;
    }
}

void processMessageLine(const std::string& line, int server_fd) {
    if (line.compare(0, 4, "PING") == 0) {
        handlePing(line, server_fd);
    } else if (line.find("PRIVMSG") != std::string::npos) {
        handlePrivmsg(line, server_fd);
    } else if (line.find("INVITE") != std::string::npos) {
        handleInvite(line, server_fd);
    }
}

bool handleIncomingMessage(int server_fd) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    int valread = recv(server_fd, buffer, sizeof(buffer) - 1, 0);
    if (valread > 0) {
        std::string message(buffer, valread);
        std::cout << "Received: " << message << std::endl;
        size_t line_start = 0;
        while (line_start < message.length()) {
            size_t line_end = message.find("\r\n", line_start);
            if (line_end == std::string::npos) break;
            std::string line = message.substr(line_start, line_end - line_start);
            line_start = line_end + 2;
            processMessageLine(line, server_fd);
        }
        return true;
    } else if (valread == 0) {
        std::cout << "Server closed connection." << std::endl;
    } else {
        std::cerr << "Error reading from server." << std::endl;
    }
    return false;
}

bool connectToServer(int& sock, const char* server_ip, int port, const std::string& pass) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return false;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported" << std::endl;
        close(sock);
        return false;
    }

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(sock);
        exit(1);
        return false;
    }

    sendCommand(sock, "PASS " + pass);
    sendCommand(sock, "NICK BotGPT");
    sendCommand(sock, "USER BotGPTUser 0 * :BotGPT");

    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port> <pass>" << std::endl;
        return 1;
    }

    const char* server_ip = argv[1];
    int port = std::atoi(argv[2]);
    std::string pass = argv[3];

    signal(SIGINT, sighandler);
    int sock;
    while (true) {
        if (connectToServer(sock, server_ip, port, pass)) {
            while (handleIncomingMessage(sock));
            close(sock);
        }
    }
    return 0;
}
