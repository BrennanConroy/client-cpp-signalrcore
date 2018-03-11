// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#include <iostream>
#include "HttpConnection.h"
#include "WebSocketsTransport.h"

int main(void)
{
	auto s = signalr::WebSocketsTransport();
	s.Start().wait();
	//auto httpConnection = signalr::HttpConnection(L"http://localhost:5000/default/negotiate");
	//httpConnection.start();
	//auto hubConnection = HubConnection(url);
	//hubConnection.On("send", msg => { std::cout << msg << std::endl; });
	//hubConnection.Start();
	//hubConnection.Invoke("Send", "test");
	std::string k;
	while (true)
	{
		std::cin >> k;
		if (k == "s")
			break;
		web::json::value args{};
		args[0] = web::json::value(utility::conversions::to_string_t(k));

		s.Send(L"Send", args);
	}
}

class HubConnection
{
public:
	HubConnection(std::string url)
	{
	}
};