/*
	log.h - The Log Class logs information on any Stream that inherits from
	the Print class.

	Created by Bruno Calou Alves, May, 2015.
	Read LICENSE for more information.
*/

#ifndef LOG_H
#define LOG_H

#include <inttypes.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef log_endl
#define log_endl '\n'
#endif

enum LogPriority {
	ASSERT 	= 'a',
	DEBUG  	= 'd',
	ERROR  	= 'e',
	INFO   	= 'i',
	VERBOSE = 'v',
	WARN   	= 'w'
};

class Log : public Print {
public:
	Log() {
		stream = &Serial;
		priority = VERBOSE;
		is_enabled = true;
		operator_tag = "";
		operator_priority = VERBOSE;
		operator_print_header = true;
		get_tag = false;
		print_tag = true;
		print_priority = true;
	}

	/*
		Enables the log object. If it's enabled, it will
		print normally
	*/
	void enable() {
		is_enabled = true;
	}

	/*
		Disables the log object. If it's disabled, it will
		not print
	*/
	void disable() {
		is_enabled = false;
	}

	/*
		Set the target to print
	*/
	void setTarget(Stream *stream) {
		this->stream = stream;
	}

	/*
		Set the priority to print. Only the messages
		with the same priority will be printed. If the
		priority of the object is VERBOSE, everything
		will be printed
	*/
	void setPriority(LogPriority priority) {
		this->priority = priority;
	}

	/*
		Calls the write method from the current stream
	*/	
	size_t write(uint8_t b) {
		if(is_enabled) {
			return stream->write(b);
		}
		return 0;
	}

	/*
		Calls the write method from the current stream
	*/
	size_t write(const uint8_t *buffer, size_t size) {
		if(is_enabled) {
			return stream->write(buffer, size);
		}
		return 0;
	}

	/*
		Prints the priority, the tag and the message in the following pattern:

		priority/tag: message

		and returns the number of bytes written. The message will be printed if:
		- The log object is enabled and
		- The message's priority is the same as the current priority or
		- The current priority is setted to verbose
	*/
	template <typename T>

	size_t print(LogPriority priority, char const * tag, T msg, bool print_priority=true, bool print_tag=true) {
		
		size_t bytes_written = 0;

		if(is_enabled && (this->priority == priority || this->priority == VERBOSE)) {
			String full_message = "";

			if(print_priority && this->print_priority) {
				full_message += (char)priority;
				full_message += "/";
			}

			if(print_tag && this->print_tag) {
				full_message += tag;
				full_message += ": ";
			}

			bytes_written += print(full_message);
			bytes_written += print(msg);
		}

		return bytes_written;
	}

	/*
		Calls the print method, prints the new line character "\n"
		and returns the number of bytes written
	*/
	template <typename T>

	size_t println(LogPriority priority, char const * tag, T msg, bool print_priority=true, bool print_tag=true) {

		size_t bytes_written = print(priority, tag, msg, print_priority, print_tag);

		if(bytes_written) {
			bytes_written += print("\n");
		}

		return bytes_written;
	}

	/*
		Overloads the << operator for a c++ style printing
	*/
	template <typename T>

	Log& operator<<(const T& msg) {
		if(get_tag) {
			operator_tag += msg;
		} else {
			//Convert string to char const *
			int size = operator_tag.length() + 1;
			char tag[size];
			operator_tag.toCharArray(tag, size);

			print(operator_priority, tag, msg, operator_print_header, operator_print_header);
			operator_print_header = false;
		}
		get_tag = false;
		return *this;
	}

	/*
		Overloads the << operator and change the priority
	*/
	Log& operator<<(const LogPriority& priority) {
		operator_priority = priority;
		get_tag = true;
		operator_tag = "";
		operator_print_header = true;
		return *this;
	}

	template <typename T>

	void assert(char const * tag, T msg) {
		println(ASSERT, tag, msg);
	}
	
	template <typename T>

	void debug(char const * tag, T msg) {
		println(DEBUG, tag, msg);
	}
	
	template <typename T>

	void error(char const * tag, T msg) {
		println(ERROR, tag, msg);
	}

	template <typename T>
	
	void info(char const * tag, T msg) {
		println(INFO, tag, msg);
	}

	template <typename T>
	
	void verbose(char const * tag, T msg) {
		println(VERBOSE, tag, msg);
	}
	
	template <typename T>

	void warn(char const * tag, T msg) {
		println(WARN, tag, msg);
	}

	/*
		Get the current stream
	*/
	Stream* getStream() {
		return stream;
	}

	/*
		Hides the tags on all messages
	*/
	void hideTag() {
		print_tag = false;
	}

	/*
		Shows the tags on all messages
	*/
	void showTag() {
		print_tag = true;
	}

	/*
		Hides the priority on all messages
	*/
	void hidePriority() {
		print_priority = false;
	}

	/*
		Shows the priority on all messages
	*/
	void showPriority() {
		print_priority = true;
	}

	/*
		Hides the priority and the tags on all messages
	*/
	void hideHeader() {
		print_tag = false;
		print_priority = false;
	}

	/*
		Shows the priority and the tags on all messages
	*/
	void showHeader() {
		print_tag = true;
		print_priority = true;
	}

	using Print::print;
	using Print::println;
	
private:
	Stream * stream;
	LogPriority priority;
	bool is_enabled;
	bool print_tag;
	bool print_priority;

	//C++ style
	LogPriority operator_priority;
	String operator_tag;
	bool operator_print_header;
	bool get_tag;
};

#endif //LOG_H