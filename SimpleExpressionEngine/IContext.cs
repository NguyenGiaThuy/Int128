using CLI.Int128;

namespace SimpleExpressionEngine
{
    public interface IContext
    {
        QInt ResolveVariable(string name);
        QInt CallFunction(string name, QInt[] arguments);
    }
}
