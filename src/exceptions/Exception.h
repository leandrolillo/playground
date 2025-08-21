#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <JavaLike.h>
#include<StringFormatter.h>
#include<string>
#include <stdio.h>
#include <stdarg.h>

class Exception {
	protected:
		String msg;

		public:
		Exception()
		{
			this->msg = "Unhandled Exception";
		}
		Exception(const char *format, ...) {
			va_list args;
			va_start(args, format);
			this->msg = StringFormatter::formatVarArgs(format, &args);
			va_end(args);
		}
		Exception(String message) {
		    this->msg = message;
		}

		std::string toString() const {
			return getMessage();
		}

		std::string getMessage() const
		{
			return this->msg;
		}
		void setMessage(const char *format, ...)
		{
			va_list args;
			va_start(args, format);
			this->msg = StringFormatter::format(format, &args);
			va_end(args);

		}
	};
#endif
