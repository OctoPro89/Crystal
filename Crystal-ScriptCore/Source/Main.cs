using System;
using System.Runtime.CompilerServices;

namespace Crystal
{
	public static class InternalCalls
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static string NativeLog(string text, int perameter);
	}
	public class Entity
	{
		public float FloatVar { get; set; }

		public Entity()
		{
			Console.WriteLine("Main Constructor");
			InternalCalls.NativeLog("C# is cool", 069);
		}

		public void PrintMessage(string message)
		{
			Console.WriteLine(message);
		}
	}
}
