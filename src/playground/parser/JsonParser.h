/*
 * JsonParser.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#pragma once

#include <Math3d.h>
#include <vector>
#include "JavaLike.h"
#include "TextParser.h"

#define START_OBJECT "{"
#define END_OBJECT "}"
#define VALUE_SEPARATOR ":"
#define START_ARRAY "["
#define END_ARRAY "]"
#define START_VECTOR "<"
#define END_VECTOR ">"

class JsonParser : public TextParser
{
	public:
		JsonParser(FileParser &parser) : TextParser(parser)
		{
			logger = LoggerFactory::getLogger("parser/JsonParser");
		}

		/**
		 * Allows tokens in double quotes and without double quotes
		 */
		String readToken() {
			if(peekToken() == "\"") {
				return readString();
			}

			return fileParser.takeToken();
		}

		String peekToken() {
					return fileParser.peekToken();
		}

		String readElement(String expectedValue, String name)
		{
			String token = fileParser.takeToken();
			if(token != expectedValue) {
				throw std::domain_error("Expected [" + name + "], got [" + token + "] at [" + fileParser.toString() + "]");
			}

			return token;
		}

		String readValueSeparator() {
			return readElement(VALUE_SEPARATOR, "ValueSeparator");
		}

		String readStartVector() {
			return readElement(START_VECTOR, "StartVector");
		}

		String readEndVector() {
			return readElement(END_VECTOR, "EndVector");
		}

		String readStartArray() {
			return readElement(START_ARRAY, "StartArray");
		}

		String readEndArray() {
			return readElement(END_ARRAY, "EndArray");
		}

		String readStartObject()
		{
			return readElement(START_OBJECT, "StartObject");
		}

		String readEndObject()
		{
			return readElement(END_OBJECT, "EndObject");
		}

		vector3 readVector3()
		{
			String token;
			real x = 0.0;
			real y = 0.0;
			real z = 0.0;

			readStartVector();
			x = readReal();
			readElement(",", "comma");
			y = readReal();
			readElement(",", "comma");
			z = readReal();
			readEndVector();

			return vector3(x, y ,z);
		}

		vector2 readVector2()
		{
			String token;
			real x = 0.0;
			real y = 0.0;

			readStartVector();
			x = readReal();
			readElement(",", "comma");
			y = readReal();
			readEndVector();

			return vector2(x, y);
		}

		std::vector<int> readIntegerArray()
		{
			std::vector<int>array;

			readStartArray();
			String token;

			while((token = fileParser.peekToken()) != END_ARRAY && token != FileParser::eof) {
				int value = readInteger();
				array.push_back(value);

				if((token = fileParser.peekToken()) == ",")
					fileParser.takeToken();
			}

			readEndArray();

			return array;
		}

		std::vector<unsigned int> readUnsignedIntegerArray()
    {
      std::vector<unsigned int>array;

      readStartArray();
      String token;

      while((token = fileParser.peekToken()) != END_ARRAY && token != FileParser::eof) {
        unsigned int value = readUnsignedInteger();
        array.push_back(value);

        if((token = fileParser.peekToken()) == ",")
          fileParser.takeToken();
      }

      readEndArray();

      return array;
    }

		std::vector<real> readRealArray()
    {
      std::vector<real>array;

      readStartArray();
      String token;

      while((token = fileParser.peekToken()) != END_ARRAY && token != FileParser::eof) {
        real value = readReal();
        array.push_back(value);

        if((token = fileParser.peekToken()) == ",")
          fileParser.takeToken();
      }

      readEndArray();

      return array;
    }

		std::vector<vector2> readVector2Array()
		{
			std::vector<vector2>array;

			readStartArray();
			String token;

			while((token = fileParser.peekToken()) != END_ARRAY && token != FileParser::eof) {
				vector2 vec = readVector2();
				array.push_back(vec);

				if((token = fileParser.peekToken()) == ",")
					fileParser.takeToken();
			}

			readEndArray();

			return array;
		}

		std::vector<vector3> readVector3Array()
		{
			std::vector<vector3>array;

			readStartArray();
			String token;

			while((token = fileParser.peekToken()) != END_ARRAY && token != FileParser::eof) {
				vector3 vec = readVector3();
				array.push_back(vec);

				if((token = fileParser.peekToken()) == ",")
					fileParser.takeToken();
			}

			readEndArray();

			return array;
		}

		std::vector<String> readStringArray()
		{
			std::vector<String>array;

			readStartArray();
			String token;

			while((token = fileParser.peekToken()) != END_ARRAY && token != FileParser::eof) {
				String string = readString();
				array.push_back(string);

				if((token = fileParser.peekToken()) == ",")
					fileParser.takeToken();
			}

			readEndArray();

			return array;
		}
};
