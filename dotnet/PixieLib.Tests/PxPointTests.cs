namespace PixieLib.Tests;

internal static class PxPointTests
{
    public static void Run()
    {
        var pt = new PxPoint(1.5, -2);
        Check.That(pt.X == 1.5 && pt.Y == -2);
        Check.That(PxPoint.Empty.IsEmpty && new PxPoint().IsEmpty);
        Check.That(!pt.IsEmpty);

        Check.That(pt + new PxPoint(1, 1) == new PxPoint(2.5, -1));
        Check.That(pt - new PxPoint(1, 1) == new PxPoint(0.5, -3));
        Check.That(pt + new PxSize(2, 3) == new PxPoint(3.5, 1));
        Check.That(pt - new PxSize(2, 3) == new PxPoint(-0.5, -5));
        Check.That(pt * 2 == new PxPoint(3, -4));
        Check.That(2 * pt == new PxPoint(3, -4));
        Check.That(pt / 2 == new PxPoint(0.75, -1));
        Check.That(+pt == pt);
        Check.That(-pt == new PxPoint(-1.5, 2));

        // A point and a size hold the same data; the conversion is explicit both ways.
        Check.That((PxSize)pt == new PxSize(1.5, -2));
        Check.That((PxPoint)new PxSize(3, 4) == new PxPoint(3, 4));
        Check.That(Check.Explicit<PxPoint, PxSize>() && Check.Explicit<PxSize, PxPoint>());
        Check.That(!Check.Implicit<PxPoint, PxSize>() && !Check.Implicit<PxSize, PxPoint>());

        // Between precisions: implicit when nothing is lost, explicit (truncating) otherwise.
        PxPoint fromFloat = new PxPointf(1.5f, 2.5f);
        PxPoint fromInt = new PxPointi(3, 4);
        Check.That(fromFloat == new PxPoint(1.5, 2.5));
        Check.That(fromInt == new PxPoint(3, 4));
        Check.That((PxPointi)new PxPoint(2.9, -2.9) == new PxPointi(2, -2));
        Check.That(Check.Implicit<PxPointf, PxPoint>() && Check.Implicit<PxPointi, PxPoint>());
        Check.That(!Check.Implicit<PxPoint, PxPointf>() && !Check.Implicit<PxPoint, PxPointi>());
        Check.That(!Check.Implicit<PxPointi, PxPointf>() && !Check.Implicit<PxPointf, PxPointi>());

        // Integer points divide like integers.
        Check.That(new PxPointi(7, -7) / 2 == new PxPointi(3, -3));
    }
}
