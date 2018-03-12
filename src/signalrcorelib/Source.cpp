// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#include <iostream>
#include "HubConnection.h"
#include "WebSocketsTransport.h"

int main(void)
{
	auto hubConnection = signalr::HubConnection(L"http://localhost:5000/default", Transport::WebSockets);
	hubConnection.Start().wait();
	std::string msg;
	while (true)
	{
		std::cin >> msg;
		if (msg == "s")
			break;
		web::json::value args{};
		args[0] = web::json::value(utility::conversions::to_string_t(msg));

		//hubConnection.Send(L"Send", args);
	}

	hubConnection.Stop().wait();
}