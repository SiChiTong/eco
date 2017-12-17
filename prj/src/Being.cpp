#include "PrecHeader.h"
#include <eco/Being.h>
////////////////////////////////////////////////////////////////////////////////
#include <eco/Project.h>
#include <eco/log/Log.h>
#include <eco/thread/State.h>
#include <eco/Implement.h>
#include "Eco.h"


namespace eco{;
////////////////////////////////////////////////////////////////////////////////
class Being::Impl
{
	ECO_IMPL_INIT(Being);
public:
	// �������Ƶ�ʡ�������Ƶ�ʣ�
	uint32_t m_live_ticks;
	eco::atomic::State m_born;
};


////////////////////////////////////////////////////////////////////////////////
ECO_IMPL(Being);
Being::Being(IN const uint32_t live_ticks)
{
	m_impl = new Impl();
	impl().m_live_ticks = live_ticks > 0 ? live_ticks : 1;
}
Being::~Being()
{
	if (impl().m_born.is_ok())
	{
		get_eco()->remove_being(this);
		impl().m_born.none();
	}
}


////////////////////////////////////////////////////////////////////////////////
bool Being::is_born() const
{
	return impl().m_born.is_ok();
}
const uint32_t Being::get_live_ticks() const
{
	return impl().m_live_ticks;
}


////////////////////////////////////////////////////////////////////////////////
void Being::born()
{
	if (impl().m_born.is_ok())
	{
		return;
	}

	if (on_born())	// ������ʼ��
	{
		get_eco()->add_being(this);
		impl().m_born.ok();
	}
}


////////////////////////////////////////////////////////////////////////////////
void Being::live()
{
	// ���������
	try
	{
		born();
		on_live();
	}
	catch (std::exception& e)
	{
		EcoWarn << "live : " << e.what();
	}
}


////////////////////////////////////////////////////////////////////////////////
const uint32_t Being::get_unit_live_tick_seconds()
{
	return get_eco()->get_unit_live_tick_seconds();
}
void Being::post_task(IN Btask& task)
{
	get_eco()->post_task(task);
}

////////////////////////////////////////////////////////////////////////////////
}