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
        private bool operatorClicked = false;
        private bool exceptionThrown = false;
        private QInt comp = new QInt("0");
        private QInt result;

        public UserControlKeypad()
        {
            InitializeComponent();
            result = comp;
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
                resetState();
            }

            operatorClicked = false;
        }

        private void DotBtn_Click(object sender, RoutedEventArgs e)
        {
            onDotClicked();
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
            operatorClicked = false;
        }

        private void ClearBtn_Click(object sender, RoutedEventArgs e)
        {
            resetState();
        }

        private void EqualBtn_Click(object sender, RoutedEventArgs e)
        {
            onEqualClicked();
        }

        private void BackspaceBtn_Click(object sender, RoutedEventArgs e)
        {
            onBackspaceClicked();
        }

        private void LeftParenthese_Click(object sender, RoutedEventArgs e)
        {

        }

        private void RightParenthese_Click(object sender, RoutedEventArgs e)
        {

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

        private void LargeTextBox_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            Keyboard.Focus(LargeTextBox);
        }

        private void LargeTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.Enter:
                    onEqualClicked();
                    break;
                case Key.Back:
                    onBackspaceClicked();
                    break;
            }
        }
        
        private void LargeTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.NumPad0:
                    onOperandClicked(0);
                    break;
                case Key.NumPad1:
                    onOperandClicked(1);
                    break;
                case Key.NumPad2:
                    onOperandClicked(2);
                    break;
                case Key.NumPad3:
                    onOperandClicked(3);
                    break;
                case Key.NumPad4:
                    onOperandClicked(4);
                    break;
                case Key.NumPad5:
                    onOperandClicked(5);
                    break;
                case Key.NumPad6:
                    onOperandClicked(6);
                    break;
                case Key.NumPad7:
                    onOperandClicked(7);
                    break;
                case Key.NumPad8:
                    onOperandClicked(8);
                    break;
                case Key.NumPad9:
                    onOperandClicked(9);
                    break;
                case Key.Add:
                    onOperatorClicked('+');
                    break;
                case Key.Subtract:
                    onOperatorClicked('-');
                    break;
                case Key.Multiply:
                    onOperatorClicked('×');
                    break;
                case Key.Divide:
                    onOperatorClicked('÷');
                    break;
                case Key.Decimal:
                    onDotClicked();
                    break;
            }
        }

        private void onOperandClicked(int num)
        {
            if (!operatorClicked)
            {
                LargeTextBox.Text += (LargeTextBox.Text.Length < 40) ? num.ToString() : "";
            }
            else
            {
                LargeTextBox.Text = num.ToString();
            }

            if (result != comp)
            {
                LargeTextBox.Text = num.ToString();
                SmallTextBox.Text = "";
                result = comp;
            }

            if (exceptionThrown)
            {
                resetState();
            }

            operatorClicked = false;
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

            if (!operatorClicked)
            {
                SmallTextBox.Text += LargeTextBox.Text + op;
            }
            else
            {
                SmallTextBox.Text = SmallTextBox.Text.Remove(SmallTextBox.Text.Length - 1) + op;
            }

            if (result != comp)
            {
                SmallTextBox.Text = result.Content + op;
                result = comp;
            }

            if (exceptionThrown)
            {
                resetState();
            }

            operatorClicked = true;
        }

        private void onEqualClicked()
        {
            if (LargeTextBox.Text.Contains('.'))
            {
                LargeTextBox.Text = LargeTextBox.Text.TrimEnd('0');
                LargeTextBox.Text =
                    (LargeTextBox.Text.LastIndexOf('.') == LargeTextBox.Text.Length - 1) ?
                    LargeTextBox.Text.Remove(LargeTextBox.Text.Length - 1) :
                    LargeTextBox.Text;
            }

            if (!operatorClicked)
            {
                if (result == comp)
                {
                    try
                    {
                        SmallTextBox.Text += LargeTextBox.Text;
                        result = Parser.Parse(SmallTextBox.Text).Eval();
                        SmallTextBox.Text += '=';
                        LargeTextBox.Text = result.Content;
                    }
                    catch (DivideByZeroException ex)
                    {
                        exceptionThrown = true;
                        LargeTextBox.Text = ex.Message;
                    }
                }
                else
                {
                    SmallTextBox.Text = LargeTextBox.Text + '=';
                }
            }
        }

        private void onBackspaceClicked()
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

        private void onDotClicked()
        {
            if (!operatorClicked)
            {
                LargeTextBox.Text += (LargeTextBox.Text.Contains('.')) ? "" : ".";
            }
            else
            {
                LargeTextBox.Text = "0.";
            }

            if (exceptionThrown)
            {
                resetState();
            }

            operatorClicked = false;
        }

        private void onParentheseClicked(char pa)
        {
        }

        private void resetState()
        {
            SmallTextBox.Text = "";
            LargeTextBox.Text = "0";
            operatorClicked = false;
            exceptionThrown = false;
            result = comp;
        }
    }
}
