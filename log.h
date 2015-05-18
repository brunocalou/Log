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
		stream->write(b);
	}

	/*
		Calls the write method from the current stream
	*/
	size_t write(const uint8_t *buffer, size_t size) {
		stream->write(buffer, size);
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

	size_t print(LogPriority priority, char const * tag, T msg) {
		
		size_t bytes_written = 0;

		if(is_enabled && (this->priority == priority || this->priority == VERBOSE)) {
			String full_message = "";

			full_message += (char)priority;
			full_message += "/";
			full_message += tag;
			full_message += ": ";
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

	size_t println(LogPriority priority, char const * tag, T msg) {

		size_t bytes_written = print(priority, tag, msg);

		if(bytes_written) {
			bytes_written += print("\n");
		}

		return bytes_written;
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

	using Print::print;
	using Print::println;
	
private:
	Stream * stream;
	LogPriority priority;
	bool is_enabled;
};

#endif //LOG_H