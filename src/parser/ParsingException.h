/*
 * ParsingException.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef PARSINGEXCEPTION_H_
#define PARSINGEXCEPTION_H_

#include "../exceptions/Exception.h"

	class ParsingException : public Exception
	{
		public:
			ParsingException(const char *format, ...)
			{
				va_list args;
				va_start(args, format);
                this->msg = StringFormatter::formatVarArgs(format, &args);
				va_end(args);
			}

		private:
	};


#endif /* PARSINGEXCEPTION_H_ */
