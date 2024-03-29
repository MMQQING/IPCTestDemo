#include "Cpclog.h"
#include <stdexcept>
#include <iostream>
#include <boost/smart_ptr.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

using namespace boost;

boost::log::sources::severity_logger< severity_levels > CpcLogs::slg;

CpcLogs::CpcLogs(void)
{
}


CpcLogs::~CpcLogs(void)
{
}

template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
    std::basic_ostream< CharT, TraitsT >& strm, severity_levels lvl)
{
    static const char* const str[] =
    {
        "emergency",
        "alert",
        "critical",
        "error",
        "warning",
        "notice",
        "info",
        "debug"
    };
    if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
        strm << str[lvl];
    else
        strm << static_cast< int >(lvl);
    return strm;
};

BOOST_LOG_ATTRIBUTE_KEYWORD(_severity, "Severity", severity_levels);
BOOST_LOG_ATTRIBUTE_KEYWORD(_timestamp, "TimeStamp", boost::posix_time::ptime);
BOOST_LOG_ATTRIBUTE_KEYWORD(_uptime, "Uptime", attrs::timer::value_type);
BOOST_LOG_ATTRIBUTE_KEYWORD(_scope, "Scope", attrs::named_scope::value_type);

void CpcLogs::init_log(severity_levels console, severity_levels logs)
{
	auto asink = logging::add_console_log(std::clog, keywords::format = expr::stream
		<< expr::format_date_time(_timestamp, "[%Y-%m-%d,%H:%M:%S.%f] ")
		<< " <" << _severity
		<< ">: " << expr::message);

	asink->set_filter(expr::attr< severity_levels >("Severity") <= console);

	logging::core::get()->add_sink(asink);

    {
        auto sink = logging::add_file_log(
            keywords::file_name = "./logs/%Y-%m-%d.log",
            keywords::rotation_size = 10*1024*1024,                  
            keywords::time_based_rotation=sinks::file::rotation_at_time_point(0,0,0),
            keywords::open_mode = std::ios::app,
            keywords::auto_flush = true
            );


        sink->locked_backend()->set_file_collector(sinks::file::make_collector(
            keywords::target = "logs",                         
            keywords::max_size = 30 * 1024 * 1024,             
            keywords::min_free_space = 100 * 1024 * 1024           
            ));

        sink->locked_backend()->scan_for_files();
        sink->set_formatter
            (
#ifdef _DEBUG
			expr::format("[%1%_%2%] [%3%] {%4%}: %5%")
#else
			expr::format("[%1%] [%3%] {%4%}: %5%")
#endif // DEBUG
				
//			% expr::attr< attrs::current_thread_id::value_type >("ThreadID")
            % expr::format_date_time(_timestamp, "%Y-%m-%d,%H:%M:%S.%f")
            % expr::attr< boost::posix_time::time_duration >("Uptime")
			% expr::attr< severity_levels >("Severity")
#ifdef _DEBUG
			% expr::format_named_scope(_scope, keywords::format = "%n[%f:%l]", keywords::depth = 1)
#else
			% expr::format_named_scope(_scope, keywords::format = "%f:%l", keywords::depth = 1)
#endif // DEBUG
			
			% expr::smessage
            );
		sink->set_filter(expr::attr< severity_levels >("Severity") <= logs);

        logging::core::get()->add_sink(sink);
    }    

    logging::add_common_attributes();
    logging::core::get()->add_thread_attribute("Scope", attrs::named_scope());
    logging::core::get()->add_thread_attribute("Uptime", attrs::timer());
}

void CpcLogs::set_level(severity_levels sl)
{
    logging::core::get()->set_filter(expr::attr< severity_levels >("Severity") <= sl);
}