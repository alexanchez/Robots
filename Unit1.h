//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cspin.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_Load;
	TButton *Button_Save;
	TLabeledEdit *Edit_Sectors;
	TListBox *ListBox_AllCommands;
	TButton *Button_Add;
	TButton *Button_Edit;
	TButton *Button_Delete;
	TEdit *Edit_Task;
	TButton *Button_GetPosition;
	TGroupBox *GroupBox3;
	TLabeledEdit *Edit_R1Pos;
	TLabeledEdit *Edit_R2Pos;
	TButton *Button_SetPosition;
	TListBox *ListBox_Log;
	TButton *Button_Fill;
	TEdit *Edit_Fill;
	TCheckBox *CheckBox_Format;
	TTimer *Timer1;
	TListBox *ListBox_R1Commands;
	TListBox *ListBox_R2Commands;
	TLabel *Label1;
	TLabel *Label2;
	TListBox *ListBox_Operator;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TButton *Button_Start;
	TButton *Button_Stop;
	TLabel *Label_Activity;
	TButton *Button_Clear;
	TLabeledEdit *Edit_Interval;
	TGroupBox *GroupBox5;
	TLabel *Label_R1Pos;
	TLabel *Label_R1Dir;
	TLabel *Label_R1Command;
	TGroupBox *GroupBox6;
	TLabel *Label_R2Pos;
	TLabel *Label_R2Dir;
	TLabel *Label_R2Command;
	TLabel *Label_Attention;
	TLabel *Label6;
	TListBox *ListBox_Router;
	TLabel *Label11;
	TLabel *Label12;
	TListBox *ListBox_Simulation;
	void __fastcall Button_LoadClick(TObject *Sender);
	void __fastcall Button_SaveClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button_AddClick(TObject *Sender);
	void __fastcall Button_DeleteClick(TObject *Sender);
	void __fastcall Button_EditClick(TObject *Sender);
	void __fastcall Button_GetPositionClick(TObject *Sender);
	void __fastcall Button_SetPositionClick(TObject *Sender);
	void __fastcall Button_FillClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ListBox_OperatorClick(TObject *Sender);
	void __fastcall Button_StartClick(TObject *Sender);
	void __fastcall Button_StopClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button_ClearClick(TObject *Sender);
	void __fastcall Edit_SectorsExit(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};

class Robot
{
	private:
		int position,InitialPos;
		int sections;
		int SelectedIndex;
		int CurrentCommand;
		AnsiString name;
		AnsiString direction;
		TStringList *commands;
	public:
		Robot(int pos,AnsiString _name)
			{position = pos;name=_name;commands=new TStringList();CurrentCommand=0;direction="Stand";SelectedIndex=-1;InitialPos=pos;}
		~Robot(){delete commands;commands=NULL;}
		AnsiString GetName(){return name;}
		void SetSectors(int sectors){sections=sectors;}
		void SetPosition(int pos)
		{
			position = pos;
			if(position==0)position=sections;//Потому что 50%50=0, а нужно 50
		}
		int GetPosition(){return position;}
		void SetDirection(int command)
		{
			if(command==position)direction="Stand";
			else
			{
				if(position>command)
				{
					if(position-command >= sections-position+command)direction="CW";
					else direction="CCW";
				}
				if(position<command)
				{
					if(sections-command+position >= command-position)direction="CW";
					else direction="CCW";
				}
			}
		}
		void SetDirection(AnsiString dir){direction=dir;}
		AnsiString GetDirection(){return direction;}
		void DefineCurrentCommand() //Во время добавления/редактирования/удаления/заполнения/чистки и когда один робот толкает другой (один свободный) (свободный робот по принуждению подчиняется другому и сдвигается в противоположную сторону движения)
		{
			if(commands->Count==0)CurrentCommand=0;
			else CurrentCommand=commands->Strings[0].ToInt();
		}
		void SetCurrentCommand(int command){CurrentCommand=command;}
		int GetCurrentCommand(){return CurrentCommand;}
		void SetInitialPosition(){InitialPos=position;}
		int GetInititalPosition(){return InitialPos;}
		AnsiString GetInitialDirectionForCommand()       //position = CurrentCommand
		{
			if(position==InitialPos)return "Stand";
			else
			{
				if(InitialPos>position)
				{
					if(InitialPos-position >= sections-InitialPos+position)return "CW";
					else return "CCW";
				}
				if(InitialPos<position)
				{
					if(sections-position+InitialPos>= position-InitialPos)return "CW";
					else return "CCW";
				}
			}
			return "Stand";
		}
		void FillList(AnsiString command){commands->Add(command);}
		void DeleteCommand(){commands->Delete(0);}
		void ClearList(){commands->Clear();}
		int GetCommandsCount(){return commands->Count;}
		int GetIndexOf(AnsiString command){return commands->IndexOf(command);}
		TStringList* GetCommandsList(){return commands;}
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
