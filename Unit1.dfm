object Form1: TForm1
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Robots'
  ClientHeight = 491
  ClientWidth = 687
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 423
    Top = 153
    Width = 125
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = 'R1 Commands'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 554
    Top = 153
    Width = 124
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = 'R2 Commands'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 120
    Top = 55
    Width = 92
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = 'Operator'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label4: TLabel
    Left = 8
    Top = 55
    Width = 88
    Height = 16
    Caption = 'All commands'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label5: TLabel
    Left = 239
    Top = 55
    Width = 47
    Height = 16
    Caption = 'Control'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label_Activity: TLabel
    Left = 619
    Top = 27
    Width = 53
    Height = 16
    Hint = 'Timer activity'
    Caption = 'Inactive'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold, fsUnderline]
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
  end
  object Label_Attention: TLabel
    Left = 423
    Top = 2
    Width = 125
    Height = 16
    Hint = 'Distance between robots'
    Alignment = taCenter
    AutoSize = False
    Caption = 'Attention! (53)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold, fsUnderline]
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
  end
  object Label6: TLabel
    Left = 315
    Top = 55
    Width = 92
    Height = 16
    Alignment = taCenter
    AutoSize = False
    Caption = 'Router'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label11: TLabel
    Left = 346
    Top = 10
    Width = 12
    Height = 13
    Caption = '10'
  end
  object Label12: TLabel
    Left = 346
    Top = 29
    Width = 12
    Height = 13
    Caption = '10'
  end
  object Button_Load: TButton
    Left = 8
    Top = 24
    Width = 89
    Height = 25
    Hint = 'Ctrl+L'
    Caption = 'Load commands'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    OnClick = Button_LoadClick
  end
  object Button_Save: TButton
    Left = 120
    Top = 24
    Width = 89
    Height = 25
    Hint = 'Ctrl+S'
    Caption = 'Save log'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    OnClick = Button_SaveClick
  end
  object Edit_Sectors: TLabeledEdit
    Left = 237
    Top = 26
    Width = 49
    Height = 21
    Alignment = taCenter
    EditLabel.Width = 43
    EditLabel.Height = 13
    EditLabel.Caption = 'Sectors'
    EditLabel.Font.Charset = DEFAULT_CHARSET
    EditLabel.Font.Color = clWindowText
    EditLabel.Font.Height = -11
    EditLabel.Font.Name = 'Tahoma'
    EditLabel.Font.Style = [fsBold]
    EditLabel.ParentFont = False
    NumbersOnly = True
    TabOrder = 2
    Text = '50'
    OnExit = Edit_SectorsExit
  end
  object ListBox_AllCommands: TListBox
    Left = 8
    Top = 73
    Width = 89
    Height = 279
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Lucida Console'
    Font.Style = []
    ItemHeight = 13
    ParentFont = False
    TabOrder = 3
  end
  object Button_Add: TButton
    Left = 219
    Top = 99
    Width = 87
    Height = 25
    Hint = 'Ctrl+A'
    Caption = 'Add'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 4
    OnClick = Button_AddClick
  end
  object Button_Edit: TButton
    Left = 219
    Top = 130
    Width = 87
    Height = 25
    Hint = 'Ctrl+E'
    Caption = 'Edit'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 5
    OnClick = Button_EditClick
  end
  object Button_Delete: TButton
    Left = 219
    Top = 161
    Width = 87
    Height = 25
    Hint = 'Ctrl+D'
    Caption = 'Delete'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 6
    OnClick = Button_DeleteClick
  end
  object Edit_Task: TEdit
    Left = 219
    Top = 72
    Width = 87
    Height = 21
    NumbersOnly = True
    TabOrder = 7
  end
  object Button_GetPosition: TButton
    Left = 219
    Top = 251
    Width = 42
    Height = 25
    Caption = 'GetPos'
    TabOrder = 8
    OnClick = Button_GetPositionClick
  end
  object GroupBox3: TGroupBox
    Left = 219
    Top = 278
    Width = 87
    Height = 74
    Caption = 'SetPosition'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 9
    object Edit_R1Pos: TLabeledEdit
      Left = 35
      Top = 16
      Width = 34
      Height = 24
      EditLabel.Width = 17
      EditLabel.Height = 16
      EditLabel.Caption = 'R1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      LabelPosition = lpLeft
      NumbersOnly = True
      ParentFont = False
      TabOrder = 0
      Text = '37'
    end
    object Edit_R2Pos: TLabeledEdit
      Left = 35
      Top = 44
      Width = 34
      Height = 24
      EditLabel.Width = 17
      EditLabel.Height = 16
      EditLabel.Caption = 'R2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      LabelPosition = lpLeft
      NumbersOnly = True
      ParentFont = False
      TabOrder = 1
      Text = '17'
    end
  end
  object Button_SetPosition: TButton
    Left = 264
    Top = 251
    Width = 42
    Height = 25
    Caption = 'SetPos'
    TabOrder = 10
    OnClick = Button_SetPositionClick
  end
  object ListBox_Log: TListBox
    Left = 8
    Top = 360
    Width = 396
    Height = 125
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Lucida Console'
    Font.Style = []
    ItemHeight = 12
    ParentFont = False
    TabOrder = 11
  end
  object Button_Fill: TButton
    Left = 219
    Top = 190
    Width = 44
    Height = 25
    Hint = 'Ctrl+F'
    Caption = 'Fill'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 12
    OnClick = Button_FillClick
  end
  object Edit_Fill: TEdit
    Left = 268
    Top = 192
    Width = 38
    Height = 21
    NumbersOnly = True
    TabOrder = 13
    Text = '10'
  end
  object CheckBox_Format: TCheckBox
    Left = 8
    Top = 3
    Width = 56
    Height = 17
    Caption = 'Format'
    Checked = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    State = cbChecked
    TabOrder = 14
  end
  object ListBox_R1Commands: TListBox
    Left = 423
    Top = 172
    Width = 125
    Height = 180
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Lucida Console'
    Font.Style = []
    ItemHeight = 13
    ParentFont = False
    TabOrder = 15
  end
  object ListBox_R2Commands: TListBox
    Left = 554
    Top = 172
    Width = 125
    Height = 180
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Lucida Console'
    Font.Style = []
    ItemHeight = 13
    ParentFont = False
    TabOrder = 16
  end
  object ListBox_Operator: TListBox
    Left = 120
    Top = 73
    Width = 89
    Height = 279
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Lucida Console'
    Font.Style = []
    ItemHeight = 13
    ParentFont = False
    TabOrder = 17
    OnClick = ListBox_OperatorClick
  end
  object Button_Start: TButton
    Left = 440
    Top = 24
    Width = 42
    Height = 25
    Hint = 'Ctrl+1'
    Caption = 'Start'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 18
    OnClick = Button_StartClick
  end
  object Button_Stop: TButton
    Left = 488
    Top = 24
    Width = 42
    Height = 25
    Hint = 'Ctrl+2'
    Caption = 'Stop'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 19
    OnClick = Button_StopClick
  end
  object Button_Clear: TButton
    Left = 219
    Top = 221
    Width = 87
    Height = 25
    Hint = 'Shift+C'
    Caption = 'Clear'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 20
    OnClick = Button_ClearClick
  end
  object Edit_Interval: TLabeledEdit
    Left = 560
    Top = 26
    Width = 49
    Height = 21
    Alignment = taCenter
    EditLabel.Width = 46
    EditLabel.Height = 13
    EditLabel.Caption = 'Interval'
    EditLabel.Font.Charset = DEFAULT_CHARSET
    EditLabel.Font.Color = clWindowText
    EditLabel.Font.Height = -11
    EditLabel.Font.Name = 'Tahoma'
    EditLabel.Font.Style = [fsBold]
    EditLabel.ParentFont = False
    NumbersOnly = True
    TabOrder = 21
    Text = '1000'
  end
  object GroupBox5: TGroupBox
    Left = 423
    Top = 62
    Width = 125
    Height = 83
    Caption = 'Robot 1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 22
    object Label_R1Pos: TLabel
      Left = 10
      Top = 21
      Width = 84
      Height = 16
      Caption = 'Position: 100'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label_R1Dir: TLabel
      Left = 10
      Top = 41
      Width = 105
      Height = 16
      Caption = 'Direction: Stand'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label_R1Command: TLabel
      Left = 10
      Top = 61
      Width = 103
      Height = 16
      Caption = 'Command: NONE'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object GroupBox6: TGroupBox
    Left = 554
    Top = 62
    Width = 125
    Height = 83
    Caption = 'Robot 2'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 23
    object Label_R2Pos: TLabel
      Left = 10
      Top = 21
      Width = 84
      Height = 16
      Caption = 'Position: 100'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label_R2Dir: TLabel
      Left = 10
      Top = 41
      Width = 105
      Height = 16
      Caption = 'Direction: Stand'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label_R2Command: TLabel
      Left = 10
      Top = 61
      Width = 103
      Height = 16
      Caption = 'Command: NONE'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object ListBox_Router: TListBox
    Left = 315
    Top = 73
    Width = 89
    Height = 279
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Lucida Console'
    Font.Style = []
    ItemHeight = 13
    ParentFont = False
    TabOrder = 24
    OnClick = ListBox_OperatorClick
  end
  object ListBox_Simulation: TListBox
    Left = 423
    Top = 360
    Width = 255
    Height = 125
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Lucida Console'
    Font.Style = []
    ItemHeight = 12
    ParentFont = False
    TabOrder = 25
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 303
    Top = 1
  end
end
