/*
 * MemPool.h
 *
 *  Created on: 21Nov.,2017
 *      Author: Anthony
 */

#ifndef MEMPOOL_H_
#define MEMPOOL_H_

template <class T, int num_maximum_objects>
class MemPool
{
private:
    uint8_t buffer[sizeof(T)*num_maximum_objects];
    bool state[num_maximum_objects];
    void init(){
            for(int i = 0; i < num_maximum_objects; i++){
                state[i] = false;
            }
        }
public:
    MemPool(){
        init();
    }
    void * allocate(){
        for(int i = 0; i < num_maximum_objects; i++){
            if(!state[i]){
                state[i] = true;
                return &buf[sizeof(T) * i];
            }

            // If we reached here, there is no memory available.
            // Come up with some method to show this.
            return 0; // TODO
        }
    }
    void deallocate(void * p){
        for(int i = 0; i < num_maximum_objects; i++){
            if(&buf[sizeof(T) * i] == p){
                state[i] = false;
                return;
            }
        }

        // TODO Throw error if we made it to here somehow.
    }
};

#endif /* MEMPOOL_H_ */
