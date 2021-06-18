#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <queue>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


enum class MsgTypes : uint32_t
{
	Start,
	Fire,
	Resp,
	Surr,
	None,
};

struct header
{
	MsgTypes id;
	uint32_t size = 0;
};
struct message
{
	header header{};
	std::vector<uint8_t> body;
	message() {
		header.id = MsgTypes::None;
		header.size = 0;
	}
	message(const message& obj) {
		header.id = obj.header.id;
		header.size = obj.header.size;
		body = obj.body;
	}
	size_t size() const
	{
		return body.size();
	}
	friend std::ostream& operator << (std::ostream& os, const message& msg)
	{
		os << "Type:" << int(msg.header.id) << " Size:" << msg.header.size << "\n";
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
