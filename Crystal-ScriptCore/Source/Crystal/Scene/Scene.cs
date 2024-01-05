using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Crystal
{
	public class Scene
	{
		static void CreateEntity(out ulong UUID)
		{
			InternalCalls.Scene_CreateEntity(out UUID);
		}

		static void DestroyEntity(ulong UUID)
		{
			InternalCalls.Scene_DestroyEntity(UUID);
		}
	}
}
