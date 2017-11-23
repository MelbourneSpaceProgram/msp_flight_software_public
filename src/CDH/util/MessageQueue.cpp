/**
 * Crimild Engine is an open source scene graph based engine which purpose
 * is to fulfill the high-performance requirements of typical multi-platform
 * two and tridimensional multimedia projects, like games, simulations and
 * virtual reality.
 *
 * Copyright (C) 2006-2010 Hernan Saez - hhsaez@gmail.com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

// Sourced from https://crimild.wordpress.com/2010/11/01/using-c-templates-to-implement-a-messaging-system/

#include "MessageQueue.h"


MessageQueue &MessageQueue::getInstance( void )
{
    static MessageQueue instance;
    return instance;
}

MessageQueue::MessageQueue( void )
{

}

MessageQueue::~MessageQueue( void )
{

}

void MessageQueue::registerDispatcher( MessageDispatcher *dispatcher )
{
    _dispatchers.push_back( dispatcher );
}

void MessageQueue::dispatchMessages( void )
{
    for ( std::list< MessageDispatcher * >::iterator it = _dispatchers.begin(); it != _dispatchers.end(); it++ ) {
        MessageDispatcher *dispatcher = *it;
        if ( dispatcher) {
            dispatcher->dispatchMessages();
        }
    }
}

void MessageQueue::discardAllMessages( void )
{
    for ( std::list< MessageDispatcher * >::iterator it = _dispatchers.begin(); it != _dispatchers.end(); it++ ) {
        MessageDispatcher *dispatcher = *it;
        if ( dispatcher ) {
            dispatcher->discardAllMessages();
        }
    }
}

void MessageQueue::reset( void )
{
    for ( std::list< MessageDispatcher * >::iterator it = _dispatchers.begin(); it != _dispatchers.end(); it++ ) {
        MessageDispatcher *dispatcher = *it;
        if ( dispatcher ) {
            dispatcher->reset();
        }
    }
}

