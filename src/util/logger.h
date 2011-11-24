#ifndef __LOG_LOGGER_H_
#define __LOG_LOGGER_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iomanip>
#include <sstream>
#include "hrio/console.h"

#define CREATE_LOGGER(x) static Logger logger(x)
#define MAKE_LOG logger.getStream()
#define LOG_STRUCT logger.getStructStream()
#define LOG_CTOR logger.getStructStream(true)
#define LOG_DTOR logger.getStructStream(false)
#define TO_CONSOLE(str) logger.toConsole(str)


/** Simple and easy to use logging class.
 *
 *  Static functions @ref start and @ref stop should be called in the
 *  beginning and in the end of the program in order to open and close
 *  the log file. Then any number of Logger objects could be instantiated
 *  each with its own name, which will be appended to all log messages
 *  created by the object.
 *
 *  In order to actually log something, a function @ref getStream should
 *  be called. It will return a filestream object where arbitrary messages
 *  can be written with << operator.
 *
 *  It also appends the current timestamp and object (module) name to the
 *  stream.
 *
 *  A second log file is created just to log the construction and destruction
 *  of all classes.
 */
class Logger {
  public:

    /** Constructor.
     *  Create a Logger instance with a specific name (which will appear
     *  in all the log entries made by this object).
     *
     *  @param n : The object (module) name appended to the stream.
     */
    Logger(const std::string& n);

    /** Get a reference to the filestream where a message can be logged.
     *
     * Current timestamp and the object name are written to the stream
     * before returning.
     */
    std::ofstream& getStream();

    /** Get a reference to the filestream for construction/destruction logging.
     *
     *  @param ctor : Set true if constructing, or false if destructing.
     *
     *  @return Reference to filestream for constructors and destructors.
     */
    std::ofstream& getStructStream(bool ctor);

    /** Get a reference to the filestream for construction/destruction logging.
     *
     *  @return Reference to filestream for constructors and destructors.
     */
    std::ofstream& getStructStream();

    void toConsole(const std::string str);

    static void setConsole(Console& console);

    /** Open a new file for logging.
     *
     *  Should be called at the beginning of the main method.
     *
     *  @param robot : The name for the beginning of the log file.
     */
    static void start(const std::string& robot);

    /** Close the log file.
     *
     *  Should be called at the end of the main method or when all logging
     *  is finished for the duration of the program.
     */
    static void stop();

  private:

    /** Hide default constructor. */
    Logger();

    /** Name of object (module). */
    std::string name;

    /** Counter for number of Constructions. */
    static int ctor;

    /** Counter for number of Destructions. */
    static int dtor;

    /** Log stream to which all objects write. */
    static std::ofstream logStream;

    /** Log stream for all construction/destruction. */
    static std::ofstream structStream;

    /** Console to log too. */
    static Console* console;
};
#endif
