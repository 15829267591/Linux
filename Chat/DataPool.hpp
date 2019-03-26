#pragma once

#include <iostream>
//思路： datapool 是做什么呢？ 生产者消费者中的交易场所  
//环形队列  用数组模拟  本质就是生产者生产，消费者消费。只要保证空和满的时候只有一个人操作
//消费者关心 空盘子的数量，消费者关心果子的数量（数据，资源数量）
//服务器的作用转发消息，所以服务器不需要关心消息本身的内容
#include <vector>
#include <string>
#include <semaphore.h>
class DataPool{
		private:
				std::vector<std::string> pool;
				int cap; //容量
				sem_t data_sem;		//信号量有一些前置条件。在什么场景下用。本质是一个计数器？？??
			                     	//信号量是用来描述临界资源的数量。（前置条件）只要申请就是你的。
									//它本身也是临界资源。PV 操作是原子性，也就是为什么不用计数器而用它
				sem_t blank_sem; 
				int product_step;
				int consume_step;
		public:
				DataPool(int cap_ = 512):cap(cap_),pool(cap_)
				{   
						sem_init(&data_sem, 0, 0);//空格子和空位置是生产者者关心的
						sem_init(&blank_sem, 0, cap);
						product_step = 0;
						consume_step = 0;
				}

				void PutMessage(std::string &msg)
				{   
						sem_wait(&blank_sem);  //P操作
						pool[product_step] = msg;
						product_step++;
						product_step %= cap;
						sem_post(&data_sem);
				}   

				void GetMessage(std::string &msg)
				{
						sem_wait(&data_sem);
						msg = pool[consume_step];
						consume_step++;
						consume_step %= cap;


				}
				~DataPool()
				{
					sem_destroy(&data_sem);
					sem_destroy(&data_sem);
				}

};
