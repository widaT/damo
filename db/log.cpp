#include <assert.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
//#include <errno.h>
#include <fcntl.h>
//#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include "log.h"
#include "ini.h"

//#define APP_MAX_LOG_CNT	1000
static int app_has_init = 0;
static int app_log_level;
static u_int app_log_size;
static char app_log_pre[32] = {0};
static char app_log_dir[256] = {0};
static int app_log_hour = 0;
static struct fds_t {
	int opfd;
	u_short day;
} app_fds_info[_APP_TRACE + 1];
static char* app_log_buffer = (char*)MAP_FAILED;
//init log
static int a  = app_log_init(config::INI::GetInstance()->Read("base","logpath").c_str(), _APP_TRACE, "log.", 0);

static inline void app_log_file_name(int lvl, char* file_name, time_t now)
{
	struct tm tm;
	localtime_r(&now, &tm);
	if (app_log_hour){
			sprintf (file_name, "%s/%s%s%04d_%02d_%02d_%02d", app_log_dir, app_log_pre,
							((char*[]){
								"emerg", "alert", "crit",
								"error", "warn", "notice",
								".", "debug", "trace"
							})[lvl],
							tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,tm.tm_hour);
	}else{
			sprintf (file_name, "%s/%s%s%04d_%02d_%02d", app_log_dir, app_log_pre,
							((char*[]){
								"emerg", "alert", "crit",
								"error", "warn", "notice",
								".", "debug", "trace"
							})[lvl],
							tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	}
}


static int app_open_fd(int lvl, time_t now)
{
	char file_name[FILENAME_MAX];
	struct tm *tm;
	int val;

	app_log_file_name (lvl, file_name, now);
	app_fds_info[lvl].opfd = open (file_name, O_WRONLY|O_CREAT|O_APPEND, 0644);
	if (app_fds_info[lvl].opfd > 0) {
		tm = localtime (&now);
		app_fds_info[lvl].day = tm->tm_yday;
		
		val = fcntl(app_fds_info[lvl].opfd, F_GETFD, 0);
		val |= FD_CLOEXEC;
		fcntl(app_fds_info[lvl].opfd, F_SETFD, val);
	}
	return app_fds_info[lvl].opfd;
}

static int app_shift_fd (int lvl, time_t now)
{
	off_t length; 
	struct tm *tm;
	
	char file_name[FILENAME_MAX];
	app_log_file_name (lvl, file_name, now);

	//文件不存在
	if (access (file_name, W_OK)){
		//fprintf (stderr, "file not exist \n");
		if(app_fds_info[lvl].opfd > 0) close (app_fds_info[lvl].opfd);
		if (unlikely (app_open_fd (lvl, now) < 0))
			return -1;
	//存在
	}else{
		//fprintf (stderr, "file  exist \n");
		if (app_fds_info[lvl].opfd < 0 && unlikely (app_open_fd (lvl, now) < 0))
					return -1;

	}
	length = lseek (app_fds_info[lvl].opfd, 0, SEEK_END);
	tm = localtime (&now);
	if (app_fds_info[lvl].day == tm->tm_yday)
		return 0;

	close (app_fds_info[lvl].opfd);
	return app_open_fd (lvl, now);
}

void app_write_log (int lvl, const char* fmt, ...)
{
	struct tm tm;
	int pos, end;
	va_list ap;
	time_t now;

	if (lvl > app_log_level)
		return ;

	now = time (NULL); 

	if (unlikely (app_shift_fd (lvl, now) < 0))
		return ;
	localtime_r(&now, &tm);

	pos = sprintf (app_log_buffer, "%02d:%02d:%02d,",
				tm.tm_hour, tm.tm_min, tm.tm_sec);
	va_start(ap, fmt);
	end = vsprintf (app_log_buffer + pos, fmt, ap);
	va_end(ap);
	write (app_fds_info[lvl].opfd, app_log_buffer, end + pos);
}

int app_log_init (const char* dir, int lvl, const char* pre_name,int  prehour)
{
	int i, ret_code = -1;

	if (access (dir, W_OK)) {
		fprintf (stderr, "access log dir %s error, %m\n", dir);
		return -3;
	}

	if (app_log_buffer == MAP_FAILED) {
		app_log_buffer = (char*)mmap (0, 4096*6, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (app_log_buffer == MAP_FAILED) {
			fprintf (stderr, "mmap log buffer failed, %m\n");
			return -4;
		}
	}

	strncpy (app_log_dir, dir, sizeof (app_log_dir) - 1);
	if (pre_name != NULL)
		strncpy (app_log_pre, pre_name, sizeof (app_log_pre) - 1);
	//app_log_size = size;
	app_log_level = lvl;
	app_log_hour = prehour;

	for (i = _APP_EMERG; i <= _APP_TRACE; i++) {
		app_fds_info[i].opfd = -1;
	}
	app_has_init = 1;
    return 0;
}

