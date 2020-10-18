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
    /// Interaction logic for UserControlConverter.xaml
    /// </summary>
    public partial class UserControlConverter : UserControl
    {
        public UserControlConverter()
        {
            InitializeComponent();
        }

        private void ProcessBtn_Click(object sender, RoutedEventArgs e)
        {
            int comboBoxIndex = FromComboBox.SelectedIndex;

            switch(comboBoxIndex)
            {
                case 0:
                    Output1Label.Content = "Binary";
                    break;
                case 1:
                    Output1Label.Content = "Decimal";
                    break;
                case 2:
                    Output1Label.Content = "Decimal";
                    break;
                default:
                    break;
            }
        }
    }
}
