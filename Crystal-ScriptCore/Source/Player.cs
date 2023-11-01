using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Crystal;

namespace Sandbox
{
	public class Player : Entity
	{
		void OnCreate()
		{
			Console.WriteLine("Player Script OnCreate");
		}

		void OnUpdate(float ts)
		{

		}
	}
}
