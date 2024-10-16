// Copyright (c) 2009-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef POCKETCOIN_NETBASE_H
#define POCKETCOIN_NETBASE_H

#if defined(HAVE_CONFIG_H)
#include <config/pocketcoin-config.h>
#endif

#include <compat.h>
#include <netaddress.h>
#include <serialize.h>
#include <util/sock.h>

#include <functional>
#include <stdint.h>
#include <string>
#include <vector>

extern int nConnectTimeout;
extern bool fNameLookup;

//! -timeout default
static const int DEFAULT_CONNECT_TIMEOUT = 5000;
//! -dns default
static const int DEFAULT_NAME_LOOKUP = true;

class proxyType
{
public:
    proxyType(): randomize_credentials(false) {}
    explicit proxyType(const CService &_proxy, bool _randomize_credentials=false): proxy(_proxy), randomize_credentials(_randomize_credentials) {}

    bool IsValid() const { return proxy.IsValid(); }

    CService proxy;
    bool randomize_credentials;
};

enum Network ParseNetwork(const std::string& net);
std::string GetNetworkName(enum Network net);
/** Return a vector of publicly routable Network names; optionally append NET_UNROUTABLE. */
std::vector<std::string> GetNetworkNames(bool append_unroutable = false);
bool SetProxy(enum Network net, const proxyType &addrProxy);
bool GetProxy(enum Network net, proxyType &proxyInfoOut);
bool IsProxy(const CNetAddr &addr);
bool SetNameProxy(const proxyType &addrProxy);
bool HaveNameProxy();
bool GetNameProxy(proxyType &nameProxyOut);
bool LookupHost(const std::string& name, std::vector<CNetAddr>& vIP, unsigned int nMaxSolutions, bool fAllowLookup);
bool LookupHost(const std::string& name, CNetAddr& addr, bool fAllowLookup);
bool Lookup(const std::string& name, CService& addr, int portDefault, bool fAllowLookup);
bool Lookup(const std::string& name, std::vector<CService>& vAddr, int portDefault, bool fAllowLookup, unsigned int nMaxSolutions);
CService LookupNumeric(const std::string& name, int portDefault = 0);
bool LookupSubNet(const std::string& strSubnet, CSubNet& subnet);
// SOCKET CreateSocket(const CService &addrConnect);

/**
 * Create a TCP socket in the given address family.
 * @param[in] address_family The socket is created in the same address family as this address.
 * @return pointer to the created Sock object or unique_ptr that owns nothing in case of failure
 */
std::unique_ptr<Sock> CreateSockTCP(const CService& address_family);

/**
 * Socket factory. Defaults to `CreateSockTCP()`, but can be overridden by unit tests.
 */
extern std::function<std::unique_ptr<Sock>(const CService&)> CreateSock;

/**
 * Try to connect to the specified service on the specified socket.
 *
 * @param addrConnect The service to which to connect.
 * @param sock The socket on which to connect.
 * @param nTimeout Wait this many milliseconds for the connection to be
 *                 established.
 * @param manual_connection Whether or not the connection was manually requested
 *                          (e.g. through the addnode RPC)
 *
 * @returns Whether or not a connection was successfully made.
 */
bool ConnectSocketDirectly(const CService &addrConnect, const Sock& sock, int nTimeout, bool manual_connection);

/**
 * Connect to a specified destination service through a SOCKS5 proxy by first
 * connecting to the SOCKS5 proxy.
 *
 * @param proxy The SOCKS5 proxy.
 * @param strDest The destination service to which to connect.
 * @param port The destination port.
 * @param sock The socket on which to connect to the SOCKS5 proxy.
 * @param nTimeout Wait this many milliseconds for the connection to the SOCKS5
 *                 proxy to be established.
 * @param[out] outProxyConnectionFailed Whether or not the connection to the
 *                                      SOCKS5 proxy failed.
 *
 * @returns Whether or not the operation succeeded.
 */
bool ConnectThroughProxy(const proxyType& proxy, const std::string& strDest, int port, const Sock& sock, int nTimeout, bool& outProxyConnectionFailed);

/** Disable or enable blocking-mode for a socket */
bool SetSocketNonBlocking(const SOCKET& hSocket, bool fNonBlocking);
/** Set the TCP_NODELAY flag on a socket */
bool SetSocketNoDelay(const SOCKET& hSocket);
void InterruptSocks5(bool interrupt);

#endif // POCKETCOIN_NETBASE_H
