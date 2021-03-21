#ifndef NETFRAMEWORK_TSQUEUE
#define NETFRAMEWORK_TSQUEUE

#include <mutex>
#include <condition_variable>
#include <queue>

namespace net {

	template<typename T>
	class tsqueue {
	public:
		tsqueue(const tsqueue<T>&) = delete;
		tsqueue(size_t max_capacity = 5000) : _max_capacity{ max_capacity } {}
		virtual ~tsqueue() {
			clear();
		}

		bool push(T new_value) {
			if (queue.size() < _max_capacity) {
				std::unique_lock<std::mutex> lck{ mtx };
				queue.push(std::move(new_value));
				cond.notify_one();
				
				return true;
			}
			return false;
		}

		size_t count() const {
			std::lock_guard<std::mutex> lck{ mtx };
			return queue.size();
		}

		void clear() {
			std::lock_guard<std::mutex> lck{ mtx };
			std::queue<T> empty;
			std::swap(queue, empty);
		}

		bool empty() const {
			std::lock_guard<std::mutex> lck{ mtx };
			return queue.empty();
		}
	private:
		std::queue<T> queue;
		std::mutex mtx;
		std::condition_variable cond;
		size_t _max_capacity;
	};
}
#endif // NETFRAMEWORK_TSQUEUE
