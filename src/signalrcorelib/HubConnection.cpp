// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#include "HubConnection.h"
#include "WebSocketsTransport.h"

namespace signalr
{
	enum MessageType
	{
		Negotiation = 0,
		Invocation,
		StreamInvocation,
		StreamItem,
		Completion,
		CancelInvocation,
		Ping
	};

	HubConnection::HubConnection(const utility::string_t& url, Transport transport)
	{
		mUrl = url;
		mTransport = new WebSocketsTransport(mUrl);
		mTransport->OnReceived([](const utility::string_t& message)
		{
			auto value = web::json::value::parse(message.substr(0, message.size() - 1));
			auto messageType = value[L"type"];
			switch (messageType.as_integer())
			{
			case MessageType::Negotiation:
				break;
			case MessageType::Invocation:
			{
				auto method = value[L"target"];
				auto args = value[L"arguments"];
				break;
			}
			case MessageType::StreamInvocation:
				break;
			case MessageType::StreamItem:
				break;
			case MessageType::Completion:
				break;
			case MessageType::CancelInvocation:
				break;
			case MessageType::Ping:
				break;
			}
		});
	}

	pplx::task<void> HubConnection::Start()
	{
		return mTransport->Start()
			.then([&]()
		{
			return SendCore(L"{\"protocol\":\"json\"}\x1e");
		});
	}

	pplx::task<void> HubConnection::Stop()
	{
		return mTransport->Stop();
	}

	pplx::task<void> HubConnection::SendCore(const utility::string_t& message)
	{
		//_ASSERT(message.is_array());
		//web::json::value invocation;
		//invocation[L"type"] = web::json::value::value(1);
		//invocation[L"target"] = web::json::value::string(target);
		//invocation[L"arguments"] = message;

		//auto request = web::websockets::client::websocket_outgoing_message();
		//request.set_utf8_message(utility::conversions::to_utf8string(invocation.serialize()) + "\x1e");
		return mTransport->Send(message);
	}

	HubConnection::~HubConnection()
	{
		mTransport->Stop().wait();
		delete mTransport;
	}
}