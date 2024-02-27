//
// Created by vadim on 27.02.24.
//

#include "Session.hpp"

#include <boost/bind.hpp>

namespace soc::network {

using namespace std::placeholders;

Session::Session(io::io_context& io_context) : socket_(io_context) {
	client_endpoint_ = socket_.remote_endpoint();
}

void Session::start() {
	io::async_read_until(socket_,
						 streambuff_,
						 '\n',
						 boost::bind(&Session::onAsyncReadComplete_,
									 this,
									 io::placeholders::error(),
									 io::placeholders::bytes_transferred()));
}

void Session::sendMsg(std::string_view msg) {
	std::iostream stream(&streambuff_);
	stream << msg;
	io::async_write(socket_,
					streambuff_,
					boost::bind(&Session::onAsyncWriteComplete_,
								this,
								io::placeholders::error(),
								io::placeholders::bytes_transferred()));
}

const tcp::socket& Session::getSocket() const {
	return socket_;
}

void Session::onAsyncReadComplete_(const boost::system::error_code& error, std::size_t bytes_transmitted) {
	if (!error) {
		if (bytes_transmitted == 0) {
			if (on_session_close_) {
				on_session_close_(client_endpoint_);
			}
		}
		auto buffptr = streambuff_.data();
		std::string msg{io::buffers_begin(buffptr), io::buffers_end(buffptr)};
		if (on_message_recv_) {
			on_message_recv_(client_endpoint_, msg);
		}
	}
	streambuff_.consume(bytes_transmitted);
}

void Session::onAsyncWriteComplete_(const boost::system::error_code& error, std::size_t bytes_transmitted) {
	if (!error) {
		//
	}
	streambuff_.consume(bytes_transmitted);
}

}