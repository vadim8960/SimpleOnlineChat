//
// Created by vadim on 23.02.24.
//

#ifndef SIMPLEONLINECHAT_SERVER_HPP
#define SIMPLEONLINECHAT_SERVER_HPP

#include <boost/asio.hpp>

#include <unordered_set>
#include <optional>

#include "Session.hpp"

namespace soc::network {

class Server {
public:
	Server(std::uint16_t port);

	void run();

	~Server();

private:
	void asyncAccept();
	void onAcceptSuccess(std::shared_ptr<Session> session, const boost::system::error_code& error);

private:
	std::unordered_set<std::shared_ptr<Session>> opened_sessions_;

	io::io_context io_context_;
	tcp::endpoint endpoint_;
	tcp::acceptor acceptor_;
};

}

#endif //SIMPLEONLINECHAT_SERVER_HPP
