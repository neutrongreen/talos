#ifndef _LOGGING
//includi stdio for file input output
#include "main.h"

class Logger{
  public:
    //constructer that needs filelocation for log file
    Logger(const char* file);
    //longs uisng a cosnt character of undefined legnth to write to file
    void log(char stringbuf[]);
  protected:
    const char* filelocation;
};



#endif /* end of include guard:  */
