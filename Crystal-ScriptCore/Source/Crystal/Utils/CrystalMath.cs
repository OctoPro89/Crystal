using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Crystal
{
	public static class CrystalMath
	{
		public static float Lerp(float a, float b, float t)
		{
			return a * (1.0f - t) + (b * t);
		}
	}
}
