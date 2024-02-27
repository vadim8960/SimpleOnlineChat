//
// Created by vadim on 27.02.24.
//

#ifndef SIMPLEONLINECHAT_SESSION_HPP
#define SIMPLEONLINECHAT_SESSION_HPP

#include <boost/asio.hpp>

#include <optional>

namespace soc::network {

namespace io = boost::asio;
using tcp = io::ip::tcp;

class Session {
public:
	using OnMessageRecv = std::function<void(const tcp::endpoint&, std::string)>;
	using OnSessionClose = std::function<void(const tcp::endpoint&)>;

public:
	Session(io::io_context& io_context);

	void start();
	void sendMsg(std::string_view msg);

	const tcp::socket& getSocket() const;

private:
	void onAsyncReadComplete_(const boost::system::error_code& error, std::size_t bytes_transmitted);
	void onAsyncWriteComplete_(const boost::system::error_code& error, std::size_t bytes_transmitted);

private:
	tcp::socket socket_;

	tcp::endpoint client_endpoint_;

	io::streambuf streambuff_;

	OnMessageRecv on_message_recv_;
	OnSessionClose on_session_close_;
};

}

#endif //SIMPLEONLINECHAT_SESSION_HPP
