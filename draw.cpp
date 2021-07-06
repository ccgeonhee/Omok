#define _CRT_SECURE_NO_WARNINGS
#include "draw.h"
#include <string.h>//�߰�

bool color;//�߰�
CDRAW *CDRAW::getInstance()
{
	static CDRAW draw;
	return &draw;
}

CDRAW::CDRAW()
{
	nX = (40 - SIZE - 1) / 2;
	nY = 11 - SIZE / 2;
}

/* ========================================================================== */
/* ȭ���� Ŀ���� ���̰ų� ����� */
void CDRAW::CursorView(char show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(hConsole , &ConsoleCursor);
}

/* ========================================================================== */
/* x, y ��ǥ�� Ŀ���� �����̴� �Լ� */
void CDRAW::gotoxy(int x, int y) /*Ŀ���̵� �Լ� �κ�*/
{
    COORD XY = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), XY);
}

/* ========================================================================== */
/* �ܼ� ������ �����ϴ� �Լ� */
void CDRAW::SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/* ========================================================================== */
/* �ܼ� ������ ���ڿ� ������ �����ϴ� �Լ� */
void CDRAW::SetColor(int color, int bgcolor) 
{

    color &= 0xf;
    bgcolor &= 0xf;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

void CDRAW::clearBuffer()
{
	while(_kbhit()) _getch();
}

void CDRAW::Notify(int type)
{
	const char *cNotify[] = {
		" ",
		" ",
		"�� �� ��� �ƹ� ������ ����   ", 
		"�Ѽ� �ֽ��ϴ�.       ",
		"�� �� ��� 33�ݼ� ����˴ϴ�. ",
		"                     ",
		"���� 33, 44, 6���� �� �� ����,",
		"���� ������ �����ϴ�."
	};

    SetColor(GRAY);
    gotoxy(nX *2, nY - 2);
    printf("%s", cNotify[type * 2]);
    gotoxy(nX *2, nY - 1);
    printf("%s", cNotify[type * 2 + 1]);
} 

int CDRAW::GetMenu(char *cTitle)
{
    int x, y, n;
    
    const char *strTitle[] = {
    	"O M O K  G A M E",
    	"�����(GOMOKU) ��",
    	"��  ��(NORMAL) ��",
    	"��  ��(RENJU)  ��",
		"��   ��   ��   ��"
	};
	
    const char *menu[] = { "    ",
                           "                         ",
                           "  1. ",
                           "                         ",
                           "  2. ",
                           "                         ",
                           "  3. ",
                           "                         ",
                           "  4. "};

    x = strlen(menu[1]);
    y = sizeof(menu) / sizeof(menu[0]);

    x = 40 - x / 2;
    y = 12 - y / 2 - 1;

    system("cls");
    SetColor(GRAY);
    for(int i = 0; i < sizeof(menu) / sizeof(menu[0]); i++)
    {
        gotoxy(x, y + i);
        if(i & 1) printf("%s", menu[i]);
        else printf("%s%s", menu[i], strTitle[i / 2]);
    }

    while(true)
    {
        n = _getch();
        if(n == ESC || n == '4') return ESC;
        else if(n >= '1' && n <= '3') break;
    }
	
	n -= '0';
	sprintf(cTitle, "OMOK GAME - %s", strTitle[n]);
    return n;
}

int CDRAW::GetMenu()
{
    int x, y, n;
    
    const char *strTitle[] = {
    	"    O M O K  G A M E",
    	"COMPUTER(BLACK) vs PLAYER",
    	"PLAYER(BLACK) vs COMPUTER",
    	"PLAYER vs PLAYER         ",
		"�� �� ��  �� �� �� ��    ",
		"�� �� ��  �� �� �� ��    ",
	};
	
    const char *menu[] = { "    ",
                           "                              ",
                           "  1. ",
                           "                              ",
                           "  2. ",
                           "                              ",
                           "  3. ",
                           "                              ",
                           "  4. ",
                           "                              ",
                           "  5. "};

    x = strlen(menu[1]);
    y = sizeof(menu) / sizeof(menu[0]);

    x = 40 - x / 2;
    y = 12 - y / 2 - 1;

    system("cls");
    SetColor(GRAY);
    for(int i = 0; i < sizeof(menu) / sizeof(menu[0]); i++)
    {
        gotoxy(x, y + i);
        if(i & 1) printf("%s", menu[i]);
        else printf("%s%s", menu[i], strTitle[i / 2]);
    }

    while(true)
    {
        n = _getch();
        if(n >= '1' && n <= '5') break;
    }
	
    return n - '0';
}

void CDRAW::computerStoneColor(int nColor)
{
	int len;
	const char *strName[] = {
		"COMPUTER ",
		"PLAYER   ",
	};

	const char *strStone = "��";
	len = strlen(strName[0]) + 2;
	
    SetColor(nColor, DARK_YELLOW);
	if (nColor == WHITE)color = 0; // WHITE�� 0, BLACK�� 1. �߰�.
	else color = 1; //�߰�.
    gotoxy((nX - 1) * 2 - len, nY);
    printf("%s", strStone);
    SetColor(GRAY, DARK_YELLOW);
    printf("%s", strName[0]);
    SetColor(nColor ==  BLACK ? WHITE : BLACK, DARK_YELLOW);
    gotoxy((nX - 1) * 2 - len, nY + 1);
    printf("%s", strStone);
    SetColor(GRAY, DARK_YELLOW);
    printf("%s", strName[1]);
}
char* CDRAW::getDesktopFolderName()     //c:\Users\UserName\Desktop\ ��ȯ. �߰�
{
	ULONG ulDataType;
	HKEY hKey;
	DWORD dwToRead = 100;
	static char strPath[100];

	char strKey[] = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders";
	RegOpenKeyEx(HKEY_CURRENT_USER, strKey, 0, KEY_READ, &hKey);
	RegQueryValueEx(hKey, "Desktop", NULL, &ulDataType, (LPBYTE)strPath, &dwToRead);
	strPath[dwToRead] = '\0';
	RegCloseKey(hKey);

	return strPath;
}
void CDRAW::stonePoint(int x, int y)
{
	//�߰�
	static int cnt = 0;
	FILE* fp, *fp2;
	char dt[90]; //����ũž �̸� �����
	char dt2[90]; //����ũž �̸� �����
	char Current_game[90] = ""; //�ֱ� ���� ��.
	char history[90] = ""; //���ӱ�� ����.

	strcpy(dt, getDesktopFolderName());
	strcpy(dt2, getDesktopFolderName());

	strcpy(Current_game, strcat(dt, "\\Current_game.txt"));
	strcpy(history, strcat(dt2, "\\history.txt"));
	
	if (cnt == 0) {
		fp2 = fopen(Current_game, "w");
	}
	fp = fopen(history, "a+");
	fp2 = fopen(Current_game, "r+");
	
	if (cnt == 0) {
		int i, j;
		unsigned char a = 0xa6;
		unsigned char b[12];

		
		//�ٵ��� �׸���
		fprintf(fp, "\n");
		fprintf(fp2, "\n");
		for (i = 1; i < 12; i++)
			b[i] = 0xa0 + i;
		fprintf(fp, "15%c%c", a, b[3]);
		fprintf(fp2, "15%c%c", a, b[3]);
		for (i = 0; i < 13; i++) {
			fprintf(fp, "%c%c", a, b[8]);
			fprintf(fp2, "%c%c", a, b[8]);
		}
		fprintf(fp, "%c%c", a, b[4]);
		fprintf(fp2, "%c%c", a, b[4]);
		fprintf(fp,"\n");
		fprintf(fp2, "\n");

		for (i = 14; i > 1; i--)
		{
			fprintf(fp, "%2d%c%c", i, a, b[7]);
			fprintf(fp2, "%2d%c%c", i, a, b[7]);
			for (j = 13; j > 00; j--) {
				fprintf(fp, "%c%c", a, b[11]);
				fprintf(fp2, "%c%c", a, b[11]);
			}
			fprintf(fp,"%c%c", a, b[9]);
			fprintf(fp2, "%c%c", a, b[9]);
			fprintf(fp, "\n");
			fprintf(fp2, "\n");
		}
		fprintf(fp, " 1%c%c", a, b[6]);
		fprintf(fp2, " 1%c%c", a, b[6]);
		for (i = 13; i > 0; i--) {
			fprintf(fp, "%c%c", a, b[10]);
			fprintf(fp2, "%c%c", a, b[10]);
		}
		fprintf(fp,"%c%c", a, b[5]);
		fprintf(fp2, "%c%c", a, b[5]);
		fprintf(fp,"\n");
		fprintf(fp2, "\n");
		
		for (int j = 0; j < 15; j++) {
			if (j == 0) {
				fprintf(fp, "   ");
				fprintf(fp2, "   ");
			}
			fprintf(fp, "%2c", 'A' + j);
			fprintf(fp2, "%2c", 'A' + j);
			if (j == 14) {
				fprintf(fp, "\n");
				fprintf(fp2, "\n");
			}
		}
		//��ǻ�� �� �� White = 0, BLACK = 1
		if(color == 1)fprintf(fp, "Computer  :  %c%d ", 'A' + x - 1, 15 - y + 1);
		else if(color == 0)fprintf(fp, "Player    :     %c%d ", 'A' + x - 1, 15 - y + 1);

	}
	if (cnt == 1) {
		if (color == 0)fprintf(fp, "Computer : %c%d\n", 'A' + x - 1, 15 - y + 1);
		else if (color == 1)fprintf(fp, "\tPlayer  :  %c%d\n", 'A' + x - 1, 15 - y + 1);
	}
	else if (cnt > 1){
		if(cnt %2 == 0)fprintf(fp, "\t     %c%d\t", 'A' + x - 1, 15 - y + 1); // ¦����°.
		else fprintf(fp, "\t%c%d\n", 'A' + x - 1, 15 - y + 1);
	}

	if (cnt % 2 == 1) {
		fseek(fp2, (34 * (y - 1)) + (2 * x) + 2, 0);
		fwrite("��", strlen("��"), 1, fp2);
	}
	else {
		fseek(fp2, (34 * (y - 1)) + (2 * x)+2, 0);
		fwrite("��", strlen("��"), 1, fp2);
	}
	//�߰�
    SetColor(GRAY);
    gotoxy((nX + SIZE + 1) * 2, nY + 1);
    printf("���� ��ġ : %c%d ", 'A' + x - 1, 15 - y + 1);
	cnt++;//�߰�
	fclose(fp);
}

// ���� �޽����� ȭ�鿡 ������ش�. 
void CDRAW::errMsg(int msg)
{
	msg -= 10;
	const char *str[] = {
		"�̹� ���� ���� �ֽ��ϴ�.\n�ٸ� ���� �����ϼ���.",
		"��� �Դϴ�.\n�ٸ� ���� �����ϼ���.",
		"��� �Դϴ�.\n�ٸ� ���� �����ϼ���.",
		"���� ������ �� �� �����ϴ�.\n�ٸ� ���� �����ϼ���.",
		"�� ���� ���� �� �ֽ��ϴ�"
	};
	MessageBox(NULL, str[msg], "Warning!", MB_OK);
}

void CDRAW::showMsg(int msg)
{
	const char *str[] = {
		"���� �� �����Դϴ�.",
		"���� �� �����Դϴ�.",
		"       ",
		"�� ���Դϴ�.       ",
		"�� ���Դϴ�.       ",
		"���º��Դϴ�.      ",
	};
	
    SetColor(GRAY);
    gotoxy((nX + SIZE + 1) * 2, nY);
    printf("%s", str[msg]);
}

void CDRAW::infoKey()
{
	const char *str[] = {
		"�����̽��� : ��  ��",
		"��  ��  Ű : ��  ��",
		"    U      : Undo  ",
		"    R      : Redo  ",
		"Delete Key : Pass  ",
		" ",
		"��� ���� ��������",
		"PASS�ϸ� ���º�"
	};
	
	SetColor(GRAY);
	for(int i = 0; i < sizeof(str) / sizeof(str[0]); i++)
	{
    	gotoxy((nX + SIZE + 1) * 2, nY + 4 + i);
    	printf(str[i]);
	}
}

// ���а� ������ ���ڸ� �˸��� 
// �ѹ��� �� ������ �����. 
bool CDRAW::endMsg(int stone)
{
	const char *winner[] = {"    �� ���Դϴ�.\n    �� ���� �� �Ͻðڽ��ϱ�?     ",
							"    �� ���Դϴ�.\n    �� ���� �� �Ͻðڽ��ϱ�?     ",
							"    ���º��Դϴ�.\n    �� ���� �� �Ͻðڽ��ϱ�?     "};
	showMsg(stone + 3);
	if(MessageBox(NULL, winner[stone], "�� ��", MB_YESNO) == IDYES) return true;
	else return false; 
}

// �ٵ��ǰ� ���� �׸���. 
void CDRAW::printData(int &x, int &y, int type)
{
 	if(x < 1)          x = 1;
	else if(y < 1)     y = 1;
	else if(x > SIZE)  x = SIZE;
	else if(y > SIZE)  y = SIZE;

	const int color[] = {BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK};
    const char *shape[] = {"�� ", "�� ", "�� ", "�� ", "�� ", "�� ", "�� ", "�� ", "�� ", "��", "��", "��"};
	
    SetColor(color[type], DARK_YELLOW);
    // ������ ������ 9���� ����� �����ϱ� 
	// ��� �׷��� ���� ��ǥ�� �ְ� ����� �����´�. 
    if(type == LINE) type = getLineNumber(x, y);
    // ������ 0-8���� �̰�, �� �ڿ� ���� ��ġ�Ǿ� ������ 9�� �����ش�. 
    else type += 9;
    gotoxy((x + nX - 1) * 2, y + nY - 1);
    printf("%s", shape[type]);
    SetColor(GRAY);
}

// ���ο� �������� ��ǥ�� ���ڿ� ���ĺ����� ǥ�����ش�.
// ���γ� ���ο� ���ڳ� ���ĺ����� �����ϸ� ��ǥ�� ǥ���� ��
// ȥ���� �� �� ������ ������ ǥ���Ѵ�. 
void CDRAW::printNum()
{	
    SetColor(BLACK, DARK_YELLOW);
    //x���� ��� 
    for(int i = 0; i < SIZE; i++) 
    {
    	gotoxy((nX + i) * 2, nY + SIZE);
    	printf("%2c", i + 'A');
	}
	//y���� ��� 
    for(int i = 0, j = SIZE; i < SIZE; i++, j--) 
    {
    	gotoxy((nX - 1) * 2, nY + j - 1);
    	printf("%2d", i + 1);
	}
	gotoxy((nX - 1) * 2, nY + SIZE);
	
	printf("  ");
    SetColor(GRAY);
}

//������
//������
//������
//������ �Ʒ��� ���� �Ѵ�. 
//0 1 2
//������
//3 4 5
//������
//6 7 8
//������
// �� ����� �ٵ����� ������̴�
// �ٵ����� ��� ����� 
// �� 9���� ��縸 ������ �׷��� �� �ִ�. 
// x�� 0�϶��� ���� ��
// y�� 0�϶��� ���� ���� 
// x,y���� ���� ����� ��ȣ�� ��ȯ�Ѵ�. 

// �ٵ��� ��ü�� �׷��ְ� �ٽ� �׷��� ������
// �׷��� �ϸ� ���� Size�� 19�� �ϰ� ������
// ��ü�� �����ؾ� �ǰ�, ���� ȭ���� ���� �Ÿ� �� �־
// �ʿ��� ���� �׶��׶� �׷��ַ� �̷� �۾��� �Ѵ� ���̴�. 
int CDRAW::getLineNumber(int x, int y)
{
	// ���� ������ 
	if(x == 1)
	{
		// x, y�� �� 0�̸� ���� ��� �𼭸� 
		if(y == 1) return 0;
		// ���� �ϴ� �𼭸� 
		else if(y == SIZE) return 6;
		// ���� ������ 
		else return 3;
	}
	// ������ ������ 
	else if(x == SIZE)
	{
		// ������ ��� �𼭸� 
		if(y == 1) return 2;
		// ������ �ϴ� �𼭸� 
		else if(y == SIZE) return 8;
		// ������ ������ 
		else return 5;
	}
	// ���� ������
	else if(y == 1) return 1;
	// �Ʒ��� ������
	else if(y == SIZE) return 7;
	// ������ ��� + ��� 
	else return 4;
}

//���� �ð��� ȭ�鿡 ǥ�����ش�. 
void CDRAW::drawTime(time_t sec)
{
	int hh, mm;
	
    hh = sec / 3600;
    mm = (sec % 3600) / 60;
    sec = sec %= 60;

	SetColor(SKY_BLUE);
	gotoxy(nX * 2 - 2, nY + SIZE +  1);
	printf("%02d : %02d : %02d", hh, mm, sec);
	SetColor(GRAY);
}
