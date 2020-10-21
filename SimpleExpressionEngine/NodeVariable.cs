using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CLI.Int128;

namespace SimpleExpressionEngine
{
    // Represents a variable (or a constant) in an expression.  eg: "2 * pi"
    public class NodeVariable : Node
    {
        public NodeVariable(string variableName)
        {
            _variableName = variableName;
        }

        string _variableName;

        public override QInt Eval(IContext ctx)
        {
            return ctx.ResolveVariable(_variableName);
        }
    }
}
