#ifndef NETFRAMEWORK_NETCONNECTION
#define NETFRAMEWORK_NETCONNECTION
#include <memory>
#include <cstdint>
#include <asio.hpp>
#include "net_message.h"
#include "net_tsqueue.h"

namespace net {

	template <typename T>
	class connection : public std::enable_shared_from_this<connection<T>>{
	public:
		// Connection can be owned by client or server
		enum class owner {
			server,
			client
		};

		connection(owner parent, asio::io_context& asio_context, asio::ip::tcp::socket socket, 
			tsqueue<owned_message<T>>& in) : _asio_context{ asio_context }, _socket{ socket },  
			_messages_in{in} {
			_owner_type = parent;
		}
		virtual ~connection() {}

		uint32_t get_id() const {
			return id;
		}

		void connect_to_client(uint32_t uid = 0) {
			if (owner_type == owner::server) {
				if (_socket.is_open()) {
					id = uid;
					read_header();
				}
			}
		}

		void connect_to_server(const asio::ip::tcp::resolver::results_type& endpoints) {
			// Only client can connect to the server
			if (_owner_type == owner::client) {
				// Request to connect to an endpoint
				asio::async_connect(_socket, endpoints,
					[this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
					if (!ec) {
						read_header();
					}
				});
			}
		}

		void disconnect() {
			if (is_connected()) {
				asio::post(_asio_context, [this]() { _socket.close(); })
			}
		}

		bool is_connected() const {
			return _socket.is_open();
		}

		// Asynchronously send a message
		void send(const message<T>& msg) {
			asio::post(_asio_context,
				[this, msg]() {
				// If the queue has a message then we assume that is being assynchronously written,
				// and we don't call "write_header" method to do not interrupt a proccess
				bool is_writing = !_messages_out.empty();
				_messages_out.push(msg);

				if (!is_writing) {
					write_header();
				}
			});
		}

	private:
		// Asycnhronously write a message header
		void write_header() {
			asio::async_write(_socket, asio::buffer(_messages_out.front().header, sizeof(message_header<T>)),
				[this](std::error_code ec, std::size_t length) {
					if (!ec) {
						if (_messages_out.front().header.size > 0) {
							write_body();
						}
						else {
							_messages_out.pop();

							// Send  the next message if the out queue is not empty
							if (!_messages_out.empty()) {
								write_header();
							}
						}
					}
					else {
						// TODO: logger error
						_socket.close();
					}
			})
		}

		// Asynchronously write a message body
		void write_body() {
			// Fill a transmission buffer with the body data and send it
			asio::async_write(_socket, asio::buffer(_messages_out.front().body.data(), _messages_out.front().body.size()),
				[this](std::error_code ec, std::size_t length) {
					if (!ec) {
						// Sending was successful, so we can delete message from queue
						_messages_out.pop();

						// If the queue has a messages try to send the next one
						if (!_messages_out.empty()) {
							write_header();
						}
					}
					else {
						// TODO: log error
						_socket.close();
					}
			});
		}

		// Asychronously read a header of a message
		void read_header() {
			asio::async_read(_socket, asio::buffer(_msg_temporay_in.header, sizeof(message_header<T>)),
				[this](std::error_code, std::size_t length) {
					if (!ec) {
						if (_msg_temporary_in.header.size > 0) {
							// Realocate a body and then try to save a message to the body
							_msg_temporary_in.body.resize(_msg_temporary_in.header.size);
							read_body();
						}
						else {
							// Body is empty, so add a message with the empty body
							add_incoming_message_queue();
						}
					}
					else {
						// TODO: Log error
						_socket.close();
					}
			});
		}

		// Asynchronously read a body of a message
		void read_body() {
			asio::async_read(_socket, asio::buffer(_msg_temporary_in.body.data(), _msg_temporary_in.body.size()),
				[this](std::error_code ec, std::size_t length) {
				if (!ec) {
					add_incoming_message_queue();
				}
				else {
					// TODO: log error out
					_socket.close();
				}
			}
			)
		}

		// When a full message is recieved, add it to the incoming queue
		void add_incoming_message_queue() {
			// Put message in the queue, creating a "owned message" object,
			// which holds a pointer to this connection
			if (_owner_type == owner::server)
				_messages_in.push({ this->shared_from_this(), _msg_temporary_in });
			else
				_messages_in.push({ nullptr, _msg_temporary_in });

			// Recieve the next message if connection is still established
			read_header();
		}

	protected:
		// Each connection has a unique socket
		asio::ip::tcp::socket _socket;
		
		// Context is shared with the whole instance 
		asio::io_context& _asio_context;

		// Holds all messages to be sent
		tsqueue<message<T>> _messages_out;

		// Reference to the incoming queue
		tsqueue<owned_message<T>>& _messages_in;

		// Temporary variable to save part of mesage until it is ready
		message<T> _msg_temporary_in;

		// Indicates what owns the connection
		owner _owner_type = owner::server;

		uint32_t id = 0;
	};
}

#endif // NETFRAMEWORK_NETCONNECTION
