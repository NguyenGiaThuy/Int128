using System;
using System.Linq;
using System.Runtime.InteropServices;
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

        private enum State
        {
            OperandClicked,
            OperatorClicked,
            LeftParentheseClicked,
            RightParentheseClicked,
            EvaluationClicked,
            ExceptionThrown
        }

        public UserControlKeypad()
        {
            InitializeComponent();
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

        private State state;

        private void OnOperandClicked(int num)
        {
            switch (state)
            {
                case State.OperandClicked:
                    if(LargeTextBox.Text.Contains('-'))
                    {
                        LargeTextBox.Text += (LargeTextBox.Text.Length < 39) ? num.ToString() : "";
                    }
                    else
                    {
                        LargeTextBox.Text += (LargeTextBox.Text.Length < 38) ? num.ToString() : "";
                    }

                    state = State.OperandClicked;
                    break;
                case State.OperatorClicked:
                    LargeTextBox.Text = num.ToString();
                    state = State.OperandClicked;
                    break;
                case State.LeftParentheseClicked:
                    LargeTextBox.Text = num.ToString();
                    state = State.OperandClicked;
                    break;
                case State.RightParentheseClicked:
                    // Do nothing
                    break;
                case State.EvaluationClicked: 
                    ResetState();
                    LargeTextBox.Text = num.ToString();
                    state = State.OperandClicked;
                    break;
                case State.ExceptionThrown: 
                    ResetState();
                    break;
            }
        }

        private void OnOperatorClicked(char op)
        {
            switch (state)
            {
                case State.OperandClicked: 
                    SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text) + op.ToString();
                    LargeTextBox.Text = "0";
                    state = State.OperatorClicked;
                    break;
                case State.OperatorClicked: 
                    SmallTextBox.Text = SmallTextBox.Text.Remove(SmallTextBox.Text.Length - 1) + op.ToString();
                    state = State.OperatorClicked;
                    break;
                case State.LeftParentheseClicked: 
                    SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text) + op.ToString();
                    LargeTextBox.Text = "0";
                    state = State.OperatorClicked;
                    break;
                case State.RightParentheseClicked: 
                    SmallTextBox.Text += op.ToString();
                    state = State.OperatorClicked;
                    break;
                case State.EvaluationClicked: 
                    SmallTextBox.Text = result.Content + op.ToString();
                    LargeTextBox.Text = "0";
                    state = State.OperatorClicked;
                    break;
                case State.ExceptionThrown: 
                    ResetState();
                    break;
            }
        }

        private void OnEvaluateClicked()
        {
            try
            {
                switch (state)
                {
                    case State.OperandClicked:
                        SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text);
                        result = Parser.Parse(SmallTextBox.Text).Eval();
                        SmallTextBox.Text += "=";
                        LargeTextBox.Text = result.Content;
                        state = State.EvaluationClicked;
                        break;
                    case State.OperatorClicked: 
                        // Do nothing
                        break;
                    case State.LeftParentheseClicked:
                        // Do nothing
                        break;
                    case State.RightParentheseClicked: 
                        result = Parser.Parse(SmallTextBox.Text).Eval();
                        SmallTextBox.Text += "=";
                        LargeTextBox.Text = result.Content;
                        state = State.EvaluationClicked;
                        break;
                    case State.EvaluationClicked:
                        SmallTextBox.Text = LargeTextBox.Text + "=";
                        state = State.EvaluationClicked;
                        break;
                    case State.ExceptionThrown: 
                        ResetState();
                        break;
                }
            }
            catch (SyntaxException e)
            {
                state = State.ExceptionThrown;
                LargeTextBox.Text = e.Message;
            }
            catch (DivideByZeroException e)
            {
                state = State.ExceptionThrown;
                LargeTextBox.Text = e.Message;
            }
            catch(OverflowException e)
            {
                state = State.ExceptionThrown;
                LargeTextBox.Text = e.Message;
            }
            catch(ExternalException)
            {
                state = State.ExceptionThrown;
                LargeTextBox.Text = "Bug occured";
            }
        }

        private int leftParentheseCount = 0;

        private void OnLeftParentheseClicked()
        {
            switch (state)
            {
                case State.OperandClicked: 
                    SmallTextBox.Text += "(";
                    LargeTextBox.Text = "0";
                    leftParentheseCount++;
                    state = State.LeftParentheseClicked;
                    break;
                case State.OperatorClicked: 
                    SmallTextBox.Text += "(";
                    leftParentheseCount++;
                    state = State.LeftParentheseClicked;
                    break;
                case State.LeftParentheseClicked: 
                    SmallTextBox.Text += "(";
                    leftParentheseCount++;
                    state = State.LeftParentheseClicked;
                    break;
                case State.RightParentheseClicked: 
                    // Do nothing
                    break;
                case State.EvaluationClicked: 
                    ResetState();
                    SmallTextBox.Text = "(";
                    leftParentheseCount++;
                    state = State.LeftParentheseClicked;
                    break;
                case State.ExceptionThrown: 
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

            switch (state)
            {
                case State.OperandClicked: 
                    SmallTextBox.Text += NormalizeExpression(LargeTextBox.Text) + ")";
                    LargeTextBox.Text = "0";
                    leftParentheseCount--;
                    state = State.RightParentheseClicked;
                    break;
                case State.OperatorClicked: 
                    SmallTextBox.Text += "0)";
                    leftParentheseCount--;
                    state = State.RightParentheseClicked;
                    break;
                case State.LeftParentheseClicked: 
                    // Do nothing
                    break;
                case State.RightParentheseClicked: 
                    SmallTextBox.Text += ")";
                    leftParentheseCount--;
                    state = State.RightParentheseClicked;
                    break;
                case State.EvaluationClicked: 
                    // Do nothing
                    break;
                case State.ExceptionThrown: 
                    ResetState();
                    break;
            }
        }

        private void OnDecimalClicked()
        {
            switch (state)
            {
                case State.OperandClicked: 
                    LargeTextBox.Text = (LargeTextBox.Text.Contains('.')) ?
                        LargeTextBox.Text : LargeTextBox.Text + ".";
                    state = State.OperandClicked;
                    break;
                case State.OperatorClicked: 
                    LargeTextBox.Text += ".";
                    state = State.OperandClicked;
                    break;
                case State.LeftParentheseClicked: 
                    LargeTextBox.Text += ".";
                    state = State.OperandClicked;
                    break;
                case State.RightParentheseClicked: 
                    // Do nothing
                    break;
                case State.EvaluationClicked: 
                    ResetState();
                    LargeTextBox.Text += ".";
                    state = State.OperandClicked;
                    break;
                case State.ExceptionThrown: 
                    ResetState();
                    break;
            }
        }

        private void OnSignClicked()
        {
            switch (state)
            {
                case State.OperandClicked: 
                    LargeTextBox.Text = (LargeTextBox.Text.Contains('-')) ?
                        LargeTextBox.Text.TrimStart('-') : "-" + LargeTextBox.Text;
                    break;
                case State.OperatorClicked: 
                    // Do nothing
                    break;
                case State.LeftParentheseClicked: 
                    // Do nothing
                    break;
                case State.RightParentheseClicked: 
                    // Do nothing
                    break;
                case State.EvaluationClicked:
                    LargeTextBox.Text = (LargeTextBox.Text.Contains('-')) ?
                        LargeTextBox.Text.TrimStart('-') : "-" + LargeTextBox.Text;
                    result = ~result + new QInt("1");
                    break;
                case State.ExceptionThrown: 
                    ResetState();
                    break;
            }
        }

        private void ResetState()
        {
            SmallTextBox.Text = "";
            LargeTextBox.Text = "0";
            state = State.OperandClicked;
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

        private void PlusMinusBtn_Click(object sender, RoutedEventArgs e)
        {
            OnSignClicked();
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
            state = State.OperandClicked;
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
            if (LargeTextBox.Text.Contains('-') &&
                LargeTextBox.Text.IndexOfAny(numbers) == -1)
            {
                LargeTextBox.Text = LargeTextBox.Text.TrimStart('-');
            }

            if (LargeTextBox.Text.Length > 1 &&
                !LargeTextBox.Text.Contains('.') &&
                state != State.ExceptionThrown)
            {
                LargeTextBox.Text =
                    (LargeTextBox.Text.IndexOfAny(numbers) != -1) ?
                    LargeTextBox.Text.TrimStart('0') :
                    LargeTextBox.Text.Remove(1);
            }
        }

        private void LargeTextBox_Loaded(object sender, RoutedEventArgs e)
        {
           Keyboard.Focus(LargeTextBox);
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
                case Key.Delete:
                    ResetState();
                    break;
            }
        }

        private void LargeTextBox_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (!Keyboard.IsKeyDown(Key.LeftShift))
            {
                switch (e.Key)
                {
                    case Key.NumPad0:
                    case Key.D0:
                        OnOperandClicked(0);
                        break;
                    case Key.NumPad1:
                    case Key.D1:
                        OnOperandClicked(1);
                        break;
                    case Key.NumPad2:
                    case Key.D2:
                        OnOperandClicked(2);
                        break;
                    case Key.NumPad3:
                    case Key.D3:
                        OnOperandClicked(3);
                        break;
                    case Key.NumPad4:
                    case Key.D4:
                        OnOperandClicked(4);
                        break;
                    case Key.NumPad5:
                    case Key.D5:
                        OnOperandClicked(5);
                        break;
                    case Key.NumPad6:
                    case Key.D6:
                        OnOperandClicked(6);
                        break;
                    case Key.NumPad7:
                    case Key.D7:
                        OnOperandClicked(7);
                        break;
                    case Key.NumPad8:
                    case Key.D8:
                        OnOperandClicked(8);
                        break;
                    case Key.NumPad9:
                    case Key.D9:
                        OnOperandClicked(9);
                        break;
                    case Key.Add:
                        OnOperatorClicked('+');
                        break;
                    case Key.Subtract:
                    case Key.OemMinus:
                        OnOperatorClicked('-');
                        break;
                    case Key.Multiply:
                        OnOperatorClicked('×');
                        break;
                    case Key.Divide:
                    case Key.OemQuestion:
                        OnOperatorClicked('÷');
                        break;
                    case Key.Decimal:
                    case Key.OemPeriod:
                        OnDecimalClicked();
                        break;
                    case Key.S:
                        OnSignClicked();
                        break;
                    case Key.Back:
                        OnBackspaceClicked();
                        break;
                }
            }
            else
            {
                switch (e.Key)
                {
                    case Key.OemPlus:
                        OnOperatorClicked('+');
                        break;
                    case Key.D8:
                        OnOperatorClicked('×');
                        break;
                    case Key.D9:
                        OnLeftParentheseClicked();
                        break;
                    case Key.D0:
                        OnRightParentheseClicked();
                        break;
                }
            }
        }
    }
}