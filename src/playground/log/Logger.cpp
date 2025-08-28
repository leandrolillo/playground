/*
 * Logger.cpp
 *
 *  Created on: Aug 16, 2022
 *      Author: leandro
 */


#include "Logger.h"

LogLevel LoggerFactory::defaultLogLevel = DEFAULT_LOG_LEVEL;
String LoggerFactory::logFileName = "playground.log";
std::set<std::unique_ptr<Logger>> LoggerFactory::loggers;
std::set<std::unique_ptr<Appender>> LoggerFactory::appenders;
