namespace PixieLib;

// Combines the hashes of the fields of a primitive. System.HashCode does not exist in net481.
internal static class PxHash
{
    public static int Combine(int a, int b)
    {
        unchecked
        {
            return (a * 397) ^ b;
        }
    }

    public static int Combine(int a, int b, int c, int d) => Combine(Combine(Combine(a, b), c), d);
}
