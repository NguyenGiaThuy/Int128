using System;

using CLI.Int128;

namespace SimpleExpressionEngine
{
    // NodeUnary for unary operations such as Negate
    class NodeUnary : Node
    {
        // Constructor accepts the two nodes to be operated on and function
        // that performs the actual operation
        public NodeUnary(Node rhs, Func<QInt, QInt> op)
        {
            _rhs = rhs;
            _op = op;
        }

        Node _rhs;                              // Right hand side of the operation
        Func<QInt, QInt> _op;               // The callback operator

        public override QInt Eval()
        {
            // Evaluate RHS
            var rhsVal = _rhs.Eval();

            // Evaluate and return
            var result = _op(rhsVal);
            return result;
        }
    }
}
