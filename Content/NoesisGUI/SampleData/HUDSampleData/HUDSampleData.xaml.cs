﻿//      *********    DO NOT MODIFY THIS FILE     *********
//      This file is regenerated by a design tool. Making
//      changes to this file can cause errors.
namespace Expression.Blend.SampleData.HUDSampleData
{
    using System; 
    using System.ComponentModel;

// To significantly reduce the sample data footprint in your production application, you can set
// the DISABLE_SAMPLE_DATA conditional compilation constant and disable sample data at runtime.
#if DISABLE_SAMPLE_DATA
    internal class HUDSampleData { }
#else

    public class HUDSampleData : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        public HUDSampleData()
        {
            try
            {
                Uri resourceUri = new Uri("/ShooterGame;component/SampleData/HUDSampleData/HUDSampleData.xaml", UriKind.RelativeOrAbsolute);
                System.Windows.Application.LoadComponent(this, resourceUri);
            }
            catch
            {
            }
        }

        private string _MatchInfo = string.Empty;

        public string MatchInfo
        {
            get
            {
                return this._MatchInfo;
            }

            set
            {
                if (this._MatchInfo != value)
                {
                    this._MatchInfo = value;
                    this.OnPropertyChanged("MatchInfo");
                }
            }
        }

        private bool _PlayerIsLowHealth = false;

        public bool PlayerIsLowHealth
        {
            get
            {
                return this._PlayerIsLowHealth;
            }

            set
            {
                if (this._PlayerIsLowHealth != value)
                {
                    this._PlayerIsLowHealth = value;
                    this.OnPropertyChanged("PlayerIsLowHealth");
                }
            }
        }

        private bool _MatchIsPlaying = false;

        public bool MatchIsPlaying
        {
            get
            {
                return this._MatchIsPlaying;
            }

            set
            {
                if (this._MatchIsPlaying != value)
                {
                    this._MatchIsPlaying = value;
                    this.OnPropertyChanged("MatchIsPlaying");
                }
            }
        }

        private double _PlayerKills = 0;

        public double PlayerKills
        {
            get
            {
                return this._PlayerKills;
            }

            set
            {
                if (this._PlayerKills != value)
                {
                    this._PlayerKills = value;
                    this.OnPropertyChanged("PlayerKills");
                }
            }
        }

        private string _MatchTime = string.Empty;

        public string MatchTime
        {
            get
            {
                return this._MatchTime;
            }

            set
            {
                if (this._MatchTime != value)
                {
                    this._MatchTime = value;
                    this.OnPropertyChanged("MatchTime");
                }
            }
        }

        private double _PlayerTeamPos = 0;

        public double PlayerTeamPos
        {
            get
            {
                return this._PlayerTeamPos;
            }

            set
            {
                if (this._PlayerTeamPos != value)
                {
                    this._PlayerTeamPos = value;
                    this.OnPropertyChanged("PlayerTeamPos");
                }
            }
        }

        private double _NumTeams = 0;

        public double NumTeams
        {
            get
            {
                return this._NumTeams;
            }

            set
            {
                if (this._NumTeams != value)
                {
                    this._NumTeams = value;
                    this.OnPropertyChanged("NumTeams");
                }
            }
        }

        private double _PlayerHealthAmount = 0;

        public double PlayerHealthAmount
        {
            get
            {
                return this._PlayerHealthAmount;
            }

            set
            {
                if (this._PlayerHealthAmount != value)
                {
                    this._PlayerHealthAmount = value;
                    this.OnPropertyChanged("PlayerHealthAmount");
                }
            }
        }

        private PrimaryWeapon _PrimaryWeapon = new PrimaryWeapon();

        public PrimaryWeapon PrimaryWeapon
        {
            get
            {
                return this._PrimaryWeapon;
            }

            set
            {
                if (this._PrimaryWeapon != value)
                {
                    this._PrimaryWeapon = value;
                    this.OnPropertyChanged("PrimaryWeapon");
                }
            }
        }

        private bool _OutOfAmmo = false;

        public bool OutOfAmmo
        {
            get
            {
                return this._OutOfAmmo;
            }

            set
            {
                if (this._OutOfAmmo != value)
                {
                    this._OutOfAmmo = value;
                    this.OnPropertyChanged("OutOfAmmo");
                }
            }
        }

        private DeathMessages _DeathMessages = new DeathMessages();

        public DeathMessages DeathMessages
        {
            get
            {
                return this._DeathMessages;
            }
        }
    }

    public class PrimaryWeapon : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        private string _IconName = string.Empty;

        public string IconName
        {
            get
            {
                return this._IconName;
            }

            set
            {
                if (this._IconName != value)
                {
                    this._IconName = value;
                    this.OnPropertyChanged("IconName");
                }
            }
        }

        private string _AmmoIconName = string.Empty;

        public string AmmoIconName
        {
            get
            {
                return this._AmmoIconName;
            }

            set
            {
                if (this._AmmoIconName != value)
                {
                    this._AmmoIconName = value;
                    this.OnPropertyChanged("AmmoIconName");
                }
            }
        }

        private double _AmmoInClip = 0;

        public double AmmoInClip
        {
            get
            {
                return this._AmmoInClip;
            }

            set
            {
                if (this._AmmoInClip != value)
                {
                    this._AmmoInClip = value;
                    this.OnPropertyChanged("AmmoInClip");
                }
            }
        }

        private double _AmmoInPocket = 0;

        public double AmmoInPocket
        {
            get
            {
                return this._AmmoInPocket;
            }

            set
            {
                if (this._AmmoInPocket != value)
                {
                    this._AmmoInPocket = value;
                    this.OnPropertyChanged("AmmoInPocket");
                }
            }
        }

        private AmmoIconAlphas _AmmoIconAlphas = new AmmoIconAlphas();

        public AmmoIconAlphas AmmoIconAlphas
        {
            get
            {
                return this._AmmoIconAlphas;
            }
        }
    }

    public class AmmoIconAlphasItem : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }

    public class AmmoIconAlphas : System.Collections.ObjectModel.ObservableCollection<AmmoIconAlphasItem>
    { 
    }

    public class DeathMessagesItem : INotifyPropertyChanged
    {
        private string _KillerDesc = string.Empty;

        public string KillerDesc
        {
            get
            {
                return _KillerDesc;
            }

            set
            {
                if (this._KillerDesc != value)
                {
                    this._KillerDesc = value;
                    this.OnPropertyChanged("KillerDesc");
                }
            }
        }

        private System.Windows.Media.Color _KillerColor = System.Windows.Media.Colors.Black;

        public System.Windows.Media.Color KillerColor
        {
            get
            {
                return _KillerColor;
            }

            set
            {
                if (this._KillerColor != value)
                {
                    this._KillerColor = value;
                    this.OnPropertyChanged("KillerColor");
                }
            }
        }

        private string _DamageTypeIconName = string.Empty;

        public string DamageTypeIconName
        {
            get
            {
                return _DamageTypeIconName;
            }

            set
            {
                if (this._DamageTypeIconName != value)
                {
                    this._DamageTypeIconName = value;
                    this.OnPropertyChanged("DamageTypeIconName");
                }
            }
        }

        private string _VictimDesc = string.Empty;

        public string VictimDesc
        {
            get
            {
                return _VictimDesc;
            }

            set
            {
                if (this._VictimDesc != value)
                {
                    this._VictimDesc = value;
                    this.OnPropertyChanged("VictimDesc");
                }
            }
        }

        private System.Windows.Media.Color _VictimColor = System.Windows.Media.Colors.Black;

        public System.Windows.Media.Color VictimColor
        {
            get
            {
                return _VictimColor;
            }

            set
            {
                if (this._VictimColor != value)
                {
                    this._VictimColor = value;
                    this.OnPropertyChanged("VictimColor");
                }
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }

    public class DeathMessages : System.Collections.ObjectModel.ObservableCollection<DeathMessagesItem>
    {
    }
#endif
}