#ifndef ECO_NET_TCP_CLIENT_H
#define ECO_NET_TCP_CLIENT_H
/*******************************************************************************
@ name


@ function


@ exception


@ note


--------------------------------------------------------------------------------
@ history ver 1.0 @
@ records: ujoy modifyed on 2016-12-14.
1.create and init this class.


--------------------------------------------------------------------------------
* copyright(c) 2015 - 2017, ujoy, reserved all right.

*******************************************************************************/
#include <eco/Project.h>
#include <eco/net/Address.h>
#include <eco/net/TcpSession.h>
#include <eco/net/DispatchRegistry.h>
#include <eco/net/TcpClientOption.h>
#include <eco/net/protocol/TcpProtocol.h>


////////////////////////////////////////////////////////////////////////////////
namespace eco{;
namespace net{;


////////////////////////////////////////////////////////////////////////////////
class ECO_API TcpClient
{
	ECO_SHARED_API(TcpClient);
public:
	// network server option.
	void set_option(IN const TcpClientOption&);
	TcpClientOption& option();
	const TcpClientOption& get_option() const;
	TcpClient& option(IN const TcpClientOption&);

	// service name.
	inline const char* get_service_name() const
	{
		return get_option().get_service_name();
	}

	// protocol head.
	template<typename ProtocolHeadT>
	inline void set_protocol_head()
	{
		set_protocol_head(new ProtocolHeadT());
	}
	void set_protocol_head(IN ProtocolHead*);
	ProtocolHead& protocol_head() const;

	// protocol
	template<typename ProtocolT>
	inline void set_protocol()
	{
		set_protocol(new ProtocolT());
	}
	void set_protocol(IN Protocol*);
	Protocol& protocol() const;

	// set connection data class.
	template<typename ConnectionDataT>
	inline void set_connection_data()
	{
		set_connection_data(&make_connection_data<ConnectionDataT>);
	}
	void set_connection_data(IN MakeConnectionDataFunc make);

	// set session data class and tcp session mode.
	template<typename SessionDataT>
	inline void set_session_data()
	{
		set_session_data(&make_session_data<SessionDataT>);
	}
	void set_session_data(IN MakeSessionDataFunc make);
	bool session_mode() const;

	// dispatch.
	DispatchRegistry& dispatcher();

	// release client.
	void close();

//////////////////////////////////////////////////////////////////// ROUTER MODE
public:
	// router mode: async call cluster init router.
	void async_connect_router(
		IN const char* router_name,
		IN eco::net::AddressSet& router_addr);

	// router mode: async connect to service of router.
	void async_connect(
		IN const char* router_name,
		IN const char* service_name);

/////////////////////////////////////////////////////////////////// SERVICE MODE
public:
	// open session.
	TcpSession open_session();

	// service mode: set address ready to connect to service.
	void set_address(
		IN eco::net::AddressSet& service_addr);
	void async_connect();

	// service mode: async connect to service.
	void async_connect(
		IN eco::net::AddressSet& service_addr);

	// async send request to server.
	void async_auth(IN TcpSession& session, IN MessageMeta& meta);

	// async send message.
	void async_send(IN eco::String& data, IN const uint32_t start);

	// async send message.
	void async_send(IN MessageMeta& meta);

#ifndef ECO_NO_PROTOBUF
	// async send protobuf.
	inline void async_send(
		IN google::protobuf::Message& msg,
		IN const uint32_t type,
		IN const uint32_t request_data,
		IN const MessageCategory category = category_message)
	{
		ProtobufCodec codec(msg);
		MessageMeta meta(codec, none_session, type, category);
		meta.set_request_data(request_data);
		async_send(meta);
	}
#endif
};


////////////////////////////////////////////////////////////////////////////////
}}
#endif