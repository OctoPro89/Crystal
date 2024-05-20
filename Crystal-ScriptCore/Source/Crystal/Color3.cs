namespace Crystal
{
	public struct Color3
	{
		public float R, G, B;

		public static Color3 White => new Color3(1.0f);

		public Color3(float scalar)
		{
			R = scalar;
			G = scalar;
			B = scalar;
		}

		public Color3(float r, float g, float b)
		{
			R = r;
			G = g;
			B = b;
		}

		public static Color3 operator +(Color3 a, Color3 b)
		{
			return new Color3(a.R + b.R, a.G + b.G, a.B + b.B);
		}

		public static Color3 operator *(Color3 vector, float scalar)
		{
			return new Color3(vector.R * scalar, vector.G * scalar, vector.B * scalar);
		}
	}
}
