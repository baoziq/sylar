/*************************************************************************
    > File Name: log.h
    > Author: baozi
    > Mail: caz69012@gmail.com 
    > Created Time: Mon Apr 28 16:36:31 2025
 ************************************************************************/
#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <stringstream>
#include <fstream>
#include <vector>

namespace sylar {
// 日记事件
class LogEvent {
	public:
		typedef std::shared_ptr<LogEvent> ptr;
		LogEvent();
	private:
		const char* m_file = nullptr;	// 文件名
		int32_t m_line = 0;				// 行号
		uint32_t m_elapse = 0;			// 程序启动开始到现在到毫秒数
		uint32_t m_threadId = 0;		// 线程id
		uint32_t mfiberId = 0;			// 携程id
		uint64_t m_time;				// 时间戳
		std::string m_content;			 
};
// 日志级别
5
	};
};
// 日记格式器
class LogFormatter{
	public:
		typedef std::shared_ptr<LogFormatter> ptr;
	;
		std::string format(LogEvent::ptr event);	
	public:
		class FormatItem {
			public:
				typedef std::shared_ptr<FormatItem> ptr;
				virtual ~FormatItem() {

				}
				virtual void format(std::ostream os, LogEvent::ptr event) = 0;
		};
		void init();
	private:
		std::string m_pattern;
		std::vector<FormatItem::ptr> m_items;
};
// 日记输出地
class LogAppender {
	public:
		typedef std::shared_ptr<LogAppender> ptr;
		virtual ~LogAppender() {};
		virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0; 
		void setFormatter(LogFormatter::ptr val) {
			m_formatter = val;
		}
		LogFormatter::ptr getFormatter() const {
			return m_formatter;
		}
	protected:
		LogLevel::Level m_level;
		LogFormatter::ptr m_formatter;

}
// 日记器
class Logger {
	public:
		typedef std::shared_ptr<Logger> ptr;
		Logger(const std::string& name = "root");
		void log(LogLevel::Level level, LogEvent::ptr event);
		void debug(LogEvent::ptr event);
		void info(LogEvent::ptr event);
		void warn(LogEvent::ptr event);
		void error(LogEvent::ptr event);
		void fatal(LogEvent::ptr event);

		void addAppender(LogAppender::ptr appender);
		void delAppender(LogAppender::ptr appender);
		LogLevel::Level getLevel() const {
			return m_level;
		}
		void setLevel(LogLevel::Level val) {
			m_level = val;
		}
	private:
		std::string m_name;		// 日志名称
		LogLevel::Level m_level;// 日志级别
		std::list<LogAppender::ptr> m_appenders;		// Appender集合

};
// 输出到控制台的Appender
class StdoutLogAppender : public LogAppender {
	public:
		typedef std::shared_ptr<StdoutLogAppender> ptr;
		void log(LogLevel::Level level, LogEvent::ptr event) override;
};
// 输出到文件到Appender
class FileLogAppender : public LogAppender {
	public:
		typedef std::shared_ptr<FileLogAppender> ptr;
		FileLogAppender(const std::string& filename);
		void log(LogLevel::Level level, LogEvent::ptr event) override;
		bool reopen();
	private:
		std::string m_filename;
		std::ofstream m_filestream;
};
}
#endif
