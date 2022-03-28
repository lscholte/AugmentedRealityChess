using Microsoft.Win32;
using System;
using System.ComponentModel;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;

namespace Gui
{

    internal class ViewModel
        : INotifyPropertyChanged
        , IDisposable
    {
        private static readonly string m_calibrationFileFilter = "CSV files (*.csv)|*.csv|All files|*.*";
        private static readonly int targetCameraFps = 60;

        private ManagedCamera m_camera = new ManagedCamera();


        public ViewModel()
        {
            Task.Run(() =>
            {
                while (true)
                {
                    Application.Current?.Dispatcher.Invoke(() =>
                    {
                        Image = m_camera.GetImage(ShowCalibrationInfo);
                        CanSaveCalibrationImage = m_camera.CanSaveCalibrationImage();
                        CanCalibrateCamera = m_camera.CanCalibrateCamera();
                    });
                    Thread.Sleep(1000 / targetCameraFps);
                }
            });
        }

        public void Dispose()
        {
            m_camera.Dispose();
        }

        #region Commands

        public ICommand LoadCameraCalibrationCommand => new Command(() =>
        {
            var dialog = new OpenFileDialog()
            {
                Filter = m_calibrationFileFilter
            };
            dialog.ShowDialog();

            bool isLoaded = m_camera.LoadCalibrationFromFile(dialog.FileName);
            if (!isLoaded)
            {
                MessageBox.Show("Failed to load camera calibration data");
                return;
            }

            IsCalibrated = isLoaded;
            ReprojectionError = m_camera.GetReprojectionError();
            CalibrationImageCount = m_camera.GetCalibrationImageCount();
        });

        public ICommand SaveCameraCalibrationCommand => new Command(() =>
        {
            var dialog = new SaveFileDialog()
            {
                Filter = m_calibrationFileFilter
            };
            dialog.ShowDialog();

            bool isSaved = m_camera.SaveCalibrationToFile(dialog.FileName);
            if (!isSaved)
            {
                MessageBox.Show("Failed to save camera calibration data");
            }
        });

        public ICommand SaveCalibrationImageCommand => new Command(() =>
        {
            m_camera.SaveCalibrationImage();
            CalibrationImageCount = m_camera.GetCalibrationImageCount();
        });

        public ICommand CalibrateCameraCommand => new Command(() =>
        {
            IsCalibrated = m_camera.CalibrateCamera();
            if (IsCalibrated)
            {
                ReprojectionError = m_camera.GetReprojectionError();
            }
        });

        public ICommand ResetCalibrationCommand => new Command(() =>
        {
            m_camera.ResetCalibration();
            CalibrationImageCount = m_camera.GetCalibrationImageCount();
            IsCalibrated = m_camera.CalibrateCamera();
            ReprojectionError = m_camera.GetReprojectionError();
        });

        public void HandleClick(Point point)
        {
            m_camera.HandleClick((float)point.X, (float)point.Y);
        }

        #endregion

        #region Properties

        private void SetValue<T>(ref T target, T value, params string[] changedPropertyNames)
        {
            target = value;
            foreach (string changedPropertyName in changedPropertyNames)
            {
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(changedPropertyName));
            }
        }

        private bool m_canSaveCalibrationImage = false;
        public bool CanSaveCalibrationImage
        {
            get => m_canSaveCalibrationImage;
            private set => SetValue(ref m_canSaveCalibrationImage, value, nameof(CanSaveCalibrationImage));
        }

        private int m_calibrationImageCount = 0;
        public int CalibrationImageCount
        {
            get => m_calibrationImageCount;
            private set => SetValue(ref m_calibrationImageCount, value, nameof(CalibrationImageCount));
        }

        private bool m_canCalibrateCamera = false;
        public bool CanCalibrateCamera
        {
            get => m_canCalibrateCamera;
            private set => SetValue(ref m_canCalibrateCamera, value, nameof(CanCalibrateCamera));
        }

        private ImageSource? m_image;
        public ImageSource? Image
        {
            get => m_image;
            private set => SetValue(ref m_image, value, nameof(Image));
        }


        public bool ShowCalibrationInfo { get; set; } = true;

        private bool m_isCalibrated = false;
        public bool IsCalibrated
        {
            get => m_isCalibrated;
            private set => SetValue(ref m_isCalibrated, value, nameof(IsCalibrated));
        }

        private double m_reprojectionError;
        public double ReprojectionError
        {
            get => m_reprojectionError;
            private set => SetValue(ref m_reprojectionError, value, nameof(ReprojectionError));
        }

        #endregion

        public event PropertyChangedEventHandler? PropertyChanged;
    }
}
