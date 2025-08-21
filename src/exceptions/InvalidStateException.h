/*
 * InvalidStateException.h
 *
 *  Created on: 16/06/2013
 *      Author: Lean
 */

#ifndef INVALIDSTATEEXCEPTION_H_
#define INVALIDSTATEEXCEPTION_H_

#include "Exception.h"

	class InvalidStateException : public Exception
	{
		public:
			InvalidStateException(const char *format, ...)
			{
				va_list args;
				va_start(args, format);
				this->msg = StringFormatter::formatVarArgs(format, &args);
				va_end(args);
			}

		private:
	};
#endif /* INVALIDSTATEEXCEPTION_H_ */
