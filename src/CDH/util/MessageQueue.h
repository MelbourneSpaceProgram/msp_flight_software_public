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

#ifndef MQ_MESSAGEQUEUE_H
#define MQ_MESSAGEQUEUE_H

#include <stddef.h>

#include <vector>
#include <list>

class Message {
public:
    virtual ~Message( void )
    { }
};

typedef Message * MessagePtr;

template< class T >
class MessageHandler {
public:
    /**
       \brief Destructor

       The destructor automatically unregister the handler
       from its corresponding message dispatcher
    */
    virtual ~MessageHandler( void );

    virtual void handleMessage( T *message ) = 0;

protected:
    /**
       \brief Default constructor

       The constructor automatically register the handler
       from its corresponding message dispatcher
    */
    MessageHandler( void );
};

class MessageDispatcher {
public:
    virtual void dispatchMessages( void ) = 0;
    virtual void discardAllMessages( void ) = 0;
    virtual void reset( void ) = 0;
};

template< class T >
class MessageDispatcherImpl : public MessageDispatcher {
    typedef T MessageImpl;
    typedef std::vector< MessageImpl * > MessageArray;
    typedef MessageHandler< T > MessageHandlerImpl;
    typedef std::vector< MessageHandlerImpl * > MessageHandlerArray;

public:
    static MessageDispatcherImpl &getInstance( void )
    {
        static MessageDispatcherImpl instance;
        return instance;
    }

private:
    MessageDispatcherImpl( void );
    virtual ~MessageDispatcherImpl( void );

public:
    virtual void dispatchMessages( void )
    {
        if ( _handlers.size() > 0 && _messages.size() ) {
            for ( unsigned int i = 0; i < _messages.size(); i++ ) {
                MessageImpl *message = _messages[ i ];
                if ( message ) {
                    for ( unsigned int j = 0; j < _handlers.size(); j++ ) {
                        MessageHandlerImpl *handler = _handlers[ j ];
                        if ( handler ) {
                            handler->handleMessage( message );
                        }
                    }
                }
            }
        }

        discardAllMessages();
    }

    virtual void discardAllMessages( void )
    {
        _messages.clear();
    }

    virtual void reset( void )
    {
        discardAllMessages();

        _handlers.clear();
    }

    void pushMessage( T *message )
    {
        _messages.push_back( message );
    }

    void registerHandler( MessageHandlerImpl *handler )
    {
        for ( unsigned int i = 0; i < _handlers.size(); i++ ) {
            if ( _handlers[ i ] == NULL ) {
                _handlers[ i ] = handler;
                return;
            }
        }

        _handlers.push_back( handler );
    }

    void unregisterHandler( MessageHandlerImpl *handler )
    {
        for ( unsigned int i = 0; i < _handlers.size(); i++ ) {
            if ( _handlers[ i ] == handler ) {
                _handlers[ i ] = NULL;
                return;
            }
        }
    }

private:
    MessageArray _messages;
    MessageHandlerArray _handlers;
};

/**
\brief Entry point for the messaging sub-system

This class is the heart of the message sub-system. It is used
to manage all message execution.

\todo This class needs proper documentation
*/
class MessageQueue {
public:
    /**
       \brief Get the queue shared instance
    */
    static MessageQueue &getInstance( void );

private:
    /**
       \brief Default constructor
    */
    MessageQueue( void );

    /**
       \brief Destructor
    */
    virtual ~MessageQueue( void );

public:
    /**
       \brief Register a message dispatcher

       \remarks This method is for internal use of the messaging
       system only. Do not use it.
    */
    void registerDispatcher( MessageDispatcher *dispatcher );

public:
    /**
       \brief Dispatch received messages
    */
    void dispatchMessages( void );

    /**
       \brief Discard all received messages
    */
    void discardAllMessages( void );

    void reset( void );

    /**
       \brief Push a new message into the queue
    */
    template< class T >
    void pushMessage( T *message )
    {
        MessageDispatcherImpl< T >::getInstance().pushMessage( message );
    }

private:
    std::list< MessageDispatcher * > _dispatchers;
};

template< class T >
MessageHandler< T >::MessageHandler( void )
{
    MessageDispatcherImpl< T >::getInstance().registerHandler( this );
}

template< class T >
MessageHandler< T >::~MessageHandler( void )
{
    MessageDispatcherImpl< T >::getInstance().unregisterHandler( this );
}

template< class T >
MessageDispatcherImpl< T >::MessageDispatcherImpl( void )
{
    MessageQueue::getInstance().registerDispatcher( this );
}

template< class T >
MessageDispatcherImpl< T >::~MessageDispatcherImpl( void )
{
}


#endif //MQ_MESSAGEQUEUE_H
