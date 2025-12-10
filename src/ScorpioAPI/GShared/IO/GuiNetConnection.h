/**************************************************************************
*                           CONFIDENTIAL                                  *
* Unauthorized access to, copying, use of or disclosure of this software, *
* or any of its features, is strictly prohibited. Your access to or       *
* possession of a copy of this software is persuant to a limited license; *
* ownership of the software and any associated media remains with TCI.    *
*                                                                         *
* Copyright 2007-2014 TCI International, Inc. All rights reserved         *
**************************************************************************/

#pragma once

#include <vector>

#ifndef _In_
#define _In_
#endif

template<typename T, size_t HEADER_SIZE, size_t RECV_BUFFER_SIZE = 65536> class CNetConnection
:
	// Inheritance
	public CAsyncSocket
{
public:
	// Functions
	CNetConnection(void);
	virtual ~CNetConnection(void) {}
	void Connect(_In_ LPCTSTR lpszHostAddress, unsigned short nHostPort);
	void Send(_In_ const T& msg);

private:
	// Functions
	virtual inline size_t BodySize(_In_ const T& msg) const = 0;
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnNetClose(int nErrorCode) = 0;
	virtual void OnNetConnect(int nErrorCode) = 0;
	virtual void OnNetReceive(_In_ const T& msg) = 0;
	virtual void OnNetSent(void) = 0;
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

	// Data
	ADDRINFOT* m_server;
	ADDRINFOT* m_addrinfo;
	std::vector<char> m_recvBuffer;
	size_t m_recvIdx;
	T m_sendMsg;
	int m_sent;
};


/////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
template<typename T, size_t HEADER_SIZE, size_t RECV_BUFFER_SIZE>
	CNetConnection<T, HEADER_SIZE, RECV_BUFFER_SIZE>::CNetConnection(void)
:
	m_recvBuffer(RECV_BUFFER_SIZE),
	m_recvIdx(0),
	m_sent(-1)
{
	AfxSocketInit();

	return;
}


/////////////////////////////////////////////////////////////////////////////
//
// Connect
//
template<typename T, size_t HEADER_SIZE, size_t RECV_BUFFER_SIZE>
	void CNetConnection<T, HEADER_SIZE, RECV_BUFFER_SIZE>::Connect(_In_ LPCTSTR lpszHostAddress, unsigned short nHostPort)
{
	m_recvIdx = 0;
	m_sent = -1;
	const int bufferSize = RECV_BUFFER_SIZE;
	ADDRINFOT hints;
#if _WIN32_WINNT < _WIN32_WINNT_VISTA
	hints.ai_flags = 0;
	hints.ai_family = PF_INET;
#else
	hints.ai_flags = AI_ALL | AI_V4MAPPED;
	hints.ai_family = PF_INET6;
#endif
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_addrlen = 0;
	hints.ai_canonname = nullptr;
	hints.ai_addr = nullptr;
	hints.ai_next = nullptr;
	CString service;
	service.Format(_T("%u"), nHostPort);

	if(GetAddrInfo(lpszHostAddress, service, &hints, &m_addrinfo) != 0)
	{
		// See if converting a IPV4 dotted address to a IPV6 IPV4-mapped address works
		std::basic_string<TCHAR> ipv6Host(_T("::ffff:"));
		ipv6Host += lpszHostAddress;
		hints.ai_flags |= AI_NUMERICHOST;

		if(GetAddrInfo(ipv6Host.c_str(), service, &hints, &m_addrinfo) != 0)
		{
			DWORD error = WSAGetLastError();
			OnNetConnect(error);
			if (error == WSAHOST_NOT_FOUND)
			{
				// Host not found.
				return;
			}
		}
	}

	m_server = m_addrinfo;
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
	static const int OFF = 0;
#endif

	if(!CreateEx(m_server, FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE) ||
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
		!SetSockOpt(IPV6_V6ONLY, &OFF, sizeof(OFF), IPPROTO_IPV6) ||
#endif
		!SetSockOpt(SO_RCVBUF, &bufferSize, sizeof(bufferSize)) ||
		!CAsyncSocket::ConnectEx(m_server))
	{
		DWORD error = WSAGetLastError();

		if(error != WSAEWOULDBLOCK)
		{
			// Error
			FreeAddrInfo(m_addrinfo);
			Close();
			OnNetConnect(error);
		}
	}
	else
	{
		OnConnect(0);
	}

	return;
}


/////////////////////////////////////////////////////////////////////////////
//
// Callback when connection is closed
//
template<typename T, size_t HEADER_SIZE, size_t RECV_BUFFER_SIZE>
	void CNetConnection<T, HEADER_SIZE, RECV_BUFFER_SIZE>::OnClose(int nErrorCode)
{
	Close();
	OnNetClose(nErrorCode);

	return;
}


/////////////////////////////////////////////////////////////////////////////
//
// Callback when connection completes
//
template<typename T, size_t HEADER_SIZE, size_t RECV_BUFFER_SIZE>
	void CNetConnection<T, HEADER_SIZE, RECV_BUFFER_SIZE>::OnConnect(int nErrorCode)
{
	if(nErrorCode == 0)
	{
		FreeAddrInfo(m_addrinfo);
		OnNetConnect(nErrorCode);
	}
	else if(m_server->ai_next != nullptr)
	{
		Close();
		m_server = m_server->ai_next;
		const int bufferSize = RECV_BUFFER_SIZE;

#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
		static const int OFF = 0;
#endif

		if(!CreateEx(m_server, FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE) ||
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
			!SetSockOpt(IPV6_V6ONLY, &OFF, sizeof(OFF), IPPROTO_IPV6) ||
#endif
			!SetSockOpt(SO_RCVBUF, &bufferSize, sizeof(bufferSize)) ||
			!CAsyncSocket::ConnectEx(m_server))
		{
			DWORD error = WSAGetLastError();

			if(error != WSAEWOULDBLOCK)
	{
				// Error
				FreeAddrInfo(m_addrinfo);
		Close();
				OnNetConnect(error);
			}
		}
		else
		{
			FreeAddrInfo(m_addrinfo);
			OnNetConnect(0);
		}
	}
	else
	{
		Close();
		FreeAddrInfo(m_addrinfo);
	OnNetConnect(nErrorCode);
	}

	return;
}


/////////////////////////////////////////////////////////////////////////////
//
// Callback when data is received
//
template<typename T, size_t HEADER_SIZE, size_t RECV_BUFFER_SIZE>
	void CNetConnection<T, HEADER_SIZE, RECV_BUFFER_SIZE>::OnReceive(int nErrorCode)
{
	if(nErrorCode != 0)
	{
		// Error
		OnClose(nErrorCode);
	}
	else
	{
		// Disable receive notifications
		AsyncSelect(FD_WRITE | FD_CONNECT | FD_CLOSE);

		// Receive data up to size of buffer
		int got = Receive(&m_recvBuffer[m_recvIdx], int(m_recvBuffer.size() - m_recvIdx));

		if(got == SOCKET_ERROR)
		{
			DWORD error = WSAGetLastError();

			if(error != WSAEWOULDBLOCK)
			{
				// Error
				OnClose(error);
			}
		}
		else if(got == 0)
		{
			// Closed
			OnClose(0);
		}
		else
		{
			// Got something
			m_recvIdx += got;

			// Loop through received messages
			size_t msgIdx = 0;
			size_t length = 0;

			while(msgIdx + HEADER_SIZE <= m_recvIdx)
			{
				length = HEADER_SIZE + BodySize(*reinterpret_cast<const T*>(&m_recvBuffer[msgIdx]));

				if(msgIdx + length > m_recvIdx)
				{
					break;
				}

				// Process complete message
				OnNetReceive(*reinterpret_cast<const T*>(&m_recvBuffer[msgIdx]));

				// Advance to next message
				msgIdx += length;
			}

			m_recvIdx -= msgIdx;

			if(m_recvIdx > 0)
			{
				// Move partial message to beginning of buffer
				memmove(&m_recvBuffer[0], &m_recvBuffer[msgIdx], m_recvIdx);

				if(m_recvIdx > HEADER_SIZE && length > m_recvBuffer.size())
				{
					// Increase space for messages
					m_recvBuffer.resize(length);
				}
			}
		}

		// Re-enable notifications
		if (m_hSocket != INVALID_SOCKET)
			AsyncSelect(FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
	}

	return;
}


/////////////////////////////////////////////////////////////////////////////
//
// Callback when more data can be sent
//
template<typename T, size_t HEADER_SIZE, size_t RECV_BUFFER_SIZE>
	void CNetConnection<T, HEADER_SIZE, RECV_BUFFER_SIZE>::OnSend(int nErrorCode)
{
	if(nErrorCode != 0)
	{
		m_sent = -1;
		OnClose(nErrorCode);
	}
	else if(m_sent != -1)
	{
		int length = int(HEADER_SIZE + BodySize(m_sendMsg)) - m_sent;
		int sent = CAsyncSocket::Send(&(reinterpret_cast<char*>(&m_sendMsg)[m_sent]), length);

		if(sent == SOCKET_ERROR)
		{
			DWORD error = WSAGetLastError();

			if(error == WSAEWOULDBLOCK)
			{
				// Nothing sent
				sent = 0;
			}
			else
			{
				// Error
				m_sent = -1;
				OnClose(error);
				return;
			}
		}

		if(sent < length)
		{
			// Partial send
			m_sent += sent;
		}
		else
		{
			// Send complete
			m_sent = -1;
			OnNetSent();
		}
	}

	return;
}


/////////////////////////////////////////////////////////////////////////////
//
// Send a message
//
template<typename T, size_t HEADER_SIZE, size_t RECV_BUFFER_SIZE>
	void CNetConnection<T, HEADER_SIZE, RECV_BUFFER_SIZE>::Send(_In_ const T& msg)
{
	ASSERT(m_sent == -1); // Can't send while another is pending
	int length = int(HEADER_SIZE + BodySize(msg));
	int sent = CAsyncSocket::Send(&msg, length);

	if(sent == SOCKET_ERROR)
	{
		DWORD error = WSAGetLastError();

		if(error == WSAEWOULDBLOCK)
		{
			// Nothing sent
			sent = 0;
		}
		else
		{
			// Error
			m_sent = -1;
			OnClose(error);
			return;
		}
	}

	if(sent < length)
	{
		// Partial send
		m_sendMsg = msg;
		m_sent = sent;
	}
	else
	{
		// Send complete
		m_sent = -1;
		OnNetSent();
	}

	return;
}
