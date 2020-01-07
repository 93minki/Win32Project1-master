// Win32Project1.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
// 5차 수정 (20.01.07)

#include "stdafx.h"
#include "Win32Project1.h"
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#define MAX_LOADSTRING 100

#define MAX_STACK_SIZE 30

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

char InputNum[MAX_STACK_SIZE];
int np;

char GetNum[MAX_STACK_SIZE];
char totalSign[10];
char arraytemp[MAX_STACK_SIZE];

char TempArray[MAX_STACK_SIZE];
char arrayclear[MAX_STACK_SIZE] = { '\0', };


int TempNum[MAX_STACK_SIZE];
char TempSign[MAX_STACK_SIZE];


char stack[MAX_STACK_SIZE];									// Stack
int top = -1;										// Init Stack Point

int Search_n = 0;									// Search Number 
int Search_s = 0;									// Search Sign

float StackNum[MAX_STACK_SIZE];									// Get Number in Stack
int nsp = -1;
char StackSymbol[MAX_STACK_SIZE];								// Get Sign in Stack
int ssp = -1;

int k = 0;
int arraySize;
int rstsum;

int priority;

int testString[MAX_STACK_SIZE];
// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
//ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
	//AttachConsole(GetCurrentProcessId());
	//freopen("CON", "w", stdout);
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	return 0;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

bool IsNumStackEmpty() {
	if (nsp < 0) {
		return true;
	}
	else {
		return false;
	}
}
bool IsSymStackEmpty() {
	if(ssp < 0) {
		return true;
	}
	else {
		return false;
	}
}

bool IsNumStackFull() {
	if (nsp > MAX_STACK_SIZE) {
		return true;
	}
	else {
		return false;
	}
}

bool IsSymStackFull() {
	if (ssp > MAX_STACK_SIZE) {
		return true;
	}
	else {
		return false;
	}
}

void NumStackPush(float value) {
	if (IsNumStackFull() == true) {
		printf("Num Stack is Full!\n");
	}
	else {
		StackNum[++nsp] = value;
	}
}

void SymStackPush(char value) {
	if (IsSymStackFull() == true) {
		printf("Sym Stack is Full!\n");
	}
	else {
		StackSymbol[++ssp] = value;
	}
}

char NumStackPop() {
	int rtnum;
	if (IsNumStackEmpty() == true) {
		printf("Num Stack is Empty!\n");
	}
	else {
		rtnum = StackNum[nsp];
		StackNum[nsp] = '\0';
		nsp--;
		return rtnum;
	}
}

char SymStackPop() {
	char rtchar;
	if (IsSymStackEmpty() == true) {
		printf("Sym Stack is Empty!\n");
	}
	else {
		rtchar = StackSymbol[ssp];
		StackSymbol[ssp] = '\0';
		ssp--;
		return rtchar;
	}
}


void showNum(char num, HWND hDlg) {
	InputNum[np] = num;
	np++;
	SetDlgItemText(hDlg, IDC_EDIT1, InputNum);

}

void showDot(char dot, HWND hDlg) {
	if (InputNum[np] == '.') {
		return;
	}
	else {
		InputNum[np] = '.';
		np++;
	}
	SetDlgItemText(hDlg, IDC_EDIT1, InputNum);
}

void showSign(char sign, HWND hDlg) {
	if (InputNum[np-1] == '+' || InputNum[np-1] == '-' || InputNum[np-1] == '*' || InputNum[np-1] == '/') {
		InputNum[np-1] = sign;
	}
	else {
		InputNum[np] = sign;
		np++;
	}
	
	SetDlgItemText(hDlg, IDC_EDIT1, InputNum);
}
// Get Stack Size 
int GetStackSize() {
	int StackSize;
	for (int a = 0; a < sizeof(InputNum); a++) {
		if (InputNum[a] == '\0') {
			StackSize = a;
			return StackSize;
		}
	}
}

void InitTempArray() {
	// Initialize Temp Array
	for (int i = 0; i < sizeof(TempArray); i++) {
		TempArray[i] = '\0';
		if (TempArray[i + 1] == '\0') {
			return;
		}
	}
	
}

void CheckFormula(HWND hDlg,int num) {
	// Show Error MessageBox if last stack element is Arithmetic Symbol
	if (InputNum[num + 1] == '\0') {
		MessageBox(hDlg, "Wrong formula!!!", "Formula Error", MB_OK);
	}
}

//int CheckPriority(char symbol) {
//
//}

void Calculation() {
	float pop1;
	float pop2;
	char opr;
	float rst;

	pop1 = NumStackPop();
	pop2 = NumStackPop();
	opr = SymStackPop();

	switch (opr) {														// 꺼낸 연산자에 따라 연산을 수행하고 그 값을 rst에 저장.
	case '+':
		rst = pop2 + pop1;
		NumStackPush(rst);												// 그 값을 스택에 다시 쌓는다.
		break;
	case '-':
		rst = pop2 - pop1;
		NumStackPush(rst);
		break;
	case '*':
		rst = pop2 * pop1;
		NumStackPush(rst);
		break;
	case '/':
		rst = pop2 / pop1;
		NumStackPush(rst);
		break;
	}
}
int makePriority(char symbol) {
	switch (symbol) {
	case '+':
		priority = 2;
		break;
	case '-':
		priority = 2;
		break;
	case '*':
		priority = 1;
		break;
	case '/':
		priority = 1;
		break;
	}
	return priority;
}

void GetExp(HWND hDlg) {

	printf("Get Number : %s\n", InputNum);
	printf("Size of Stack : %d\n", GetStackSize());
	int StackSize = GetStackSize();
	int oldpr = 4;
	bool hpr = false;
	int pr;
	int pop1;
	int pop2;
	bool calcFlag = false;
	int rst = 0;
	char opr;

	for (int i = 0; i < StackSize; i++) {
		// if Stack Value is Sign 
		if (InputNum[i] == '+' || InputNum[i] == '-' || InputNum[i] == '*' || InputNum[i] == '/') {
			pr = makePriority(InputNum[i]);											// 현재 연산자의 우선순위를 구한다.
			Search_s = i;
			for (int tp = 0; Search_n < Search_s; Search_n++, tp++) {
				TempArray[tp] = InputNum[Search_n];
			}
			Search_n = Search_s + 1;
			NumStackPush(atof(TempArray));											// 피연산자를 스택에 쌓는다.
			InitTempArray();
			if (calcFlag) {
				Calculation();
				SymStackPush(InputNum[i]);
				calcFlag = false;
			}
			else if (oldpr == 4) {														// 첫 번째 연산자.
				SymStackPush(InputNum[i]);											// 연산자를 스택에 쌓는다.
				oldpr = pr;
			}
			else if (oldpr < pr) {													// 새로운 연산자 우선순위가 기존 우선순위 보다 높을 경우 */ -> +-
				Calculation();
				SymStackPush(InputNum[i]);
				
				oldpr = pr;

			}
			else if (oldpr == pr) {													// 새로운 연산자 우선순위가 기존 우선순위와 같을 경우
			
				Calculation();
				SymStackPush(InputNum[i]);
				oldpr = pr;
			}
			else if (oldpr > pr) {													// 새로운 연산자 우선순위가 기존 우선순위 보다 낮을 경우 +- -> */
				SymStackPush(InputNum[i]);
				calcFlag = true;
				//oldpr = pr;
			}
		}

		if (InputNum[i + 1] == '\0') {
			Search_s = i + 1;
			InitTempArray();
			for (int tp = 0; Search_n < Search_s; Search_n++, tp++) {
				TempArray[tp] = InputNum[Search_n];
			}
			NumStackPush(atof(TempArray));											// 피연산자를 스택에 쌓는다.
			if (calcFlag) {
				Calculation();
				calcFlag = false;
			}
			//TempNum[tn] = atof(TempArray);
			for(int x = 0 ; x<MAX_STACK_SIZE;x++){
				/*NumStackPush(atof(TempArray));*/
				printf("SymStack[0] = %c \n", StackSymbol[0]);
				printf("SymStack[1] = %c \n", StackSymbol[1]);
				Calculation();
				if (IsSymStackEmpty()) {
					printf("NumStack : %s\n", StackNum);
					printf("NumStack[0] = %f \n", StackNum[0]);
					printf("NumStack[1] = %f \n", StackNum[1]);

					printf("SymStack[0] = %c \n", StackNum[0]);
					printf("SymStack[1] = %c \n", StackNum[1]);
					InitTempArray();
					SetDlgItemInt(hDlg, IDC_EDIT2, StackNum[0], TRUE);
					return;
				}
			}
		}
	}
}

void showOutput(HWND hDlg) {
	
	GetExp(hDlg);

}

void rmArray(HWND hDlg) {
	if (np == 0)
	{
		return;
	}
	InputNum[np-1] = '\0';
	np--;
	SetDlgItemText(hDlg, IDC_EDIT1, InputNum);
	if (np == 0)
	{
		InputNum[np] = '0';
		SetDlgItemText(hDlg, IDC_EDIT1, InputNum);
		return;
	}
}

void ClearArray(HWND hDlg) {
	for (int a = 0; a < sizeof(InputNum); a++) {
		InputNum[a] = '\0';
	}
	//int i = 0;
	SetDlgItemText(hDlg, IDC_EDIT1, InputNum);
	
	printf("InputNum : %s \n", InputNum);
}
// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//printf("DlgProc\n");
    UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:

		SetDlgItemText(hDlg, IDC_EDIT1, "0");
		printf("init\n");
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		//printf("btn0\n");
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:	// 0
			printf("0\n");
			showNum('0', hDlg);
			break;
		case IDC_BUTTON2:	// .
			printf(".\n");
			showDot('.', hDlg);
			break;
		case IDC_BUTTON3:	// Enter
			printf("Enter\n");
			showOutput(hDlg);
			break;
		case IDC_BUTTON4:	// 1
			printf("1\n");
			showNum('1', hDlg);
			break;
		case IDC_BUTTON5:	// 2
			printf("2\n");
			showNum('2', hDlg);
			break;
		case IDC_BUTTON6:	// 3
			printf("3\n");
			showNum('3', hDlg);
			break;
		case IDC_BUTTON7:	// 4
			printf("4\n");
			showNum('4', hDlg);
			break;
		case IDC_BUTTON8:	// 5
			printf("5\n");
			showNum('5', hDlg);
			break;
		case IDC_BUTTON9:	// 6
			printf("6\n");
			showNum('6', hDlg);
			break;
		case IDC_BUTTON10:	// 7
			printf("7\n");
			showNum('7', hDlg);
			break;
		case IDC_BUTTON11:	// 8
			printf("8\n");
			showNum('8', hDlg);
			break;
		case IDC_BUTTON12:	// 9
			printf("9\n");
			showNum('9', hDlg);
			break;
		case IDC_BUTTON13:	// /
			printf("/\n");
			showSign('/', hDlg);
			break;
		case IDC_BUTTON14:	// *
			printf("*\n");
			showSign('*', hDlg);
			break;
		case IDC_BUTTON15:	// -
			printf("-\n");
			showSign('-', hDlg);
			break;
		case IDC_BUTTON16:	// Clear
			printf("Clear\n");
			ClearArray(hDlg);
			break;
		case IDC_BUTTON17:	// +
			printf("+\n");
			showSign('+', hDlg);
			break;
		case IDC_BUTTON18:	// C
			printf("C\n");
			rmArray(hDlg);
			break;
		}
		break;
	case WM_CLOSE:
		FreeConsole();
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
		
