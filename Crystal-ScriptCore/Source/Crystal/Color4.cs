namespace Crystal
{
	public struct Color4
	{
		public float R, G, B, A;

		public static Color4 White => new Color4(1.0f);

		public Color4(float scalar)
		{
			R = scalar;
			G = scalar;
			B = scalar;
			A = scalar;
		}

		public Color4(float r, float g, float b, float a)
		{
			R = r;
			G = g;
			B = b;
			A = a;
		}

		public Color3 RGB
		{
			get => new Color3(R, G, B);
			set
			{
				R = value.R;
				G = value.G;
				B = value.B;
			}
		}

		public static Color4 operator +(Color4 a, Color4 b)
		{
			return new Color4(a.R + b.R, a.G + b.G, a.B + b.B, a.A + b.A);
		}

		public static Color4 operator *(Color4 color, float scalar)
		{
			return new Color4(color.R * scalar, color.G * scalar, color.B * scalar, color.A * scalar);
		}
	}
}
