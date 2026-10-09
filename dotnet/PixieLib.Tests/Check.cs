using System.Reflection;
using System.Runtime.CompilerServices;

namespace PixieLib.Tests;

// A failed check prints where it is and is counted; Main returns non-zero when any failed.
internal static class Check
{
    public static int Failures { get; private set; }

    public static void That(bool condition, [CallerArgumentExpression(nameof(condition))] string expression = "",
        [CallerFilePath] string file = "", [CallerLineNumber] int line = 0)
    {
        if (condition)
            return;
        Failures++;
        Console.WriteLine($"{Path.GetFileName(file)}:{line}: check failed: {expression}");
    }

    public static void Throws<TException>(Action action, [CallerArgumentExpression(nameof(action))] string expression = "",
        [CallerFilePath] string file = "", [CallerLineNumber] int line = 0) where TException : Exception
    {
        try
        {
            action();
        }
        catch (TException)
        {
            return;
        }
        catch (Exception)
        {
        }
        Failures++;
        Console.WriteLine($"{Path.GetFileName(file)}:{line}: did not throw {typeof(TException).Name}: {expression}");
    }

    // Whether TFrom converts to TTo implicitly or explicitly, through an operator of either type.
    public static bool Implicit<TFrom, TTo>() => HasOperator<TFrom, TTo>("op_Implicit");

    public static bool Explicit<TFrom, TTo>() => HasOperator<TFrom, TTo>("op_Explicit");

    private static bool HasOperator<TFrom, TTo>(string name) =>
        typeof(TFrom).GetMethods(BindingFlags.Public | BindingFlags.Static)
            .Concat(typeof(TTo).GetMethods(BindingFlags.Public | BindingFlags.Static))
            .Any(m => m.Name == name && m.ReturnType == typeof(TTo)
                && m.GetParameters() is { Length: 1 } p && p[0].ParameterType == typeof(TFrom));
}
