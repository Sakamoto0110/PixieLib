using System;
using System.Runtime.InteropServices;

namespace PixieLib;

// A position, in __T__. Same layout as pxPoint_t in C++ (docs/layout.md). A point and a size hold the
// same data, so each converts to the other, explicitly; a point moves by a size.
[StructLayout(LayoutKind.Sequential)]
public struct PxPoint__S__ : IEquatable<PxPoint__S__>
{
    public static readonly PxPoint__S__ Empty = default;

    private __T__ x;
    private __T__ y;

    public PxPoint__S__(__T__ x, __T__ y)
    {
        this.x = x;
        this.y = y;
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

    public readonly bool IsEmpty => x == 0 && y == 0;

    public static PxPoint__S__ operator +(PxPoint__S__ a, PxPoint__S__ b) => new PxPoint__S__(a.x + b.x, a.y + b.y);
    public static PxPoint__S__ operator -(PxPoint__S__ a, PxPoint__S__ b) => new PxPoint__S__(a.x - b.x, a.y - b.y);
    public static PxPoint__S__ operator +(PxPoint__S__ pt, PxSize__S__ size) => new PxPoint__S__(pt.x + size.Width, pt.y + size.Height);
    public static PxPoint__S__ operator -(PxPoint__S__ pt, PxSize__S__ size) => new PxPoint__S__(pt.x - size.Width, pt.y - size.Height);
    public static PxPoint__S__ operator *(PxPoint__S__ pt, __T__ k) => new PxPoint__S__(pt.x * k, pt.y * k);
    public static PxPoint__S__ operator *(__T__ k, PxPoint__S__ pt) => pt * k;
    public static PxPoint__S__ operator /(PxPoint__S__ pt, __T__ k) => new PxPoint__S__(pt.x / k, pt.y / k);
    public static PxPoint__S__ operator +(PxPoint__S__ pt) => pt;
    public static PxPoint__S__ operator -(PxPoint__S__ pt) => new PxPoint__S__(-pt.x, -pt.y);

    public static explicit operator PxSize__S__(PxPoint__S__ pt) => new PxSize__S__(pt.x, pt.y);
    public static explicit operator PxPoint__S__(PxSize__S__ size) => new PxPoint__S__(size.Width, size.Height);

    // Between precisions: implicit when nothing is lost, explicit (truncating) otherwise.
#if PX_DOUBLE
    public static implicit operator PxPoint(PxPointf pt) => new PxPoint(pt.X, pt.Y);
    public static implicit operator PxPoint(PxPointi pt) => new PxPoint(pt.X, pt.Y);
    public static explicit operator PxPointf(PxPoint pt) => new PxPointf((float)pt.x, (float)pt.y);
    public static explicit operator PxPointi(PxPoint pt) => new PxPointi((int)pt.x, (int)pt.y);
#elif PX_FLOAT
    public static explicit operator PxPointf(PxPointi pt) => new PxPointf(pt.X, pt.Y);
    public static explicit operator PxPointi(PxPointf pt) => new PxPointi((int)pt.x, (int)pt.y);
#endif

#if PX_DOUBLE
    // With System.Drawing (4.4): from it nothing is lost, so the conversion is implicit; back to it the
    // value is rounded (Point) or narrowed (PointF), so it is explicit.
    public static implicit operator PxPoint(System.Drawing.Point pt) => new PxPoint(pt.X, pt.Y);
    public static implicit operator PxPoint(System.Drawing.PointF pt) => new PxPoint(pt.X, pt.Y);
    public static explicit operator System.Drawing.Point(PxPoint pt) =>
        new System.Drawing.Point((int)Math.Round(pt.x), (int)Math.Round(pt.y));
    public static explicit operator System.Drawing.PointF(PxPoint pt) => new System.Drawing.PointF((float)pt.x, (float)pt.y);
#endif

    public static bool operator ==(PxPoint__S__ a, PxPoint__S__ b) => a.x == b.x && a.y == b.y;
    public static bool operator !=(PxPoint__S__ a, PxPoint__S__ b) => !(a == b);

    public readonly bool Equals(PxPoint__S__ other) => this == other;
    public override readonly bool Equals(object? obj) => obj is PxPoint__S__ other && Equals(other);
    public override readonly int GetHashCode() => PxHash.Combine(x.GetHashCode(), y.GetHashCode());
    public override readonly string ToString() => PxText.Tuple(PxText.Number(x), PxText.Number(y));
}
