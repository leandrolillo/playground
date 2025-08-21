#ifndef _INVALID_ARGUMENT_EXCEPTION_
	#define _INVALID_ARGUMENT_EXCEPTION_

	#include "Exception.h"

	class InvalidArgumentException : public Exception
	{
		public:
			InvalidArgumentException(const char *format, ...)
			{
				va_list args;
				va_start(args, format);
                this->msg = StringFormatter::formatVarArgs(format, &args);
				va_end(args);
			}
			
		private:
	};

#endif
