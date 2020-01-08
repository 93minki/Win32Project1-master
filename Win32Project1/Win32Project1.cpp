// Win32Project1.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
// 8�� ���� (20.01.08)
// -���� ���� �ذ�
// -Clear ��� ����

#include "stdafx.h"
#include "Win32Project1.h"
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#define MAX_LOADSTRING 100

#define MAX_STACK_SIZE 30

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

char InputNum[MAX_STACK_SIZE];
char TempArray[MAX_STACK_SIZE];
float StackNum[MAX_STACK_SIZE];									// Get Number in Stack
char StackSymbol[MAX_STACK_SIZE];								// Get Sign in Stack
char showfloat[MAX_STACK_SIZE];

int np;
int Search_n = 0;									// Search Number 
int Search_s = 0;									// Search Sign

int nsp = -1;
int ssp = -1;
int priority;

bool fclean = false;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
	//AttachConsole(GetCurrentProcessId());
	//freopen("CON", "w", stdout);
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	return 0;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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

float NumStackPop() {
	float rtnum;
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


void ShowNum(char num, HWND hDlg) {
	InputNum[np] = num;
	np++;
	SetDlgItemText(hDlg, IDC_EDIT1, InputNum);

}

void ShowDot(char dot, HWND hDlg) {
	if (InputNum[np] == '.') {
		return;
	}
	else {
		InputNum[np] = '.';
		np++;
	}
	SetDlgItemText(hDlg, IDC_EDIT1, InputNum);
}

void ShowSign(char sign, HWND hDlg) {
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

bool CheckFormula(HWND hDlg,int num) {
	// Show Error MessageBox if last stack element is Arithmetic Symbol
	if (InputNum[num + 1] == '\0') {
		MessageBox(hDlg, "Wrong formula!!!", "Formula Error", MB_OK);
		return true;
	}
	return false;
}

void Calculation() {
	float pop1;
	float pop2;
	char opr;
	float rst;

	pop1 = NumStackPop();
	pop2 = NumStackPop();
	opr = SymStackPop();

	switch (opr) {														// ���� �����ڿ� ���� ������ �����ϰ� �� ���� rst�� ����.
	case '+':
		rst = pop2 + pop1;
		NumStackPush(rst);												// �� ���� ���ÿ� �ٽ� �״´�.
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


void ShowOutput(HWND hDlg) {
	
	printf("Get Number : %s\n", InputNum);
	printf("Size of Stack : %d\n", GetStackSize());
	int StackSize = GetStackSize();
	int oldpr = 4;																	// �ʱ� �켱������ 4�� �����Ͽ� � �����ȣ�� ���͵� �Էµǵ��� ����
	int pr;
	int pop1;
	int pop2;
	bool calcFlag = false;															// ������ �켱������ ���� ���� �����ؾ� �ϴ� ��� Flag�� ����ؼ� ����
	bool shiftFlag = false;
	int rst = 0;
	char opr;

	for (int i = 0; i < StackSize; i++) {
		// if Stack Value is Sign 
		if (InputNum[i] == '+' || InputNum[i] == '-' || InputNum[i] == '*' || InputNum[i] == '/') {
			if (CheckFormula(hDlg,i)) {												// �߸��� ���� ���� ex. +,-,*,/ ���� ��� 3+ , 3+4+, ��
				return;
			}
			pr = makePriority(InputNum[i]);											// ���� �������� �켱������ ���Ѵ�.
			Search_s = i;															// Ž�������� �ִ밪�� ���� i������ ���� Ž������ �ּҰ��� �ʱ⿡ 0���� �����Ǿ� �ִ�.
			for (int tp = 0; Search_n < Search_s; Search_n++, tp++) {				// �ּ� Ž������ ���� �ִ� Ž���������� ( = ����)�� TempArray�� ����
				TempArray[tp] = InputNum[Search_n];
			}
			Search_n = Search_s + 1;												// Ž�������� �ּҰ��� �����ȣ �������� ���� ex. 123+456+789 [0]~[3] [4(������)] [5] ~
			if (shiftFlag) {
				NumStackPush(atof(TempArray)*-1);									// -�����ڿ� -��ȣ�� �����ϱ� ���ؼ� ���� �����Ѵ�. 5-40 => 5 + (-40)
				shiftFlag = false;			
			}
			else {
				NumStackPush(atof(TempArray));										// �ǿ����� ���ڷ� ��ȯ�Ͽ� ���ÿ� �״´�.
			}
			InitTempArray();														// atof�� ����ؼ� ���ڿ��� ���ڷ� ��ȯ�ϰ�, TempArray�� �ʱ�ȭ�Ѵ�.
			if (calcFlag) {															// ������ �켱������ ���� ���� ����ؾ� �� ��� Flag�� On�ǰ� Flag�� ���� ������ ����.
				Calculation();
				SymStackPush(InputNum[i]);
				calcFlag = false;
			}
			else if (oldpr == 4) {													// ù ��° ������. ������ �켱���� ���� ���� ���ÿ� �׿��� �Ѵ�. (oldpr �ʱⰪ�� 4�� ó���� �����ϱ� ���� ����)
				SymStackPush(InputNum[i]);											// �����ڸ� ���ÿ� �״´�.
				oldpr = pr;															// ���� �켱������ ���� �켱������ �ȴ�.
			}
			else if (oldpr < pr) {													// ���ο� ������ �켱������ ���� �켱���� ���� ���� ��� */ -> +-
				Calculation();														// ����
				SymStackPush(InputNum[i]);											// ���� �� �����ȣ�� ���ÿ� �״´�.
				oldpr = pr;															// ���� ������ �켱������ ���� ������ �켱������ ��ü�ȴ�.
			}
			else if (oldpr == pr) {													// ���ο� ������ �켱������ ���� �켱������ ���� ���
				Calculation();														// ����
				SymStackPush(InputNum[i]);											// ���� �� �����ȣ�� ���ÿ� �״´�.
				oldpr = pr;															// ���� ������ �켱������ ���� ������ �켱������ ��ü�ȴ�.
			}
			else if (oldpr > pr) {													// ���ο� ������ �켱������ ���� �켱���� ���� ���� ��� +- -> */
				SymStackPush(InputNum[i]);											// ������ ���� �ʰ� ���ÿ� �׾Ƶд�.
				calcFlag = true;													// ���ο� �����ڰ� ���̱� ���� ���� ������ �����ϵ��� Flag�� On�Ѵ�.
				//oldpr = pr;														// ���ÿ� ������ �ʰ� �ٷ� ������ �ؾ��ϱ� ������ ������ �켱������ �״�� ����
			}
			if (InputNum[i] == '-') {												// Ž���� �����ڰ� '-' �� ��� �׿��ִ� ������ '-'�� '+'�� ��ü�Ѵ�.
				StackSymbol[ssp] = '+';	
				shiftFlag = true;													// Flag�� ����ؼ� �� ������ ������ ���ڴ� -�� �پ(������ �Ǿ�) ���ÿ� ���δ�.
			}
		}

		if (InputNum[i + 1] == '\0') {
			Search_s = i + 1;
			InitTempArray();
			for (int tp = 0; Search_n < Search_s; Search_n++, tp++) {
				TempArray[tp] = InputNum[Search_n];
			}
			if (shiftFlag) {
				NumStackPush(atof(TempArray)*-1);
			}
			else {
				NumStackPush(atof(TempArray));											// �ǿ����ڸ� ���ÿ� �״´�.
			}
			
			for (int x = 0; !IsSymStackEmpty(); x++) {
				Calculation();
			}
			InitTempArray();
			printf("StackNum[0] : %f\n", StackNum[0]);
			sprintf(showfloat, "%0.2f", StackNum[0]);
			SetDlgItemText(hDlg, IDC_EDIT2, showfloat);
			fclean = true;
			break;

		}
	}
}

void RmArray(HWND hDlg) {
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
	if (fclean) {
		NumStackPop();												// ������� ������. �׳� ���� �ǹ� ����
		priority = 0;
	}
	for (int a = 0; a < sizeof(InputNum); a++) {
		if (InputNum[a] == '\0') {
			break;
		}
		InputNum[a] = '\0';
	}
	for (int b = 0; b < sizeof(showfloat); b++) {
		if (showfloat[b] == '\0') {
			break;
		}
		showfloat[b] = '\0';
	}
	InputNum[0] = '0';
	np = 0;
	Search_n = 0;
	Search_s = 0;
	SetDlgItemText(hDlg, IDC_EDIT1, "0");
	SetDlgItemText(hDlg, IDC_EDIT2, "0");
}
// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//printf("DlgProc\n");
    UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:

		SetDlgItemText(hDlg, IDC_EDIT1, "0");
		SetDlgItemText(hDlg, IDC_EDIT2, "0");
		printf("init\n");
		return (INT_PTR)TRUE;
		
	case WM_COMMAND:
		//printf("btn0\n");
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:	// 0
			printf("0\n");
			ShowNum('0', hDlg);
			break;
		case IDC_BUTTON2:	// .
			printf(".\n");
			ShowDot('.', hDlg);
			break;
		case IDC_BUTTON3:	// Enter
			printf("Enter\n");
			ShowOutput(hDlg);
			break;
		case IDC_BUTTON4:	// 1
			printf("1\n");
			ShowNum('1', hDlg);
			break;
		case IDC_BUTTON5:	// 2
			printf("2\n");
			ShowNum('2', hDlg);
			break;
		case IDC_BUTTON6:	// 3
			printf("3\n");
			ShowNum('3', hDlg);
			break;
		case IDC_BUTTON7:	// 4
			printf("4\n");
			ShowNum('4', hDlg);
			break;
		case IDC_BUTTON8:	// 5
			printf("5\n");
			ShowNum('5', hDlg);
			break;
		case IDC_BUTTON9:	// 6
			printf("6\n");
			ShowNum('6', hDlg);
			break;
		case IDC_BUTTON10:	// 7
			printf("7\n");
			ShowNum('7', hDlg);
			break;
		case IDC_BUTTON11:	// 8
			printf("8\n");
			ShowNum('8', hDlg);
			break;
		case IDC_BUTTON12:	// 9
			printf("9\n");
			ShowNum('9', hDlg);
			break;
		case IDC_BUTTON13:	// /
			printf("/\n");
			ShowSign('/', hDlg);
			break;
		case IDC_BUTTON14:	// *
			printf("*\n");
			ShowSign('*', hDlg);
			break;
		case IDC_BUTTON15:	// -
			printf("-\n");
			ShowSign('-', hDlg);
			break;
		case IDC_BUTTON16:	// Clear
			printf("Clear\n");
			ClearArray(hDlg);
			break;
		case IDC_BUTTON17:	// +
			printf("+\n");
			ShowSign('+', hDlg);
			break;
		case IDC_BUTTON18:	// C
			printf("C\n");
			RmArray(hDlg);
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
		
