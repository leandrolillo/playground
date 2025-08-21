/*
 * TextParser.h
 *
 *  Created on: Apr 14, 2021
 *      Author: leandro
 */

#ifndef SRC_PARSER_TEXTPARSER_H_
#define SRC_PARSER_TEXTPARSER_H_

#include <FileParser.h>
#include <Math3d.h>
#include <ParsingException.h>

class TextParser {
protected:
    Logger *logger = LoggerFactory::getLogger("parser/TextParser");
    FileParser &fileParser;

public:
    TextParser(FileParser &parser) : fileParser(parser)
    {
    }

    unsigned int getColumn() const
    {
        return fileParser.getColumn();
    }

    unsigned int getLine() const
    {
        return fileParser.getLine();
    }


    String readString()
    {
        String value;
        String token = fileParser.takeToken();
        if(token != "\"")
            throw ParsingException("Expected %s, got [%s] at %s", "\"", token.c_str(), fileParser.toString().c_str());

        char character;
        while((character = fileParser.takeByte()) != '\"' && character != FileParser::charEof) {
            value += character;
        }

        return value;
    }

    bool readBoolean()
    {
        String token = fileParser.takeToken();

        if(token == "true") {
            return true;
        } else if (token == "false") {
            return false;
        }

        throw ParsingException("Expected [true|false], got [%s] at %s", token.c_str(), fileParser.toString().c_str());
    }

    unsigned int readUnsignedInteger() {
        unsigned int value = 0;

        String token = fileParser.takeToken();
        char digit;
        bool readValue = false;

        unsigned int currentPosition = 0;
        while(currentPosition < token.length() && '0' <= (digit = token.at(currentPosition++)) && digit <= '9')
        {
            value = value * 10 + (unsigned int)(digit - '0');
            readValue = true;
        }

        if(!readValue)
            throw ParsingException("Unexpected %s at %s", fileParser.takeToken().c_str(), fileParser.toString().c_str());

        return value;
    }


    int readInteger() {
        int sign = readSign();
        int unsignedInteger = readUnsignedInteger();
        return (int)unsignedInteger * sign;
    }

    real readReal() {
        fileParser.setTokenSeparator("  # ()[]{},.:;<>+-*/^�=|&!�?\n\r\"\'\\eE�");
        real mantissa = readMantissa();
        int exponent = readExponent();

        fileParser.setDefaultSpecialCharacters();

        return mantissa * pow(10.0, exponent);
    }

    String takeToken() {
        return fileParser.takeToken();
    }

    String peekToken() {
        return fileParser.peekToken();
    }

    String takeLine() {
    	return fileParser.takeLine();
    }

    const String &getFilename() const {
    	return fileParser.getFilename();
    }

protected:
    real readDecimals()
    {
        real value = 0;
        real modifier = 0.1;

        String token = fileParser.takeToken();
        char digit;
        bool readValue = false;

        unsigned int currentPosition = 0;
        while(currentPosition < token.length() && '0' <= (digit = token.at(currentPosition++)) && digit <= '9')
        {
            value = value  + (real)(digit - '0') * modifier;
            modifier *= 0.1;
            readValue = true;
        }

        if(!readValue)
            throw ParsingException("Unexpected %s at %s", fileParser.takeToken().c_str(), fileParser.toString().c_str());

        return value;
    }

    real readMantissa()
    {
        real integer = 0.0;
        real decimal = 0.0;

        real mantissaSign = readSign();

        if(fileParser.peekToken() != ".")
            integer = readUnsignedInteger();

        if(fileParser.peekToken() == ".") {
            fileParser.takeToken();

            String token = fileParser.peekToken();

            if('0' <= token.at(0) && token.at(0) <= '9')
                decimal = readDecimals();
        }

        return mantissaSign * (integer + decimal);

    }

    int readExponent()
    {
        real value = 0.0;
        String token = fileParser.peekToken();
        if(token.at(0) == 'e' || token.at(0) == 'E') {
            fileParser.takeToken();
            value = readInteger();
        }
        return value;
    }
    char readSign()
    {
        int sign = 1;
        String token = fileParser.peekToken();
        if(token == "-") {
            sign = -1;
            fileParser.takeToken();
        } else if(token == "+")
            fileParser.takeToken();

        return sign;
    }

    String toString() {
    	return fileParser.toString();
    }

};



#endif /* SRC_PARSER_TEXTPARSER_H_ */
