/*
 * InvalidOperationException.h
 *
 *  Created on: 29/06/2013
 *      Author: Lean
 */

#ifndef INVALIDOPERATIONEXCEPTION_H_
#define INVALIDOPERATIONEXCEPTION_H_

#include "Exception.h"

	class InvalidOperationException : public Exception
	{
		public:
			InvalidOperationException(const char *format, ...)
			{
				va_list args;
				va_start(args, format);
                this->msg = StringFormatter::formatVarArgs(format, &args);
				va_end(args);
			}

		private:
	};
#endif /* INVALIDOPERATIONEXCEPTION_H_ */
