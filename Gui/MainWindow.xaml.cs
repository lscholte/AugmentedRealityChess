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

namespace Gui
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            DataContext = new ViewModel();
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            (DataContext as IDisposable)?.Dispose();
            DataContext = null;
        }

        public void OnImageLeftClick(object sender, MouseEventArgs args)
        {
            Point p = args.GetPosition(Image);
            p.X /= Image.ActualWidth;
            p.Y /= Image.ActualHeight;

            (DataContext as ViewModel)?.HandleLeftClick(p);
        }

        public void OnImageRightClick(object sender, MouseEventArgs args)
        {
            (DataContext as ViewModel)?.HandleRightClick();
        }
    }
}
