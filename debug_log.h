#ifndef __debug_log__
#define __debug_log__

//#define __WRITE_FILE__

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

void DEBUG_LOG(const char *msg, ...);

class Debug_LOG {
private:
	Debug_LOG();
	~Debug_LOG();
	void create_log_file();
public:
	static Debug_LOG *log();
	void write_log(const char *msg);
private:
	static Debug_LOG *m_log;
	time_t tim;
	struct tm *t;
	FILE *fp;
	char filepath[32];
	char message[256];
	struct tm last_log_time;
};

#endif
