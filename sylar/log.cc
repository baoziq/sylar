#include "log.h"
#include <map>
namespace syalr {
	const char* LogLevel::ToString(Loglevel::Level level) {
		switch(level) {
#define XX(name) \
			case LogLevel::name: \
				return #name;	\
				break;
			XX(DEBUG);
			XX(INFO);
			XX(WARN);
			XX(ERROR);
			XX(FATAL);
#undef XX
			default:
				return "UNKNOW";
			return "UNKONW";
		}
class MessageFormatItem : public LogFormatter::FormatItem {
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
			os << event->getContent();
		}
};
class LevelFormatItem : public LogFormatter::FormatItem {
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
			os << LogLevel::ToString(level);
		}
};
	
class ElapseFormatItem : public LogFormatter::FormatItem {
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
			os << event->getElapse(); 
		}
};

class NameFormatItem : public LogFormatter::FormatItem {
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
			os << logger->getName();
		}
};
class ThreadIdFormatItem : public LogFormatter::FormatItem {
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
			os << logger->getThreadId();
		}
};
class FiberIdFormatItem : public LogFormatter::FormatItem {
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
			os << logger->getFiberId();
		}
};
class DateTimeFormatItem : public LogFormatter::FormatItem {
	public:
		DateTimeFormatItem(const std::string& format = "%Y:%m:%d %H:%S")
			:mformat(format) {

			}
		void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
			os << logger->getTime();
		}
	private:
		std::stringm_format;
};
class FilenameFormatItem : public LogFormatter::FormatItem {
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
			os << logger->getFile();
		}
};
class LineFormatItem : public LogFormatter::FormatItem {
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
			os << logger->getLine();
		}
};
class NewLineFormatItem : public LogFormatter::FormatItem {
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
			os << std::endl; 
		}
};
class StringFormatItem : public LogFormatter::FormatItem {
	public:
		StringFormatItem(const std::string& str)
			:FormatItem(str), m_string(str) {}
		void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override{
			os << m_string;
		}
	private:
		std::string m_string;
};
	Logger::Logger(const std::string& name)
		:m_name(name){

	}
	void Logger::addAppender(LogAppender::ptr appender) {
		m_appenders.push_back(appender);
	}
	void Logger::delAppender(LogAppender::ptr appender) {
		for (auto it = m_appenders.begin(); it != m_appenders.end(); it++) {
			if (*it == appender) {
				m_appenders.erase(it);
				break;
			}
		}
	}
	void Logger::log(LogLevel level, LogEvent::ptr event) {
		if (level >= m_level) {
			for (auto& it : m_appenders) {
				i->log(level, event);
			}
		}
	}
	void Logger::debug(LogEvent::ptr event) {
		debug(LogLevel::DEBUG, event);
	}
	void Logger::info(LogEvent::ptr event) {
		debug(LogLevel::INFO, event);
	}
	void Logger::warn(LogEvent::ptr event) {
		debug(LogLevel::WARN, event);
	}
	void Logger::error(LogEvent::ptr event) {
		debug(LogLevel::ERROR, event);
	}
	void Logger::fatal(LogEvent::ptr event) {
		debug(LogLevel::FATAl, event);
	}
	FileLogAppender::FileLogAppender(const std::string& filename)
		:m_filename(filename) {
	}
	void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
		if (level >= m_level) {
			m_filestream << m_formatter.format(logger, level, event);
		}
	}
	bool FileLogAppender::reopen() {
		if (m_filestream) {
			m_filestream.close();
		}
		m_filestream.open(m_filename);
	return !!m_filestream;
}
void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
	if (level >= m_level) {
		std::cout << m_formatter.format(logger, level, event);
	}
}
LogFormatter::LogFormatter(const std::string& pattern) 
	:m_pattern(pattern) {

}
std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
	std::stringstream ss;
	for (auto& i : m_items) {
		i->format(ss, logger, level, event);
	}
	return ss.str();
}
void LogFormatter::init() {
	std::vector<std::tuple<std::string, std::stirng, int>> vec;
	std::string nstr;
	for (size_t i = 0; i < m_pattern.size(); i++) {
		if (m_pattern[i] != '%') {
			str.append(1, m_pattern[i]);
			continue;
		}
		if ((i + 1) < m_pattern.size()) {
			if (m_pattern[i + 1] == '%') {
				nstr.append(1, '%');
				continue;
			}
		}
		size_t n = i = 1;
		int fmt_status = 0;
		size_t fmt_begin = 0;
		std::string str;
		std::string fmt;
		while (n < m_pattern.size()) {
			if (isspace(m_pattern[n])) {
				break;
			}
			if (fmt_status == 0) {

			}
			if (m_pattern[n] == '{') {
				 str = m_pattern.substr[i + 1, n - i - 1];	
				 fmt_status = 1;	// 解析格式
				 ++n;
				 fmt_begin = n;
				 continue;
			}
			if (fmt_status == 1) {
				if (m_patter[n] == '}') {
					fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin);
					fmt_status = 2;
					break;
				}
			}
		}
		if (fmt_status == 0) {
			if (!nstr.empty()) {
				vec.push_back(std::make_pair(nstr, "", 0));
			}
			str = m_pattern.substr(i + 1, n - i - 1);
			vec.push_back(std::make_tuple(str, fmt, 1));
			i = n;
		} else if (fmt_status == 1) {
			std::cout << "pattern parse error:" << m_pattern << " - " << m_pattern.substr[i] << std::endl;
			vec.push_back(std::make_tuple("pattern_error", fmt, 0));
		} else if (fmt_status == 2) {
			if (!nstr.empty()) {
				vec.push_back(std::make_pair(nstr, "", 0));
			}
			vec.push_back(std::make_tuple(str, fmt, 1));
			i = n;
		}
	}
	if (!nstr.empty()) {
		vec.push_back(std::make_pair(nstr, "", 0));
	}
	static std::map<std::string, string::function<FormatItem::ptr(cosnt std::string& str)>> s_format_items = {
#define XX(str, C)	\
		{*str, [](const std::string& fmt) {return FormatItem::ptr(new C{fmt});},
		XX(m, MessageFormatItem),
		XX(p, LevelFormatItem),
		XX(r, ElapseFormatItem),
		XX(c, NameFormatItem),
		XX(t, ThreadIdFormatItem),
		XX(n, NewLineFormatItem),
		XX(d, DateTimeFormatItem),
		XX(f, FileNameFormatItem),
		XX(l, LineFormatItem),

#undef XX
	};
	for (auto& i : vec) {
		if (std::get<2>(i) == 0) {
			m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
		} else {
			auto it = s_format_items.find(std::get<0>(i));
			if (it == s_format_items.end()) {
				m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
			} else {
				m_items_push_back(it->second(std::get<i>(i)));
			}
		}
		std::cout << std::get<0>(i) << " - " << std::get<1>(i) << " - " << std::get<2>(i) << std::endl;
	}
}

}
