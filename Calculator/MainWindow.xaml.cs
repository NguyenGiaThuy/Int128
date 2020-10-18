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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private UserControlKeypad userControlKeypad = new UserControlKeypad();
        private UserControlConverter userControlConverter = new UserControlConverter();

        public MainWindow()
        {
            InitializeComponent();
            ContentGrid.Children.Clear();
            ContentGrid.Children.Add(userControlKeypad);
        }

        private void FeaturesList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            int index = FeaturesList.SelectedIndex;

            switch(index)
            {
                case 0:
                    ContentGrid.Children.Clear();
                    ContentGrid.Children.Add(userControlKeypad);
                    break;
                case 1:
                    ContentGrid.Children.Clear();
                    ContentGrid.Children.Add(userControlConverter);
                    break;
                default:
                    break;
            }
        }
    }
}
