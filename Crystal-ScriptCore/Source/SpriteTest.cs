using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Crystal;

namespace Sandbox
{
	class SpriteTest : Entity
	{
		private SpriteRendererComponent spriteRenderer;

		void OnCreate()
		{
			spriteRenderer = GetComponent<SpriteRendererComponent>();
			Console.WriteLine(spriteRenderer.AtlasCoordsX + "Atlas X");
			Console.WriteLine(spriteRenderer.AtlasCoordsY + "Atlas Y");
			Console.WriteLine(spriteRenderer.Color + "Color");
		}
	}
}
