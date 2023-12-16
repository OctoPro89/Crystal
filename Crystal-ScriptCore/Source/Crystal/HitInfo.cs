using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Crystal
{
	public struct HitInfo
	{
		public HitInfo(float xa, float ya, float xb, float yb, ulong hitEntity)
		{
			XA = xa;
			YA = ya;
			XB = xb;
			YB = yb;
			entityHit = hitEntity;
		}
		public readonly float XA, YA, XB, YB;
		public readonly ulong entityHit;
	}
}
