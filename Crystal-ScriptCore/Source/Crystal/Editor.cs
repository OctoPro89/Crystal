using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Crystal
{
	public class Editor
	{
		public static void Log(string message)
		{
			InternalCalls.Editor_ConsoleLog(message);
		}

		public static void Warn(string message)
		{
			InternalCalls.Editor_ConsoleWarn(message);
		}

		public static void Error(string message)
		{
			InternalCalls.Editor_ConsoleError(message);
		}

		public static void ClearConsole()
		{
			InternalCalls.Editor_ConsoleClear();
		}
	}
}
