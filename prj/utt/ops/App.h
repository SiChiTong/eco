#ifndef ECO_CMD_COMMAND_APP_H
#define ECO_CMD_COMMAND_APP_H
/*******************************************************************************
@名称
eco.cmd.test.app

@功能
1.测试eco.cmd框架。


--------------------------------------------------------------------------------
@修改记录
日期			版本		修改人			修改内容
2015\06\20		1.0			uJoy			创建与初始化类。

--------------------------------------------------------------------------------
* 版权所有(c) 2015 - 2017, mrs corp, 保留所有权利。

*******************************************************************************/
#include <eco/App.h>


namespace eco{;
namespace cmd{;
namespace test{;


////////////////////////////////////////////////////////////////////////////////
class App : public eco::App
{
protected:
	virtual void on_cmd() override;

public:
	App();
};

ECO_APP(App, GetApp);
}}}
#endif