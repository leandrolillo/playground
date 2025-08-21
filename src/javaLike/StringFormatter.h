/*
 * StringFormatter.h
 *
 *  Created on: May 6, 2021
 *      Author: leandro
 */

#ifndef SRC_UTIL_STRINGFORMATTER_H_
#define SRC_UTIL_STRINGFORMATTER_H_

#include<JavaLike.h>
#include<stdio.h>

class StringFormatter
{
public:
        static String format(const char *formato, ...)
        {
            String result;

            va_list args;
            va_start(args, formato);
            result = formatVarArgs(formato, &args);
            va_end(args);

            return result;
        }

        static String formatVarArgs(const char *formato, va_list *args) {
            String result;
            char *tempBuffer = null; //TODO: review if it is better to use a fixed buffer and truncate logs length
            if(vasprintf(&tempBuffer, formato, *args)) {
                result = tempBuffer;
                free(tempBuffer);
            }

            return result;
        }
};



#endif /* SRC_UTIL_STRINGFORMATTER_H_ */
