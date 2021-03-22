#ifndef NETFRAMEWORK_SERVER
#define NETFRAMEWORK_SERVER

#include <cstdint> 
#include <deque>
#include <memory>
#include <asio.hpp>
#include <thread>
#include "net_message.h"
#include "net_connection.h"

namespace net {

	template<typename T>
	class server_interface {
	public:
		server_interface(uint16_t port)
			: _asio_acceptor{ _context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port) }
		{}

		virtual ~server_interface() {
			stop();
		}

		bool start() {
			try {
				wait_for_client_connection();
				
				// Launch asio context in it's own thread
				_thread_context = std::thread{ [this]() { _context.run(); } };
			}
			catch (std::exception& e) {
				// TODO: add log message
				return false;
			}

			// TODO: add success log
			return true
		}

		// Stop the server
		void stop() {
			_context.stop();

			if (_thread_context.joinable())
				_thread_context.join();
		}

		// Asynchronously wait for connection
		void wait_for_client_connection() {
			_asio_acceptor.async_accept(
				[this](std::error_code ec, asio::ip::tcp::socket socket) {
				if (!ec) {
					// Create a new connection
					std::shared_ptr<connection<T>> newconn =
						std::make_shared<connection<T>>(connection<T>::owner::server,
							_context, std::move(socket), _messages_in);

					// User may deny a connection
					if (on_client_connect(newconn)) {
						_connections.push_back(std::move(newconn));

						// Wait for data from client
						_connections.back()->connect_to_client(_id_counter++);

					}
					else {
						// TODO: log connection denied
					}
				}
				else {
					// TODO: log error connection error
				}

				// Wait for another connection
				wait_for_client_connection();
			});
		}

		// Send message to a client
		void message_client(std::shared_ptr<connection<T>> client, const message<T>& msg) {
			if (client && client->is_connected()) {
				client->send(msg);
			}
			else {
				// Call user defined disconnect handler
				on_client_disconnect(client);
				client.reset();
				
				_connections.erase(
					std::remove(_connections.begin(), _connections.end(), client), _connections.end()
				);
			}
		}

		// Force server to respond to a client
		void update(size_t max_messages = -1, bool wait = false) {
			if (wait)
				_messages_in.wait();

			size_t message_count = 0;
			while (message_count < max_messages) {
				owned_message<T> msg;
				if (_messages_in.try_pop(msg)) {
					on_message(msg.remote, msg.msg);

					message_count++;
				}
			}
		};

	protected:
		// Called when a client connects, user can drop a connection with this method
		virtual bool on_client_connect(std::shared_ptr<connection<T>> client) {
			return false;
		}

		// Called when a client disconnected
		virtual void on_client_disconnect(std::shared_ptr<connection<T>> client) {}
		
		// Called whem a message arrives
		virtual void on_message(std::shared_ptr<connection<T>> client, message<T>& msg) {}

		// Thread safe queue for incoming message packets
		tsqueue<owned_message<T>> _messages_in;

		// Container of active validated connections
		std::deque<std::shared_ptr<connection<T>>> _connections;

		asio::io_context _context;
		std::thread _thread_context;

		asio::ip::tcp::acceptor _asio_acceptor;

		// Client indentifier
		uint32_t _id_counter = 10000;
	};
}
#endif // NETFRAMEWORK_SERVER
