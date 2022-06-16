#ifndef __REST_FAST_RESPONSE__
#define __REST_FAST_RESPONSE__

#include <proxygen/httpserver/ResponseBuilder.h>
#include <string>

namespace rest {
  void sendError(proxygen::ResponseBuilder& builder, int status_code,
		 std::string status_msg, std::string error_msg);
}

#endif
