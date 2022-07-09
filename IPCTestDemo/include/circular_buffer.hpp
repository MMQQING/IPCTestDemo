#pragma once
#include <list>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "condition_variable.hpp"

namespace ptool{
	namespace buffer{
		template<typename T>
		class circular_buffer
		{
			typedef unsigned int									size_t;
			typedef std::shared_ptr<T>								element_ptr;
			typedef typename boost::detail::sp_element<T>::type		element_type;
			typedef ptool::condition::condition_variable			con;
			
		public:
			explicit circular_buffer(size_t n, size_t len) : max_size(n){ init(len); }
			~circular_buffer(){
			}
			/*!
			//��ȡ��Ч����
			*/
			element_ptr get_free(){ 
				mu_free.lock();
				if (_f.empty()){ 
					mu_free.unlock(); 
					return NULL;
				}
				element_ptr p = _f.front();	
				_f.pop_front();	
				mu_free.unlock();
				return p;
			}
			/*!
			//������Ч����
			*/
			void push_free(element_ptr p){ 
				mu_free.lock();
				_f.push_back(p); 
				mu_free.unlock();
			}
			/*!
			//��ȡ��Ч����
			*/
			element_ptr get_availabe(){ 
				mu_avai.lock();
				if (_d.empty()){
					mu_avai.unlock(); 
					return NULL;
				}
				element_ptr p = _d.front();	
				_d.pop_front(); 
				mu_avai.unlock();
				return p;
			}
			/*!
			//������Ч����
			*/
			void push_availab(element_ptr p){ 
				mu_avai.lock();
				_d.push_back(p); 
				mu_avai.unlock(); 
				co.notify_one();
			}

			//�����̵߳ȴ��������ݻ��˳��ȴ�������get����
			void wait(){
				co.wait();
			}

			void send_notify()
			{
				co.notify_no_time();
			}

			//����wait�ȴ�
			void wait_no_time(){
				co.notify_one();
			}
			//��ȡʣ�໺��������
			size_t get_free_count()
			{
				size_t ret = _f.size();
				return ret;
			}
			//���û�����
			void reset()
			{
				std::list<element_ptr> ls;
				mu_avai.lock();
				std::list<element_ptr>::iterator iter = _d.begin();
				for (; iter != _d.end(); ++iter)
					ls.push_back(*iter);
				_d.clear();
				mu_avai.unlock();
				//
				mu_free.lock();
				iter = ls.begin();
				for (;iter != ls.end(); ++iter)
					_f.push_back(*iter);
				mu_free.unlock();
			}
		private:
			void init(size_t len){
				for (size_t i = 0; i < max_size; ++i){
					element_ptr t = std::make_shared<T>(len);
					_f.push_back(t);
				}
			}

		private:
			std::list<element_ptr>					_d;
			std::list<element_ptr>					_f;
			boost::mutex							mu_avai;
			boost::mutex							mu_free;
			size_t									max_size;
			con										co;
		};
	}
}
