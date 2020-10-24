using System.Windows;
using System.Windows.Controls;

namespace Calculator
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            ContentGrid.Children.Clear();
            ContentGrid.Children.Add(new UserControlKeypad());
        }

        private void FeaturesList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            int index = FeaturesList.SelectedIndex;

            switch(index)
            {
                case 0:
                    ContentGrid.Children.Clear();
                    ContentGrid.Children.Add(new UserControlKeypad());
                    break;
                case 1:
                    ContentGrid.Children.Clear();
                    ContentGrid.Children.Add(new UserControlConverter());
                    break;
                default:
                    break;
            }
        }
    }
}
