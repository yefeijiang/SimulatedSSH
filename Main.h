//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
#define ReadBuffer 10240 //设置ReadBuffer的大小
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TStatusBar *StatusBar1;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TToolButton *ToolButton5;
	TToolButton *ToolButton6;
	TToolButton *ToolButton7;
	TToolButton *ToolButton8;
	TToolButton *ToolButton9;
	TPanel *Panel1;
	TSplitter *Splitter1;
	TPanel *Panel2;
	TTreeView *TreeView1;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TImageList *ImageList_48x48;
	TImageList *ImageList_32x32;
	TImageList *ImageList_16x16;
	TMemo *Memo1;
	TButton *Button1;
	TMemo *Memo2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	AnsiString RunDos(const AnsiString &DosApp);
	AnsiString cmdfile,tempbat,tempresultfile,cmd;
	void getenv();
	AnsiString getcmdbat(AnsiString destip,AnsiString appuser,AnsiString appuserpwd,AnsiString rootpasswd);
	void deletetempfile();
	TStringList *pDeviceList;
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
