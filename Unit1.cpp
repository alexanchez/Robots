// ---------------------------------------------------------------------------

#include <vcl.h>
#include <time.h>
#include <algorithm>
#pragma hdrstop

#include "Unit1.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TForm1 *Form1;
Robot * R1 = new Robot(8,"1"), *R2 = new Robot(13,"2");
int sectors;
bool update = true;
TStringList *OperatorList_Temp = new TStringList();//Для симуляции
//10 -  оператор
// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
    srand(time(NULL));
}
// ---------------------------------------------------------------------------
void DefineDistanceBetweenRobots();

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(Key=='A' && Shift.Contains(ssCtrl))Button_AddClick(Form1);    //Добавить команду
	if(Key=='E' && Shift.Contains(ssCtrl))Button_EditClick(Form1);   //Редактировать команду
	if(Key=='D' && Shift.Contains(ssCtrl))Button_DeleteClick(Form1); //Удалить команду
	if(Key=='F' && Shift.Contains(ssCtrl))Button_FillClick(Form1);   //Заполнить(сгенерировать) команды
	if(Key=='C' && Shift.Contains(ssShift))Button_ClearClick(Form1); //Очистить команды
	if(Key=='L' && Shift.Contains(ssCtrl))Button_LoadClick(Form1);   //Загрузить команды в маршрутизатор
	if(Key=='S' && Shift.Contains(ssCtrl))Button_SaveClick(Form1);   //Сохранить журнал событий
	if(Key=='1' && Shift.Contains(ssCtrl))Button_StartClick(Form1);  //Запустить таймер
	if(Key=='2' && Shift.Contains(ssCtrl))Button_StopClick(Form1);   //Остановить таймер
	if(Key==char(27))Close();                                        //Выход
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
	ListBox_Operator->Items->LoadFromFile("Commands_Input.txt");
	sectors = Edit_Sectors->Text.ToInt();
	R1->SetPosition(Edit_R1Pos->Text.ToInt());R1->SetInitialPosition();
	R2->SetPosition(Edit_R2Pos->Text.ToInt());R2->SetInitialPosition();
	Label_R1Pos->Caption="Position: "+AnsiString(R1->GetPosition());
	Label_R2Pos->Caption="Position: "+AnsiString(R2->GetPosition());
	Label_R1Dir->Caption="Direction: "+R1->GetDirection();
	Label_R2Dir->Caption="Direction: "+R2->GetDirection();
	if(R1->GetCurrentCommand()==0)Label_R1Command->Caption="Command: NONE";
	else Label_R1Command->Caption="Command: "+AnsiString(R1->GetCurrentCommand());
	if(R2->GetCurrentCommand()==0)Label_R2Command->Caption="Command: NONE";
	else Label_R2Command->Caption="Command: "+AnsiString(R2->GetCurrentCommand());
	DefineDistanceBetweenRobots();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete R1,R2;R1=NULL;R2=NULL;
}
// ---------------------------------------------------------------------------

int GetDistanceBetweenRobots()
{
	int R1Pos = R1->GetPosition(), R2Pos = R2->GetPosition();
	if(R1Pos==R2Pos)return 0;
	if(R1Pos>R2Pos) return R1Pos-R2Pos <= sectors-R1Pos+R2Pos ? R1Pos-R2Pos : sectors-R1Pos+R2Pos;
	if(R1Pos<R2Pos) return sectors-R2Pos+R1Pos <= R2Pos-R1Pos ? sectors-R2Pos+R1Pos : R2Pos-R1Pos;
	return 0;
}
// ---------------------------------------------------------------------------

void DefineDistanceBetweenRobots() //Расстояние между двумя роботами---------
{
	int mindistance = GetDistanceBetweenRobots();
	if(mindistance>=8){Form1->Label_Attention->Caption="Safe ("+AnsiString(mindistance)+")";Form1->Label_Attention->Font->Color=clGreen;}
	if(mindistance<8){Form1->Label_Attention->Caption="Attention! ("+AnsiString(mindistance)+")";Form1->Label_Attention->Font->Color=(TColor)RGB(255,128,0);}
	if(mindistance<4){Form1->Label_Attention->Caption="Crash! ("+AnsiString(mindistance)+")";Form1->Label_Attention->Font->Color=clRed;Form1->Button_StopClick(Form1);ShowMessage("Crash!!!");}
}
// ---------------------------------------------------------------------------

void PerformCommands() //Робот 1 - главный  // +1 - движение;4 - минимальное расстояние между роботами
{
	AnsiString R1Result,R2Result;
	bool R1Block=false,R2Block=false; 					//Движение может происходить только 1 раз для каждого робота за 1 такт
	bool R1SaveCommand=false,R2SaveCommand=false;
	AnsiString R1InititalPosition=R1->GetInititalPosition();
	DefineDistanceBetweenRobots();
	// Робот 1----------------------------------------------------------------
	if(R1->GetCurrentCommand()>=1) 						// Если есть команда
	{
		if(R1->GetPosition()==R1->GetCurrentCommand())  // Если робот выполнил команду - удалить и обновить команду, и записать инфу
		{
			if(Form1->CheckBox_Format->Checked)R1Result.sprintf("%-0s%-4s%-0s%-4s%-2s%-4s%-5s%-0s","1 - ",AnsiString(R1->GetPosition()),"== 1|",AnsiString(R1->GetInititalPosition()),"-",AnsiString(R1->GetPosition()),R1->GetInitialDirectionForCommand(),"|");//| == 2|13|
			else R1Result="1 - "+AnsiString(R1->GetPosition())+" == 1|"+AnsiString(R1->GetInititalPosition())+" - "+AnsiString(R1->GetPosition())+" "+R1->GetInitialDirectionForCommand()+"|";
			Form1->ListBox_AllCommands->Items->Add("1 - "+AnsiString(R1->GetPosition()));
			Form1->ListBox_Router->Items->Delete(Form1->ListBox_Router->Items->IndexOf("1 - "+AnsiString(R1->GetPosition())));
			if(R1->GetCurrentCommand()!=10)Form1->ListBox_Operator->Items->Delete(Form1->ListBox_Operator->Items->IndexOf(AnsiString(R1->GetPosition())));
			R1->DeleteCommand();
			R1->DefineCurrentCommand();
			R1->SetInitialPosition();
			R1->SetDirection("Stand");
			R1SaveCommand=true;
			R1Block=true; //Зафиксировать направление "Stand" при выполнении команды на 1 такт (R2 не сможет сдвинуть не смотря на то, что R1 не двигался)
		}
		else 											//  Если робот выполняет команду
		{
			R1->SetDirection(R1->GetCurrentCommand());
			if(R1->GetPosition()<R2->GetPosition()) //--R1--R2--
			{
				if(R1->GetDirection()=="CW")
				{
					if(R1->GetPosition()+1+4<=R2->GetPosition())
					{
						R1->SetPosition(R1->GetPosition()+1);R1Block=true;   // 40 43 47
						if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
						  {R2->SetDirection("CW");R2->SetPosition((R2->GetPosition()+1)%sectors);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
					}
					else {R1->SetDirection("Stand");R2->SetDirection("CW");R2->SetPosition((R2->GetPosition()+1)%sectors);R2->DefineCurrentCommand();R2Block=true;} // R1 толкает R2(нужно проверить на актуальность текущую команду R2 на случай, если расстояние к другой команде стало ближе, чем к этой (39 текущая,44 другая => Позиции:41(2слева-3справа)->42(3слева-2справа)) )
				}                                                                                                                                                   // R1 стоит, пока R2 отодвигается
				if(R1->GetDirection()=="CCW")
				{
					if(R1->GetPosition()-1-4>=0)
					{
						R1->SetPosition(R1->GetPosition()-1);R1Block=true;  // 5 45 50 (N=50)
						if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
						  {R2->SetDirection("CCW");R2->SetPosition(R2->GetPosition()-1);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
					}
					else
					{
						if(R2->GetPosition()<= R1->GetPosition()-1-4+sectors)
						{
							R1->SetPosition(R1->GetPosition()-1);R1Block=true; // 4 45 49 (N=50)
                            if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
							  {R2->SetDirection("CCW");R2->SetPosition(R2->GetPosition()-1);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
						}
						else {R1->SetDirection("Stand");R2->SetDirection("CCW");R2->SetPosition(R2->GetPosition()-1);R2->DefineCurrentCommand();R2Block=true;} // 4 45 50 (N=50)
					}
				}
			}
			else
			if(R1->GetPosition()>R2->GetPosition())  //--R2--R1--
			{
				if(R1->GetDirection()=="CW")
				{
					if(R1->GetPosition()+1+4<=sectors)
					{
						R1->SetPosition(R1->GetPosition()+1); 	  // 44 46 4
						if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
						  {R2->SetDirection("CW");R2->SetPosition(R2->GetPosition()+1);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
					}
					else
					{
						if((R1->GetPosition()+1+4)%sectors<=R2->GetPosition())
						{
							R1->SetPosition((R1->GetPosition()+1)%sectors);R1Block=true;// 44 50 4
							if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
							  {R2->SetDirection("CW");R2->SetPosition(R2->GetPosition()+1);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
						}
						else {R1->SetDirection("Stand");R2->SetDirection("CW");R2->SetPosition(R2->GetPosition()+1);R2->DefineCurrentCommand();R2Block=true;}        // 50 8 4
					}
				}
				if(R1->GetDirection()=="CCW")
				{
					if(R1->GetPosition()-1-4>=R2->GetPosition())
					{
						R1->SetPosition(R1->GetPosition()-1);R1Block=true;// 50 46 45 (N=50)
						if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
						  {R2->SetDirection("CCW");R2->SetPosition(R2->GetPosition()-1);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
					}
					else {R1->SetDirection("Stand");R2->SetDirection("CCW");R2->SetPosition(R2->GetPosition()-1);R2->DefineCurrentCommand();R2Block=true;}    //5 45 1 (N=50) (будет 5 45 50)
				}
			}
		}
	}
	else R1->SetDirection("Stand");
	// Робот 2--------------------------------------------------------------------
	if(R2Block == false)                                //Если R2 не двигался
	{
	 if(R2->GetCurrentCommand()>=1) 					// Если есть команда
	 {
		if(R2->GetPosition()==R2->GetCurrentCommand())  // Если робот выполнил команду - удалить и обновить команду, и записать инфу
		{
			if(Form1->CheckBox_Format->Checked)R2Result.sprintf("%-0s%-4s%-0s%-4s%-2s%-4s%-5s%-0s","2 - ",AnsiString(R2->GetPosition()),"== 2|",AnsiString(R2->GetInititalPosition()),"-",AnsiString(R2->GetPosition()),R2->GetInitialDirectionForCommand(),"|");
			else R2Result="2 - "+AnsiString(R2->GetPosition())+" == 2|"+AnsiString(R2->GetInititalPosition())+" - "+AnsiString(R2->GetPosition())+" "+R2->GetInitialDirectionForCommand()+"|";
			Form1->ListBox_AllCommands->Items->Add("2 - "+AnsiString(R2->GetPosition()));
			Form1->ListBox_Router->Items->Delete(Form1->ListBox_Router->Items->IndexOf("2 - "+AnsiString(R2->GetPosition())));
            if(R2->GetCurrentCommand()!=10)Form1->ListBox_Operator->Items->Delete(Form1->ListBox_Operator->Items->IndexOf(AnsiString(R2->GetPosition())));
			R2->DeleteCommand();
			R2->DefineCurrentCommand();
			R2->SetInitialPosition();
			R2->SetDirection("Stand");
			R2SaveCommand=true;
		}
		else 											// Если робот выполняет команду
		{
			R2->SetDirection(R2->GetCurrentCommand());
			if(R2->GetPosition()<R1->GetPosition()) //--R2--R1--
			{
				if(R2->GetDirection()=="CW")
				{
					if(R2->GetPosition()+1+4<=R1->GetPosition())
					{
						R2->SetPosition(R2->GetPosition()+1);R2Block=true;   // 40 43 47
						if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4 &&
						   R1->GetCurrentCommand()==0 && R1Block==false)
						  {R1->SetDirection("CW");R1->SetPosition((R1->GetPosition()+1)%sectors);}        //R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду
					}
					else
					{
						R2->SetDirection("Stand");
						if(R1->GetCurrentCommand()==0 && R1Block==false)
						  {R1->SetDirection("CW");R1->SetPosition((R1->GetPosition()+1)%sectors);}        // R2 толкает R1(Если R1 свободный и не двигался)
					}
				}                                                                                         // R2 стоит, пока R1 отодвигается
				if(R2->GetDirection()=="CCW")
				{
					if(R2->GetPosition()-1-4>=0)
					{
						R2->SetPosition(R2->GetPosition()-1);R2Block=true;      // 5 45 50 (N=50)
						if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4  &&
						   R1->GetCurrentCommand()==0 && R1Block==false)
						  {R1->SetDirection("CCW");R1->SetPosition(R1->GetPosition()-1);}				  //R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду
					}
					else
					{
						if(R1->GetPosition()<= R2->GetPosition()-1-4+sectors)
						{
							R2->SetPosition(R2->GetPosition()-1);R2Block=true;  // 4 45 49 (N=50)
                            if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4  &&
							   R1->GetCurrentCommand()==0 && R1Block==false)
							  {R1->SetDirection("CCW");R1->SetPosition(R1->GetPosition()-1);}			  //R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду
						}
						else
						{
							R2->SetDirection("Stand");
							if(R1->GetCurrentCommand()==0 && R1Block==false)
							  {R1->SetDirection("CCW");R1->SetPosition(R1->GetPosition()-1);} // 4 45 50 (N=50)
						}
					}
				}
			}
			else
			if(R2->GetPosition()>R1->GetPosition())  //--R1--R2--
			{
				if(R2->GetDirection()=="CW")
				{
					if(R2->GetPosition()+1+4<=sectors)
					{
						R2->SetPosition(R2->GetPosition()+1);R2Block=true;  // 44 46 4
						if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4 &&
						   R1->GetCurrentCommand()==0 && R1Block==false)
						  {R1->SetDirection("CW");R1->SetPosition(R1->GetPosition()+1);}//R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду
					}
					else
					{
						if((R2->GetPosition()+1+4)%sectors<=R1->GetPosition())
						{
							R2->SetPosition((R2->GetPosition()+1)%sectors);R2Block=true; // 44 50 4
							if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4 &&
							   R1->GetCurrentCommand()==0 && R1Block==false)
							  {R1->SetDirection("CW");R1->SetPosition(R1->GetPosition()+1);}//R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду

						}
						else
						{
							R2->SetDirection("Stand");
							if(R1->GetCurrentCommand()==0 && R1Block==false)
							  {R1->SetDirection("CW");R1->SetPosition(R1->GetPosition()+1);}        // 50 8 4
						}
					}
				}
				if(R2->GetDirection()=="CCW")
				{
					if(R2->GetPosition()-1-4>=R1->GetPosition())
					{
						R2->SetPosition(R2->GetPosition()-1);R2Block=true; // 50 46 45 (N=50)
						if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4  &&
						   R1->GetCurrentCommand()==0 && R1Block==false)
						  {R1->SetDirection("CCW");R1->SetPosition(R1->GetPosition()-1);}//R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду
					}
					else
					{
						R2->SetDirection("Stand");
						if(R1->GetCurrentCommand()==0 && R1Block==false)
						  {R1->SetDirection("CCW");R1->SetPosition(R1->GetPosition()-1);}    //5 45 1 (N=50) (будет 5 45 50)
					}
				}
			}
		}
	 }
	 else R2->SetDirection("Stand");
	}
	// Сохранение выполненных команд в журнал (часть результата пишется во время выполнения роботом команды)
	AnsiString r1=R1Result,r2=R2Result;// !Для сохранения форматировки, используя sprintf!
	if(R1SaveCommand)
	{
		AnsiString R2RealDirection = R2->GetDirection();//Если R2 только достиг своей команды (14 - 14 CW), но он на данный момент стоит
		if(R2->GetPosition()==R2->GetCurrentCommand())R2RealDirection="Stand";
		if(Form1->CheckBox_Format->Checked)R1Result.sprintf("%-0s%-0s%-4s%-2s%-4s%-5s%-0s",R1Result," == 2|",AnsiString(R2->GetPosition()),"-",AnsiString(R2->GetCurrentCommand()),R2RealDirection,"|");
		else R1Result+=" == 2|"+AnsiString(R2->GetPosition())+" - "+AnsiString(R2->GetCurrentCommand())+" "+R2RealDirection+"|";
		Form1->ListBox_Log->Items->Add(R1Result);
	}
	if(R2SaveCommand)
	{
		AnsiString R1RealDirection = R1->GetDirection();//Если R1 только достиг своей команды (14 - 14 CW), но он на данный момент стоит
		if(R1->GetPosition()==R1->GetCurrentCommand())R1RealDirection="Stand";
		if(Form1->CheckBox_Format->Checked)R2Result.sprintf("%-0s%-0s%-4s%-2s%-4s%-5s%-0s",R2Result," == 1|",AnsiString(R1->GetPosition()),"-",AnsiString(R1->GetCurrentCommand()),R1RealDirection,"|");
		else R2Result+=" == 1|"+AnsiString(R1->GetPosition())+" - "+AnsiString(R1->GetCurrentCommand())+" "+R1RealDirection+"|";
		Form1->ListBox_Log->Items->Add(R2Result);
	}
	//------
	DefineDistanceBetweenRobots();
}
// ---------------------------------------------------------------------------
/*
Если R1 свободный, а R2 выполняет команду в сторону R1,то R1 подчиняется R2 (R1<-R2, т.е.R2 толкает R1)
Если R2 свободный, а R1 выполняет команду в сторону R2,то R2 подчиняется R1 (R1->R2, т.е.R1 толкает R2)
Если R1 выполняет команду, R2 выполняет команду, то R2 подчиняется R1 (R1->R2, т.е.R1 толкает R2) и R2 переопределяет команду (если это оптимально)
 */

void __fastcall TForm1::Button_StartClick(TObject *Sender)
{
	// Определение количества секторов-----------
	if (Edit_Sectors->Text!="" && Edit_Sectors->Text.ToInt()>=10)sectors = Edit_Sectors->Text.ToInt();
	else {sectors = 50;Edit_Sectors->Text = "50";}
	R1->SetSectors(sectors);R2->SetSectors(sectors);
	Edit_Sectors->Enabled=false;
	// Определение интервала---------------------
	if (Edit_Interval->Text!="" && Edit_Interval->Text.ToInt()>=1000)Timer1->Interval = Edit_Interval->Text.ToInt();
	else {Timer1->Interval = 1000;Edit_Interval->Text="1000";}
	Edit_Interval->Enabled=false;
	// Удаление команд, выходящих за предел количества секторов (в случае наличии таковых)
	for(int i=0;i<ListBox_Operator->Items->Count;i++)
		if(ListBox_Operator->Items->Strings[i].ToInt()>sectors)
		   ListBox_Operator->Items->Delete(i);
	//-------------------------------------------
	update=true;// Изменения после чистки команд, которые выходили за предел количества секторов
	Timer1->Enabled=true;
	Label_Activity->Caption="Active";
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button_StopClick(TObject *Sender)
{
	Edit_Sectors->Enabled=true;
	Edit_Interval->Enabled=true;
	Timer1->Enabled=false;
	Label_Activity->Caption="Inactive";
}
// ---------------------------------------------------------------------------

int GetDistance(int command,int position)//BetweenRobotAndCommand
{
	if(position==command)return 0;//расстояние = слева <= справа ? вернуть слева : вернуть справа;
	if(position>command)return position-command <= sectors-position+command ? position-command : sectors-position+command;
	if(position<command)return sectors-command+position <= command-position ? sectors-command+position : command-position;
	return 0;
}
// ---------------------------------------------------------------------------

int GetOptimalCommand(int position)
{
	int command,minvalue,index,MinDistance=100000;
	for(int i=0;i<OperatorList_Temp->Count;i++)
	{
		command=OperatorList_Temp->Strings[i].ToInt();
		minvalue=GetDistance(command,position)+GetDistance(10,command);//Расстояние робот-команда + расстояние команда-оператор
		if(MinDistance>minvalue){MinDistance=minvalue;index=i;}  //Если pos=8, команды 6(1) 10(2), активной станет 6(1)
	}
	return OperatorList_Temp->Strings[index].ToInt();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	bool info=true;// False - Показать текущие команды роботов после любого изменения; True - Показать информацию после выполнения движений роботов
	// Обработка---------------------------------
	if(update) // Обновить список команд каждого робота (во время добавления/редактирования/удаления/заполнения/очистки)
	{
		ListBox_AllCommands->Items->Add("----------");
		ListBox_Log->Items->Add("-------------------------------------------------------");
		// Оператор---------
		ListBox_Router->Clear();
		int R1CurrentPos = R1->GetPosition(),R2CurrentPos = R2->GetPosition();
		AnsiString R1CurrentDir = R1->GetDirection(),R2CurrentDir = R2->GetDirection();
		int R1InterPos = R1->GetPosition(),R2InterPos = R2->GetPosition(),R1InterCommand=0,R2InterCommand=0;
		bool R1GotCommand = false,R2GotCommand = false;
		TStringList *OperatorList = new TStringList();
		OperatorList->Assign(ListBox_Operator->Items);
		//ListBox_AllCommands->Items->Clear();//----------------------------------!!!!!!!!!!!!!!!!!!!!XXXX
		ListBox_Simulation->Clear();
		for(int i=0;i<OperatorList->Count;i++)
		{
			OperatorList_Temp->Assign(OperatorList);
			R1GotCommand=false;R2GotCommand=false;
			R1InterPos=R1->GetPosition();R2InterPos=R2->GetPosition();
			if(R1InterCommand==0)R1InterCommand=GetOptimalCommand(R1InterPos); else R1GotCommand=true;
			if(R2InterCommand==0)R2InterCommand=GetOptimalCommand(R2InterPos); else R2GotCommand=true;
			if(R1InterCommand==R2InterCommand)
			{     //Расстояние робот-команда + расстояние команда-оператор
				if(OperatorList->Count==1)
				{
					if(GetDistance(R1InterCommand,R1InterPos)+GetDistance(10,R1InterCommand)<=
					   GetDistance(R2InterCommand,R2InterPos)+GetDistance(10,R2InterCommand))
					{
						R2InterCommand=0;
						if(!R1GotCommand){/*ListBox_Router->Items->Add("1 - "+AnsiString(R1InterCommand));*/R1GotCommand=true;}
					}
					else
					{
						R1InterCommand=0;
						if(!R2GotCommand){/*ListBox_Router->Items->Add("2 - "+AnsiString(R2InterCommand));*/R2GotCommand=true;}
					}
				}
				else
				{
					if(GetDistance(R1InterCommand,R1InterPos)+GetDistance(10,R1InterCommand)<=
					   GetDistance(R2InterCommand,R2InterPos)+GetDistance(10,R2InterCommand))
					   {
						 int R1Real1Command = R1InterCommand;
						 OperatorList_Temp->Delete(OperatorList_Temp->IndexOf(R1InterCommand));//ShowMessage(OperatorList_Temp->Text);
						 R1InterCommand=GetOptimalCommand(R1InterPos);//Следующая(вторая) оптимальная команда для R1
						 if(GetDistance(R2InterCommand,R1InterPos)+GetDistance(10,R2InterCommand)+  //Время выполнения R1 первых двух оптимальных команд <= Время выполнения R2 первой оптимильной команды
							GetDistance(R1InterCommand,10)+GetDistance(10,R1InterCommand)<=         //R2InterCommand - в роли первой оптимальной команды для R1; R1InterCommand - в роли второй оптимальной команды для R1
							GetDistance(R2InterCommand,R2InterPos)+GetDistance(10,R2InterCommand))
						 {                                                                          //R1 берет сразу две команды на себя (первую и вторую оптимальную)
							if(OperatorList_Temp->Count!=0)                                         //Если есть >=3 команд (R2 может взять третью по счету оптимальную команду)
							{
							  OperatorList_Temp->Delete(OperatorList_Temp->IndexOf(R1InterCommand));
							  R2InterCommand=GetOptimalCommand(R2InterPos);//Следующая(вторая) оптимальная команда для R2
							  if(!R2GotCommand){/*ListBox_Router->Items->Add("2 - "+AnsiString(R2InterCommand));*/R2GotCommand=true;}
							}
							if(!R1GotCommand){/*ListBox_Router->Items->Add("1 - "+AnsiString(R1Real1Command));*/R1InterCommand=R1Real1Command;R1GotCommand=true;}
						 }
						 else
						 {
							if(!R1GotCommand){/*ListBox_Router->Items->Add("1 - "+AnsiString(R1InterCommand));*/R1GotCommand=true;}
							if(!R2GotCommand){/*ListBox_Router->Items->Add("2 - "+AnsiString(R2InterCommand));*/R2GotCommand=true;}
						 }
					   }
					else
					{
						 int R2Real1Command = R2InterCommand;
						 OperatorList_Temp->Delete(OperatorList_Temp->IndexOf(R2InterCommand));//ShowMessage(OperatorList_Temp->Text);
						 R2InterCommand=GetOptimalCommand(R2InterPos);//Следующая(вторая) оптимальная команда для R2
						 if(GetDistance(R1InterCommand,R2InterPos)+GetDistance(10,R1InterCommand)+  //Время выполнения R2 первых двух оптимальных команд <= Время выполнения R1 первой оптимильной команды
							GetDistance(R2InterCommand,10)+GetDistance(10,R2InterCommand)<=         //R1InterCommand - в роли первой оптимальной команды для R2
							GetDistance(R1InterCommand,R1InterPos)+GetDistance(10,R1InterCommand))
						 {                                                                          //R2 берет сразу две команды на себя (первую и вторую оптимальную)
							if(OperatorList_Temp->Count!=0)                                         //Если есть >=3 команд (R1 может взять третью по счету оптимальную команду)
							{
							  OperatorList_Temp->Delete(OperatorList_Temp->IndexOf(R2InterCommand));
							  R1InterCommand=GetOptimalCommand(R1InterPos);//Следующая(вторая) оптимальная команда для R1
							  if(!R1GotCommand){/*ListBox_Router->Items->Add("1 - "+AnsiString(R1InterCommand));*/R1GotCommand=true;}
							}
							if(!R2GotCommand){/*ListBox_Router->Items->Add("2 - "+AnsiString(R2Real1Command));*/R2InterCommand=R2Real1Command;R2GotCommand=true;}
						 }
						 else
						 {
							if(!R1GotCommand){/*ListBox_Router->Items->Add("1 - "+AnsiString(R1InterCommand));*/R1GotCommand=true;}
							if(!R2GotCommand){/*ListBox_Router->Items->Add("2 - "+AnsiString(R2InterCommand));*/R2GotCommand=true;}
						 }
                    }
				}
			}
			else
			{
			   if(!R1GotCommand){/*ListBox_Router->Items->Add("1 - "+AnsiString(R1InterCommand));*/R1GotCommand=true;}
			   if(!R2GotCommand){/*ListBox_Router->Items->Add("2 - "+AnsiString(R2InterCommand));*/R2GotCommand=true;}
			}
			if(R1GotCommand && R1->GetIndexOf("10")==-1)/*ListBox_Router->Items->Add("1 - 10")*/; //Если робот только получил задание
			if(R2GotCommand && R1->GetIndexOf("10")==-1)/*ListBox_Router->Items->Add("2 - 10")*/; //Если робот только получил задание

			//Симуляция движения------------------------------------------------
			bool R1Block=false,R2Block=false; 					//Движение может происходить только 1 раз для каждого робота за 1 такт
			bool R1Free=false,R2Free=false;
			if(R1GotCommand && R1InterCommand!=10){R1->ClearList();R1->FillList(AnsiString(R1InterCommand));R1->FillList("10");R1->DefineCurrentCommand();}
			if(R2GotCommand && R2InterCommand!=10){R2->ClearList();R2->FillList(AnsiString(R2InterCommand));R2->FillList("10");R2->DefineCurrentCommand();}
			if(R1GotCommand || R2GotCommand) //Если получена хоть какая-то команда
			for(;;)
			 {
				R1Block=false;R2Block=false;
				// Робот 1------------------------------------------------------
				if(R1->GetCurrentCommand()>=1) 						// Если есть команда
				{
					if(R1->GetPosition()==R1->GetCurrentCommand())  // Если робот выполнил команду - удалить и обновить команду, и записать инфу
					{
						ListBox_Simulation->Items->Add("1 - "+AnsiString(R1->GetPosition())+" == 1|"+AnsiString(R1->GetInititalPosition())+" - "+AnsiString(R1->GetPosition())+" "+R1->GetInitialDirectionForCommand()+"|");
						ListBox_Router->Items->Add("1 - "+AnsiString(R1->GetPosition()));
						if(R1->GetCurrentCommand()!=10){OperatorList->Delete(OperatorList->IndexOf(AnsiString(R1->GetCurrentCommand())));R1InterCommand=10;}//Если робот достиг команды и уже направляется к оператору - удалить команду оператора
						R1->DeleteCommand();
						R1->DefineCurrentCommand();
						R1->SetInitialPosition();
						R1->SetDirection("Stand");
						if(R1->GetCurrentCommand()==0){R1Free=true;R1InterCommand=0;}  //Для остановки симуляции по достижению робота его команды
						R1Block=true; //Зафиксировать направление "Stand" при выполнении команды на 1 такт (R2 не сможет сдвинуть не смотря на то, что R1 не двигался)
					}
					else 											//  Если робот выполняет команду
					{
						R1->SetDirection(R1->GetCurrentCommand());
						if(R1->GetPosition()<R2->GetPosition()) //--R1--R2--
						{
							if(R1->GetDirection()=="CW")
							{ 															// 30 R1(40) R2(41) 42 R2(46)
								if(R1->GetPosition()+1+4<=R2->GetPosition())
								{
									R1->SetPosition(R1->GetPosition()+1);R1Block=true;   // 40 43 47
									if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
									  {R2->SetDirection("CW");R2->SetPosition((R2->GetPosition()+1)%sectors);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
								}
								else {R1->SetDirection("Stand");R2->SetDirection("CW");R2->SetPosition((R2->GetPosition()+1)%sectors);R2->DefineCurrentCommand();R2Block=true;} // R1 толкает R2(нужно проверить на актуальность текущую команду R2 на случай, если расстояние к другой команде стало ближе, чем к этой (39 текущая,44 другая => Позиции:41(2слева-3справа)->42(3слева-2справа)) )
							}                                                                                                                                                   // R1 стоит, пока R2 отодвигается
							if(R1->GetDirection()=="CCW")
							{
								if(R1->GetPosition()-1-4>=0)
								{
									R1->SetPosition(R1->GetPosition()-1);R1Block=true;  // 5 45 50 (N=50)
									if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
									  {R2->SetDirection("CCW");R2->SetPosition(R2->GetPosition()-1);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
								}
								else
								{
									if(R2->GetPosition()<= R1->GetPosition()-1-4+sectors)
									{
										R1->SetPosition(R1->GetPosition()-1);R1Block=true; // 4 45 49 (N=50)
										if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
										  {R2->SetDirection("CCW");R2->SetPosition(R2->GetPosition()-1);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
									}
									else {R1->SetDirection("Stand");R2->SetDirection("CCW");R2->SetPosition(R2->GetPosition()-1);R2->DefineCurrentCommand();R2Block=true;} // 4 45 50 (N=50)
								}
							}
						}
						else
						if(R1->GetPosition()>R2->GetPosition())  //--R2--R1--
						{
							if(R1->GetDirection()=="CW")
							{
								if(R1->GetPosition()+1+4<=sectors)
								{
									R1->SetPosition(R1->GetPosition()+1); 	  // 44 46 4
									if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
									  {R2->SetDirection("CW");R2->SetPosition(R2->GetPosition()+1);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
								}
								else
								{
									if((R1->GetPosition()+1+4)%sectors<=R2->GetPosition())
									{
										R1->SetPosition((R1->GetPosition()+1)%sectors);R1Block=true;// 44 50 4
										if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
										  {R2->SetDirection("CW");R2->SetPosition(R2->GetPosition()+1);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
									}
									else {R1->SetDirection("Stand");R2->SetDirection("CW");R2->SetPosition(R2->GetPosition()+1);R2->DefineCurrentCommand();R2Block=true;}        // 50 8 4
								}
							}
							if(R1->GetDirection()=="CCW")
							{
								if(R1->GetPosition()-1-4>=R2->GetPosition())
								{
									R1->SetPosition(R1->GetPosition()-1);R1Block=true;// 50 46 45 (N=50)
									if(R1->GetPosition()!=R1->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4)
									  {R2->SetDirection("CCW");R2->SetPosition(R2->GetPosition()-1);R2->DefineCurrentCommand();R2Block=true;}//R1 будет толкать R2 в будущем, потому что он ещё не выполнил команду
								}
								else {R1->SetDirection("Stand");R2->SetDirection("CCW");R2->SetPosition(R2->GetPosition()-1);R2->DefineCurrentCommand();R2Block=true;}    //5 45 1 (N=50) (будет 5 45 50)
							}
						}
					}
				}
				else R1->SetDirection("Stand");
				// Робот 2--------------------------------------------------------------------
				if(R2Block == false)                                //Если R2 не двигался
				{
				 if(R2->GetCurrentCommand()>=1) 					// Если есть команда
				 {
					if(R2->GetPosition()==R2->GetCurrentCommand())  // Если робот выполнил команду - удалить и обновить команду, и записать инфу
					{
						ListBox_Simulation->Items->Add("2 - "+AnsiString(R2->GetPosition())+" == 2|"+AnsiString(R2->GetInititalPosition())+" - "+AnsiString(R2->GetPosition())+" "+R2->GetInitialDirectionForCommand()+"|");
                        ListBox_Router->Items->Add("2 - "+AnsiString(R2->GetPosition()));
						if(R2->GetCurrentCommand()!=10){OperatorList->Delete(OperatorList->IndexOf(AnsiString(R2->GetCurrentCommand())));R2InterCommand=10;}//Если робот достиг команды и уже направляется к оператору - удалить команду оператора
						R2->DeleteCommand();
						R2->DefineCurrentCommand();
						R2->SetInitialPosition();
						R2->SetDirection("Stand");
						if(R2->GetCurrentCommand()==0){R2Free=true;R2InterCommand=0;}  //Для остановки симуляции по достижению робота его команды
					}
					else 											// Если робот выполняет команду
					{
						R2->SetDirection(R2->GetCurrentCommand());
						if(R2->GetPosition()<R1->GetPosition()) //--R2--R1--
						{
							if(R2->GetDirection()=="CW")
							{ 															// 30 R1(40) R2(41) 42 R2(46)
								if(R2->GetPosition()+1+4<=R1->GetPosition())
								{
									R2->SetPosition(R2->GetPosition()+1);R2Block=true;   // 40 43 47
									if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4 &&
									   R1->GetCurrentCommand()==0 && R1Block==false)
									  {R1->SetDirection("CW");R1->SetPosition((R1->GetPosition()+1)%sectors);}//R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду
								}
								else
								{
									R2->SetDirection("Stand");
									if(R1->GetCurrentCommand()==0 && R1Block==false)
									  {R1->SetDirection("CW");R1->SetPosition((R1->GetPosition()+1)%sectors);}        // R2 толкает R1(Если R1 свободный и не двигался)
								}
							}                                                                                     // R2 стоит, пока R1 отодвигается
							if(R2->GetDirection()=="CCW")
							{
								if(R2->GetPosition()-1-4>=0)
								{
									R2->SetPosition(R2->GetPosition()-1);R2Block=true;// 5 45 50 (N=50)
									if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4  &&
									   R1->GetCurrentCommand()==0 && R1Block==false)
									  {R1->SetDirection("CCW");R1->SetPosition(R1->GetPosition()-1);}//R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду
								}
								else
								{
									if(R1->GetPosition()<= R2->GetPosition()-1-4+sectors)
									{
										R2->SetPosition(R2->GetPosition()-1);R2Block=true; // 4 45 49 (N=50)
										if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4  &&
										   R1->GetCurrentCommand()==0 && R1Block==false)
										  {R1->SetDirection("CCW");R1->SetPosition(R1->GetPosition()-1);}//R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду
									}
									else
									{
										R2->SetDirection("Stand");
										if(R1->GetCurrentCommand()==0 && R1Block==false)
										  {R1->SetDirection("CCW");R1->SetPosition(R1->GetPosition()-1);} // 4 45 50 (N=50)
									}
								}
							}
						}
						else
						if(R2->GetPosition()>R1->GetPosition())  //--R1--R2--
						{
							if(R2->GetDirection()=="CW")
							{
								if(R2->GetPosition()+1+4<=sectors)
								{
									R2->SetPosition(R2->GetPosition()+1);R2Block=true;  // 44 46 4
									if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4 &&
									   R1->GetCurrentCommand()==0 && R1Block==false)
									  {R1->SetDirection("CW");R1->SetPosition(R1->GetPosition()+1);}//R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду
								}
								else
								{
									if((R2->GetPosition()+1+4)%sectors<=R1->GetPosition())
									{
										R2->SetPosition((R2->GetPosition()+1)%sectors);R2Block=true; // 44 50 4
										if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4 &&
										   R1->GetCurrentCommand()==0 && R1Block==false)
										  {R1->SetDirection("CW");R1->SetPosition(R1->GetPosition()+1);}//R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду

									}
									else
									{
										R2->SetDirection("Stand");
										if(R1->GetCurrentCommand()==0 && R1Block==false)
										  {R1->SetDirection("CW");R1->SetPosition(R1->GetPosition()+1);}        // 50 8 4
									}
								}
							}
							if(R2->GetDirection()=="CCW")
							{
								if(R2->GetPosition()-1-4>=R1->GetPosition())
								{
									R2->SetPosition(R2->GetPosition()-1);R2Block=true; // 50 46 45 (N=50)
									if(R2->GetPosition()!=R2->GetCurrentCommand() && GetDistanceBetweenRobots()<1+4  &&
									   R1->GetCurrentCommand()==0 && R1Block==false)
									  {R1->SetDirection("CCW");R1->SetPosition(R1->GetPosition()-1);}//R2 будет толкать R1 в будущем, потому что он ещё не выполнил команду
								}
								else
								{
									R2->SetDirection("Stand");
									if(R1->GetCurrentCommand()==0 && R1Block==false)
									  {R1->SetDirection("CCW");R1->SetPosition(R1->GetPosition()-1);}    //5 45 1 (N=50) (будет 5 45 50)
								}
							}
						}
					}
				 }
				 else R2->SetDirection("Stand");
				}
				if(R1Free||R2Free)//{i=-1;break;} //Остановка симуляции и пересмотр команд оператора
				{
					if(OperatorList->Count==0)
					 {	if(R1InterCommand==0&&R2InterCommand==0){i=-1;break;}}
					else if(OperatorList->Count>=1){i=-1;break;}
				}
			 }
			//--------------------------------
			Label11->Caption=R1->GetCurrentCommand();
			Label12->Caption=R2->GetCurrentCommand();
			//break;
		}
		R1->SetDirection(R1CurrentDir);R1->SetPosition(R1CurrentPos);R1->SetInitialPosition();
		R2->SetDirection(R2CurrentDir);R2->SetPosition(R2CurrentPos);R2->SetInitialPosition();
		R1->ClearList();R2->ClearList();
		R1->SetCurrentCommand(0);R2->SetCurrentCommand(0);
		delete OperatorList;OperatorList=NULL;

		// Маршрутизатор----
		R1->ClearList();R2->ClearList();
		//Если робот везет к оператору товар(осталась последняя команда - 10)
		//if(ListBox_R1Commands->Items->Count==1 && ListBox_R1Commands->Items->Strings[0]=="10")R1->FillList(ListBox_R1Commands->Items->Strings[0]);
		//if(ListBox_R2Commands->Items->Count==1 && ListBox_R2Commands->Items->Strings[0]=="10")R2->FillList(ListBox_R2Commands->Items->Strings[0]);
		for(int i=0;i<ListBox_Router->Items->Count;i++)
			if(ListBox_Router->Items->Strings[i].c_str()[0]=='1')R1->FillList(ListBox_Router->Items->Strings[i].SubString(5,ListBox_Router->Items->Strings[i].Length()));
			else R2->FillList(ListBox_Router->Items->Strings[i].SubString(5,ListBox_Router->Items->Strings[i].Length()));
		R1->DefineCurrentCommand();R2->DefineCurrentCommand();

		// Информация-------
		if(R1->GetCurrentCommand()==0)Label_R1Command->Caption="Command: NONE";
		else Label_R1Command->Caption="Command: "+AnsiString(R1->GetCurrentCommand());
		if(R2->GetCurrentCommand()==0)Label_R2Command->Caption="Command: NONE";
		else Label_R2Command->Caption="Command: "+AnsiString(R2->GetCurrentCommand());
		info=false;
		//------------------
		update=false;
	}
	PerformCommands();
	// Вывод на экран----------------------------
	ListBox_R1Commands->Items->Assign(R1->GetCommandsList());
	ListBox_R2Commands->Items->Assign(R2->GetCommandsList());
	Label_R1Pos->Caption="Position: "+AnsiString(R1->GetPosition());
	Label_R2Pos->Caption="Position: "+AnsiString(R2->GetPosition());
	Label_R1Dir->Caption="Direction: "+R1->GetDirection();
	Label_R2Dir->Caption="Direction: "+R2->GetDirection();
	if(info)
	{
		if(R1->GetCurrentCommand()==0)Label_R1Command->Caption="Command: NONE";
		else Label_R1Command->Caption="Command: "+AnsiString(R1->GetCurrentCommand());
		if(R2->GetCurrentCommand()==0)Label_R2Command->Caption="Command: NONE";
		else Label_R2Command->Caption="Command: "+AnsiString(R2->GetCurrentCommand());
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button_LoadClick(TObject *Sender)
{
	ListBox_Operator->Items->LoadFromFile("Commands_Input.txt");
}
// ---------------------------------------------------------------------------
void __fastcall TForm1::Button_SaveClick(TObject *Sender)
{
	ListBox_Log->Items->SaveToFile("Commands_Output.txt");
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ListBox_OperatorClick(TObject *Sender)
{
	if(ListBox_Operator->ItemIndex>=0)
		Edit_Task->Text = ListBox_Operator->Items->Strings[ListBox_Operator->ItemIndex];
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button_AddClick(TObject *Sender)
{
	if(Edit_Task->Text!="" && Edit_Task->Text.ToInt()>0 && Edit_Task->Text.ToInt()<=sectors && Edit_Task->Text.ToInt()!=10)
	{
		ListBox_Operator->Items->Add(Edit_Task->Text);
		update=true;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button_EditClick(TObject *Sender)
{
	if(Edit_Task->Text!="" && Edit_Task->Text.ToInt()>0 && Edit_Task->Text.ToInt()<=sectors && Edit_Task->Text.ToInt()!=10
	   && ListBox_Operator->ItemIndex>=0)
	{
		ListBox_Operator->Items->Strings[ListBox_Operator->ItemIndex]=Edit_Task->Text;
		update=true;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button_DeleteClick(TObject *Sender)
{
	if(ListBox_Operator->ItemIndex>=0)
	{
		ListBox_Operator->Items->Delete(ListBox_Operator->ItemIndex);
		update=true;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button_FillClick(TObject *Sender)
{
	if(Edit_Fill->Text=="")return;
	sectors = Edit_Sectors->Text.ToInt();
	int n = Edit_Fill->Text.ToInt(),number;
	ListBox_Operator->Clear();
	for(int i=0;i<n;i++)
	{
	   for(;;){number = rand()%sectors+1;if(number!=10)break;}//10 - Оператор
	   ListBox_Operator->Items->Add(AnsiString(number));// robot - command (ex 2 - 15)
	}
	update = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button_ClearClick(TObject *Sender)
{
	ListBox_Operator->Clear();
	update=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button_GetPositionClick(TObject *Sender)
{
	Label_R1Pos->Caption="Position: "+AnsiString(R1->GetPosition());
	Label_R2Pos->Caption="Position: "+AnsiString(R2->GetPosition());
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button_SetPositionClick(TObject *Sender)
{
	R1->SetPosition(Edit_R1Pos->Text.ToInt());
	R2->SetPosition(Edit_R2Pos->Text.ToInt());
	Label_R1Pos->Caption="Position: "+AnsiString(R1->GetPosition());
	Label_R2Pos->Caption="Position: "+AnsiString(R2->GetPosition());
	R1->SetInitialPosition();R2->SetInitialPosition();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Edit_SectorsExit(TObject *Sender)
{
   if (Edit_Sectors->Text!="" && Edit_Sectors->Text.ToInt()>=10)sectors = Edit_Sectors->Text.ToInt();
   else {sectors = 50;Edit_Sectors->Text = "50";}
   R1->SetSectors(sectors);R2->SetSectors(sectors);
}
//---------------------------------------------------------------------------


