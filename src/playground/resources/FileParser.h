/*
 * FileParser.h
 *
 *  Created on: 03/02/2013
 *      Author: Lean
 */

#pragma once

#include <string>
#include<stdio.h>
#include "JavaLike.h"
#include "Logger.h"

class FileParser {
public:
    inline static const String eof = {-1};
    inline static const String eol = "\n";

    static const char charEof { (char)-1 };
    static const char charNewl {'\n'};
    static const char charEol {'\0'};

private:
    String filename;
    FILE *fileStream = null;
    Logger *logger = LoggerFactory::getLogger("resources/FileParser");

    const char *tokenSeparator;
    const char *blanks;
    char commentChar;

    unsigned int line;
    unsigned int column;

    //TODO: Move this to a stack and make its methods public
    unsigned int lineBackup;
    unsigned int columnBackup;
    fpos_t position;

    bool isInSet(char character, const char *set) {

        const char *currentTokenSeparator = set;
        while (*currentTokenSeparator != '\0') {
            if (*currentTokenSeparator == character) {
                return true;
            }

            currentTokenSeparator++;
        }

        return false;
    }

    bool isTokenSeparator(char character) {
        return isInSet(character, tokenSeparator) || character == FileParser::charEof ;
    }

    bool isBlank(char character) {
        return isInSet(character, blanks);
    }

    void takeUntilEOL() {
        char caracter;
        while ((caracter = takeByte()) != FileParser::charNewl && caracter != FileParser::charEof )
            ;
    }

    void takeBlanks() {
        while (isBlank(takeByte()))
            ;

        if (feof(fileStream))
            fseek(fileStream, 0, SEEK_END);
        else
            fseek(fileStream, -1, SEEK_CUR);
    }

    void takeBlanksAndComments() {
        char character;
        char exit;

        do {
            exit = true;
            while (isBlank(character = takeByte()))
                ;
            if (character == commentChar) {
                takeUntilEOL();
                exit = false;
            }
        } while (!exit);

        if (feof(fileStream))
            fseek(fileStream, 0, SEEK_END);
        else
            fseek(fileStream, -1, SEEK_CUR);
    }

    void popPosition() {
        line = lineBackup;
        column = columnBackup;

        if (fsetpos(fileStream, &position) != 0)
            throw std::range_error("Could not restore stream position");
    }
    void pushPosition() {
        lineBackup = line;
        columnBackup = column;

        if (fgetpos(fileStream, &position) != 0)
            throw std::range_error("Could not save stream position");

    }

public:
    FileParser(const String filename) {
        if (filename == "")
            throw std::invalid_argument("FileName can't be null");

        if ((fileStream = fopen(filename.c_str(), "rb")) == null) {
            logger->error("Error opening [%s]", filename.c_str());
            throw std::invalid_argument("Error opening [" + filename + "]");
        }

        this->filename = filename;

        logger->verbose("File opened for reading binary [%s]", this->filename.c_str());

        this->setDefaultSpecialCharacters();

        position = -1;
        line = 1;
        column = 1;
        lineBackup = line;
        columnBackup = column;
    }

    /* Rule of five and copy-and-swap*/
    friend void swap(FileParser& first, FileParser& second)
	{
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;

		// by swapping the members of two objects, the two objects are effectively swapped
		swap(first.fileStream, second.fileStream);
		swap(first.filename, second.filename);
		swap(first.position, second.position);
		swap(first.line, second.line);
		swap(first.column, second.column);
		swap(first.lineBackup, second.lineBackup);
		swap(first.columnBackup, second.columnBackup);
	}

    FileParser(const FileParser &other) : FileParser(other.filename) {
    	fseek(this->fileStream, ftell(other.fileStream), SEEK_SET);

    	this->position = other.position;
    	this->line = other.line;
    	this->lineBackup = other.lineBackup;
    	this->columnBackup = other.columnBackup;
    }

    FileParser(FileParser && other) {
    	swap(*this, other);
    }

    FileParser &operator =(FileParser other) {
    	swap(*this, other);
    	return *this;
    }

    ~FileParser() {
        this->close();
    }

    FILE* getStream() {
		return fileStream;
	}

    /**
     * returns the number of elements (of size_t) actually read. If that number < count then there was an error reading or reached eof.
     */
    size_t read(void *data, size_t size, size_t count) {
        return fread(data, size, count, fileStream);
    }

    bool skip(size_t size, size_t count) {
        if (fseek(fileStream, size * count, SEEK_CUR) != 0) {
            if (feof(fileStream))
                fseek(fileStream, 0, SEEK_END);

            return false;
        }

        return true;
    }

    void close() {
        if (fileStream != null) {
            logger->verbose("Closing file [%s]", filename.c_str());
            fclose(fileStream);
            fileStream = null;
        }
    }

    const String& getFilename() const {
        return filename;
    }

    int peekByte() {
        pushPosition();
        int byte = takeByte();
        popPosition();

        return byte;
    }

    int takeByte() {
        int byte = fgetc(fileStream);
        //logger->info("Took byte [%d] ['%c'] from %s", byte, byte, getFilename().c_str());
        if ((char) byte == FileParser::charNewl) {
            line++;
            column = 0;
        } else if ((char) byte != '\r' && (char) byte != FileParser::charEof) {
            column++;
        }

        return byte;
    }

    String peekLine() {
        pushPosition();
        String line = takeLine();
        popPosition();

        return line;
    }
    String takeLine() {
        String line;
        char character;

        while ((character = takeByte()) != FileParser::charNewl && character != FileParser::charEof )
            line += character;

        return line;
    }

    String peekToken() {
        pushPosition();
        String token = takeToken();
        popPosition();
        return token;
    }

    String takeToken() {
        char tokenBuffer[256] = "";

        char *token = tokenBuffer;

        unsigned short longitud = 0;

        takeBlanksAndComments();

        while (!isTokenSeparator(*token = takeByte())) {
            token++;
            longitud++;
        }

        if (longitud == 0)
            *(token + 1) = FileParser::charEol;
        else {
            *token = FileParser::charEol;
            fseek(fileStream, -1, SEEK_CUR);
        }

        return (tokenBuffer);
    }

    unsigned int getColumn() const {
        return column;
    }

    unsigned int getLine() const {
        return line;
    }

    const char* getTokenSeparator() const {
        return tokenSeparator;
    }

    void setTokenSeparator(const char *tokenSeparator) {
        this->tokenSeparator = tokenSeparator;
    }

    void setDefaultSpecialCharacters() {
        tokenSeparator = "	# ()[]{},.:;<>+-*/^®=|&!ø?\n\r\"\'\\"; // + std::to_string((char)-1)).c_str();
        blanks = " \n\r\t";
        commentChar = '#';
    }
    unsigned int size() {
        pushPosition();
        fseek(fileStream, 0, SEEK_END);
        unsigned int size = ftell(fileStream);
        popPosition();
        return size;
    }

    String  toString() {
    	return String(this->filename + ":(" + std::to_string(this->getLine()) + ":" + std::to_string(this->getColumn()) + ")");
    }
};
