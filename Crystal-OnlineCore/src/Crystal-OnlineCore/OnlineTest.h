#pragma once
#include <OctoServer.hpp>

namespace Crystal
{
	class OnlineTest : OctoNet::Server
	{
		using OctoNet::Server::Server; /* Get the constructor */
	public:
		inline virtual void OnConnect(OctoNet_TCPConnection* newConnection) override
		{
			printf("WORKING LETS GOOO");
		}
	};
}
