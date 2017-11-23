/*
 * StaticQueue.h
 *
 *  Created on: 23Nov.,2017
 *      Author: Anthony
 */

#ifndef STATICQUEUE_H_
#define STATICQUEUE_H_

template <typename T, int queue_size, int n_consumers>
class StaticQueue
{
private:
    MemPool<T, queue_size> queue[n_consumers];
    //MemPool<T, queue_size>;
public:
    StaticQueue();
    virtual ~StaticQueue();

};

#endif /* STATICQUEUE_H_ */
