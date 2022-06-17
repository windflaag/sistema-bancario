#ifndef __COMMON_FAST_RESPONSE__
#define __COMMON_FAST_RESPONSE__

#include <proxygen/httpserver/ResponseBuilder.h>
#include <string>

namespace common {
  void sendError(proxygen::ResponseBuilder& builder, int status_code,
		 std::string status_msg, std::string error_msg);
}

#endif
