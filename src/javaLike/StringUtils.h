/*
 * StringUtils.h
 *
 *  Created on: Oct 9, 2022
 *      Author: leandro
 */

#ifndef SRC_JAVALIKE_STRINGUTILS_H_
#define SRC_JAVALIKE_STRINGUTILS_H_

#include <JavaLike.h>
#include <sstream>
#include <vector>

class StringUtils {
public:
	//TODO: this should return a new string?
	static String trim(String text) {
		text.erase(text.begin(), std::find_if(text.begin(), text.end(),
				[](unsigned char ch) {
		        	return !std::isspace(ch);
		    	}
		));

		text.erase(std::find_if(text.rbegin(), text.rend(),
				[](unsigned char ch) {
					return !std::isspace(ch);
				}
		).base(), text.end());

		return text;
	}

	//TODO: this should return a new string?
	static String toLowercase(String text) {
		std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c){ return std::tolower(c); });
		return text;
	}

	static std::vector<String> split(String text, char separator) {
		std::vector<String> result;

		String token;
		std::istringstream stream(text);
		while(std::getline(stream, token, separator)) {
			result.push_back(token);
		}

		return result;
	}

	static String join(std::vector<String> &tokens, char separator) {
		String result;

		for(std::vector<String>::iterator tokenIterator = tokens.begin(); tokenIterator != tokens.end(); tokenIterator++) {
			result += *tokenIterator;

			if(tokenIterator != tokens.end() - 1 ) {
				result += separator;
			}
		}

		return result;
	}
};
#endif /* SRC_JAVALIKE_STRINGUTILS_H_ */
