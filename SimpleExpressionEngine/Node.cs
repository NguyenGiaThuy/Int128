using CLI.Int128;

namespace SimpleExpressionEngine
{
    // Node - abstract class representing one node in the expression 
    public abstract class Node
    {
        public abstract QInt Eval();
    }
}
