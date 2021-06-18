#pragma once
#include "../First/common.h"


class connection : public std::enable_shared_from_this<connection>
{
protected:
	asio::ip::tcp::socket m_socket;
	message msg_in;
	message msg_out;
public:
	asio::io_context& m_asioContext;
	std::mutex mutex;
	std::condition_variable cond;
	bool msg_got;
	bool msg_send;
	bool connected;
	connection(asio::io_context& asioContext, asio::ip::tcp::socket socket)
		: m_asioContext(asioContext), m_socket(std::move(socket)), connected(false), msg_got(false), msg_send(true) { }

	void Connect(const asio::ip::tcp::resolver::results_type& endpoints) {
		asio::async_connect(m_socket, endpoints,
			[this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
			{
				if (!ec)
				{
					std::cout << "Succesful connect\n";
					connected = true;
					cond.notify_all();
					read_msg();
				}
				else {
					std::cout << "unsuccesful connect\n";
				}
			});
	}

	~connection() {
		Disconnect();
	}
	void Disconnect()
	{
		if (IsConnected())
			asio::post(m_asioContext, [this]() { m_socket.close(); });
	}
	bool IsConnected() const
	{
		return m_socket.is_open();
	}

	void read_msg()
	{
		//std::cout << "try read\n";
		while (msg_got) {
			std::unique_lock<std::mutex> lk{ mutex };
			cond.wait(lk);
		}
		//std::cout << "read\n";
		asio::async_read(m_socket, asio::buffer(&msg_in.header, sizeof(header)),
			[this](std::error_code ec, std::size_t length) {
				//std::cout << "something\n";
				if (!ec) {
					//std::cout << "Read header succes.\n";
					//std::cout << length <<" "<< sizeof(header) << "\n";
					//std::cout << msg_in;
					msg_in.body.resize(msg_in.header.size);
					//std::cout << msg_in.header.size << msg_in.size();
					asio::async_read(m_socket, asio::buffer(msg_in.body.data(), msg_in.size()),
						[this](std::error_code ec, std::size_t length) {
							if (!ec) {
								//std::cout << "Read body succes.\n";
							}
							else
							{
								std::cout << "Read body fail." << ec << "\n";
								m_socket.close();
							}
							{
								std::lock_guard<std::mutex> lk{ mutex };
								msg_got = true;
							}
							cond.notify_all();
							read_msg();
						});
				}
				else
				{
					std::cout << "Read header fail." << ec << "\n";
					m_socket.close();
				}
			});
		//std::cout << "read1\n";
	}

	void write_msg(message new_msg)
	{
		set_msg(new_msg);
		//std::cout << "try write\n";
		while (msg_send) {
			std::unique_lock<std::mutex> lk{ mutex };
			cond.wait(lk);
		}
		//std::cout << "write\n";
		//std::cout << sizeof(header) << "\n";
		//std::cout << msg_out.body.data() << "\n";
		//std::cout << msg_out << "\n";
		asio::async_write(m_socket, asio::buffer(&msg_out.header, sizeof(header)),
			[this](std::error_code ec, std::size_t length)
			{
				if (!ec) {
					//std::cout << "Write head succes.\n";
					//std::cout << length << " " << sizeof(header) << "\n";
					//std::cout << msg_out << "\n";
					asio::async_write(m_socket, asio::buffer(msg_out.body.data(), msg_out.size()),
						[this](std::error_code ec, std::size_t length)
						{
							if (!ec) {
								//std::cout << "Write body succes.\n";
							}
							else
							{
								std::cout << "Write body fail." << ec << "\n";
								m_socket.close();
							}
							{
								std::lock_guard<std::mutex> lk{ mutex };
								msg_send = true;
							}
							cond.notify_all();
						});
				}
				else
				{
					std::cout << "Write head fail."<< ec <<"\n";
					m_socket.close();
				}
			});
	}

	void set_msg(message new_msg) {
		//std::cout << "try set\n";
		while (!msg_send) {
			std::unique_lock<std::mutex> lk{ mutex };
			cond.wait(lk);
		}
		msg_send = false;
		cond.notify_all();
		msg_out = new_msg;
		//std::cout << "set\n";
	}
	message get_msg() {
		//std::cout << "try get\n";
		while (!msg_got) {
			std::unique_lock<std::mutex> lk{ mutex };
			cond.wait(lk);
		}
		msg_got = false;
		cond.notify_all();
		//std::cout << "get\n";
		return msg_in;
	}
};