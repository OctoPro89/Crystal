using Crystal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class FollowCamera : Entity
{
	private ulong id;

	void OnCreate()
	{
		id = ID;
	}

	void OnUpdate(float ts)
	{
		Console.WriteLine("ID: " + id);

		if (ID != id)
			Console.WriteLine("Incorrect ID!");

		this.Translation = this.Translation;
	}
}
