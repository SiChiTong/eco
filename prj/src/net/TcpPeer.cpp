#include "PrecHeader.h"
#include "TcpPeer.ipp"
////////////////////////////////////////////////////////////////////////////////
#include <eco/net/Ecode.h>
#include <eco/log/Log.h>
#include <eco/net/protocol/StringCodec.h>
#include <eco/net/protocol/TcpProtocol.h>
#include <eco/net/protocol/WebSocketShakeHand.h>
#include <eco/net/protocol/WebSocketProtocol.h>



namespace eco{;
namespace net{;
ECO_OBJECT_IMPL(TcpPeer);
EcoThreadLocal char s_data_head[32] = {0};
////////////////////////////////////////////////////////////////////////////////
void TcpPeer::Impl::async_recv_next()
{
	m_connector->async_read_head(s_data_head, head_size());
}
void TcpPeer::Impl::async_recv_shakehand()
{
	m_state.set_websocket();
	m_connector->async_read_until(
		WebSocketShakeHand::size(), WebSocketShakeHand::head_end());
}


////////////////////////////////////////////////////////////////////////////////
void TcpPeer::Impl::on_connect(
	IN bool is_connected,
	IN const eco::Error* e)
{
	if (!is_connected)
	{
		EcoNet(EcoError, *this, "connect", *e);
		return;
	}

	// "client" connect callback.
	set_connected();				// set peer state.
	if (handler().websocket())
	{
		assert(false);
		m_state.set_websocket();
	}
	else
	{
		m_state.set_ready();
		m_handler->on_connect();	// notify handler.
		async_recv_next();
	}
}


////////////////////////////////////////////////////////////////////////////////
inline void TcpPeer::Impl::handle_websocket_shakehand(
	IN const char* data_head, IN const uint32_t head_size)
{
	if (!m_state.websocket())
	{
		return;
	}

	// handle websockets shakehands head : "Get ..."
	if (eco::find(data_head, head_size, "GET ") != data_head)
	{
		EcoNetLog(EcoError, *this)
			<< "web socket shakehand invalid 'Get '.";
		return;
	}
	WebSocketShakeHand shake_hand;
	if (!shake_hand.parse(data_head))
	{
		EcoNetLog(EcoError, *this) << "web socket shakehand invalid.";
		notify_close(nullptr);
		return;
	}
	async_send(shake_hand.response(), 0);
	m_state.set_ready();	// websocket connection has build.
}


////////////////////////////////////////////////////////////////////////////////
void TcpPeer::Impl::on_read_head(
	IN char* data_head,
	IN const uint32_t head_size,
	IN const eco::Error* err)
{
	if (err != nullptr)	// if peerection occur error, close it.
	{
		EcoNet(EcoWarn, *this, "recv_head", *err);
		notify_close(err);
		return; 
	}

	// parse message body length from protocol head.
	eco::Error e;
	uint32_t data_size = 0;
	if (!protocol_head().decode_data_size(data_size, data_head, head_size, e))
	{
		EcoNet(EcoError, *this, "recv_head", e);
		notify_close(&e);
		return;
	}

	// when recv head from peer, means peer is alive.
	m_state.set_peer_live(true);

	// allocate memory for store coming data.
	eco::String data;
	data.resize(head_size + data_size);
	strncpy(&data[0], data_head, head_size);
	if (data_size == 0)		// empty message.
	{
		m_handler->on_read(this, data);
		async_recv_next();		// recv next coming data.
		return;
	}

	// recv data from peer.
	m_connector->async_read_data(data, head_size);
}


////////////////////////////////////////////////////////////////////////////////
void TcpPeer::Impl::on_read_data(
	IN eco::String& data,
	IN const eco::Error* e)
{
	if (e != nullptr)	// if peer occur error, release it.
	{
		EcoNet(EcoWarn, *this, "recv_data", *e);
		notify_close(e);
		return;
	}

	if (!m_state.ready())
	{
		if (m_state.websocket())
		{
			handle_websocket_shakehand(data.c_str(), data.size());
		}
		async_recv_next();
		return;
	}

	// when recv message from peer, means peer is active.
	// post data message to tcp server.
	m_state.set_peer_active(true);
	m_handler->on_read(this, data);
	async_recv_next();		// recv next coming data.
}


////////////////////////////////////////////////////////////////////////////////
void TcpPeer::Impl::on_write(IN const uint32_t size, IN const eco::Error* e)
{
	if (e != nullptr)
	{
		notify_close(e);
		return;
	}

	// notify tcp handler.
	m_handler->on_send(this, size);
}


//##############################################################################
//##############################################################################
TcpPeer::ptr TcpPeer::make(
	IN IoService* io,
	IN TcpPeerHandler* hdl)
{
	TcpPeer::ptr peer(new TcpPeer);
	peer->impl().m_peer_observer = peer;
	peer->impl().make(io, hdl);
	return peer;
}
void TcpPeer::set_handler(IN TcpPeerHandler* v)
{
	impl().set_handler(v);
}
TcpPeerHandler& TcpPeer::handler()
{
	return impl().handler();
}
ConnectionData* TcpPeer::data()
{
	return impl().m_data.get();
}
int64_t TcpPeer::get_id() const
{
	return impl().get_id();
}
TcpState& TcpPeer::state()
{
	return impl().state();
}
const TcpState& TcpPeer::get_state() const
{
	return impl().get_state();
}
void TcpPeer::set_connected()
{
	impl().set_connected();
}
void TcpPeer::set_option(IN bool no_delay)
{
	impl().m_connector->set_option(no_delay);
}
void TcpPeer::async_connect(IN const Address& addr)
{
	impl().async_connect(addr);
}
void TcpPeer::async_recv()
{
	impl().async_recv_next();
}
void TcpPeer::async_recv_shakehand()
{
	impl().async_recv_shakehand();
}
void TcpPeer::async_send(IN eco::String& data, IN const uint32_t start)
{
	impl().async_send(data, start);
}
void TcpPeer::async_send(IN MessageMeta& meta, IN Protocol& prot)
{
	impl().async_send(meta, prot);
}
void TcpPeer::close()
{
	impl().close();
}
void TcpPeer::notify_close(IN const eco::Error* e)
{
	impl().notify_close(e);
}
void TcpPeer::async_resp(IN Codec& codec, IN const uint32_t type,
	IN const Context& c, IN Protocol& prot, IN const bool last)
{
	MessageMeta meta(codec, none_session, type, c.m_meta.m_category);
	meta.set_request_data(c.m_meta.m_request_data, c.m_meta.m_option);
	meta.set_last(last);
	impl().async_send(meta, prot);
}


////////////////////////////////////////////////////////////////////////////////
}}