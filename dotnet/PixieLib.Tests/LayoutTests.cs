using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace PixieLib.Tests;

// The layout is the contract with C++ (docs/layout.md): the same sizes and offsets the static_asserts
// of cpp/include/pixie check, both as P/Invoke sees them (Marshal) and in managed memory (Unsafe).
internal static class LayoutTests
{
    public static void Run()
    {
        Layout<PxPoint>(16, ("x", 0), ("y", 8));
        Layout<PxPointf>(8, ("x", 0), ("y", 4));
        Layout<PxPointi>(8, ("x", 0), ("y", 4));

        Layout<PxSize>(16, ("width", 0), ("height", 8));
        Layout<PxSizef>(8, ("width", 0), ("height", 4));
        Layout<PxSizei>(8, ("width", 0), ("height", 4));

        Layout<PxRect>(32, ("x", 0), ("y", 8), ("width", 16), ("height", 24));
        Layout<PxRectf>(16, ("x", 0), ("y", 4), ("width", 8), ("height", 12));
        Layout<PxRecti>(16, ("x", 0), ("y", 4), ("width", 8), ("height", 12));

        Layout<PxRegion>(32, ("x1", 0), ("y1", 8), ("x2", 16), ("y2", 24));
        Layout<PxRegionf>(16, ("x1", 0), ("y1", 4), ("x2", 8), ("y2", 12));
        Layout<PxRegioni>(16, ("x1", 0), ("y1", 4), ("x2", 8), ("y2", 12));

        Layout<PxPadding>(32, ("left", 0), ("top", 8), ("right", 16), ("bottom", 24));
        Layout<PxPaddingf>(16, ("left", 0), ("top", 4), ("right", 8), ("bottom", 12));
        Layout<PxPaddingi>(16, ("left", 0), ("top", 4), ("right", 8), ("bottom", 12));

        Layout<PxColorRgba>(4, ("r", 0), ("g", 1), ("b", 2), ("a", 3));
        Layout<PxColorHsl>(32, ("h", 0), ("s", 8), ("l", 16), ("a", 24));

        // In memory: R, G, B, A, the order OpenGL reads with GL_RGBA and GL_UNSIGNED_BYTE.
        var bytes = MemoryMarshal.AsBytes(new[] { new PxColorRgba(0x11, 0x22, 0x33, 0x44) }.AsSpan()).ToArray();
        Check.That(bytes.SequenceEqual(new byte[] { 0x11, 0x22, 0x33, 0x44 }));
    }

    private static void Layout<T>(int size, params (string Field, int Offset)[] fields) where T : struct
    {
        string name = typeof(T).Name;
        Check.That(Marshal.SizeOf<T>() == size, $"{name}: Marshal.SizeOf == {size}");
        Check.That(Unsafe.SizeOf<T>() == size, $"{name}: Unsafe.SizeOf == {size}");
        foreach (var (field, offset) in fields)
            Check.That((int)Marshal.OffsetOf<T>(field) == offset, $"{name}.{field} at {offset}");
        Check.That(!RuntimeHelpers.IsReferenceOrContainsReferences<T>(), $"{name} holds no references");
    }
}
