// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#include "WebSocketsTransport.h"

namespace signalr
{
	WebSocketsTransport::WebSocketsTransport()
	{
		mWebSocket = web::web_sockets::client::websocket_callback_client();
	}

	pplx::task<void> WebSocketsTransport::Start()
	{
		return mWebSocket.connect(L"ws://localhost:5000/default")
		.then([=]()
		{
			auto negotiate = web::websockets::client::websocket_outgoing_message();

			negotiate.set_utf8_message("{\"protocol\":\"json\"}\x1e");
			mWebSocket.set_message_handler([](const web::websockets::client::websocket_incoming_message &message)
			{
				return message.extract_string().then([](const std::string& response)
				{
					std::cout << response << std::endl;
				});
			});

			return mWebSocket.send(negotiate);
		});
	}

	pplx::task<void> WebSocketsTransport::Send(const utility::string_t& target, const web::json::value& message)
	{
		web::json::value invocation;
		invocation[L"type"] = web::json::value::value(1);
		invocation[L"target"] = web::json::value::string(target);
		invocation[L"arguments"] = message;

		auto request = web::websockets::client::websocket_outgoing_message();
		request.set_utf8_message(utility::conversions::to_utf8string(invocation.serialize()) + "\x1e");

		return mWebSocket.send(request);
	}

	WebSocketsTransport::~WebSocketsTransport()
	{
		mWebSocket.close().wait();
	}
}