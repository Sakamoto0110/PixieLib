using System;
using System.Runtime.InteropServices;

namespace PixieLib;

// Space around something, one value per side, in __T__, in the same order as PxRegion: left, top,
// right, bottom. Same layout as pxPadding_t in C++ (docs/layout.md).
[StructLayout(LayoutKind.Sequential)]
public struct PxPadding__S__ : IEquatable<PxPadding__S__>
{
    public static readonly PxPadding__S__ Empty = default;

    private __T__ left;
    private __T__ top;
    private __T__ right;
    private __T__ bottom;

    public PxPadding__S__(__T__ all) : this(all, all, all, all)
    {
    }

    public PxPadding__S__(__T__ left, __T__ top, __T__ right, __T__ bottom)
    {
        this.left = left;
        this.top = top;
        this.right = right;
        this.bottom = bottom;
    }

    public __T__ Left
    {
        readonly get => left;
        set => left = value;
    }

    public __T__ Top
    {
        readonly get => top;
        set => top = value;
    }

    public __T__ Right
    {
        readonly get => right;
        set => right = value;
    }

    public __T__ Bottom
    {
        readonly get => bottom;
        set => bottom = value;
    }

    // The space taken across and along.
    public readonly __T__ Horizontal => left + right;
    public readonly __T__ Vertical => top + bottom;

    public readonly bool IsEmpty => left == 0 && top == 0 && right == 0 && bottom == 0;

    // Between precisions: implicit when nothing is lost, explicit (truncating) otherwise.
#if PX_DOUBLE
    public static implicit operator PxPadding(PxPaddingf p) => new PxPadding(p.Left, p.Top, p.Right, p.Bottom);
    public static implicit operator PxPadding(PxPaddingi p) => new PxPadding(p.Left, p.Top, p.Right, p.Bottom);
    public static explicit operator PxPaddingf(PxPadding p) => new PxPaddingf((float)p.left, (float)p.top, (float)p.right, (float)p.bottom);
    public static explicit operator PxPaddingi(PxPadding p) => new PxPaddingi((int)p.left, (int)p.top, (int)p.right, (int)p.bottom);
#elif PX_FLOAT
    public static explicit operator PxPaddingf(PxPaddingi p) => new PxPaddingf(p.Left, p.Top, p.Right, p.Bottom);
    public static explicit operator PxPaddingi(PxPaddingf p) => new PxPaddingi((int)p.left, (int)p.top, (int)p.right, (int)p.bottom);
#endif

    public static bool operator ==(PxPadding__S__ a, PxPadding__S__ b) =>
        a.left == b.left && a.top == b.top && a.right == b.right && a.bottom == b.bottom;
    public static bool operator !=(PxPadding__S__ a, PxPadding__S__ b) => !(a == b);

    public readonly bool Equals(PxPadding__S__ other) => this == other;
    public override readonly bool Equals(object? obj) => obj is PxPadding__S__ other && Equals(other);
    public override readonly int GetHashCode() =>
        PxHash.Combine(left.GetHashCode(), top.GetHashCode(), right.GetHashCode(), bottom.GetHashCode());
    public override readonly string ToString() =>
        PxText.Tuple(PxText.Number(left), PxText.Number(top), PxText.Number(right), PxText.Number(bottom));
}
