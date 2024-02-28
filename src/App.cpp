//
// Created by vadim on 22.02.24.
//

#include "App.hpp"

#include <boost/asio.hpp>

#include <optional>
#include <iostream>

namespace soc {

App::App(int argc, char **argv) : server_(2007) {

}

int App::process() {
	server_.run();
	return 0;
}

}