#ifndef NETFRAMEWORK_MESSAGE
#define NETFRAMEWORK_MESSAGE

#include <cstdint>
#include <vector>
#include <iostream>

namespace net {
	template <typename T>
	struct message_header {
		T id{};
		uint32_t size = 0;
	};

	template <typename T>
	struct message {
		message_header<T> header;
		std::vector<uint8_t> body;

		size_t size() {
			return body.size();
		}

		friend std::ostream& operator<<(std::ostream& os, message<T>& msg) {
			os << "ID: " << static_cast<int>(msg.header.id) << "size: " << size << std::endl;
			return os;
		}

		template <typename DataType>
		friend message<T>& operator<<(message<T>& msg, const DataType& data) {
			static_assert(std::is_pod<DataType>::value, "Data is too complex to be pushed into body");

			size_t i = msg.body.size();

			msg.body.resize(i + sizeof(DataType));

			std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

			msg.header.size = msg.size();

			return msg;
		}

		template <typename DataType>
		friend message<T>& operator>>(message<T>& msg, DataType& data) {
			static_assert(std::is_pod<DataType>::value, "Data is too complex to be pushed into body");

			size_t i = msg.body.size() - sizeof(DataType);

			std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

			msg.body.resize(i);

			msg.header.size = msg.size();

			return msg;
		}
	};

	template <typename T>
	struct connection;

	template <typename T>
	struct owned_message {
		std::shared_ptr<connection<T>> remote = nullptr;
		message<T> msg;

		friend std::ostream& operator<<(std::ostream& os, owned_message<T> msg) {
			os << msg.msg;
			return os;
		}
	};
}

#endif // NETFREMEWORK_MESSAGE