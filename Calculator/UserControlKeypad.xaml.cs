using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

using CLI.Int128;
using SimpleExpressionEngine;

namespace Calculator
{
    /// <summary>
    /// Interaction logic for UserControlKeypad.xaml
    /// </summary>
    public partial class UserControlKeypad : UserControl
    {
        private char[] numbers = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
        private QInt result = null;

        private enum States
        {
            OperandClicked,
            OperatorClicked,
            LeftParentheseClicked,
            RightParentheseClicked,
            DecimalClicked,
            SignClicked,
            EvaluationClicked,
            ExceptionThrown
        }

        public UserControlKeypad()
        {
            InitializeComponent();
        }

        private void PlusMinusBtn_Click(object sender, RoutedEventArgs e)
        {
        }

        private void DotBtn_Click(object sender, RoutedEventArgs e)
        {
            OnDecimalClicked();
        }

        private void ZeroBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperandClicked(0);
        }

        private void OneBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperandClicked(1);
        }

        private void TwoBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperandClicked(2);
        }

        private void ThreeBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperandClicked(3);
        }

        private void FourBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperandClicked(4);
        }

        private void FiveBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperandClicked(5);
        }

        private void SixBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperandClicked(6);
        }

        private void SevenBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperandClicked(7);
        }

        private void EightBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperandClicked(8);
        }

        private void NineBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperandClicked(9);
        }

        private void AddBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperatorClicked('+');
        }

        private void SubtractBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperatorClicked('-');
        }

        private void MultiplyBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperatorClicked('×');
        }

        private void DivideBtn_Click(object sender, RoutedEventArgs e)
        {
            OnOperatorClicked('÷');
        }

        private void LeftParenthese_Click(object sender, RoutedEventArgs e)
        {
            OnLeftParentheseClicked();
        }

        private void RightParenthese_Click(object sender, RoutedEventArgs e)
        {
            OnRightParentheseClicked();
        }

        private void EqualBtn_Click(object sender, RoutedEventArgs e)
        {
            OnEvaluateClicked();
        }

        private void ClearEntryBtn_Click(object sender, RoutedEventArgs e)
        {
            LargeTextBox.Text = "0";
            states = States.OperandClicked;
        }

        private void BackspaceBtn_Click(object sender, RoutedEventArgs e)
        {
            OnBackspaceClicked();
        }

        private void ClearBtn_Click(object sender, RoutedEventArgs e)
        {
            ResetState();
        }

        private void LargeTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (LargeTextBox.Text.Length > 1 &&
                !LargeTextBox.Text.Contains('.') &&
                states != States.ExceptionThrown)
            {
                LargeTextBox.Text =
                    (LargeTextBox.Text.IndexOfAny(numbers) != -1) ?
                    LargeTextBox.Text.TrimStart('0') :
                    LargeTextBox.Text.Remove(1);
            }
        }

        private void LargeTextBox_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            Keyboard.Focus(LargeTextBox);
        }

        private void LargeTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.Enter:
                    OnEvaluateClicked();
                    break;
                case Key.Back:
                    OnBackspaceClicked();
                    break;
            }
        }

        private void LargeTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.NumPad0:
                    OnOperandClicked(0);
                    break;
                case Key.NumPad1:
                    OnOperandClicked(1);
                    break;
                case Key.NumPad2:
                    OnOperandClicked(2);
                    break;
                case Key.NumPad3:
                    OnOperandClicked(3);
                    break;
                case Key.NumPad4:
                    OnOperandClicked(4);
                    break;
                case Key.NumPad5:
                    OnOperandClicked(5);
                    break;
                case Key.NumPad6:
                    OnOperandClicked(6);
                    break;
                case Key.NumPad7:
                    OnOperandClicked(7);
                    break;
                case Key.NumPad8:
                    OnOperandClicked(8);
                    break;
                case Key.NumPad9:
                    OnOperandClicked(9);
                    break;
                case Key.Add:
                    OnOperatorClicked('+');
                    break;
                case Key.Subtract:
                    OnOperatorClicked('-');
                    break;
                case Key.Multiply:
                    OnOperatorClicked('×');
                    break;
                case Key.Divide:
                    OnOperatorClicked('÷');
                    break;
                case Key.Decimal:
                    OnDecimalClicked();
                    break;
            }
        }

        private void OnBackspaceClicked()
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

        private States states;

        private void OnOperandClicked(int num)
        {
            switch (states)
            {
                case States.OperandClicked: // WORKED
                    LargeTextBox.Text += (LargeTextBox.Text.Length < 40) ? num.ToString() : "";
                    states = States.OperandClicked;
                    break;
                case States.OperatorClicked: // WORKED
                    LargeTextBox.Text = num.ToString();
                    states = States.OperandClicked;
                    break;
                case States.LeftParentheseClicked:
                    LargeTextBox.Text = num.ToString();
                    states = States.OperandClicked;
                    break;
                case States.RightParentheseClicked: // WORKED
                    // Do nothing
                    break;
                case States.DecimalClicked: // WORKED
                    LargeTextBox.Text += (LargeTextBox.Text.Length < 40) ? num.ToString() : "";
                    states = States.OperandClicked;
                    break;
                case States.SignClicked: // WORKED
                    LargeTextBox.Text += (LargeTextBox.Text.Length < 40) ? num.ToString() : "";
                    states = States.OperandClicked;
                    break;
                case States.EvaluationClicked: // WORKED
                    ResetState();
                    LargeTextBox.Text = num.ToString();
                    states = States.OperandClicked;
                    break;
                case States.ExceptionThrown: // WORKED
                    ResetState();
                    break;
            }
        }

        private void OnOperatorClicked(char op)
        {
            switch (states)
            {
                case States.OperandClicked: // WORKED
                    SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text) + op.ToString();
                    states = States.OperatorClicked;
                    break;
                case States.OperatorClicked: // WORKED
                    SmallTextBox.Text = SmallTextBox.Text.Remove(SmallTextBox.Text.Length - 1) + op.ToString();
                    states = States.OperatorClicked;
                    break;
                case States.LeftParentheseClicked: // WORKED
                    SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text) + op.ToString();
                    states = States.OperatorClicked;
                    break;
                case States.RightParentheseClicked: // WORKED
                    SmallTextBox.Text += op.ToString();
                    states = States.OperatorClicked;
                    break;
                case States.DecimalClicked: // WORKKED
                    SmallTextBox.Text = NormalizeExpression(LargeTextBox.Text) + op.ToString();
                    states = States.OperatorClicked;
                    break;
                case States.SignClicked: // WORKED
                    SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text) + op.ToString();
                    states = States.OperatorClicked;
                    break;
                case States.EvaluationClicked: // WORKED
                    SmallTextBox.Text = result.Content + op.ToString();
                    states = States.OperatorClicked;
                    break;
                case States.ExceptionThrown: // WORKED
                    ResetState();
                    break;
            }
        }

        private void OnEvaluateClicked()
        {
            try
            {
                switch (states)
                {
                    case States.OperandClicked: //WORKED 
                        SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text);
                        result = Parser.Parse(SmallTextBox.Text).Eval();
                        SmallTextBox.Text += "=";
                        LargeTextBox.Text = result.Content;
                        states = States.EvaluationClicked;
                        break;
                    case States.OperatorClicked: // WORKED
                        // Do nothing
                        break;
                    case States.LeftParentheseClicked:
                        // Do nothing
                        break;
                    case States.RightParentheseClicked: // WORKED
                        result = Parser.Parse(SmallTextBox.Text).Eval();
                        SmallTextBox.Text += "=";
                        LargeTextBox.Text = result.Content;
                        states = States.EvaluationClicked;
                        break;
                    case States.DecimalClicked: // WORKED
                        SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text);
                        result = Parser.Parse(SmallTextBox.Text).Eval();
                        SmallTextBox.Text += "=";
                        LargeTextBox.Text = result.Content;
                        states = States.EvaluationClicked;
                        break;
                    case States.SignClicked: // WORKED
                        SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text);
                        result = Parser.Parse(SmallTextBox.Text).Eval();
                        SmallTextBox.Text += "=";
                        LargeTextBox.Text = result.Content;
                        states = States.EvaluationClicked;
                        break;
                    case States.EvaluationClicked: // WORKED
                        // Do nothing
                        break;
                    case States.ExceptionThrown: // WORKED
                        ResetState();
                        break;
                }
            }
            catch (SyntaxException e)
            {
                states = States.ExceptionThrown;
                LargeTextBox.Text = e.Message;
            }
            catch (DivideByZeroException e)
            {
                states = States.ExceptionThrown;
                LargeTextBox.Text = e.Message;
            }
        }

        private int leftParentheseCount = 0;

        private void OnLeftParentheseClicked()
        {
            switch (states)
            {
                case States.OperandClicked: // WORKED
                    SmallTextBox.Text += "(";
                    leftParentheseCount++;
                    states = States.LeftParentheseClicked;
                    break;
                case States.OperatorClicked: // WORKED
                    SmallTextBox.Text += "(";
                    leftParentheseCount++;
                    states = States.LeftParentheseClicked;
                    break;
                case States.LeftParentheseClicked: // WORKED
                    SmallTextBox.Text += "(";
                    leftParentheseCount++;
                    states = States.LeftParentheseClicked;
                    break;
                case States.RightParentheseClicked: // WORKED
                    // Do nothing
                    break;
                case States.DecimalClicked: // WORKED
                    SmallTextBox.Text += "(";
                    leftParentheseCount++;
                    states = States.LeftParentheseClicked;
                    break;
                case States.SignClicked: // WORKED
                    SmallTextBox.Text += "(";
                    leftParentheseCount++;
                    states = States.LeftParentheseClicked;
                    break;
                case States.EvaluationClicked: // WORKED
                    ResetState();
                    SmallTextBox.Text = "(";
                    leftParentheseCount++;
                    states = States.LeftParentheseClicked;
                    break;
                case States.ExceptionThrown: // WORKED
                    ResetState();
                    break;
            }
        }

        private void OnRightParentheseClicked()
        {
            if (leftParentheseCount == 0)
            {
                return;
            }

            switch (states)
            {
                case States.OperandClicked: // WORKED
                    SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text) + ")";
                    leftParentheseCount--;
                    states = States.RightParentheseClicked;
                    break;
                case States.OperatorClicked: // WORKED
                    SmallTextBox.Text += "0)";
                    leftParentheseCount--;
                    states = States.RightParentheseClicked;
                    break;
                case States.LeftParentheseClicked: // WORKED
                    // Do nothing
                    break;
                case States.RightParentheseClicked: // WORKED
                    SmallTextBox.Text += ")";
                    leftParentheseCount--;
                    states = States.RightParentheseClicked;
                    break;
                case States.DecimalClicked: // WORKED
                    SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text) + ")";
                    leftParentheseCount--;
                    states = States.RightParentheseClicked;
                    break;
                case States.SignClicked: // WORKED
                    SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text) + ")";
                    leftParentheseCount--;
                    states = States.RightParentheseClicked;
                    break;
                case States.EvaluationClicked: // WORKED
                    // Do nothing
                    break;
                case States.ExceptionThrown: // WORKED
                    ResetState();
                    break;
            }
        }

        private void OnDecimalClicked()
        {
            switch (states)
            {
                case States.OperandClicked:
                    LargeTextBox.Text += (LargeTextBox.Text.Contains('.')) ? LargeTextBox.Text : LargeTextBox.Text + ".";
                    states = States.DecimalClicked;
                    break;
                case States.OperatorClicked:
                    // Do nothing
                    break;
                case States.LeftParentheseClicked:
                    // Do nothing
                    break;
                case States.RightParentheseClicked:
                    // Do nothing
                    break;
                case States.DecimalClicked:
                    // Do nothing
                    break;
                case States.SignClicked:
                    LargeTextBox.Text += (LargeTextBox.Text.Contains('.')) ? LargeTextBox.Text : LargeTextBox.Text + ".";
                    states = States.DecimalClicked;
                    break;
                case States.EvaluationClicked:
                    ResetState();
                    LargeTextBox.Text += (LargeTextBox.Text.Contains('.')) ? LargeTextBox.Text : LargeTextBox.Text + ".";
                    states = States.DecimalClicked;
                    break;
                case States.ExceptionThrown:
                    ResetState();
                    break;
            }
        }

        private void ResetState()
        {
            SmallTextBox.Text = "";
            LargeTextBox.Text = "0";
            states = States.OperandClicked;
            leftParentheseCount = 0;
            result = null;
        }

        private string NormalizeExpression(string exp)
        {
            string res = exp;
            res = (res.Contains('.')) ? res.TrimEnd('0') : res;
            res = (res[res.Length - 1] == '.') ? res.TrimEnd('.') : res;
            return res;
        }
    }
}

// BUG: 00.5