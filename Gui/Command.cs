using System;
using System.Windows.Input;

namespace Gui
{
    /// <summary>
    /// Performs an action. Allows binding an action to a button in WPF.
    /// </summary>
    internal class Command
        : ICommand
    {
        public delegate void VoidHandler();

        private VoidHandler Action { get; }

        public Command(VoidHandler action)
        {
            Action = action;
        }

#pragma warning disable 67
        public event EventHandler? CanExecuteChanged;
#pragma warning restore 67

        public bool CanExecute(object? parameter)
        {
            return true;
        }

        public void Execute(object? parameter)
        {
            Action();
        }
    }
}
