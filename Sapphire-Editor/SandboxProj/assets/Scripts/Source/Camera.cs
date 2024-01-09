using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Crystal;

namespace Sandbox
{
	public class Camera : Entity
	{
		private Entity player;
		bool FollowPlayer = false;
		public float ZfromPlayer;

		void OnCreate()
		{
			player = FindEntityByName("Player");
		}
		//cool beans
		void OnUpdate(float ts)
		{
			if (player != null && FollowPlayer)
			{
				Translation = new Vector3(player.Translation.XY, Translation.Z);
			}
			else if (player == null)
			{
				Console.Write("Uh oh!");
			}
		}
	}
}
