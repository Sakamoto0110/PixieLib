namespace PixieLib.Tests;

internal static class PxRectTests
{
    public static void Run()
    {
        var r = new PxRect(1, 2, 3, 4);
        Check.That(r.Right == 4 && r.Bottom == 6);
        Check.That(new PxRect(new PxPoint(1, 2), new PxSize(3, 4)) == r);
        Check.That(PxRect.Empty.IsEmpty);
        Check.That(!r.IsEmpty);

        // The left and top edges are inside, the right and bottom ones are not.
        Check.That(r.Contains(new PxPoint(1, 2)));
        Check.That(r.Contains(new PxPoint(3.999, 5.999)));
        Check.That(!r.Contains(new PxPoint(4, 2)));
        Check.That(!r.Contains(new PxPoint(1, 6)));
        Check.That(!r.Contains(new PxPoint(0.999, 2)));
        Check.That(!r.Contains(new PxPoint(1, 1.999)));

        var ri = new PxRecti(0, 0, 2, 2);
        Check.That(ri.Contains(new PxPointi(1, 1)) && !ri.Contains(new PxPointi(2, 1)));

        PxRect fromInt = ri;
        Check.That(fromInt == new PxRect(0, 0, 2, 2));
        Check.That((PxRecti)new PxRect(1.9, 2.9, 3.9, 4.9) == new PxRecti(1, 2, 3, 4));
        Check.That(Check.Implicit<PxRectf, PxRect>() && !Check.Implicit<PxRect, PxRectf>());
    }
}
