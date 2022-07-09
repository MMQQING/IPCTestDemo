#pragma once
#include <string>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>

enum severity_levels
{
    slog_emergency = 0,                //slog_emerg
    slog_alert = 1,                    //slog_alert
    slog_critical = 2,                 //slog_crit
    slog_error = 3,                    //slog_error 
    slog_warning = 4,                  //slog_warning
    slog_notice = 5,                   //slog_notice
    slog_info = 6,                     //slog_info
    slog_debug = 7                     //slog_debug
};

class CpcLogs
{
public:
    CpcLogs(void);
    ~CpcLogs(void);
	/*	功能：初始化过滤等级
	参数：
	@console:小于值的消息在命令窗显示
	@logs:小于值的信息写入日志文件
	返回值：无
	*/
	static void init_log(severity_levels console, severity_levels logs);

	/*	功能：设置全局过滤等级（暂时为效果，待测试）
	参数：
	@sl:显示小于值的消息
	返回值：无
	*/
	static void set_level(severity_levels sl); 
	static boost::log::sources::severity_logger< severity_levels > slg;
};

#define BOOST_SLOG(slog_lvl) BOOST_LOG_FUNCTION();BOOST_LOG_SEV(CpcLogs::slg, slog_lvl)

//调用init_log后，使用以下函数即可执行
#define CPCLOG_EMERGENCY BOOST_SLOG(slog_emergency)
#define CPCLOG_ALERT BOOST_SLOG(slog_alert)
#define CPCLOG_CRITICAL BOOST_SLOG(slog_critical)
#define CPCLOG_ERROR BOOST_SLOG(slog_error)
#define CPCLOG_WARNING BOOST_SLOG(slog_warning)
#define CPCLOG_NOTICE BOOST_SLOG(slog_notice)
#define CPCLOG_INFO BOOST_SLOG(slog_info)
#define CPCLOG_DEBUG BOOST_SLOG(slog_debug)