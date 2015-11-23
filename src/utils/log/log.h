#include "easylogging++.h"

#define ENABLE_THE_LOGGING START_EASYLOGGINGPP

#define THE_LOG_TRACE()   LOG(TRACE)
#define THE_LOG_DEBUG()   LOG(DEBUG)
#define THE_LOG_INFO()    LOG(INFO)
#define THE_LOG_WARNING() LOG(WARNING)
#define THE_LOG_ERROR()   LOG(ERROR)
#define THE_LOG_FATAL()   LOG(FATAL)

#define THE_LOG_TRACE_IF(condition)   LOG_IF(condition, TRACE)
#define THE_LOG_DEBUG_IF(condition)   LOG_IF(condition, DEBUG)
#define THE_LOG_INFO_IF(condition)    LOG_IF(condition, INFO)
#define THE_LOG_WARNING_IF(condition) LOG_IF(condition, WARNING)
#define THE_LOG_ERROR_IF(condition)   LOG_IF(condition, ERROR)
#define THE_LOG_FATAL_IF(condition)   LOG_IF(condition, FATAL)

#define THE_ERROR(stream, action) THE_LOG_ERROR() << stream; action;
#define THE_ERROR_IF(condition, stream, action) THE_LOG_ERROR_IF(condition) << stream; if(condition) action;