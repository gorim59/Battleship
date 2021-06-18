#pragma once
#include <iostream>
#include <asio.hpp>
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

enum class MsgTypes : uint32_t
{
	Start,
	Pick,
	Res,
	Surr,
};

struct message_header
{
	MsgTypes id;
	uint32_t size = 0;
};
struct message
{
	message_header header{};
	std::vector<uint8_t> body;
	size_t size() const
	{
		return body.size();
	}
	friend std::ostream& operator << (std::ostream& os, const message& msg)
	{
		os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
		return os;
	}
	template<typename DataType>
	friend message& operator << (message& msg, const DataType& data)
	{
		size_t i = msg.body.size();
		msg.body.resize(msg.body.size() + sizeof(DataType));
		std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
		msg.header.size = msg.size();
		return msg;
	}
	template<typename DataType>
	friend message& operator >> (message& msg, DataType& data)
	{
		size_t i = msg.body.size() - sizeof(DataType);
		std::memcpy(&data, msg.body.data() + i, sizeof(DataType));
		msg.body.resize(i);
		msg.header.size = msg.size();
		return msg;
	}
};
class connection;

struct owned_message
{
	std::shared_ptr<connection> remote = nullptr;
	message msg;

	friend std::ostream& operator<<(std::ostream& os, const owned_message& msg)
	{
		os << msg.msg;
		return os;
	}
};