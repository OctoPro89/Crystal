using System;
using System.Runtime.CompilerServices;

namespace Crystal
{
	public class Main
	{
		public float FloatVar { get; set; }

		public Main()
		{
			Console.WriteLine("Main Constructor");
			NativeLog("C# is cool", 069);
		}

		public void PrintMessage(string message)
		{
			Console.WriteLine(message);
		}

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern static string NativeLog(string text, int perameter);
	}
}
