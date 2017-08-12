#ifndef _BASIC_FAULT_OBSERVER_H_
#define _BASIC_FAULT_OBSERVER_H_

#include <src/observers/observer.hpp>

class BasicFaultObserver : public Observer {

public:
    void update(I2CSensor* sensor);
};

#endif // _BASIC_FAULT_OBSERVER_H_
