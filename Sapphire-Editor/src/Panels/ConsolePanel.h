#pragma once
#include "crystalpch.h"

class MsgType
{
public:
	MsgType(int Type, std::string Msg)
		: type(Type), msg(std::move(Msg)) {}

	int type;
	std::string msg;
};

namespace Crystal
{
	class ConsolePanel
	{
	public:
		ConsolePanel();

		void OnImGuiRender();
		void Log(std::string message);
		void Warn(std::string message);
		void Error(std::string message);
		void Clear();
	private:
		std::vector<MsgType> Messages =
		{

		};
	};
}
