using System;
using System.Runtime.InteropServices;

namespace PixieLib;

// A rectangle as a corner and a size, in __T__. Same layout as pxRect_t in C++ (docs/layout.md).
// PxRegion holds the same thing as two corners.
[StructLayout(LayoutKind.Sequential)]
public struct PxRect__S__ : IEquatable<PxRect__S__>
{
    public static readonly PxRect__S__ Empty = default;

    private __T__ x;
    private __T__ y;
    private __T__ width;
    private __T__ height;

    public PxRect__S__(__T__ x, __T__ y, __T__ width, __T__ height)
    {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }

    public PxRect__S__(PxPoint__S__ location, PxSize__S__ size) : this(location.X, location.Y, size.Width, size.Height)
    {
    }

    public __T__ X
    {
        readonly get => x;
        set => x = value;
    }

    public __T__ Y
    {
        readonly get => y;
        set => y = value;
    }

    public __T__ Width
    {
        readonly get => width;
        set => width = value;
    }

    public __T__ Height
    {
        readonly get => height;
        set => height = value;
    }

    public readonly __T__ Right => x + width;
    public readonly __T__ Bottom => y + height;

    public readonly bool IsEmpty => x == 0 && y == 0 && width == 0 && height == 0;

    // The left and top edges are inside, the right and bottom ones are not.
    public readonly bool Contains(PxPoint__S__ pt) => pt.X >= x && pt.X < Right && pt.Y >= y && pt.Y < Bottom;

    // Between precisions: implicit when nothing is lost, explicit (truncating) otherwise.
#if PX_DOUBLE
    public static implicit operator PxRect(PxRectf r) => new PxRect(r.X, r.Y, r.Width, r.Height);
    public static implicit operator PxRect(PxRecti r) => new PxRect(r.X, r.Y, r.Width, r.Height);
    public static explicit operator PxRectf(PxRect r) => new PxRectf((float)r.x, (float)r.y, (float)r.width, (float)r.height);
    public static explicit operator PxRecti(PxRect r) => new PxRecti((int)r.x, (int)r.y, (int)r.width, (int)r.height);
#elif PX_FLOAT
    public static explicit operator PxRectf(PxRecti r) => new PxRectf(r.X, r.Y, r.Width, r.Height);
    public static explicit operator PxRecti(PxRectf r) => new PxRecti((int)r.x, (int)r.y, (int)r.width, (int)r.height);
#endif

#if PX_DOUBLE
    // With System.Drawing (4.4): from it nothing is lost, so the conversion is implicit; back to it the
    // values are rounded (Rectangle) or narrowed (RectangleF), so it is explicit.
    public static implicit operator PxRect(System.Drawing.Rectangle r) => new PxRect(r.X, r.Y, r.Width, r.Height);
    public static implicit operator PxRect(System.Drawing.RectangleF r) => new PxRect(r.X, r.Y, r.Width, r.Height);
    public static explicit operator System.Drawing.Rectangle(PxRect r) =>
        new System.Drawing.Rectangle((int)Math.Round(r.x), (int)Math.Round(r.y), (int)Math.Round(r.width), (int)Math.Round(r.height));
    public static explicit operator System.Drawing.RectangleF(PxRect r) =>
        new System.Drawing.RectangleF((float)r.x, (float)r.y, (float)r.width, (float)r.height);
#endif

    public static bool operator ==(PxRect__S__ a, PxRect__S__ b) =>
        a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
    public static bool operator !=(PxRect__S__ a, PxRect__S__ b) => !(a == b);

    public readonly bool Equals(PxRect__S__ other) => this == other;
    public override readonly bool Equals(object? obj) => obj is PxRect__S__ other && Equals(other);
    public override readonly int GetHashCode() =>
        PxHash.Combine(x.GetHashCode(), y.GetHashCode(), width.GetHashCode(), height.GetHashCode());
    public override readonly string ToString() =>
        PxText.Tuple(PxText.Number(x), PxText.Number(y), PxText.Number(width), PxText.Number(height));
}
