object Form1: TForm1
  Left = 271
  Top = 246
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Create Resources Tool'
  ClientHeight = 147
  ClientWidth = 553
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 19
    Width = 22
    Height = 13
    Caption = 'Path'
  end
  object Path: TEdit
    Left = 40
    Top = 16
    Width = 505
    Height = 21
    TabOrder = 0
  end
  object GoButton: TButton
    Left = 228
    Top = 56
    Width = 97
    Height = 65
    Caption = 'GO!'
    TabOrder = 1
    OnClick = GoButtonClick
  end
end
