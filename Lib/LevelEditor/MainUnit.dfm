object MainForm: TMainForm
  Left = 265
  Top = 141
  Width = 763
  Height = 748
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Action Force II Remake Level Editor'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object BackBuffer: TImage
    Left = 0
    Top = 0
    Width = 640
    Height = 480
    Visible = False
  end
  object Label1: TLabel
    Left = 664
    Top = 80
    Width = 34
    Height = 13
    Caption = #1082#1086#1101#1092#1092
  end
  object VScrollBar: TScrollBar
    Left = 641
    Top = 0
    Width = 17
    Height = 481
    Kind = sbVertical
    PageSize = 0
    TabOrder = 0
    OnChange = VScrollBarChange
  end
  object HScrollBar: TScrollBar
    Left = 0
    Top = 481
    Width = 641
    Height = 17
    PageSize = 0
    TabOrder = 1
    OnChange = HScrollBarChange
  end
  object TilesPaletteCtrl: TTabControl
    Left = 0
    Top = 497
    Width = 665
    Height = 216
    Style = tsFlatButtons
    TabOrder = 2
    OnChange = TilesPaletteCtrlChange
    object TilesScrollBox: TScrollBox
      Left = 0
      Top = 20
      Width = 665
      Height = 193
      TabOrder = 0
    end
  end
  object LayersListBox: TCheckListBox
    Left = 664
    Top = 128
    Width = 65
    Height = 33
    OnClickCheck = LayersListBoxClickCheck
    ItemHeight = 13
    TabOrder = 3
    OnClick = LayersListBoxClick
  end
  object CoeffLabel: TEdit
    Left = 664
    Top = 96
    Width = 41
    Height = 21
    ReadOnly = True
    TabOrder = 4
    Text = '1'
  end
  object ShowGridCheckBox: TCheckBox
    Left = 664
    Top = 8
    Width = 73
    Height = 25
    Caption = #1089#1077#1090#1082#1072
    Checked = True
    State = cbChecked
    TabOrder = 5
    OnClick = ShowGridCheckBoxClick
  end
  object PlusButton: TButton
    Left = 704
    Top = 96
    Width = 17
    Height = 9
    Caption = '+'
    TabOrder = 6
    OnClick = PlusButtonClick
  end
  object MinusButton: TButton
    Left = 704
    Top = 104
    Width = 17
    Height = 10
    Caption = '-'
    TabOrder = 7
    OnClick = MinusButtonClick
  end
  object OpenButton: TButton
    Left = 680
    Top = 560
    Width = 73
    Height = 33
    Caption = #1054#1090#1082#1088#1099#1090#1100
    TabOrder = 8
    OnClick = OpenButtonClick
  end
  object SaveButton: TButton
    Left = 680
    Top = 600
    Width = 73
    Height = 33
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 9
    OnClick = SaveButtonClick
  end
  object HighlightCheckBox: TCheckBox
    Left = 664
    Top = 24
    Width = 73
    Height = 25
    Caption = #1087#1086#1076#1089#1074#1077#1090#1082#1072
    TabOrder = 10
    OnClick = HighlightCheckBoxClick
  end
  object AnimationCheckBox: TCheckBox
    Left = 664
    Top = 48
    Width = 73
    Height = 17
    Caption = #1072#1085#1080#1084#1072#1094#1080#1103
    TabOrder = 11
    OnClick = AnimationCheckBoxClick
  end
  object OpenDialog: TOpenDialog
    Filter = #1060#1072#1081#1083#1099' '#1091#1088#1086#1074#1085#1077#1081' (*.lev)|*.lev|'#1042#1089#1077' '#1092#1072#1081#1083#1099' (*.*)|*.*'
    Left = 664
    Top = 440
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'lev'
    Filter = #1060#1072#1081#1083#1099' '#1091#1088#1086#1074#1085#1077#1081' (*.lev)|*.lev|'#1042#1089#1077' '#1092#1072#1081#1083#1099' (*.*)|*.*'
    Left = 664
    Top = 472
  end
  object AnimatedTilesTimer: TTimer
    Enabled = False
    OnTimer = AnimatedTilesTimerTimer
    Left = 704
    Top = 640
  end
end
