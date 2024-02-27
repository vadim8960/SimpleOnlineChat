//
// Created by vadim on 23.02.24.
//

#include "Server.hpp"

namespace soc::network {

Server::Server(std::uint16_t port) : acceptor_(io_) {

}

void Server::run() {
	io_context_.run();
}

}