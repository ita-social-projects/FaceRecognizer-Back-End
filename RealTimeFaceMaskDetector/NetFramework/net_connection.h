#ifndef NETFRAMEWORK_NETCONNECTION
#define NETFRAMEWORK_NETCONNECTION
#include <memory>
#include <asio.hpp>
#include "net_message.h"
#include "net_tsqueue.h"

namespace net {

	template <typename T>
	class connection : public std::enable_shared_from_this<connection<T>>{
	public:
		connection() {}
		virtual ~connection() {}

		bool connect_to_server();
		bool disconnect();
		bool is_connected() const;

		bool send(const message<T>& msg);

	private:
		asio::ip::tcp::socket _socket;
		asio::io_context& _asio_context;

		tsqueue<message<T>> _q_messages_out;
		tsqueue<owned_message<T>>& _q_messages_in;
	};
}

#endif // NETFRAMEWORK_NETCONNECTION
