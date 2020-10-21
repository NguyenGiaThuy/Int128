using CLI.Int128;

namespace SimpleExpressionEngine
{
    // NodeNumber represents a literal number in the expression
    class NodeNumber : Node
    {
        public NodeNumber(QInt number)
        {
            _number = number;
        }

        QInt _number;             // The number

        public override QInt Eval()
        {
            // Just return it.  Too easy.
            return _number;
        }
    }
}
