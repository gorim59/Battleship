#include "../First/Game.h"

int main() {;
	std::mutex mutex;
	std::condition_variable cond;
	asio::error_code ec;
	asio::io_context context;
	asio::ip::tcp::resolver resolver(context);
	asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", std::to_string(60000));
	std::unique_ptr<connection> conn;
	conn = std::make_unique<connection>(context, asio::ip::tcp::socket(context));
	conn->Connect(endpoints);
	std::thread thread = std::thread([&]() { context.run(); });
	while (!conn->connected) {
		std::unique_lock<std::mutex> lk{ conn->mutex };
		conn->cond.wait(lk);
	}
	std::unique_ptr<Game> game;
	game = std::make_unique<Game>(std::move(conn));
	game->join();
	game->m_connection->Disconnect();
	game->m_connection->m_asioContext.stop();
	if (thread.joinable()) thread.join();
	game->m_connection.release();
	game.release();
	return 0;
}