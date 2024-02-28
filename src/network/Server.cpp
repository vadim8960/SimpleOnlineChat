//
// Created by vadim on 23.02.24.
//

#include "Server.hpp"

#include <boost/bind.hpp>

namespace soc::network {

Server::Server(std::uint16_t port) : endpoint_(tcp::v4(), port),
									 acceptor_(io_context_, endpoint_) {
	asyncAccept();
}

void Server::run() {
	io_context_.run();
}

Server::~Server() {

}

void Server::asyncAccept() {
	auto sess = std::make_shared<Session>(io_context_);

	acceptor_.async_accept(sess->getSocket(),
						   boost::bind(&Server::onAcceptSuccess,
									   this,
									   sess,
									   io::placeholders::error()));
}

void Server::onAcceptSuccess(std::shared_ptr<Session> session, const boost::system::error_code& error) {
	opened_sessions_.insert(session);
	session->start();
	asyncAccept();
}

}