using System.Globalization;
using System.Numerics;

namespace PixieLib;

// How a primitive becomes text, the same as in C++ (docs/layout.md, 3): its fields in order, in
// parentheses, separated by ", ". A number is written with a point in any culture, in the shortest
// form that reads back as the same value, and never in scientific notation: 100000 stays 100000, not
// 1E+05. NaN and the infinities come out as nan, inf and -inf, like std::to_chars.
internal static class PxText
{
    private static readonly CultureInfo Invariant = CultureInfo.InvariantCulture;

    public static string Tuple(params string[] fields) => "(" + string.Join(", ", fields) + ")";

    public static string Number(int value) => value.ToString(Invariant);

    public static string Number(byte value) => value.ToString(Invariant);

    public static string Number(double value)
    {
        if (double.IsNaN(value))
            return "nan";
        if (double.IsInfinity(value))
            return value > 0 ? "inf" : "-inf";
        if (value == 0)
            return IsNegative(value) ? "-0" : "0";

        // The fewest digits that read back as the same value. "R" does it on .NET, but on net481 it
        // can give 17 digits where 16 are enough, and the parser of net481 is not exact, so it can
        // take a candidate that is too short for the value. Both targets search the same way, and
        // decide without a parser whether a candidate reads back as the value.
        long bits = System.BitConverter.DoubleToInt64Bits(value);
        int biased = (int)((bits >> 52) & 0x7FF);
        long fraction = bits & 0xFFFFFFFFFFFFFL;
        string text = value.ToString("R", Invariant);
        for (int digits = 1; digits <= 17; digits++)
        {
            string candidate = value.ToString("G" + digits, Invariant);
            if (ReadsBack(candidate, biased == 0 ? fraction : fraction | (1L << 52), biased == 0 ? -1074 : biased - 1075,
                          fraction == 0 && biased > 1))
            {
                text = candidate;
                break;
            }
        }
        return WithoutExponent(text);
    }

    public static string Number(float value)
    {
        if (float.IsNaN(value))
            return "nan";
        if (float.IsInfinity(value))
            return value > 0 ? "inf" : "-inf";
        if (value == 0)
            return IsNegative(value) ? "-0" : "0";

        int bits = System.BitConverter.ToInt32(System.BitConverter.GetBytes(value), 0);
        int biased = (bits >> 23) & 0xFF;
        int fraction = bits & 0x7FFFFF;
        string text = value.ToString("R", Invariant);
        for (int digits = 1; digits <= 9; digits++)
        {
            string candidate = value.ToString("G" + digits, Invariant);
            if (ReadsBack(candidate, biased == 0 ? fraction : fraction | (1 << 23), biased == 0 ? -149 : biased - 150,
                          fraction == 0 && biased > 1))
            {
                text = candidate;
                break;
            }
        }
        return WithoutExponent(text);
    }

    // Whether the decimal text reads back as the value mantissa * 2^exponent, without a parser: it does
    // when it falls between the halfway points to the two neighbors, and on a halfway point when the
    // mantissa is even (the rounding of IEEE). Below a power of two the neighbor is twice as close
    // (closerBelow). Everything is counted in quarters of the last place, so the bounds are integers.
    private static bool ReadsBack(string text, long mantissa, int exponent, bool closerBelow)
    {
        int e = text.IndexOfAny(new[] { 'E', 'e' });
        string significand = (e < 0 ? text : text.Substring(0, e)).TrimStart('-');
        int power = e < 0 ? 0 : int.Parse(text.Substring(e + 1), NumberStyles.AllowLeadingSign, Invariant);
        int point = significand.IndexOf('.');
        if (point >= 0)
        {
            power -= significand.Length - point - 1;
            significand = significand.Remove(point, 1);
        }

        // text = digits * 10^power, and in quarters of 2^exponent it is numerator / denominator.
        BigInteger numerator = BigInteger.Parse(significand, Invariant);
        BigInteger denominator = BigInteger.One;
        if (power >= 0)
            numerator *= BigInteger.Pow(10, power);
        else
            denominator = BigInteger.Pow(10, -power);
        if (2 - exponent >= 0)
            numerator <<= 2 - exponent;
        else
            denominator <<= exponent - 2;

        bool even = (mantissa & 1) == 0;
        int low = numerator.CompareTo((4 * (BigInteger)mantissa - (closerBelow ? 1 : 2)) * denominator);
        int high = numerator.CompareTo((4 * (BigInteger)mantissa + 2) * denominator);
        return (low > 0 || (low == 0 && even)) && (high < 0 || (high == 0 && even));
    }

    // -0 compares equal to 0, so the sign comes from the bits; net481 also drops it when formatting.
    private static bool IsNegative(double value) => System.BitConverter.DoubleToInt64Bits(value) < 0;

    // Moves the decimal point of "1.5E+20" or "5E-324" to where the exponent says, with zeros.
    private static string WithoutExponent(string text)
    {
        int e = text.IndexOfAny(new[] { 'E', 'e' });
        if (e < 0)
            return text;

        bool negative = text[0] == '-';
        string mantissa = text.Substring(negative ? 1 : 0, e - (negative ? 1 : 0));
        int exponent = int.Parse(text.Substring(e + 1), NumberStyles.AllowLeadingSign, Invariant);
        int point = mantissa.IndexOf('.');
        string digits = point < 0 ? mantissa : mantissa.Remove(point, 1);
        int integerDigits = (point < 0 ? mantissa.Length : point) + exponent;

        string plain;
        if (integerDigits <= 0)
            plain = "0." + new string('0', -integerDigits) + digits;
        else if (integerDigits >= digits.Length)
            plain = digits + new string('0', integerDigits - digits.Length);
        else
            plain = digits.Substring(0, integerDigits) + "." + digits.Substring(integerDigits);
        return negative ? "-" + plain : plain;
    }
}
