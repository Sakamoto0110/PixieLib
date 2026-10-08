namespace PixieLib.Tests;

internal static class PxRegionTests
{
    public static void Run()
    {
        var rg = new PxRegion(1, 2, 4, 6);
        Check.That(rg.Width == 3 && rg.Height == 4);
        Check.That(PxRegion.Empty.IsEmpty);
        Check.That(!rg.IsEmpty);

        // The same thing as a rectangle, converted explicitly both ways.
        Check.That((PxRegion)new PxRect(1, 2, 3, 4) == rg);
        Check.That((PxRect)rg == new PxRect(1, 2, 3, 4));
        Check.That(!Check.Implicit<PxRect, PxRegion>() && !Check.Implicit<PxRegion, PxRect>());

        // The same edges as PxRect.Contains.
        var r = new PxRect(1, 2, 3, 4);
        foreach (var pt in new[] { new PxPoint(1, 2), new PxPoint(3.999, 5.999), new PxPoint(4, 2), new PxPoint(1, 6), new PxPoint(0.5, 3) })
            Check.That(rg.Contains(pt) == r.Contains(pt), $"Contains{pt} agrees with PxRect");
        Check.That(rg.Contains(new PxPoint(1, 2)) && !rg.Contains(new PxPoint(4, 2)) && !rg.Contains(new PxPoint(1, 6)));

        PxRegion fromInt = new PxRegioni(0, 0, 2, 2);
        Check.That(fromInt == new PxRegion(0, 0, 2, 2));
        Check.That(Check.Implicit<PxRegioni, PxRegion>() && !Check.Implicit<PxRegion, PxRegioni>());
    }
}
