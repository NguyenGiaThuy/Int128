using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;


namespace Calculator
{
    /// <summary>
    /// Interaction logic for UserControlKeypad.xaml
    /// </summary>
    public partial class UserControlKeypad : UserControl
    {
        private char[] numbers = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
        private bool operatorPressed = false;

        private void onNumberPressed(int num)
        {
            if(!operatorPressed)
            {
                LargeTextBox.Text += num.ToString();
            }
            else
            {
                LargeTextBox.Text = num.ToString();
            }

            operatorPressed = false;
        }

        private void onOperatorPressed(char op)
        {
            if(LargeTextBox.Text.Contains('.'))
            {
                LargeTextBox.Text = LargeTextBox.Text.TrimEnd('0');
                LargeTextBox.Text =
                    (LargeTextBox.Text.LastIndexOf('.') == LargeTextBox.Text.Length - 1) ?
                    LargeTextBox.Text.Remove(LargeTextBox.Text.Length - 1) :
                    LargeTextBox.Text;
            }

            if(!operatorPressed)
            {
                SmallTextBox.Text += LargeTextBox.Text + op;
            }
            else
            {
                SmallTextBox.Text = SmallTextBox.Text.Remove(SmallTextBox.Text.Length - 1) + op;
            }

            operatorPressed = true;
        }

        public UserControlKeypad()
        {
            InitializeComponent();
        }

        private void PlusMinusBtn_Click(object sender, RoutedEventArgs e)
        {
            if(LargeTextBox.Text != "0")
            {
                LargeTextBox.Text =
                    (LargeTextBox.Text.Contains('-')) ?
                    LargeTextBox.Text.TrimStart('-') :
                    '-' + LargeTextBox.Text;
            }

            operatorPressed = false;
        }

        private void DotBtn_Click(object sender, RoutedEventArgs e)
        {
            if(!operatorPressed)
            {
                LargeTextBox.Text += (LargeTextBox.Text.Contains('.')) ? "" : ".";
            }
            else
            {
                LargeTextBox.Text = "0.";
            }

            operatorPressed = false;
        }

        private void ZeroBtn_Click(object sender, RoutedEventArgs e)
        {
            onNumberPressed(0);
        }

        private void OneBtn_Click(object sender, RoutedEventArgs e)
        {
            onNumberPressed(1);
        }

        private void TwoBtn_Click(object sender, RoutedEventArgs e)
        {
            onNumberPressed(2);
        }

        private void ThreeBtn_Click(object sender, RoutedEventArgs e)
        {
            onNumberPressed(3);
        }

        private void FourBtn_Click(object sender, RoutedEventArgs e)
        {
            onNumberPressed(4);
        }

        private void FiveBtn_Click(object sender, RoutedEventArgs e)
        {
            onNumberPressed(5);
        }

        private void SixBtn_Click(object sender, RoutedEventArgs e)
        {
            onNumberPressed(6);
        }

        private void SevenBtn_Click(object sender, RoutedEventArgs e)
        {
            onNumberPressed(7);
        }

        private void EightBtn_Click(object sender, RoutedEventArgs e)
        {
            onNumberPressed(8);
        }

        private void NineBtn_Click(object sender, RoutedEventArgs e)
        {
            onNumberPressed(9);
        }

        private void AddBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperatorPressed('+');
        }

        private void SubtractBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperatorPressed('-');
        }

        private void MultiplyBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperatorPressed('×');
        }

        private void DivideBtn_Click(object sender, RoutedEventArgs e)
        {
            onOperatorPressed('÷');
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
        }

        private void BackspaceBtn_Click(object sender, RoutedEventArgs e)
        {
            if(LargeTextBox.Text.Length == 1 ||
                (LargeTextBox.Text.Length == 2 &&
                LargeTextBox.Text.Contains('-')))
            {
                LargeTextBox.Text = "0";
            }
            else if(LargeTextBox.Text.Length > 0)
            {
                LargeTextBox.Text = LargeTextBox.Text.Remove(LargeTextBox.Text.Length - 1);
            }
        }

        private void LargeTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if(LargeTextBox.Text.Length > 1 && !LargeTextBox.Text.Contains('.'))
            {
                LargeTextBox.Text =
                    (LargeTextBox.Text.IndexOfAny(numbers) != -1) ?
                    LargeTextBox.Text.TrimStart('0') :
                    LargeTextBox.Text.Remove(1);
            }
        }

        private void SmallTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
    }
}
