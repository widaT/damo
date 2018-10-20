#ifndef _LOG_H
#define _LOG_H
#include <sys/cdefs.h>
#include <sys/types.h>
#include <arpa/inet.h>
//#include "func.h"
using namespace std;

#ifndef likely
#define likely(x)  __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)  __builtin_expect(!!(x), 0)
#endif

#define _APP_EMERG 		0  /* system is unusable               */
#define _APP_ALERT		1  /* action must be taken immediately */
#define _APP_CRIT		2  /* critical conditions              */
#define _APP_ERROR		3  /* error conditions                 */
#define _APP_WARNING		4  /* warning conditions               */
#define _APP_NOTICE		5  /* normal but significant condition */
#define _APP_INFO		6  /* informational                    */
#define _APP_DEBUG		7  /* debug-level messages             */
#define _APP_TRACE		8  /* trace-level messages             */

#define _DETAIL(level, fmt, args...) \
	app_write_log (level, "[%s][%d]%s: " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##args) 
	
#define _SIMPLY(level, fmt, args...) app_write_log(level, fmt "\n", ##args)

#define APP_ERROR_LOG(fmt, args...)	_DETAIL(_APP_ERROR, fmt, ##args)
#define APP_CRIT_LOG(fmt, args...)	_DETAIL(_APP_CRIT, fmt, ##args)
#define APP_ALERT_LOG(fmt, args...)	_DETAIL(_APP_ALERT, fmt, ##args)
#define APP_EMERG_LOG(fmt, args...)	_DETAIL(_APP_EMERG, fmt, ##args)

#define APP_WARN_LOG(fmt, args...)	_SIMPLY(_APP_WARNING, fmt, ##args)
#define APP_NOTI_LOG(fmt, args...)	_SIMPLY(_APP_NOTICE, fmt, ##args)
#define APP_INFO_LOG(fmt, args...)	_DETAIL(_APP_INFO, fmt, ##args)
#define APP_DEBUG_LOG(fmt, args...)	_SIMPLY(_APP_DEBUG, fmt, ##args)


#define APP_ERROR_RETURN(X, Y) do{ \
	APP_ERROR_LOG X; \
	return Y; \
}while (0)


/*
 * log api
 */
//__attr_cdecl__ __attr_nonnull__(1)

int app_log_init (const char* dir, int lvl, const char* pre_name,int pre_hour);

//__attr_cdecl__ __attr_nonnull__(2)
void app_write_log (int lvl, const char* fmt, ...);

//__attr_cdecl__ __attr_nonnull__(3)
//extern void boot_log (int OK, int dummy, const char* fmt, ...);

#endif
