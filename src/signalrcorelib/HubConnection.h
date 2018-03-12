// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#pragma once

#include <cpprest\uri.h>
#include "Transports.h"
#include "ITransport.h"

namespace signalr
{
	class HubConnection
	{
	public:
		HubConnection(const utility::string_t& url, Transport transport = Transport::WebSockets);
		pplx::task<void> Start();
		pplx::task<void> Stop();
		//On
		//Stream
		//Invoke
		pplx::task<void> Send(const utility::string_t& target, const utility::string_t& arguments = L"");

		~HubConnection();
	private:
		pplx::task<void> SendCore(const utility::string_t& message);
		utility::string_t mUrl;
		ITransport* mTransport;
	};
}