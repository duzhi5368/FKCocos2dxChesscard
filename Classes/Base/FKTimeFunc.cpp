#include "FKTimeFunc.h"
//------------------------------------------------------------
unsigned long FKTimeFunc::GetMillisecondNow()
{
	struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}
//------------------------------------------------------------