﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Crystal;

namespace Sandbox
{
	public class CamController : Entity
	{
		private Entity player;
		bool FollowPlayer = true;
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
