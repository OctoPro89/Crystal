using System;

namespace Crystal
{
	public class Main
	{
		public float FloatVar { get; set; }

		public Main()
		{
			Console.WriteLine("Main Constructor");
		}

		public void PrintMessage(string message)
		{
			Console.WriteLine(message);
		}
	}
}
