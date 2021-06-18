#include <common.h>

int main() {
	std::cout << "hello\n";
	// asio::error_code ec;
	// asio::io_context ic;
	// asio::io_context::work idleWork(ic);
	// std::thread tic = std::thread([&]() {ic.run(); });
	// asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);
	// asio::ip::tcp::socket socket(ic);
	// socket.connect(endpoint, ec);
	// std::cout << ec.message();
	message msg;
	msg << 5;
	msg << 6;
	std::cout << msg;
	int a;
	int b;
	msg >> a;
	msg >> b;
	std::cout << a << " next " << b;
	return 0;
}