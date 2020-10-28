using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
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
using System.Text.RegularExpressions;

using CLI.Int128;

namespace Calculator
{
    /// <summary>
    /// Interaction logic for UserControlConverter.xaml
    /// </summary>
    public partial class UserControlConverter : UserControl
    {
        public UserControlConverter()
        {
            InitializeComponent();
        }

        private void ResetState()
        {
            InputTextbox.Text = "";
            OutputTextBox1.Text = "";
            OutputTextBox2.Text = "";
        }

        private void FromComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (IsLoaded)
            {
                ResetState();

                int selectedIndex = FromComboBox.SelectedIndex;
                switch (selectedIndex)
                {
                    case 0:
                        OutputLabel1.Content = "To binary";
                        OutputLabel2.Content = "To hexadecimal";
                        break;
                    case 1:
                        OutputLabel1.Content = "To decimal";
                        OutputLabel2.Content = "To hexadecimal";
                        break;
                    case 2:
                        OutputLabel1.Content = "To decimal";
                        OutputLabel2.Content = "To binary";
                        break;
                    default:
                        break;
                }
            }

            Keyboard.Focus(InputTextbox);
        }

        private void InputTextbox_Loaded(object sender, RoutedEventArgs e)
        {
            Keyboard.Focus(InputTextbox);
        }

        private void ProcessBtn_Click(object sender, RoutedEventArgs e)
        {
            string strToConvert = InputTextbox.Text;
            string strAfterConvert;

            if (!strToConvert.Equals(""))
            {
                int selectedIndex = FromComboBox.SelectedIndex;
                switch (selectedIndex)
                {
                    case 0:
                        strAfterConvert = QInt.DecToBin(strToConvert);
                        OutputTextBox1.Text = strAfterConvert;
                        strAfterConvert = QInt.BinToHex(strAfterConvert);
                        OutputTextBox2.Text = strAfterConvert;
                        break;
                    case 1:
                        strAfterConvert = QInt.BinToDec(strToConvert);
                        OutputTextBox1.Text = strAfterConvert;
                        strAfterConvert = QInt.BinToHex(strToConvert);
                        OutputTextBox2.Text = strAfterConvert;
                        break;
                    case 2:
                        strAfterConvert = QInt.HexToBin(strToConvert);
                        OutputTextBox1.Text = strAfterConvert;
                        strAfterConvert = QInt.BinToDec(strAfterConvert);
                        OutputTextBox2.Text = strAfterConvert;
                        break;
                }
            }
        }

        private void InputTextbox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                string strToConvert = InputTextbox.Text;
                string strAfterConvert;

                if (!strToConvert.Equals(""))
                {
                    int comboBoxIndex = FromComboBox.SelectedIndex;
                    switch (comboBoxIndex)
                    {
                        case 0:
                            strAfterConvert = QInt.DecToBin(strToConvert);
                            OutputTextBox1.Text = strAfterConvert;
                            strAfterConvert = QInt.BinToHex(strAfterConvert);
                            OutputTextBox2.Text = strAfterConvert;
                            break;
                        case 1:
                            strAfterConvert = QInt.BinToDec(strToConvert);
                            OutputTextBox1.Text = strAfterConvert;
                            strAfterConvert = QInt.BinToHex(strToConvert);
                            OutputTextBox2.Text = strAfterConvert;
                            break;
                        case 2:
                            strAfterConvert = QInt.HexToBin(strToConvert);
                            OutputTextBox1.Text = strAfterConvert;
                            strAfterConvert = QInt.BinToDec(strAfterConvert);
                            OutputTextBox2.Text = strAfterConvert;
                            break;
                    }
                }
            }

            Keyboard.Focus(InputTextbox);
        }

        private Regex regexHex = new Regex("[0-9a-fA-F]+");
        private Regex regexBin = new Regex("[0-1]+");
        private Regex regexDec = new Regex("[0-9]+");

        private void InputTextbox_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            int selectedIndex = FromComboBox.SelectedIndex;
            switch (selectedIndex)
            {
                case 0:
                    if (InputTextbox.Text.Contains("-"))
                    {
                        e.Handled = !regexDec.IsMatch(e.Text);
                    }
                    else
                    {
                        e.Handled = !Regex.IsMatch(e.Text, "[-0-9]+");
                    }
                    break;
                case 1:
                    e.Handled = !regexBin.IsMatch(e.Text);
                    break;
                case 2:
                    e.Handled = !regexHex.IsMatch(e.Text);
                    break;
            }
        }


        private void InputTextbox_TextChanged(object sender, TextChangedEventArgs e)
        {
            int selectedIndex = FromComboBox.SelectedIndex;
            switch (selectedIndex)
            {
                case 0:
                    InputTextbox.MaxLength = (InputTextbox.Text.Contains('-')) ? 39 : 38;
                    break;
                case 1:
                    InputTextbox.MaxLength = 128;
                    break;
                case 2:
                    InputTextbox.MaxLength = 32;
                    break;
            }

            int cursorPos = InputTextbox.CaretIndex;
            if (InputTextbox.Text.Contains(" "))
            {
                InputTextbox.Text = Regex.Replace(InputTextbox.Text, @"\s+", "");
                InputTextbox.Select(cursorPos - 1, 0);
            }
        }
    }
}
