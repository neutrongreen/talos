//include logging file
#include "logging/logging.h"

//setup log initzation. just sets inetral file to sent file
Logger::Logger(const char* file){
  filelocation = file;
}

void Logger::log(char stringbuf[]){
  //create file varbale
  FILE *fp;
  //open file form recorded adress
  //make sure you can onyl append to file
  fp = fopen(filelocation, "a+");
  //write to file stringbuf
  fputs(stringbuf, fp);
  //close file to cleanup prefonmanes
  //might need to change if updates happen frequently
  fclose(fp);
}
