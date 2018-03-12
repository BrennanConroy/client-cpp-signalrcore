// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#include "HubConnection.h"
#include "WebSocketsTransport.h"

namespace signalr
{
	enum MessageType
	{
		Negotiation = 0,
		Invocation = 1,
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
		mTransport->OnReceived([&](const utility::string_t& message)
		{
			auto value = web::json::value::parse(message.substr(0, message.size() - 1));
			auto messageType = value[L"type"];
			switch (messageType.as_integer())
			{
			case MessageType::Negotiation:
				// unused...
				break;
			case MessageType::Invocation:
			{
				auto method = value[L"target"];
				auto args = value[L"arguments"];
				_ASSERT(args.is_array());
				auto m = method.serialize();
				// serializing a string creates "method", strip quotes, also figure out a better way to do this
				auto handler = mHandlers.find(m.substr(1, m.size() - 2));
				if (handler != mHandlers.end())
				{
					(*handler).second(args.serialize());
				}
				break;
			}
			case MessageType::StreamInvocation:
				// Sent to server only, should not be received by client
				throw std::runtime_error("Received unexpected message type 'StreamInvocation'.");
			case MessageType::StreamItem:
				// TODO
				break;
			case MessageType::Completion:
				// TODO
				break;
			case MessageType::CancelInvocation:
				// Sent to server only, should not be received by client
				throw std::runtime_error("Received unexpected message type 'CancelInvocation'.");
			case MessageType::Ping:
				// TODO
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

	pplx::task<void> HubConnection::Send(const utility::string_t& target, const utility::string_t& arguments)
	{
		auto args = web::json::value::parse(arguments);
		_ASSERT(args.is_array());

		web::json::value invocation;
		invocation[L"type"] = web::json::value::value(MessageType::Invocation);
		invocation[L"target"] = web::json::value::string(target);
		invocation[L"arguments"] = args;
		return SendCore(invocation.serialize() + L"\x1e");
	}

	pplx::task<void> HubConnection::SendCore(const utility::string_t& message)
	{
		return mTransport->Send(message);
	}

	// TODO: Multiple functions registered on the same method
	// TODO: Ability to unregister functions
	void HubConnection::On(const utility::string_t& method, std::function<void(const utility::string_t&)> func)
	{
		mHandlers[method] = func;
	}

	HubConnection::~HubConnection()
	{
		mTransport->Stop().wait();
		delete mTransport;
	}
}