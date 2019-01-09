object Form1: TForm1
  Left = 747
  Top = 250
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Configuration'
  ClientHeight = 375
  ClientWidth = 169
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 130
    Height = 18
    Caption = 'Screen options'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Verdana'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 112
    Width = 127
    Height = 18
    Caption = 'Sound Options'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Verdana'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 8
    Top = 192
    Width = 61
    Height = 18
    Caption = 'Cheats'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Verdana'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object cb_f: TCheckBox
    Left = 8
    Top = 32
    Width = 97
    Height = 17
    Caption = 'Full screen'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object cb_ie: TCheckBox
    Left = 8
    Top = 216
    Width = 121
    Height = 17
    Caption = 'Infinite Energy'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
  end
  object cb_ia: TCheckBox
    Left = 8
    Top = 240
    Width = 97
    Height = 17
    Caption = 'Infinite Ammo'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
  end
  object cb_t: TCheckBox
    Left = 8
    Top = 264
    Width = 121
    Height = 17
    Caption = 'Stop All Timers'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
  end
  object cb_fps: TCheckBox
    Left = 8
    Top = 56
    Width = 97
    Height = 17
    Caption = 'Show FPS'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object cb_dj: TCheckBox
    Left = 8
    Top = 160
    Width = 137
    Height = 17
    Caption = 'Disable Jukebox'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
  end
  object cb_l: TCheckBox
    Left = 8
    Top = 288
    Width = 129
    Height = 17
    Caption = 'Get All Lift Cards'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
  end
  object cb_kd: TCheckBox
    Left = 8
    Top = 312
    Width = 161
    Height = 17
    Caption = 'Get 50 Keys and Disks'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
  end
  object cb_ns: TCheckBox
    Left = 8
    Top = 136
    Width = 97
    Height = 17
    Caption = 'No Sound'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
  end
  object cb_se: TCheckBox
    Left = 8
    Top = 80
    Width = 137
    Height = 17
    Caption = 'Enable Scrolling'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
  end
  object btnSave: TButton
    Left = 28
    Top = 344
    Width = 113
    Height = 25
    Caption = 'Save Configuration'
    TabOrder = 10
    OnClick = btnSaveClick
  end
end
