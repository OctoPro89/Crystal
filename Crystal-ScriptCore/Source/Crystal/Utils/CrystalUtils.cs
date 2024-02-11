using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Crystal
{
	public static class CrystalUtils
	{
		private static Random random = new Random();

		/* Generate random float within a range */

		public static float RandomFloat(float min, float max)
		{
			return (float)random.NextDouble() * (max - min) + min;
		}

		/* Generate random integer within a range */
		public static int RandomInt(int min, int max)
		{
			return random.Next(min, max);
		}

		/* Generate random point inside a Vector */
		public static Vector2 Vec2Rand(Vector2 vec, float min, float max)
		{
			float randomX = RandomFloat(min, max);
			float randomY = RandomFloat(min, max);
			return new Vector2(randomX, randomY);
		}

		/* Generate random point inside a Vector */
		public static Vector3 Vec3Rand(Vector2 vec, float min, float max)
		{
			float randomX = RandomFloat(min, max);
			float randomY = RandomFloat(min, max);
			float randomZ = RandomFloat(min, max);
			return new Vector3(randomX, randomY, randomZ);
		}

		/* Generate random point inside a Vector */
		public static Vector4 Vec4Rand(Vector2 vec, float min, float max)
		{
			float randomX = RandomFloat(min, max);
			float randomY = RandomFloat(min, max);
			float randomZ = RandomFloat(min, max);
			float randomW = RandomFloat(min, max);
			return new Vector4(randomX, randomY, randomZ, randomW);
		}

		/* Shuffle an array */
		public static T[] ShuffleArray<T>(T[] array)
		{
			return array.OrderBy(x => random.Next()).ToArray();
		}

		/* Randomize boolean value */
		public static bool RandomBool()
		{
			return random.NextDouble() > 0.5;
		}

		/* Randomly select an element from a list */
		public static T RandomElementFromList<T>(List<T> list)
		{
			return list[random.Next(0, list.Count)];
		}

		/* Calls a function after a delay */
		public static void FuncAfterDelay(float delayInSeconds, Action func)
		{
			ThreadPool.QueueUserWorkItem(state =>
			{
				Thread.Sleep((int)(delayInSeconds * 1000)); /* Sleep for the given time in miliseconds */
				func?.Invoke();
			});
		}
	}
}
