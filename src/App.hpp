//
// Created by vadim on 22.02.24.
//

#ifndef SIMPLEONLINECHAT_APP_HPP
#define SIMPLEONLINECHAT_APP_HPP

#include "network/Server.hpp"

namespace soc {

class App {
private:

public:
	App(int argc, char **argv);
	int process();

private:
	network::Server server_;
};

}

#endif //SIMPLEONLINECHAT_APP_HPP
