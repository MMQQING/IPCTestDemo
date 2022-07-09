/*!
	//由boost条件变量封装成信号量
*/
#pragma once
#include <mutex>
#include <atomic>
#include <cstdint>
#include <condition_variable>

namespace ptool{
	namespace condition{
		class condition_variable
		{
			typedef std::condition_variable_any			con_type;
			typedef std::atomic<uint32_t>					atomic_type;
		public:
			condition_variable() : n(0){}
			~condition_variable(){}

			void notify_one(){
				mu_notify.lock();
				++n;
				con.notify_one();
				mu_notify.unlock();
			}

			void notify_all(){
				mu_notify.lock();
				++n;
				con.notify_all();
				mu_notify.unlock();
			}

			void notify_no_time() {
				mu_notify.lock();
				con.notify_one();
				mu_notify.unlock();
			}

			void wait(){
				mu_notify.lock();
				con.wait(mu_notify, std::bind(&condition_variable::pred, this));
				--n;
				mu_notify.unlock();
			}
		private:
			bool pred(){
				return n > 0 ? true : false;
			}
		private:
			atomic_type					n;
			con_type					con;
			std::mutex					mu_notify;
		};
	}
}
