using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;

using CLI.Int128;

namespace Calculator
{
    /// <summary>
    /// Interaction logic for UserControlKeypad.xaml
    /// </summary>
    internal class StringToFormula
    {
        private string[] _operators = { "-", "+", "÷", "×" };
        private Func<QInt, QInt, QInt>[] _operations = {
            (a1, a2) => a1 - a2,
            (a1, a2) => a1 + a2,
            (a1, a2) => a1 / a2,
            (a1, a2) => a1 * a2
        };

        public QInt Eval(string expression)
        {
            List<string> tokens = getTokens(expression);
            Stack<QInt> operandStack = new Stack<QInt>();
            Stack<string> operatorStack = new Stack<string>();
            int tokenIndex = 0;

            while (tokenIndex < tokens.Count)
            {
                string token = tokens[tokenIndex];
                if (token == "(")
                {
                    string subExpr = getSubExpression(tokens, ref tokenIndex);
                    operandStack.Push(Eval(subExpr));
                    continue;
                }
                if (token == ")")
                {
                    throw new ArgumentException("Mis-matched parentheses in expression");
                }

                //If this is an operator  
                if (Array.IndexOf(_operators, token) >= 0)
                {
                    while (operatorStack.Count > 0 && Array.IndexOf(_operators, token) < Array.IndexOf(_operators, operatorStack.Peek()))
                    {
                        string op = operatorStack.Pop();
                        QInt arg2 = operandStack.Pop();
                        QInt arg1 = operandStack.Pop();
                        operandStack.Push(_operations[Array.IndexOf(_operators, op)](arg1, arg2));
                    }
                    operatorStack.Push(token);
                }
                else
                {
                    operandStack.Push(new QInt(token));
                }
                tokenIndex += 1;
            }

            while (operatorStack.Count > 0)
            {
                string op = operatorStack.Pop();
                QInt arg2 = operandStack.Pop();
                QInt arg1 = operandStack.Pop();
                operandStack.Push(_operations[Array.IndexOf(_operators, op)](arg1, arg2));
            }

            return operandStack.Pop();
        }

        private string getSubExpression(List<string> tokens, ref int index)
        {
            StringBuilder subExpr = new StringBuilder();
            int parenlevels = 1;
            index += 1;
            while (index < tokens.Count && parenlevels > 0)
            {
                string token = tokens[index];
                if (tokens[index] == "(")
                {
                    parenlevels += 1;
                }

                if (tokens[index] == ")")
                {
                    parenlevels -= 1;
                }

                if (parenlevels > 0)
                {
                    subExpr.Append(token);
                }

                index += 1;
            }

            if ((parenlevels > 0))
            {
                throw new ArgumentException("Mis-matched parentheses in expression");
            }

            return subExpr.ToString();
        }

        private List<string> getTokens(string expression)
        {
            string operators = "()×÷+-";
            List<string> tokens = new List<string>();
            StringBuilder sb = new StringBuilder();

            foreach (char c in expression.Replace(" ", string.Empty))
            {
                if (operators.IndexOf(c) >= 0)
                {
                    if ((sb.Length > 0))
                    {
                        tokens.Add(sb.ToString());
                        sb.Length = 0;
                    }
                    tokens.Add(c.ToString());
                }
                else
                {
                    sb.Append(c);
                }
            }

            if ((sb.Length > 0))
            {
                tokens.Add(sb.ToString());
            }

            return tokens;
        }
    }

    public partial class UserControlKeypad : UserControl
    {
        private char[] numbers = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
        private bool operatorPressed = false;
        private bool exceptionThrown = false;
        private QInt result = new QInt("0");

        public UserControlKeypad()
        {
            InitializeComponent();
        }

        private void PlusMinusBtn_Click(object sender, RoutedEventArgs e)
        {
            if (LargeTextBox.Text != "0")
            {
                LargeTextBox.Text =
                    (LargeTextBox.Text.Contains('-')) ?
                    LargeTextBox.Text.TrimStart('-') :
                    '-' + LargeTextBox.Text;
            }

            if (exceptionThrown)
            {
                SmallTextBox.Text = "";
                LargeTextBox.Text = "0";
                operatorPressed = false;
                exceptionThrown = false;
            }

            operatorPressed = false;
        }

        private void DotBtn_Click(object sender, RoutedEventArgs e)
        {
            if (!operatorPressed)
            {
                LargeTextBox.Text += (LargeTextBox.Text.Contains('.')) ? "" : ".";
            }
            else
            {
                LargeTextBox.Text = "0.";
            }

            if (exceptionThrown)
            {
                SmallTextBox.Text = "";
                LargeTextBox.Text = "0";
                operatorPressed = false;
                exceptionThrown = false;
            }

            operatorPressed = false;
        }

        private void ZeroBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperandClicked(0);
        }

        private void OneBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperandClicked(1);
        }

        private void TwoBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperandClicked(2);
        }

        private void ThreeBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperandClicked(3);
        }

        private void FourBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperandClicked(4);
        }

        private void FiveBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperandClicked(5);
        }

        private void SixBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperandClicked(6);
        }

        private void SevenBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperandClicked(7);
        }

        private void EightBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperandClicked(8);
        }

        private void NineBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperandClicked(9);
        }

        private void AddBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperatorClicked('+');
        }

        private void SubtractBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperatorClicked('-');
        }

        private void MultiplyBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperatorClicked('×');
        }

        private void DivideBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperatorClicked('÷');
        }

        private void ClearEntryBtn_Click(object sender, RoutedEventArgs e)
        {
            LargeTextBox.Text = "0";
            operatorPressed = false;
        }

        private void ClearBtn_Click(object sender, RoutedEventArgs e)
        {
            SmallTextBox.Text = "";
            LargeTextBox.Text = "0";
            operatorPressed = false;
        }

        private void EqualBtn_Click(object sender, RoutedEventArgs e)
        {
            // REALLY HARD
            if (!operatorPressed)
            {
                try
                {
                    SmallTextBox.Text += LargeTextBox.Text;
                    StringToFormula stf = new StringToFormula();
                    QInt temp = stf.Eval(SmallTextBox.Text);
                    result = temp;
                    SmallTextBox.Text += '=';
                    LargeTextBox.Text = result.Content;
                }
                catch (DivideByZeroException ex)
                {
                    exceptionThrown = true;
                    LargeTextBox.Text = ex.Message;
                }
            }
        }

        private void BackspaceBtn_Click(object sender, RoutedEventArgs e)
        {
            if (LargeTextBox.Text.Length == 1 ||
                (LargeTextBox.Text.Length == 2 &&
                LargeTextBox.Text.Contains('-')))
            {
                LargeTextBox.Text = "0";
            }
            else if (LargeTextBox.Text.Length > 0)
            {
                LargeTextBox.Text = LargeTextBox.Text.Remove(LargeTextBox.Text.Length - 1);
            }
        }

        private void LargeTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (LargeTextBox.Text.Length > 1 && !LargeTextBox.Text.Contains('.') && !exceptionThrown)
            {
                LargeTextBox.Text =
                    (LargeTextBox.Text.IndexOfAny(numbers) != -1) ?
                    LargeTextBox.Text.TrimStart('0') :
                    LargeTextBox.Text.Remove(1);
            }
        }

        private void onOperandClicked(int num)
        {
            if (!operatorPressed)
            {
                LargeTextBox.Text += num.ToString();
            }
            else
            {
                LargeTextBox.Text = num.ToString();
            }

            if (result != new QInt("0"))
            {
                LargeTextBox.Text = num.ToString();
                SmallTextBox.Text = "";
                result = new QInt("0");
            }

            if(exceptionThrown)
            {
                SmallTextBox.Text = "";
                LargeTextBox.Text = "0";
                operatorPressed = false;
                exceptionThrown = false;
            }

            operatorPressed = false;
        }

        private void onOperatorClicked(char op)
        {
            if (LargeTextBox.Text.Contains('.'))
            {
                LargeTextBox.Text = LargeTextBox.Text.TrimEnd('0');
                LargeTextBox.Text =
                    (LargeTextBox.Text.LastIndexOf('.') == LargeTextBox.Text.Length - 1) ?
                    LargeTextBox.Text.Remove(LargeTextBox.Text.Length - 1) :
                    LargeTextBox.Text;
            }

            if (!operatorPressed)
            {
                SmallTextBox.Text += LargeTextBox.Text + op;
            }
            else
            {
                SmallTextBox.Text = SmallTextBox.Text.Remove(SmallTextBox.Text.Length - 1) + op;
            }

            if (result != new QInt("0"))
            {
                SmallTextBox.Text = result.Content + op;
                result = new QInt("0");
            }

            if (exceptionThrown)
            {
                SmallTextBox.Text = "";
                LargeTextBox.Text = "0";
                operatorPressed = false;
                exceptionThrown = false;
            }

            operatorPressed = true;
        }
    }
}
