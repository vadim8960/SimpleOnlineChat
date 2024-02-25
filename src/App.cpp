//
// Created by vadim on 22.02.24.
//

#include "App.hpp"

#include <boost/asio.hpp>

#include <optional>
#include <iostream>

namespace soc {

namespace io = boost::asio;

using io_context = io::io_context;
using tcp = io::ip::tcp;

class session : public std::enable_shared_from_this<session> {
public:
	session(tcp::socket&& socket) : socket_(std::move(socket)) {}

	void async_read() {
		io::async_read_until(socket_, streambuff_in_, '\n',
							 [self = shared_from_this()](const boost::system::error_code& err,
														 std::size_t bytes_transferred) {
							   if (bytes_transferred) {
								   io::streambuf::const_buffers_type tmp = self->streambuff_in_.data();
								   std::string to{io::buffers_begin(tmp),
												  io::buffers_begin(tmp) + bytes_transferred};
								   self->async_write(to);
								   self->streambuff_in_.consume(bytes_transferred);
							   }
							 });
	}

	void async_write(const std::string& data) {
		std::iostream out(&streambuff_out_);
		out << socket_.remote_endpoint() << " says: " << data;
		io::async_write(socket_, streambuff_out_,
						[self = shared_from_this()](const boost::system::error_code& err,
													std::size_t bytes_transferred) {
						  self->streambuff_out_.consume(bytes_transferred);
						  self->async_read();
						});
	}

private:
	tcp::socket socket_;
	io::streambuf streambuff_in_;
	io::streambuf streambuff_out_;
};

class server {
public:
	server(io_context& io, std::uint16_t port) : io_context_(io),
												 endpoint_(tcp::v4(), port),
												 acceptor_(io_context_, endpoint_) {}

	void async_accept() {
		socket_.emplace(io_context_);

		acceptor_.async_accept(*socket_, [&](boost::system::error_code error) {
		  std::make_shared<session>(std::move(*socket_))->async_read();
		  async_accept();
		});
	}

private:
	io_context& io_context_;
	tcp::endpoint endpoint_;
	tcp::acceptor acceptor_;
	std::optional<tcp::socket> socket_;
};

App::App(int argc, char **argv) {

}

int App::process() {
	io_context io;
	server srv(io, 2007);
	srv.async_accept();
	io.run();
	return 0;
}

}