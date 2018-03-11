// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#pragma once

#include <cpprest\ws_client.h>
#include <cpprest\json.h>

namespace signalr
{
	class WebSocketsTransport
	{
	public:
		WebSocketsTransport();
		pplx::task<void> Start();
		pplx::task<void> Send(const utility::string_t& target, const web::json::value& arguments);

		~WebSocketsTransport();
	private:
		web::web_sockets::client::websocket_callback_client mWebSocket;
	};
}