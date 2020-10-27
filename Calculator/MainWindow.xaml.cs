using System.Windows;

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
        }

        private void ConverterList_MouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            ContentGrid.Children.Clear();
            ContentGrid.Children.Add(new UserControlConverter());
        }

        private void KeypadList_MouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            ContentGrid.Children.Clear();
            ContentGrid.Children.Add(new UserControlKeypad());
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            ContentGrid.Children.Clear();
            ContentGrid.Children.Add(new UserControlKeypad());
        }
    }
}
