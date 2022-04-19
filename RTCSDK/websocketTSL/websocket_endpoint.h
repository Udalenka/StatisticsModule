/**
 * This file is part of mediasoup_client project.
 * Author:    Jackie Ou
 * Created:   2021-11-01
 **/

#pragma once

#include "connection_metadata.h"
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>
#include <string>
#include <vector>

namespace vi {
	class WebsocketEndpoint {
	public:
		WebsocketEndpoint();

		~WebsocketEndpoint();

        int connect(std::string const& uri, std::shared_ptr<IConnectionObserver> observer, const std::string& subprotocol = "");

		void close(int id, websocketpp::close::status::value code, const std::string& reason);

		void sendText(int id, const std::string& data);

		void sendBinary(int id, const std::vector<uint8_t>& data);

		void sendPing(int id, const std::string& data);

		void sendPong(int id, const std::string& data);

		ConnectionMetadata::ptr getMetadata(int id) const;

	private:
		typedef std::map<int, ConnectionMetadata::ptr> ConnectionList;

        Client _endpoint;
		websocketpp::lib::shared_ptr<websocketpp::lib::thread> _thread;

		ConnectionList _connectionList;
		int _nextId;
	};
}

