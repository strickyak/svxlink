/**
@file	 AsyncTcpConnection.h
@brief   Contains a class for handling exiting TCP connections
@author  Tobias Blomberg
@date	 2003-04-12

This file contains a class to handle exiting TCP connections
to a remote host. See usage instructions in the class definition.

\verbatim
Async - A library for programming event driven applications
Copyright (C) 2003  Tobias Blomberg

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
\endverbatim
*/

/** @example AsyncTcpConnection_demo.cpp
An example of how to use the Async::TcpConnection class
*/



#ifndef ASYNC_TCP_CONNECTION_INCLUDED
#define ASYNC_TCP_CONNECTION_INCLUDED


/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/

#include <sigc++/signal_system.h>

#include <string>


/****************************************************************************
 *
 * Project Includes
 *
 ****************************************************************************/

#include <AsyncIpAddress.h>


/****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Forward declarations
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Namespace
 *
 ****************************************************************************/

namespace Async
{

/****************************************************************************
 *
 * Forward declarations of classes inside of the declared namespace
 *
 ****************************************************************************/

class FdWatch;
class IpAddress;


/****************************************************************************
 *
 * Defines & typedefs
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Exported Global Variables
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Class definitions
 *
 ****************************************************************************/

/**
@brief	A class for handling exiting TCP connections
@author Tobias Blomberg
@date   2003-12-07

This class is used to handle an existing TCP connection. It is not meant to
be used directly but could be. It it mainly created to handle connections
for Async::TcpClient and Async::TcpServer.
*/
class TcpConnection : public SigC::Object
{
  public:
    /**
     * @brief Reason code for disconnects
     */
    typedef enum
    { 
      DR_HOST_NOT_FOUND,      ///< The specified host was not found in the DNS
      DR_REMOTE_DISCONNECTED, ///< The remote host disconnected
      DR_SYSTEM_ERROR,	      ///< A system error occured (check errno)
      DR_RECV_BUFFER_OVERFLOW ///< Receiver buffer overflow
    } DisconnectReason;
    
    /**
     * @brief The default length of the reception buffer
     */
    static const int DEFAULT_RECV_BUF_LEN = 1024;
      
    /**
     * @brief 	Constructor
     * @param 	sock  	      The socket for the connection to handle
     * @param 	remote_addr   The remote IP-address of the connection
     * @param 	remote_port   The remote TCP-port of the connection
     * @param 	recv_buf_len  The length of the receiver buffer to use
     */
    TcpConnection(int sock, const IpAddress& remote_addr, short remote_port,
      	      	  size_t recv_buf_len = DEFAULT_RECV_BUF_LEN);
    
    /**
     * @brief 	Destructor
     */
    ~TcpConnection(void);
    
    /**
     * @brief 	Disconnect from the remote host
     *
     * Call this function to disconnect from the remote host. If already
     * disconnected, nothing will be done. The disconnected signal is not
     * emitted when this function is called
     */
    void disconnect(void);
    
    /**
     * @brief 	Write data to the TCP connection
     * @param 	buf   The buffer containing the data to send
     * @param 	count The number of bytes to send from the buffer
     * @return	Returns the number of bytes written or -1 on failure
     */
    int write(const void *buf, int count);
    
    /**
     * @brief 	Return the IP-address of the remote host
     * @return	Returns the IP-address of the remote host
     *
     * This function returns the IP-address of the remote host.
     */
    const IpAddress& remoteHost(void) const { return remote_addr; }
    
    /**
     * @brief 	Return the remote port used
     * @return	Returns the remote port
     */
    short remotePort(void) const { return remote_port; }
    
    /**
     * @brief 	A signal that is emitted when a connection has been terminated
     * @param 	con   	The connection object
     * @param 	reason  The reason for the disconnect
     */
    SigC::Signal2<void, TcpConnection *, DisconnectReason> disconnected;
    
    /**
     * @brief 	A signal that is emitted when data has been received on the
     *	      	connection
     * @param 	buf   A buffer containg the read data
     * @param 	count The number of bytes in the buffer
     * @return	Return the number of processed bytes
     *
     * This signal is emitted when data has been received on this connection.
     * The buffer will contain the bytes read from the operating system.
     * The slot must return the number of bytes that has been processed. The
     * bytes not processed will be stored in the receive buffer for this class
     * and presented again to the slot when more data arrives. The new data
     * will be appended to the old data.
     */
    SigC::Signal3<int, TcpConnection *, void *, int> dataReceived;
    
    /**
     * @brief 	A signal that is emitted when the send buffer status changes
     * @param 	is_full Set to \em true if the buffer is full or \em false
     *	      	      	if a buffer full condition has been cleared
     */
    SigC::Signal1<void, bool> sendBufferFull;

        
  protected:
    
  private:
    IpAddress 	remote_addr;
    short       remote_port;
    size_t      recv_buf_len;
    int       	sock;
    FdWatch *   rd_watch;
    FdWatch *   wr_watch;
    char *    	recv_buf;
    size_t      recv_buf_cnt;
    
    void recvHandler(FdWatch *watch);
    void writeHandler(FdWatch *watch);

};  /* class TcpConnection */


} /* namespace */

#endif /* ASYNC_TCP_CONNECTION_INCLUDED */



/*
 * This file has not been truncated
 */

