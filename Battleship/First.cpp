#include "../First/Game.h"

int main() {
	asio::error_code ec;
	asio::io_context context;
	std::mutex mutex;
	std::condition_variable cond;
	asio::ip::tcp::acceptor m_asioAcceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 60000));
	connection m_connection(context, asio::ip::tcp::socket(context));
	std::unique_ptr<Game> game;
	m_asioAcceptor.async_accept(
		[&](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec)
			{
				std::cout << "New Connection " << socket.remote_endpoint() << "\n";
				game = std::make_unique<Game>(std::make_unique<connection>(context, std::move(socket)));
				game->m_connection->read_msg();
				cond.notify_all();
			}
			else
			{
				std::cout << "Connection Error: " << ec.message() << "\n";
			}
		});
	std::thread thread = std::thread([&]() { context.run(); });
	while (!game) {
		std::unique_lock<std::mutex> lk{ mutex };
		cond.wait(lk);
	}
	game->start();
	game->m_connection->Disconnect();
	game->m_connection->m_asioContext.stop();
	if (thread.joinable())
		thread.join();
	game->m_connection.release();
	game.release();
	return 0;
}