using System;
using System.IO;

namespace Crystal
{
	public struct Vector3
	{
		public float X, Y, Z;

		public static Vector3 Zero => new Vector3(0.0f);

		public Vector3(float scalar)
		{
			X = scalar;
			Y = scalar;
			Z = scalar;
		}

		public Vector3(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}

		public Vector3(Vector2 XY, float z)
		{
			X = XY.X;
			Y = XY.Y;
			Z = z;
		}

		public Vector2 XY
		{
			get => new Vector2(X, Y);
			set
			{
				X = value.X;
				Y = value.Y;
			}
		}

		public static Vector3 operator +(Vector3 a, Vector3 b)
		{
			return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
		}

		public static Vector3 operator *(Vector3 vector, float scalar)
		{
			return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
		}

		public static Vector3 Cross(Vector3 a, Vector3 b)
		{
			return new Vector3(
				a.Y * b.Z - a.Z * b.Y,
				a.Z * b.X - a.X * b.Z,
				a.X * b.Y - a.Y * b.X
			);
		}

		public static float Dot(Vector3 a, Vector3 b)
		{
			return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
		}

		public static float Magnitude(Vector3 vector)
		{
			return (float)Math.Sqrt(vector.X * vector.X + vector.Y * vector.Y + vector.Z * vector.Z);
		}

		public static Vector3 Normalize(Vector3 vector)
		{
			float magnitude = Magnitude(vector);
			return new Vector3(vector.X / magnitude, vector.Y / magnitude, vector.Z / magnitude);
		}
		public static float Angle(Vector3 from, Vector3 to)
		{
			return (float)Math.Acos(Dot(from, to) / (Magnitude(from) * Magnitude(to)));
		}
	}
}
