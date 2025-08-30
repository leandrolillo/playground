/*
 * Logger.h
 *
 *  Created on: 21/01/2013
 *      Author: Lean
 */

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include<sys/stat.h>
#include <set>
#include <map>
#include <string.h>
#include <errno.h>
#include "JavaLike.h"
#include "StringFormatter.h"

enum class LogLevel {
    ERROR=0,
    WARN,
    INFO,
    DEBUG,
    VERBOSE
};

constexpr LogLevel DEFAULT_LOG_LEVEL = LogLevel::INFO;
//constexpr LogLevel DEFAULT_LOG_LEVEL = LogLevel::DEBUG;

/**
 * Appenders
 */
class Appender {
protected:
    String target;
public:
    static struct tm* getCurrentTime() {
        time_t t;
        time(&t);
        return localtime(&t);
    }
public:
    Appender(const String &target) {
        this->target = target;
    }

    virtual ~Appender() {
    }

    const String& getTarget() const {
        return target;
    }
    virtual void append(const char *message) = 0;
    virtual void flush() = 0;
};

class FileAppender: public Appender {
private:
    FILE *fileHandler = null;

public:
    FileAppender(String target) : Appender(target) {
    	getFileHandler();
    }

    /* Rule of five and copy-and-swap*/
    friend void swap(FileAppender& first, FileAppender& second)
	{
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;

		// by swapping the members of two objects, the two objects are effectively swapped
		swap(first.fileHandler, second.fileHandler);
		swap(first.target, second.target);
	}

    FileAppender(const FileAppender &other) : FileAppender(other.target) {
    }

    FileAppender(FileAppender && other) : FileAppender(other.target) {
    	swap(*this, other);
    }

    FileAppender &operator =(FileAppender other) {
    	swap(*this, other);
    	return *this;
    }

    ~FileAppender() {
        if (getFileHandler() != null) {
            fclose(getFileHandler());
        }
    }

    void append(const char *message) override {
        fprintf(getFileHandler(), "%s", message); // use ("%s", message) instead of directly message to get rid of warnings.
    }

    void flush() override {
        FILE *fileHandler = getFileHandler();
        fflush(fileHandler);
    }

protected:
    const char* getFileName() const {
        return this->getTarget().c_str();
    }

    FILE* getFileHandler() {
        if (fileHandler == null) {
#ifdef DEBUG
            printf("File handler is NULL\n");
#endif
            FILE *existingFile;
            if ((existingFile = fopen(getFileName(), "rt"))) {
                struct stat fileStats;
                fclose(existingFile);

                printf("Log file already exists, checking if new version is required...\n");

                if (!stat(getFileName(), &fileStats)) {
                    struct tm *dateTime = localtime(&fileStats.st_ctime);
                    char textBuffer[100];
                    strftime(textBuffer, 100, "%Y%m%d", dateTime);

                    int fileYear = dateTime->tm_year;
                    int fileDay = dateTime->tm_yday;

                    dateTime = Appender::getCurrentTime();

                    if (fileYear != dateTime->tm_year || fileDay != dateTime->tm_yday) {
                        printf("Log file was NOT created today\n");
                        char newfileName[256];
                        snprintf(newfileName, sizeof(newfileName), "%s.%s", getFileName(), textBuffer);
                        printf("Renaming old log file to [%s]\n", newfileName);

                        if (rename(getFileName(), newfileName)) {
                            printf("Error renaming existing log file [%s] to [%s]\n", getFileName(), newfileName);
                            perror(getFileName());
                        }

                        if (remove(getFileName())) {
                            printf("Error removing log file [%s]\n", getFileName());
                            perror(getFileName());
                        }

                        printf("Opening new log file\n");
                        if ((fileHandler = fopen(getFileName(), "wt")) == null) {
                            printf("Error opening logger file [%s]\n", getFileName());
                            perror(getFileName());
                            fileHandler = null;
                        }
                    } else {
                        printf("Appending to existing log file\n");
                        if (!(fileHandler = fopen(getFileName(), "at"))) {
                            printf("Error opening logger file [%s]\n", getFileName());
                            perror(getFileName());
                            fileHandler = null;
                        }
                    }
                }
            } else {
                printf("Appending to existing log file\n");
                if (!(fileHandler = fopen(getFileName(), "wt"))) {
                    printf("Error opening logger file [%s]\n", getFileName());
                    perror(getFileName());
                    fileHandler = null;
                }
            }
        }
        return fileHandler;
    }
};

class StdoutAppender: public Appender {
public:
    StdoutAppender() : Appender("stdout") {

    }

    void append(const char *message) override {
        printf("%s", message); // use ("%s", message) instead of directly message to get rid of warnings.
    }

    void flush() override {
    }
};


/**
 * Logger
 */

class Logger {
protected:
    String basename;
    std::map<LogLevel, std::set<Appender*>> appendersByLogLevel;
    LogLevel logLevel = LogLevel::INFO;

public:
    Logger(const String &basename) {
        this->basename = basename;
    }
//    ~Logger() {
//        printf("Deleting logger %s\n",  this->basename.c_str());
//
//    }

    const String& getBasename() const {
        return this->basename;
    }

    void setLogLevel(LogLevel logLevel) {
        this->logLevel = logLevel;
    }

protected:
    std::map<LogLevel, String> logLevelToString = {{LogLevel::VERBOSE, "VERBOSE"}, {LogLevel::DEBUG, "DEBUG"}, {LogLevel::INFO, "INFO"}, {LogLevel::WARN, "WARNING"}, {LogLevel::ERROR, "ERROR"}};

    void messageAppenders(LogLevel &logLevel, const String &message) {
    	auto iterator = appendersByLogLevel.find(logLevel);

    	if(iterator != appendersByLogLevel.end()) {
			auto &appenders = iterator->second;

			for (auto &appender : appenders) {
				appender->append(message.c_str());
				appender->flush();
			}
    	}
    }

    void printMessage(LogLevel logLevel, const char *formato, va_list *args = null) {
        if (this->appendersByLogLevel.find(logLevel) != appendersByLogLevel.end()) {
            char textBuffer[256];
            strftime(textBuffer, sizeof(textBuffer), "%d/%m/%Y %H:%M:%S", Appender::getCurrentTime());

            messageAppenders(logLevel, StringFormatter::format("%s - %s - %s: ", textBuffer, logLevelToString.at(logLevel).c_str(), basename.c_str()));
            messageAppenders(logLevel, StringFormatter::formatVarArgs(formato, args));
            messageAppenders(logLevel, "\n");
        }
    }

public:
    void addAppender(LogLevel logLevel, Appender *appender) {
        this->appendersByLogLevel[logLevel].insert(appender);
    }

    void addAppender(Appender *appender) {
    	this->appendersByLogLevel[LogLevel::VERBOSE].insert(appender);
		this->appendersByLogLevel[LogLevel::DEBUG].insert(appender);
		this->appendersByLogLevel[LogLevel::INFO].insert(appender);
		this->appendersByLogLevel[LogLevel::ERROR].insert(appender);
		this->appendersByLogLevel[LogLevel::WARN].insert(appender);
	}

    void info(const char *formato, ...) {
        if(logLevel >= LogLevel::INFO) {
            va_list args;
            va_start(args, formato);
            printMessage(LogLevel::INFO, formato, &args);
            va_end(args);
        }
    }

    void debug(const char *formato, ...) {
        if(logLevel >= LogLevel::DEBUG) {
			va_list args;
			va_start(args, formato);
			printMessage(LogLevel::DEBUG, formato, &args);
			va_end(args);
        }
    }
    void error(String message) {
      printMessage(LogLevel::ERROR, message.c_str());
    }
    void error(const char *formato, ...) {
        va_list args;
        va_start(args, formato);
        printMessage(LogLevel::ERROR, formato, &args);
        va_end(args);

        //This does not seem to be very reliable
//        if (errno != 0 && strlen(strerror(errno)) > 0) {
//            printMessage(LogLevel::ERROR, StringFormatter::format("errno [%d]: [%s]", errno, strerror(errno)).c_str(), &args);
//            errno = 0;
//        }
    }

    void warn(const char *formato, ...) {
        if(logLevel >= LogLevel::WARN) {
            va_list args;
            va_start(args, formato);
            printMessage(LogLevel::WARN, formato, &args);
            va_end(args);
        }
    }

    void verbose(const char *formato, ...) {
        if(logLevel >= LogLevel::VERBOSE) {
			va_list args;
			va_start(args, formato);
			printMessage(LogLevel::VERBOSE, formato, &args);
			va_end(args);
        }
    }
};

/**
 * Logger Factory
 */

class LoggerFactory {
private:
    static LogLevel defaultLogLevel;
    static String logFileName;
    static std::set<std::unique_ptr<Appender>> appenders;
    static std::set<std::unique_ptr<Logger>> loggers;


public:
    static void setLogFileName(const String &logFilename) {
      logFileName = logFilename;
    }
    static void doNotLogToFile() {
      logFileName = "";
    }
    static void setDefaultLogLevel(LogLevel defaultLevel) {
      defaultLogLevel = defaultLevel;
    }
    static Appender* getAppender(String output) {
        for (auto &appender : appenders) {
            if (appender->getTarget() == output) {
                return appender.get();
            }
        }

        Appender *appender = null;

        if (output == "stdout") {
            appender = new StdoutAppender();
        } else {
            appender = new FileAppender(output);
        }

        LoggerFactory::appenders.insert(std::unique_ptr<Appender>(appender));
        return appender;
    }

    static Logger* getLogger(String basename) {
        for (auto &logger : loggers) {
            if (logger->getBasename() == basename) {
                return logger.get();
            }
        }

        Logger *logger = new Logger(basename);
        logger->setLogLevel(defaultLogLevel);
        if(!logFileName.empty()) {
          logger->addAppender(getAppender(logFileName));
          logger->addAppender(LogLevel::ERROR, getAppender("stdout"));
        } else {
          logger->addAppender(getAppender("stdout"));
        }


        LoggerFactory::loggers.insert(std::unique_ptr<Logger>(logger));

        return logger;
    }
};
