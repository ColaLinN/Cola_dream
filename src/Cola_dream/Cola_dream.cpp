#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include<graphics.h>
#include<conio.h>
#include<math.h>
#include<Windows.h>
#include<time.h>
#include <mmsystem.h>

#pragma comment(lib,"Winmm.lib")	// 引用 Windows Multimedia API

#define Width 800	//画面宽
#define High 600	//画面高
#define PI 3.1415924	//Π的值

//*******************************函数***********************//
void startup();				// 数据初始化
void show();				//显示画面
void enemy_HP();			//敌机血量判断
void player_HP();			//玩家血量判断
void e_bullet_1(int enemy_x_1, int enemy_y_1);//敌机子弹一函数，波形弹
void e_bullet_2(int enemy_x_1, int enemy_y_1);//敌机子弹二函数，随机弹
void e_bullet_3(int enemy_x_1, int enemy_y_1);//敌机子弹三函数，重弹
void e_bullet_4(int enemy_x_1, int enemy_y_1);//敌机子弹四函数，以敌机为中点四周重弹函数
void e_bullet_5(int enemy_x_1, int enemy_y_1);//敌机子弹三函数，激光
void player_bullet_1();		//玩家子弹函数
void player_bullet_2();		//玩家子弹二
void updateWithoutInput();	//与用户输入无关的更新
void updateWithInput();		//与用户输入有关的更新
void invincible();			//玩家无敌状态
void story();				//游戏剧情
void dialogue(int i);		//人物对话
char get();					//对话暂停
void stop();				//暂停界面
void gameover();			// 游戏结束、后续处理

//*******************************函数***********************//


//#######################全局变量##########################//
int player_life;						//玩家生命
int player_life_number=7;				//玩家生命条数设置
int invincible_mode = 1;				//玩家无敌模式
int invincible_kb[5];					//玩家按键无敌
int invincible_time = 0;				//无敌模式减敌方血量的计时
int player_x, player_y;					//玩家坐标

int player_bullet_map[4][20][7] = { 0 };//我方子弹
int player_bullet_layer = 3;			//我方子弹层数
int player_bullet_layer_number = 20;	//每层弹数
int time_i_1 = 0;
float p_b2_x , p_b2_y ;					// 玩家攻击小球的初始位置
float p_b2_vx , p_b2_vy ;				//小球的速度

//int player_bullet_map[Width][High] = { 0 };	//
//int player_bullet_x, player_bullet_y;//玩家子弹坐标
//int player_bullet_move=2;				//玩家子弹移动
int shoot = 0;//射击键
int enemy_life_number;					//敌机生命条数
int enemy_life_number_1 = 5;			//敌机生命条数设置
int enemy_life_length = 2000;			//敌机生命量
int enemy_life;							//敌机生命
int enemy_x, enemy_y;					//敌机坐标
int enemy_y_a, enemy_x_a;				//敌机移动加速度

int map[800][600] = { 0 };				//敌机子弹一坐标
int bullet_1_map[100][5] = { 0 };		//【0】是x，【1】是y，【2】是enemy_bullet_length,[][3]是enemy_bullet_length
int bullet_layer = 1;					//子弹一层数
int bullet_e2_map[150][5] = { 0 };      // 敌机子弹二的地图【0】是x，【1】是y，【2】是alive，【3】是角度，【4】暂无
int bullet_e3_map[150][5] = { 0 };		//敌机子弹三的地图【0】是x，【1】是y，【2】是alive，【3】是角度，【4】暂无
int e_bullet_3_number;					//敌机子弹三数量
int bullet_e4_map[5][5] = { 0 };		//敌机子弹四的地图【0】是x，【1】是y，【2】是alive，【3】是角度，【4】加速度
int e_bullet_4_number;					//敌机子弹四数量
int bullet_e5_map[5][5] = { 0 };		//敌机子弹五的地图【0】是x，【1】是y，【2】是alive，【3】是角度，【4】加速度
int final_chapter_number;				//最终章的判定

SYSTEMTIME ti;							//保存时间
int time_i;//敌机子弹一判定

IMAGE img_bg_begin;						//开始背景图片
IMAGE img_bg[12];						//背景图片
IMAGE img_png[10];						//返回图标等
IMAGE img_player[6], img_p_bullet;		//人物素材
IMAGE img_enemy[20];					//敌机素材

int layer = 0;							//指代程序在哪层g
int layer_bgm[10] = { 0 };				//每层游戏的bmg

int gameover_x = 1;						//游戏结束

//#######################全局变量##########################//

int main()
{
	initgraph(Width, High);
	startup();									// 数据初始化	
	while (gameover_x)							//  游戏循环执行
	{
		show();									// 显示画面
		updateWithoutInput();					// 与用户输入无关的更新
		updateWithInput();						// 与用户输入有关的更新
	}
	gameover();									// 游戏结束、后续处理
	return 0;
}

// 数据初始化
void startup()// 数据初始化
{
	srand((unsigned)time(NULL));				//定随机种子
	HWND GetHWnd();								// 获取绘图窗口句柄(HWND)
	TCHAR* GetEasyXVer();						// 获取 EasyX 当前版本
	LOGFONT f;
	gettextstyle(&f);						 // 获取当前字体设置
	setbkmode(TRANSPARENT);					// 设置文字背景为透明色
	f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&f);						 // 设置字体样式
	loadimage(&img_player[0], _T("img\\角色贴图1.jpg"));//角色贴图
	loadimage(&img_player[1], _T("img\\角色贴图遮罩1.jpg"));
	loadimage(&img_player[2], _T("img\\角色贴图2.jpg"));
	loadimage(&img_player[3], _T("img\\角色贴图遮罩2.jpg"));
	loadimage(&img_enemy[0], _T("img\\敌机1.jpg"));
	loadimage(&img_enemy[1], _T("img\\敌机遮罩1.jpg"));

	loadimage(&img_bg[0], _T("img\\开始.jpg"));		//开始背景
	loadimage(&img_bg[1], _T("img\\游戏设置.jpg"));//开始背景
	loadimage(&img_bg[2], _T("img\\游戏介绍.jpg"));//游戏介绍
	loadimage(&img_bg[3], _T("img\\关卡选择.jpg"));	//关卡选择
	loadimage(&img_bg[4], _T("img\\背景图.jpg"));	//第一章
	loadimage(&img_bg[5], _T("img\\背景图.jpg"));	//第二章
	loadimage(&img_bg[6], _T("img\\背景图.jpg"));	//第三章
	loadimage(&img_bg[7], _T("img\\背景图.jpg"));	//最终章
	loadimage(&img_bg[8], _T("img\\gameover.jpg"));	//游戏结束
	loadimage(&img_bg[9], _T("img\\胜利.jpg"));		//游戏胜利
	loadimage(&img_bg[10], _T("img\\最终胜利.jpg"));//游戏最终胜利

	loadimage(&img_png[0], _T("img\\贴图.jpg"));	//初始化图标
	loadimage(&img_png[1], _T("img\\贴图遮罩.jpg"));//初始化图标
	loadimage(&img_png[2], _T("img\\对话贴图.jpg"));//故事对话贴图
	loadimage(&img_png[3], _T("img\\对话贴图遮罩.jpg"));//故事对话贴图遮罩

	player_x = 50;//坐标初始化
	player_y = High / 2;
	enemy_x = 1 * Width / 2;
	enemy_y = 3 * High / 20;
	player_life = player_life_number;		//玩家生命条数初始化
	enemy_life = enemy_life_length;			//敌机生命量
	enemy_life_number = enemy_life_number_1;//敌机生命条数初始化
	enemy_y_a = 0;
	enemy_x_a = 2;
	e_bullet_3_number = 5;
	e_bullet_4_number = 5;
	p_b2_x = 320.f; p_b2_y = 240.f; // 玩家攻击小球的初始位置
	p_b2_vx = 0.f; p_b2_vy = 0.f;//小球的速度
	BeginBatchDraw();
}

//显示画面
void show()	// 显示画面
{
	if (layer == 0)//开始界面
	{
		if (layer_bgm[0] == 0)
		{
			mciSendString(TEXT("close all"), NULL, 0, NULL);
			mciSendString(TEXT(" open music\\徐梦圆-color-X+3D.wav alias color-X+3D"), NULL, 0, NULL);
			mciSendString(TEXT("play color-X+3D"), NULL, 0, NULL);
			for(int i=0;i<10;i++)
				layer_bgm[i] = 0; 
			layer_bgm[0] = 1;
		}
		//outtextxy(50, 59, _T("第二关"));
		putimage(0, 0, &img_bg[0]);
		FlushBatchDraw();
	}
	if (layer == 1)//游戏设置
	{
		putimage(0, 0, &img_bg[1]);
		putimage(19, 526, 68, 65, &img_png[1], 19, 526, NOTSRCERASE);
		putimage(19, 526, 68, 65, &img_png[0], 19, 526, SRCINVERT);
		//玩家血量设置
		if (player_life_number == 15){
			putimage(370, 148, 32, 32, &img_png[1], 370, 148, NOTSRCERASE);
			putimage(370, 148, 32, 32, &img_png[0], 370, 148, SRCINVERT);
		}
		if (player_life_number == 7) { 
			putimage(501, 148, 32, 32, &img_png[1], 370, 148, NOTSRCERASE); 
			putimage(501, 148, 32, 32, &img_png[0], 370, 148, SRCINVERT); 
		}
		if (player_life_number == 3) {
			putimage(641, 148, 32, 32, &img_png[1], 370, 148, NOTSRCERASE);
			putimage(641, 148, 32, 32, &img_png[0], 370, 148, SRCINVERT);
		}
		//敌方血量设置
		if (enemy_life_number_1 == 10) {
			putimage(370, 277, 32, 32, &img_png[1], 370, 148, NOTSRCERASE);
			putimage(370, 277, 32, 32, &img_png[0], 370, 148, SRCINVERT);
		}
		if (enemy_life_number_1 == 5) {
			putimage(501, 277, 32, 32, &img_png[1], 370, 148, NOTSRCERASE);
			putimage(501, 277, 32, 32, &img_png[0], 370, 148, SRCINVERT);
		}
		if (enemy_life_number_1 == 1) {
			putimage(641, 277, 32, 32, &img_png[1], 370, 148, NOTSRCERASE);
			putimage(641, 277, 32, 32, &img_png[0], 370, 148, SRCINVERT);
		}
		//无敌设置
		if (invincible_mode == 1) {
			putimage(370, 406, 32, 32, &img_png[1], 370, 148, NOTSRCERASE);
			putimage(370, 406, 32, 32, &img_png[0], 370, 148, SRCINVERT);
		}
		if (invincible_mode == 0) {
			putimage(501, 406, 32, 32, &img_png[1], 370, 148, NOTSRCERASE);
			putimage(501, 406, 32, 32, &img_png[0], 370, 148, SRCINVERT);
		}
		FlushBatchDraw();
	}
	if (layer == 2)//游戏介绍
	{
		putimage(0, 0, &img_bg[2]);
		putimage(19, 526, 68, 65, &img_png[1], 19, 526, NOTSRCERASE);
		putimage(19, 526, 68, 65, &img_png[0], 19, 526, SRCINVERT);
		FlushBatchDraw();
	}
	if (layer == 3)//开始游戏
	{
		putimage(0, 0, &img_bg[3]);
		FlushBatchDraw();
	}
	if (layer == 4)//第一章
	{
		if (layer_bgm[1] == 0)//打开音乐
		{
			story();
			for (int i = 0; i<5; i++)
				layer_bgm[i] = 0;
			layer_bgm[1] = 1;
		}
		putimage(0, 0, &img_bg[4]);//背景
		putimage(727, 526, 65, 65, &img_png[1], 727, 526, NOTSRCERASE);//暂停图标
		putimage(727, 526, 65, 65, &img_png[0], 727, 526, SRCINVERT);
		invincible();//无敌模式判定与角色贴图输出
		putimage(enemy_x-50, enemy_y-50, 100, 100, &img_enemy[1], 0, 0, NOTSRCERASE);//敌机绘制
		putimage(enemy_x-50, enemy_y-50, 100, 100, &img_enemy[0], 0, 0, SRCINVERT);
		//e_bullet_1(10, 1 * High / 10);//敌机子弹一计算	
		e_bullet_4(enemy_x, enemy_y );//敌机子弹四计算
		e_bullet_5(enemy_x, enemy_y);//敌机子弹五计算
		player_bullet_1();		//我机子弹二计算
		for (int x_pb = 0; x_pb < player_bullet_layer; x_pb++)//我机子弹判定
		{
			for (int y_pb = 0; y_pb < player_bullet_layer_number; y_pb++)
			{
				if (20 > fabs(player_bullet_map[x_pb][y_pb][0] - enemy_x) && 20 > fabs(player_bullet_map[x_pb][y_pb][1] - enemy_y))
				{
					enemy_life -= 1;
				}
				setfillcolor(RGB(71, 181, 99));
				setlinecolor(RGB(71, 181, 99));
				//fillrectangle(player_bullet_map[x_pb][y_pb][0] - 5, player_bullet_map[x_pb][y_pb][1] - 3, player_bullet_map[x_pb][y_pb][0] + 5, player_bullet_map[x_pb][y_pb][1] + 3);
				fillcircle(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 2);
				//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[1], 81, 144, NOTSRCERASE);
				//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[0], 81, 144, SRCINVERT);
			}
		}
		for (int x1 = 0; x1 < e_bullet_4_number; x1++)//敌机子弹四判定
		{
			if (invincible_kb[0] == 1)bullet_e4_map[x1][2] = 0;
			if (bullet_e4_map[x1][2] == 1)
			{
				if ((30>= fabs(player_x - bullet_e4_map[x1][0])) && 30 >= fabs(player_y - bullet_e4_map[x1][1]))
				{
					if (invincible_kb[1] == 0)
					player_life -= 1;
					bullet_e4_map[x1][2] = 0;
				}
				setlinecolor(BLACK);
				setfillcolor(BLACK);
				fillcircle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 20);
				setlinecolor(WHITE);
				setfillcolor(WHITE);
				fillcircle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 10);
				setlinecolor(BLUE);
				setfillcolor(BLUE);
				circle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 23);
				circle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 24);
				circle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 28);
				circle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 27);
				circle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 30);
			}
		}
		for (int x1 = 0; x1 < 1; x1++)//敌机子弹五判定
		{
			if (invincible_kb[0] == 1)bullet_e5_map[x1][2] = 0;
			if (bullet_e5_map[x1][2] == 1)
			{
				if ((30 >= fabs(player_x - enemy_x)) && fabs(player_y-enemy_y) <= fabs(enemy_y - bullet_e5_map[x1][1]))
				{
					if (invincible_kb[1] == 0)
					player_life -= 1;
					bullet_e5_map[x1][2] = 0;
				}
				setlinecolor(WHITE);
				setfillcolor(WHITE);
				fillroundrect(enemy_x - 10, enemy_y, enemy_x + 10, bullet_e5_map[x1][1],0,0);
				setlinecolor(WHITE);
				setfillcolor(WHITE);
				fillroundrect(enemy_x - 30, enemy_y, enemy_x - 20, bullet_e5_map[x1][1], 0, 0);
				fillroundrect(enemy_x + 30, enemy_y, enemy_x + 20, bullet_e5_map[x1][1], 0, 0);
				setlinecolor(BLUE);
				setfillcolor(BLUE);
				fillroundrect(enemy_x - 20, enemy_y, enemy_x - 10, bullet_e5_map[x1][1], 0, 0);
				fillroundrect(enemy_x + 20, enemy_y, enemy_x + 10, bullet_e5_map[x1][1], 0, 0);
			}
		}
		enemy_HP();//敌机血量条
		player_HP();//我机血量
		FlushBatchDraw();
		Sleep(1);
	}
	if (layer == 5)//第二章
	{
		if (layer_bgm[2] == 0)//打开音乐
		{
			story();
			for (int i = 0; i<5; i++)
				layer_bgm[i] = 0;
			layer_bgm[2] = 1;
		}
		putimage(0, 0, &img_bg[5]);//背景
		putimage(727, 526, 65, 65, &img_png[1], 727, 526, NOTSRCERASE);//暂停图标
		putimage(727, 526, 65, 65, &img_png[0], 727, 526, SRCINVERT);
		invincible();//无敌模式判定与角色贴图输出
		putimage(enemy_x - 50, enemy_y - 50, 100, 100, &img_enemy[1], 0, 0, NOTSRCERASE);//敌机绘制
		putimage(enemy_x - 50, enemy_y - 50, 100, 100, &img_enemy[0], 0, 0, SRCINVERT);
		e_bullet_3(enemy_x, enemy_y);//敌机子弹三计算
		player_bullet_1();//我机子弹一计算
		for (int x_pb = 0; x_pb < player_bullet_layer; x_pb++)//我机子弹判定
		{
			for (int y_pb = 0; y_pb < player_bullet_layer_number; y_pb++)
			{
				if (20 > fabs(player_bullet_map[x_pb][y_pb][0] - enemy_x) && 20 > fabs(player_bullet_map[x_pb][y_pb][1] - enemy_y))
				{
					enemy_life -= 1;
				}
				setfillcolor(RGB(71, 181, 99));
				setlinecolor(RGB(71, 181, 99));
				//fillrectangle(player_bullet_map[x_pb][y_pb][0] - 5, player_bullet_map[x_pb][y_pb][1] - 3, player_bullet_map[x_pb][y_pb][0] + 5, player_bullet_map[x_pb][y_pb][1] + 3);
				fillcircle(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 2);
				//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[1], 81, 144, NOTSRCERASE);
				//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[0], 81, 144, SRCINVERT);
			}
		}
		for (int x1 = 0; x1 < e_bullet_3_number; x1++)//敌机子弹三判定
		{
			if (invincible_kb[0] == 1)bullet_e3_map[x1][2] = 0;
			if (bullet_e3_map[x1][2] == 1)
			{
				if ((30>= fabs(player_x - bullet_e3_map[x1][0])) && 30 >= fabs(player_y - bullet_e3_map[x1][1]))
				{
					if (invincible_kb[1] == 0)
					player_life -= 1;
					bullet_e3_map[x1][2] = 0;
				}
				setlinecolor(BLACK);
				setfillcolor(BLACK);
				fillcircle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 20);
				setlinecolor(WHITE);
				setfillcolor(WHITE);
				fillcircle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 10);
				setlinecolor(RED);
				setfillcolor(RED);
				circle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 23);
				circle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 24);
				circle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 28);
				circle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 27);
				circle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 30);
			}
		}
		if (30 > fabs(p_b2_x - enemy_x) && 30 > fabs(p_b2_y - enemy_y))
		{
			enemy_life -= 100;
		}
		enemy_HP();//敌机血量条
		player_HP();//我机血量
		FlushBatchDraw();
		Sleep(1);
	}
	if (layer == 6)
	{ 
		if (layer_bgm[3] == 0)//打开音乐
		{
			story();
			for (int i = 0; i<5; i++)
				layer_bgm[i] = 0;
			layer_bgm[3] = 1;
		}
		putimage(0, 0, &img_bg[6]);//背景
		putimage(727, 526, 65, 65, &img_png[1], 727, 526, NOTSRCERASE);//暂停图标
		putimage(727, 526, 65, 65, &img_png[0], 727, 526, SRCINVERT);
		invincible();//无敌模式判定与角色贴图输出
		putimage(enemy_x - 50, enemy_y - 50, 100, 100, &img_enemy[1], 0, 0, NOTSRCERASE);//敌机绘制
		putimage(enemy_x - 50, enemy_y - 50, 100, 100, &img_enemy[0], 0, 0, SRCINVERT);
		//e_bullet_1(10, 1 * High / 10);//敌机子弹一1计算
		//e_bullet_1(780, 1 * High / 10);//敌机子弹一2计算
		e_bullet_2(enemy_x, enemy_y);//敌机子弹二计算
		player_bullet_1();//我机子弹计算
		//if ((10 > fabs(player_x - enemy_x)) && fabs(player_y - enemy_y))//与敌机相撞
		//	gameover_x = 0;
		for (int x1 = 0; x1 < 150; x1++)//敌机子弹二绘制
		{
			if (invincible_kb[0] == 1)bullet_e2_map[x1][2] = 0;
			if (bullet_e2_map[x1][2] == 1)
			{
				if ((2>= fabs(player_x - bullet_e2_map[x1][0])) && 2 >= fabs(player_y - bullet_e2_map[x1][1]))
				{
					if (invincible_kb[1] == 0)
					player_life -= 1;
					bullet_e2_map[x1][2] = 0;
				}
				setlinecolor(WHITE);
				setfillcolor(WHITE);
				//putimage(bullet_e2_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[1], 62, 144, NOTSRCERASE);
				//putimage(bullet_e2_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[0], 62, 144, SRCINVERT);
				fillcircle(bullet_e2_map[x1][0], bullet_e2_map[x1][1], 2);
			}
		}
		for (int x_pb = 0; x_pb < player_bullet_layer; x_pb++)//我机子弹一判定
		{
			for (int y_pb = 0; y_pb < player_bullet_layer_number; y_pb++)
			{
				if (20 > fabs(player_bullet_map[x_pb][y_pb][0] - enemy_x) && 20 > fabs(player_bullet_map[x_pb][y_pb][1] - enemy_y))
				{
					enemy_life -= 1;
				}
				setfillcolor(RGB(71, 181, 99));
				setlinecolor(RGB(71, 181, 99));
				//fillrectangle(player_bullet_map[x_pb][y_pb][0] - 5, player_bullet_map[x_pb][y_pb][1] - 3, player_bullet_map[x_pb][y_pb][0] + 5, player_bullet_map[x_pb][y_pb][1] + 3);
				fillcircle(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 2);
				//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[1], 81, 144, NOTSRCERASE);
				//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[0], 81, 144, SRCINVERT);
			}
		}
		enemy_HP();//敌机血量条
		player_HP();//我机血量
		FlushBatchDraw();
		Sleep(1);
	}
	if (layer == 7)
	{
		if (layer_bgm[4] == 0)//打开音乐
		{
			story();
			for (int i = 0; i<5; i++)
				layer_bgm[i] = 0;
			layer_bgm[4] = 1;
		}
		putimage(0, 0, &img_bg[7]);//背景
		putimage(727, 526, 65, 65, &img_png[1], 727, 526, NOTSRCERASE);//暂停图标
		putimage(727, 526, 65, 65, &img_png[0], 727, 526, SRCINVERT);
		invincible();//无敌模式判定与角色贴图输出
		putimage(enemy_x - 50, enemy_y - 50, 100, 100, &img_enemy[1], 0, 0, NOTSRCERASE);//敌机绘制
		putimage(enemy_x - 50, enemy_y - 50, 100, 100, &img_enemy[0], 0, 0, SRCINVERT);
		if (enemy_life_number <0)
		{
			e_bullet_1(10, 1 * High / 10);//敌机子弹一计算	
			player_bullet_1();		//我机子弹一计算
			for (int x_pb = 0; x_pb < player_bullet_layer; x_pb++)//我机子弹一判定
			{
				for (int y_pb = 0; y_pb < player_bullet_layer_number; y_pb++)
				{
					if (20 > fabs(player_bullet_map[x_pb][y_pb][0] - enemy_x) && 20 > fabs(player_bullet_map[x_pb][y_pb][1] - enemy_y))
					{
						enemy_life -= 1;
					}
					setfillcolor(RGB(71, 181, 99));
					setlinecolor(RGB(71, 181, 99));
					//fillrectangle(player_bullet_map[x_pb][y_pb][0] - 5, player_bullet_map[x_pb][y_pb][1] - 3, player_bullet_map[x_pb][y_pb][0] + 5, player_bullet_map[x_pb][y_pb][1] + 3);
					fillcircle(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 2);
					//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[1], 81, 144, NOTSRCERASE);
					//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[0], 81, 144, SRCINVERT);
				}
			}
			for (int x1 = 0; x1<Width; x1++)//敌机子弹一判定
			{
				for (int y1 = 0; y1 < High; y1++)
				{
					if (invincible_kb[0] == 1)map[x1][y1] = 0;
					if (map[x1][y1] == 1)
					{
						if ((2 > fabs(player_x - x1)) && 2 > fabs(player_y - y1))
						{
							if (invincible_kb[1] == 0)
							player_life -= 1;
							map[x1][y1] = 0;
						}
						setlinecolor(RED);
						setfillcolor(RED);
						fillcircle(x1, y1,2);
						//putimage(x1, y1, 18, 15, &img_player[1], 81, 144, NOTSRCERASE);
						//putimage(x1, y1, 18, 15, &img_player[0], 81, 144, SRCINVERT);
					}
				}
			}
		}
		if(enemy_life_number>4&& enemy_life_number <=20)
		{
			e_bullet_4(enemy_x, enemy_y);//敌机子弹四计算
			e_bullet_5(enemy_x, enemy_y);//敌机子弹五计算
			player_bullet_1();		//我机子弹二计算
			for (int x_pb = 0; x_pb < player_bullet_layer; x_pb++)//我机子弹判定
			{
				for (int y_pb = 0; y_pb < player_bullet_layer_number; y_pb++)
				{
					if (20 > fabs(player_bullet_map[x_pb][y_pb][0] - enemy_x) && 20 > fabs(player_bullet_map[x_pb][y_pb][1] - enemy_y))
					{
						enemy_life -= 1;
					}
					setfillcolor(RGB(71, 181, 99));
					setlinecolor(RGB(71, 181, 99));
					//fillrectangle(player_bullet_map[x_pb][y_pb][0] - 5, player_bullet_map[x_pb][y_pb][1] - 3, player_bullet_map[x_pb][y_pb][0] + 5, player_bullet_map[x_pb][y_pb][1] + 3);
					fillcircle(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 2);
					//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[1], 81, 144, NOTSRCERASE);
					//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[0], 81, 144, SRCINVERT);
				}
			}
			for (int x1 = 0; x1 < e_bullet_4_number; x1++)//敌机子弹四判定
			{
				if (invincible_kb[0] == 1)bullet_e4_map[x1][2] = 0;
				if (bullet_e4_map[x1][2] == 1)
				{
					if ((30 >= fabs(player_x - bullet_e4_map[x1][0])) && 30 >= fabs(player_y - bullet_e4_map[x1][1]))
					{
						if (invincible_kb[1] == 0)
						player_life -= 1;
						bullet_e4_map[x1][2] = 0;
					}
					setlinecolor(BLACK);
					setfillcolor(BLACK);
					fillcircle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 20);
					setlinecolor(WHITE);
					setfillcolor(WHITE);
					fillcircle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 10);
					setlinecolor(BLUE);
					setfillcolor(BLUE);
					circle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 23);
					circle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 24);
					circle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 28);
					circle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 27);
					circle(bullet_e4_map[x1][0], bullet_e4_map[x1][1], 30);
				}
			}
			for (int x1 = 0; x1 < 1; x1++)//敌机子弹五判定
			{
				if (invincible_kb[0] == 1)bullet_e5_map[x1][2] = 0;
				if (bullet_e5_map[x1][2] == 1)
				{
					if ((30 >= fabs(player_x - enemy_x)) && fabs(player_y - enemy_y) <= fabs(enemy_y - bullet_e5_map[x1][1]))
					{
						if (invincible_kb[1] == 0)
						player_life -= 1;
						bullet_e5_map[x1][2] = 0;
					}
					setlinecolor(WHITE);
					setfillcolor(WHITE);
					fillroundrect(enemy_x - 10, enemy_y, enemy_x + 10, bullet_e5_map[x1][1], 0, 0);
					setlinecolor(WHITE);
					setfillcolor(WHITE);
					fillroundrect(enemy_x - 30, enemy_y, enemy_x - 20, bullet_e5_map[x1][1], 0, 0);
					fillroundrect(enemy_x + 30, enemy_y, enemy_x + 20, bullet_e5_map[x1][1], 0, 0);
					setlinecolor(BLUE);
					setfillcolor(BLUE);
					fillroundrect(enemy_x - 20, enemy_y, enemy_x - 10, bullet_e5_map[x1][1], 0, 0);
					fillroundrect(enemy_x + 20, enemy_y, enemy_x + 10, bullet_e5_map[x1][1], 0, 0);
				}
			}
		}
		if (enemy_life_number == 4)
		{
			e_bullet_3(enemy_x, enemy_y);//敌机子弹三计算
			player_bullet_1();//我机子弹一计算
			for (int x_pb = 0; x_pb < player_bullet_layer; x_pb++)//我机子弹判定
			{
				for (int y_pb = 0; y_pb < player_bullet_layer_number; y_pb++)
				{
					if (20 > fabs(player_bullet_map[x_pb][y_pb][0] - enemy_x) && 20 > fabs(player_bullet_map[x_pb][y_pb][1] - enemy_y))
					{
						enemy_life -= 1;
					}
					setfillcolor(RGB(71, 181, 99));
					setlinecolor(RGB(71, 181, 99));
					//fillrectangle(player_bullet_map[x_pb][y_pb][0] - 5, player_bullet_map[x_pb][y_pb][1] - 3, player_bullet_map[x_pb][y_pb][0] + 5, player_bullet_map[x_pb][y_pb][1] + 3);
					fillcircle(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 2);
					//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[1], 81, 144, NOTSRCERASE);
					//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[0], 81, 144, SRCINVERT);
				}
			}
			for (int x1 = 0; x1 < e_bullet_3_number; x1++)//敌机子弹三判定
			{
				if (invincible_kb[0] == 1)bullet_e3_map[x1][2] = 0;
				if (bullet_e3_map[x1][2] == 1)
				{
					if ((30 >= fabs(player_x - bullet_e3_map[x1][0])) && 30 >= fabs(player_y - bullet_e3_map[x1][1]))
					{
						if (invincible_kb[1] == 0)
							player_life -= 1;
						bullet_e3_map[x1][2] = 0;
					}
					setlinecolor(BLACK);
					setfillcolor(BLACK);
					fillcircle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 20);
					setlinecolor(WHITE);
					setfillcolor(WHITE);
					fillcircle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 10);
					setlinecolor(RED);
					setfillcolor(RED);
					circle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 23);
					circle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 24);
					circle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 28);
					circle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 27);
					circle(bullet_e3_map[x1][0], bullet_e3_map[x1][1], 30);
				}
			}
		}
		if (enemy_life_number >= 1 && enemy_life_number <= 3)
		{
			e_bullet_2(enemy_x, enemy_y);//敌机子弹二计算
			player_bullet_1();//我机子弹计算
			for (int x1 = 0; x1 < 150; x1++)//敌机子弹二绘制
			{
				if (invincible_kb[0] == 1)bullet_e2_map[x1][2] = 0;
				if (bullet_e2_map[x1][2] == 1)
				{
					if ((2 >= fabs(player_x - bullet_e2_map[x1][0])) && 2 >= fabs(player_y - bullet_e2_map[x1][1]))
					{
						if (invincible_kb[1] == 0)
						player_life -= 1;
						bullet_e2_map[x1][2] = 0;
					}
					setlinecolor(WHITE);
					setfillcolor(WHITE);
					fillcircle(bullet_e2_map[x1][0], bullet_e2_map[x1][1], 2);
				}
			}
			for (int x_pb = 0; x_pb < player_bullet_layer; x_pb++)//我机子弹一判定
			{
				for (int y_pb = 0; y_pb < player_bullet_layer_number; y_pb++)
				{
					if (20 > fabs(player_bullet_map[x_pb][y_pb][0] - enemy_x) && 20 > fabs(player_bullet_map[x_pb][y_pb][1] - enemy_y))
					{
						enemy_life -= 1;
					}
					setfillcolor(RGB(71, 181, 99));
					setlinecolor(RGB(71, 181, 99));
					//fillrectangle(player_bullet_map[x_pb][y_pb][0] - 5, player_bullet_map[x_pb][y_pb][1] - 3, player_bullet_map[x_pb][y_pb][0] + 5, player_bullet_map[x_pb][y_pb][1] + 3);
					fillcircle(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 2);
					//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[1], 81, 144, NOTSRCERASE);
					//putimage(player_bullet_map[x_pb][y_pb][0], player_bullet_map[x_pb][y_pb][1], 18, 15, &img_player[0], 81, 144, SRCINVERT);
				}
			}
		}
		if (enemy_life_number == 1)
		{
			mciSendString(TEXT("close all"), NULL, 0, NULL);
			mciSendString(TEXT(" open music\\seve.wav alias C4"), NULL, 0, NULL);
			mciSendString(TEXT("play  C4"), NULL, 0, NULL);
			Sleep(2300);
			settextcolor(BLACK);
			settextstyle(30, 0, _T("楷体"));

			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
			dialogue(1); outtextxy(30, 500, _T("可乐：（沉默）")); FlushBatchDraw(); get();
			dialogue(1); outtextxy(30, 500, _T("可乐：你是食梦者？")); FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500, _T("：我不是")); 			FlushBatchDraw(); get();
			dialogue(1); outtextxy(30, 500, _T("可乐：你是！")); 		FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500,_T("：你太焦虑了孩子")); 	FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500, _T("：这里只有一个世界"));	FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500, _T("：这个世界从来就没有食梦者，只有我们")); 			FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500, _T("：你把几乎所有人都看成了敌人，这是不对的")); 		FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500, _T("：你需要一些帮助（治疗ε=ε=ε=(~￣▽￣)~）")); 	FlushBatchDraw(); get();
			dialogue(1); outtextxy(30, 500, _T("可乐：或许吧")); 					FlushBatchDraw(); get();
			dialogue(1); outtextxy(30, 500, _T("可乐：但你要怎么帮？")); 			FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500, _T("：你要快乐，要信任他人"));			FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500, _T("：这个世界上不仅只有你的敌人")); 	FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500, _T("：还有你的朋友，你的家人")); 		FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500, _T("：你应该放松")); 					FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500, _T("：因为在这个世界上，你真正的敌人")); FlushBatchDraw(); get();
			dialogue(3); outtextxy(30, 500, _T("：是你自己")); 						FlushBatchDraw(); get();
			int i = 1;
			while (i) 
			{
				putimage(0, 0, &img_bg[10]);
				FlushBatchDraw();
				MOUSEMSG m;			// 定义鼠标消息
				m = GetMouseMsg();	//这个函数用于获得鼠标消息
				if (m.mkLButton)
				{
					// 鼠标按键
					if (m.x >= 400 && m.x < 500 && m.y >= 300 && m.y <= 362)
					{
						Sleep(200);
						gameover();
					}
					if (m.x >= 636 && m.x < 736 && m.y >= 300 && m.y <= 362)
					{
						Sleep(200);
						startup();
						i = 0;
						layer = 0;
					}
				}
			}

		}
		enemy_HP();//敌机血量条
		player_HP();//我机血量
		FlushBatchDraw();
		Sleep(1);
	}
	if (layer == 8)
	{
	}
	if (layer == 9)
	{

	}
}

//敌方血量计算
void enemy_HP()//敌方血量计算
{
	putimage(0, 6, 800, 33, &img_png[1], 0, 6, NOTSRCERASE);
	putimage(0, 6, 800, 33, &img_png[0], 0, 6, SRCINVERT);
	TCHAR s[5];//用于输出剩余生命
	settextcolor(BLACK);
	settextstyle(20, 0, _T("宋体"));
	_stprintf_s(s, _T("HP%d"), enemy_life_number);
	outtextxy(5, 9, s);
	if (enemy_life_number > 0)
	{
		if (enemy_life >= enemy_life_length / 2)
		{
			setlinecolor(RED);
			setfillcolor(RED);
			fillroundrect(114, 11, 114 + 680 * enemy_life / enemy_life_length, 33, 0, 30);
		}
		if (enemy_life <= enemy_life_length / 2 && enemy_life >= enemy_life_length / 4)
		{
			setlinecolor(BLUE);
			setfillcolor(BLUE);
			fillroundrect(114, 11, 114 + 680 * enemy_life / enemy_life_length, 33, 0, 30);
		}
		if (enemy_life <= enemy_life_length / 4 && enemy_life > 0)
		{
			setlinecolor(GREEN);
			setfillcolor(GREEN);
			fillroundrect(114, 11, 114 + 680 * enemy_life / enemy_life_length, 33, 0, 30);
		}
		if (enemy_life <= 0)
		{
			enemy_life_number -= 1;
			enemy_life = enemy_life_length;
		}
	}
	if (enemy_life_number == 0)
	{
		int i = 1, layer_1 = layer;
		mciSendString(TEXT("close all"), NULL, 0, NULL);
		mciSendString(TEXT(" open music\\Takemehand.wav alias C3"), NULL, 0, NULL);
		mciSendString(TEXT("play  C3"), NULL, 0, NULL);
		while (i)
		{
			MOUSEMSG m;			// 定义鼠标消息
			m = GetMouseMsg();	//这个函数用于获得鼠标消息
			//if (layer_1 != 7)
			//{
				putimage(0, 0, &img_bg[9]);
				FlushBatchDraw();
				if (m.mkLButton)
				{
					// 鼠标按键
					if (m.x >= 400 && m.x < 500 && m.y >= 300 && m.y <= 362)
					{
						Sleep(200);
						gameover();
					}
					if (m.x >= 521 && m.x < 621 && m.y >= 300 && m.y <= 362)
					{
						Sleep(200);
						startup();
						i = 0;
						layer = 0;
					}
					if (m.x >= 636 && m.x < 736 && m.y >= 300 && m.y <= 362)
					{
						Sleep(200);
						startup();
						i = 0;
						layer = layer_1 + 1;
					}
				}
			//}
			//else
			//{
			//	putimage(0, 0, &img_bg[10]);
			//	FlushBatchDraw();
			//	if (m.mkLButton)
			//	{
			//		// 鼠标按键
			//		if (m.x >= 400 && m.x < 500 && m.y >= 300 && m.y <= 362)
			//		{
			//			Sleep(200);
			//			gameover();
			//		}
			//		if (m.x >= 636 && m.x < 736 && m.y >= 300 && m.y <= 362)
			//		{
			//			Sleep(200);
			//			startup();
			//			i = 0;
			//			layer = 0;
			//		}
			//	}
			//}
		}
	}
}

//玩家生命
void player_HP()//玩家生命
{
	putimage(0, 523, 244, 72, &img_png[1], 166, 523, NOTSRCERASE);
	putimage(0, 523, 244, 72, &img_png[0], 166, 523, SRCINVERT);
	if (player_life > 0)
	{
		TCHAR s[5];//用于输出剩余生命
		settextcolor(BLACK);
		settextstyle(40, 0, _T("宋体"));
		_stprintf_s(s, _T("%d"), player_life );
		outtextxy(79, 537, s);
	}
	if(player_life == 0)
	{
		int i = 1,layer_1=layer;
		while (i)
		{
			putimage(0, 0, &img_bg[8]);
			FlushBatchDraw();
			MOUSEMSG m;			// 定义鼠标消息
			m = GetMouseMsg();	//这个函数用于获得鼠标消息
			if (m.mkLButton)
			{
				// 鼠标按键
				if (m.x >= 27 && m.x < 128 && m.y >= 333 && m.y <= 394)
				{
					Sleep(200);
					gameover();
				}
				if (m.x >= 158 && m.x < 258 && m.y >= 333 && m.y <= 394)
				{
					Sleep(200);
					startup();
					i = 0;
					layer = 0;
				}
				if (m.x >= 287 && m.x < 388 && m.y >= 333 && m.y <= 394)
				{
					Sleep(200);
					startup();
					i = 0;
					layer = layer_1;
				}
			}
		}
	}
}

//敌方子弹一，固定波
void e_bullet_1(int enemy_x_1,int enemy_y_1)//敌方子弹一计算
{
		int bullet_density = 60;//子弹密度
		for (int n = 0; n < bullet_layer; n++)//计算子弹的层
		{
			for (; bullet_1_map[n][2]< 700; )//计算每层子弹的移动
			{
				double a;
				for (int i = 0; i <bullet_density; i++)//计算一层子弹的弧度
				{
					//a_rand += bullet_density / (bullet_density - 1);
					a = i * 2 * PI / bullet_density;	//子弹弧度
					if (bullet_1_map[n][3]!=1)							//a >= a_rand * 2 * PI / bullet_density && a <= (a_rand + bullet_density / 8) * 2 * PI / bullet_density
					{
						bullet_1_map[n][0] = enemy_x_1 ;
						bullet_1_map[n][1] = enemy_y_1 ;
						bullet_1_map[n][3] = 1;
					}
					else//判断子弹不依赖敌机坐标
					{
						bullet_1_map[n][0] = enemy_x_1 + (int)(bullet_1_map[n][2] * cos(a));
						bullet_1_map[n][1] = enemy_y_1 - (int)(bullet_1_map[n][2] * sin(a));
						/*bullet_1_map[n][0] = bullet_1_map[n][0] + (int)(bullet_1_map[n][2] * cos(a));
						bullet_1_map[n][1] = bullet_1_map[n][0] - (int)(bullet_1_map[n][2] * sin(a));*/
						
					}
					if (bullet_1_map[n][0]>0 && bullet_1_map[n][0]<Width&&bullet_1_map[n][1]>50 && bullet_1_map[n][1]<High)//判断子弹在地图内
						map[bullet_1_map[n][0]][bullet_1_map[n][1]] = 1;
				}
				break;
			}
			bullet_1_map[n][2] += 1;//子弹移动的加速度
		}
		if (bullet_layer <= 100)//子弹层数计算增加
		{
			if (time_i > 60)
			{
				bullet_layer++;
				time_i = 0;
			}
			else time_i++;
		}
		else
		{
			for (int n = 0; n < bullet_layer; n++)//恢复状态
			{
				bullet_1_map[n][2] = 0;
				bullet_1_map[n][3] = 0;
			}	
			bullet_layer = 1;
		}
	}

//敌方子弹二，随机弹
void e_bullet_2(int enemy_x_1, int enemy_y_1)//敌机子弹二随机弹
{
	int i;
	double angle;
	//for (int n = 0; n < 3; n++)
	
		for (i = 0; i<150; i++)// 初始化点的初始坐标
		{

			if (bullet_e2_map[i][2] == 0)
			{
				//srand((rand()%100));
				bullet_e2_map[i][3] = rand() % 1800+rand()%200;
				bullet_e2_map[i][0] = enemy_x_1-10+rand() % 20;
				bullet_e2_map[i][1] = enemy_y_1+rand()% 20;
				bullet_e2_map[i][2] = 1;
			}
			else
			{
				//angle = ((bullet_e2_map[i][3] - rand() % 200) * 2 * PI )/ 3600;
				angle = ((bullet_e2_map[i][3]) * 2 * PI) / 3600;
				bullet_e2_map[i][0] += (int)(4*cos(angle));
				bullet_e2_map[i][1] += (int)(4*sin(angle));
			}
			if (bullet_e2_map[i][0]<0 || bullet_e2_map[i][0]>800 || bullet_e2_map[i][1]<0|| bullet_e2_map[i][1] > 600)
			{
				bullet_e2_map[i][2] = 0;
				bullet_e2_map[i][3] = 0;
			}
		}
	//for(int n=0;n<200;n++)//随机星点
	//{
	//	int x, y, c;
	//	x = rand() % 640+ enemy_x_1;
	//	y = rand() % 480+ enemy_y_1;
	//	c = RGB(rand() % 256, rand() % 256, rand() % 256);
	//	setlinecolor(c);//设置子弹颜色
	//	setfillcolor(c);
	//	fillcircle(enemy_x, enemy_y, 20);
	//	//putpixel(x, y, c);
	//}
}

//敌方子弹三，重弹
void e_bullet_3(int enemy_x_1, int enemy_y_1)
{
	int i;
	//double angle;
	//for (int n = 0; n < 3; n++)
	for (i = 0; i<e_bullet_3_number; i++)
	{
		if (bullet_e3_map[i][2] == 0)// 初始化点的初始坐标
		{
			//srand((rand()%100));
			bullet_e3_map[i][3] = 200;
			bullet_e3_map[i][0] = 20 + rand() % 780;
			bullet_e3_map[i][1] = 0;
			bullet_e3_map[i][2] = 1;
		}
		else
		{
			//angle = ((bullet_e3_map[i][3] - rand() % 200) * 2 * PI) / 3600;
			//bullet_e3_map[i][0] += (int)(4 * cos(angle));
			bullet_e3_map[i][1] += 3;
		}
		if (bullet_e3_map[i][0]<0 || bullet_e3_map[i][0]>800 || bullet_e3_map[i][1]<0 || bullet_e3_map[i][1] > 600)
		{
			bullet_e3_map[i][2] = 0;
			bullet_e3_map[i][3] = 0;
		}
	}
}

//敌方子弹四，以敌机为中点四周重弹
void e_bullet_4(int enemy_x_1, int enemy_y_1)
{
	int i;
	double angle;
	for (i = 0; i<e_bullet_4_number; i++)
	{
		if (bullet_e4_map[i][2] == 0)// 初始化点的初始坐标
		{
			bullet_e4_map[i][4] = 1;
			bullet_e4_map[i][3] = 300+rand()%1200;
			bullet_e4_map[i][0] = enemy_x_1;
			bullet_e4_map[i][1] = enemy_y_1;
			bullet_e4_map[i][2] = 1;
		}
		else
		{
			bullet_e4_map[i][4] += 1;
			angle = ((bullet_e4_map[i][3]) * 2 * PI) /3600;
			bullet_e4_map[i][0] += (int)(2 * cos(angle));
			bullet_e4_map[i][1] += (int)(3 * sin(angle));
			//bullet_e4_map[i][1] += 4;
		}
		if (bullet_e4_map[i][0]<0 || bullet_e4_map[i][0]>800 || bullet_e4_map[i][1]<0 || bullet_e4_map[i][1] > 600)
		{
			bullet_e4_map[i][2] = 0;
			bullet_e4_map[i][3] = 0;
			bullet_e4_map[i][4] = 0;
		}
	}
}

//敌方子弹五，激光
void e_bullet_5(int enemy_x_1, int enemy_y_1)
{
	int i;
	//double angle;
	for (i = 0; i<1; i++)
	{
		if (bullet_e5_map[i][2] == 0)// 初始化点的初始坐标
		{
			bullet_e5_map[i][4] = 1;
			bullet_e5_map[i][0] = enemy_x_1;
			bullet_e5_map[i][1] = enemy_y_1;
			bullet_e5_map[i][2] = 1;
		}
		else
			if (bullet_e5_map[i][4] <= 2)
			{
				bullet_e5_map[i][4] += 1;
				bullet_e5_map[i][1] += bullet_e5_map[i][4];
			}
			else
			{
				bullet_e5_map[i][1] += bullet_e5_map[i][4];
			}
		if (bullet_e5_map[i][1] > 800 && bullet_e5_map[i][1] < 2000)
		{
			bullet_e5_map[i][2] = 2;
		}
		if (bullet_e5_map[i][0]<0 || bullet_e5_map[i][0]>800 || bullet_e5_map[i][1]<0 || bullet_e5_map[i][1] > 2000)
		{
			bullet_e5_map[i][2] = 0;
			bullet_e5_map[i][3] = 0;
			bullet_e4_map[i][4] = 0;
		}
	}
}

//我方子弹一,普通子弹
void player_bullet_1()
{	
	if (1)
	{
		for (int i = 0; i<player_bullet_layer; i++)
		{
			for (int j = 0; j < player_bullet_layer_number; j++)
			{
				if (player_bullet_map[i][j][6] == 0)
				{
					player_bullet_map[i][j][2] = 6;
					player_bullet_map[i][j][0] = player_x +7- (int)(player_bullet_layer * 15 / 2) + 15 * i;
					player_bullet_map[i][j][1] = player_y -5* j - player_bullet_map[i][j][2];

					player_bullet_map[i][j][6] = 1;

					shoot = 2;
				}
				else
				{
					if (player_bullet_map[i][j][0] >0 && player_bullet_map[i][j][0] < Width&&player_bullet_map[i][j][1] >0 && player_bullet_map[i][j][1] < High)	//判断子弹在地图内
					{
						player_bullet_map[i][j][0] = player_bullet_map[i][j][0];
						player_bullet_map[i][j][1] = player_bullet_map[i][j][1] - player_bullet_map[i][j][2];

						//setlinecolor(RGB(71, 181, 99));
						//setfillcolor(RGB(71, 181, 99));
						//fillrectangle(player_bullet_map[i][j][0] - 5, player_bullet_map[i][j][1] - 5, player_bullet_map[i][j][0] + 5, player_bullet_map[i][j][1] + 5);
					}
					else
						if (player_bullet_map[i][j][6] == 1)
						{

							player_bullet_map[i][j][0] = 0;
							player_bullet_map[i][j][1] = 0;
							player_bullet_map[i][j][2] = 0;
							player_bullet_map[i][j][6] = 0;

						}
				}
			}
		}
	}
	
	if (time_i_1 > 60)
	{
		time_i_1 = 0;
	}
	else time_i_1++;

}

//我方子弹二，弹力弹
void player_bullet_2()
{
	// 模拟阻力的效果
	p_b2_vx *= 0.99f;
	p_b2_vy *= 0.99f;

	// ds = dv * dt
	p_b2_x += p_b2_vx;
	p_b2_y += p_b2_vy;

	setlinecolor(RED);
	setfillcolor(RED);
	fillcircle((int)p_b2_x, (int)p_b2_y, 30); // 画球
	line((int)p_b2_x, (int)p_b2_y, player_x, player_y); // 画线
}

//与用户无关的更新
void updateWithoutInput()	// 与用户输入无关的更新
{
	if (layer == 6)
	{
		enemy_y += enemy_y_a;
		enemy_x += enemy_x_a;
		//if (enemy_y > 560 || enemy_y<50)
		//{
		//	enemy_y_a = -enemy_y_a;
		//}
		if (enemy_x < 200 || enemy_x >700)
		{
			enemy_x_a = -enemy_x_a;
		}
	}
	if (layer == 7)
	{
		if (enemy_life_number >= 1 && enemy_life_number <= 3)
		{
			enemy_y += enemy_y_a;
			enemy_x += enemy_x_a;
			//if (enemy_y > 560 || enemy_y<50)
			//{
			//	enemy_y_a = -enemy_y_a;
			//}
			if (enemy_x < 200 || enemy_x >700)
			{
				enemy_x_a = -enemy_x_a;
			}
		}
	}
}

//与用户输入有关的更新
void updateWithInput()	// 与用户输入有关的更新
{
	if (layer == 0)
	{
		MOUSEMSG m;			// 定义鼠标消息
		while (MouseHit())  //这个函数用于检测当前是否有鼠标消息
		{
			m = GetMouseMsg();
			if (m.mkLButton)
			{
				// 鼠标按键
				if (m.x >= 94 && m.x < 359 && m.y >= 165 && m.y <= 359)
				{
					mciSendString(TEXT(" open music\\瓷器.wav alias C"), NULL, 0, NULL);
					mciSendString(TEXT("play  C"), NULL, 0, NULL);
					Sleep(200);
					layer = 3;
				}
				if (m.x >= 94 && m.x < 359 && m.y >= 260 && m.y <= 312)
				{
					Sleep(200);
					layer = 2;
				}
				if (m.x >= 94 && m.x < 359 && m.y >= 356 && m.y <= 412)
				{
					Sleep(200);
					mciSendString(TEXT(" open music\\瓷器.wav alias C"), NULL, 0, NULL);
					mciSendString(TEXT("play  C"), NULL, 0, NULL);
					layer = 1;
				}
				if (m.x >= 94 && m.x < 359 && m.y >= 456 && m.y <= 513)
				{
					Sleep(200);
					gameover();
				}
			}
		}
	}
	if (layer == 1)
	{
		MOUSEMSG m;			// 定义鼠标消息
		while (MouseHit())  //这个函数用于检测当前是否有鼠标消息
		{
			m = GetMouseMsg();
			if (m.mkLButton)
			{
				// 返回
				if (m.x >= 20 && m.x < 88 && m.y >= 526 && m.y <= 598)
				{
					Sleep(200);
					layer = 0;
				}
				// 玩家生命
				if (m.x >= 370 && m.x < 402 && m.y >= 148 && m.y <=180)
				{
					player_life_number = 15;
				}
				if (m.x >= 501 && m.x < 535 && m.y >= 148 && m.y <= 180)
				{
					player_life_number = 7;
				}
				if (m.x >= 641 && m.x < 673 && m.y >= 148 && m.y <= 180)
				{
					player_life_number = 3;
				}
				// 敌机生命
				if (m.x >= 370 && m.x < 402 && m.y >= 277 && m.y <= 310)
				{
					enemy_life_number_1 = 10;
				}
				if (m.x >= 501 && m.x < 535 && m.y >= 277 && m.y <= 310)
				{
					enemy_life_number_1 = 5;
				}
				if (m.x >= 641 && m.x < 673 && m.y >= 277 && m.y <= 310)
				{
					enemy_life_number_1 = 1;
				}
				//无敌状态
				if (m.x >= 370 && m.x < 402 && m.y >= 406 && m.y <= 440)
				{
					invincible_mode = 1;
				}
				if (m.x >= 501 && m.x < 535 && m.y >= 406 && m.y <= 440)
				{
					invincible_mode = 0;
				}
			}
		}
	}
	if (layer == 2)
	{
		MOUSEMSG m;			// 定义鼠标消息
		while (MouseHit())  //这个函数用于检测当前是否有鼠标消息
		{
			m = GetMouseMsg();
			if (m.mkLButton)
			{
				// 返回
				if (m.x >= 20 && m.x < 88 && m.y >= 526 && m.y <= 598)
				{
					Sleep(200);
					layer = 0;
				}
			}
		}
	}
	if (layer == 3)
	{
		MOUSEMSG m;			// 定义鼠标消息
		while (MouseHit())  //这个函数用于检测当前是否有鼠标消息
		{
			m = GetMouseMsg();
			if (m.mkLButton)
			{
				// 鼠标按键
				if (m.x >= 94 && m.x < 359 && m.y >= 77 && m.y <= 121)
				{
					startup();
					layer = 4;
				}
				if (m.x >= 94 && m.x < 359 && m.y >= 165 && m.y <= 359)
				{
					startup();
					layer = 5;
				}
				if (m.x >= 94 && m.x < 359 && m.y >= 260 && m.y <= 312)
				{
					startup();
					layer = 6;
				}
				if (m.x >= 94 && m.x < 359 && m.y >= 356 && m.y <= 412)
				{
					startup();
					layer = 7;
				}
				if (m.x >= 94 && m.x < 359 && m.y >= 456 && m.y <= 513)
				{
					Sleep(200);
					layer = 0;
				}
			}
		}
	}
	if (layer == 4)
	{
		int  player_x_a = 0, player_y_a = 0;
		int a = 3, b = 2;
		if ((GetAsyncKeyState(0x41) & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000))//a
		{
			if (player_x>10)
				player_x -= a; player_x_a = -b;
		}
		if ((GetAsyncKeyState(0x44) & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000))//d
		{
			if (player_x<790)
				player_x += a; player_x_a = b;
		}
		if ((GetAsyncKeyState(0x57) & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000))//w
		{
			if (player_y>70)
				player_y -= a; player_y_a = -b;
		}
		if ((GetAsyncKeyState(0x53) & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000))//s
		{
			if (player_y<600)
				player_y += a; player_y_a = b;
		}
		/*player_x += player_x_a;
		player_y += player_y_a;*/
		player_x_a = 0;
		player_y_a = 0;
		MOUSEMSG m;			// 定义鼠标消息
		while (MouseHit())  //这个函数用于检测当前是否有鼠标消息
		{
			m = GetMouseMsg();
			if (m.mkLButton)
			{
				// 鼠标按键
				if (m.x >= 727 && m.x < 792 && m.y >= 526 && m.y <= 595)
				{
					Sleep(200);
					stop();
				}
			}
		}
	}
	if (layer == 5)
	{
		MOUSEMSG m;			// 定义鼠标消息
		if (MouseHit())  //这个函数用于检测当前是否有鼠标消息
		{
			m = GetMouseMsg();
			player_x = m.x;
			player_y = m.y;
			if (m.mkLButton)
			{
				// 鼠标按键
				if (m.x >= 727 && m.x < 792 && m.y >= 526 && m.y <= 595)
				{
					Sleep(200);
					stop();
				}
			}
		}
		//if (MouseHit())
		//{
		//	MOUSEMSG m = GetMouseMsg();
		//	 弹力公式 F = k * x
		//	player_x = m.x;
		//	player_y = m.y;
		//	float fx = m.x - p_b2_x;
		//	float fy = m.y - p_b2_y + 100; // 加上重力	 
		//	p_b2_vx += (fx / 100);// 速度时间公式 Vt = V0 + a * t  // a = F / M
		//	p_b2_vy += (fy / 100);
		//}
		//else
		//{
		//	float fx = player_x - p_b2_x;
		//	float fy = player_y - p_b2_y + 100; // 加上重力	 
		//	p_b2_vx += (fx / 100);// 速度时间公式 Vt = V0 + a * t  // a = F / M
		//	p_b2_vy += (fy / 100);
		//}
	}
	if (layer == 6)
	{
		int  player_x_a = 0, player_y_a = 0;
		int a = 3, b = 2;
		if ((GetAsyncKeyState(0x41) & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000))//a
		{
if (player_x > 10)
player_x -= a; player_x_a = -b;
		}
		if ((GetAsyncKeyState(0x44) & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000))//d
		{
			if (player_x < 790)
				player_x += a; player_x_a = b;
		}
		if ((GetAsyncKeyState(0x57) & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000))//w
		{
			if (player_y > 70)
				player_y -= a; player_y_a = -b;
		}
		if ((GetAsyncKeyState(0x53) & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000))//s
		{
			if (player_y < 600)
				player_y += a; player_y_a = b;
		}
		/*player_x += player_x_a;
		player_y += player_y_a;*/
		player_x_a = 0;
		player_y_a = 0;

		//if ((GetAsyncKeyState(0x4a) & 0x8000))//j键发射
		//{
		//	shoot = 1;
		//}
		MOUSEMSG m;			// 定义鼠标消息
		while (MouseHit())  //这个函数用于检测当前是否有鼠标消息
		{
			m = GetMouseMsg();
			if (m.mkLButton)
			{
				// 鼠标按键
				if (m.x >= 727 && m.x < 792 && m.y >= 526 && m.y <= 595)
				{
					Sleep(200);
					stop();
				}
			}
		}
	}
	if (layer == 7)
	{
		//if (enemy_life_number > 4||enemy_life_number<=3)
		//{
			int  player_x_a = 0, player_y_a = 0;
			int a = 3, b = 2;
			if ((GetAsyncKeyState(0x41) & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000))//a
			{
				if (player_x > 10)
					player_x -= a; player_x_a = -b;
			}
			if ((GetAsyncKeyState(0x44) & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000))//d
			{
				if (player_x < 790)
					player_x += a; player_x_a = b;
			}
			if ((GetAsyncKeyState(0x57) & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000))//w
			{
				if (player_y > 70)
					player_y -= a; player_y_a = -b;
			}
			if ((GetAsyncKeyState(0x53) & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000))//s
			{
				if (player_y < 600)
					player_y += a; player_y_a = b;
			}
			/*player_x += player_x_a;
			player_y += player_y_a;*/
			player_x_a = 0;
			player_y_a = 0;
			MOUSEMSG m;			// 定义鼠标消息
			while (MouseHit())  //这个函数用于检测当前是否有鼠标消息
			{
				m = GetMouseMsg();
				if (m.mkLButton)
				{
					// 鼠标按键
					if (m.x >= 727 && m.x < 792 && m.y >= 526 && m.y <= 595)
					{
						Sleep(200);
						stop();
					}
				}
			}
		//}
		//if (enemy_life_number == 4)
		//{
		//	MOUSEMSG m;			// 定义鼠标消息
		//	if (MouseHit())  //这个函数用于检测当前是否有鼠标消息
		//	{
		//		m = GetMouseMsg();
		//		player_x = m.x;
		//		player_y = m.y;
		//		if (m.mkLButton)
		//		{
		//			// 鼠标按键
		//			if (m.x >= 727 && m.x < 792 && m.y >= 526 && m.y <= 595)
		//			{
		//				Sleep(200);
		//				stop();
		//			}
		//		}
		//	}
		//}
	}
	if (layer == 8)
	{

	}
	if (layer == 9)
	{

	}
}

//无敌模式
void invincible()//无敌模式
{

	if (invincible_mode == 1)
	{
		if ((GetAsyncKeyState(0x4A) & 0x8000))//L
		{
			invincible_kb[2] = 1;
			enemy_life_number -= 1;
			Sleep(100);
			setlinecolor(RED);//光圈
			setfillcolor(RED);
			//putimage(bullet_e5_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[1], 62, 144, NOTSRCERASE);
			//putimage(bullet_e5_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[0], 62, 144, SRCINVERT);
			circle(player_x, player_y, 35);
			circle(player_x, player_y, 37);
			circle(player_x, player_y, 39);
			circle(player_x, player_y, 40);
			circle(player_x, player_y, 43);
			circle(player_x, player_y, 45);
			setlinecolor(BLUE);//光圈
			setfillcolor(BLUE);
			//putimage(bullet_e5_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[1], 62, 144, NOTSRCERASE);
			//putimage(bullet_e5_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[0], 62, 144, SRCINVERT);
			circle(player_x, player_y, 35);
			circle(player_x, player_y, 37);

			putimage(player_x - 50, player_y - 50, 100, 100, &img_player[3], 0, 0, NOTSRCERASE);//角色图
			putimage(player_x - 50, player_y - 50, 100, 100, &img_player[2], 0, 0, SRCINVERT);
			setlinecolor(WHITE);//玩家判定点绘制
			setfillcolor(WHITE);
			fillcircle(player_x, player_y, 2);
		}
		else
		{
			invincible_kb[2] = 0;
		}
		fillcircle(player_x, player_y, 2);
		if ((GetAsyncKeyState(0x4B) & 0x8000) )//J
		{
			setlinecolor(RED);//光圈
			setfillcolor(RED);
			//putimage(bullet_e5_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[1], 62, 144, NOTSRCERASE);
			//putimage(bullet_e5_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[0], 62, 144, SRCINVERT);
			circle(player_x, player_y, 30);
			circle(player_x, player_y, 32);
			circle(player_x, player_y, 35);
			circle(player_x, player_y, 37);
			circle(player_x, player_y, 39);
			circle(player_x, player_y, 40);
			circle(player_x, player_y, 43);
			circle(player_x, player_y, 45);
			invincible_kb[0] = 1;
			putimage(player_x - 50, player_y - 50, 100, 100, &img_player[3], 0, 0, NOTSRCERASE);//角色图
			putimage(player_x - 50, player_y - 50, 100, 100, &img_player[2], 0, 0, SRCINVERT);
			setlinecolor(WHITE);//玩家判定点绘制
			setfillcolor(WHITE);
			fillcircle(player_x, player_y, 2);

		}
		else 
		{
			invincible_kb[0] = 0;
		}
		if ((GetAsyncKeyState(0x4C) & 0x8000))//K
		{
			invincible_kb[1] = 1;
			setlinecolor(BLUE);//光圈
			setfillcolor(BLUE);
			//putimage(bullet_e5_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[1], 62, 144, NOTSRCERASE);
			//putimage(bullet_e5_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[0], 62, 144, SRCINVERT);
			circle(player_x, player_y, 30);
			circle(player_x, player_y, 32);
			circle(player_x, player_y, 35);
			circle(player_x, player_y, 37);
			putimage(player_x - 50, player_y - 50, 100, 100, &img_player[3], 0, 0, NOTSRCERASE);//角色图
			putimage(player_x - 50, player_y - 50, 100, 100, &img_player[2], 0, 0, SRCINVERT);
			setlinecolor(WHITE);//玩家判定点绘制
			setfillcolor(WHITE);
			fillcircle(player_x, player_y, 2);
		}
		else
		{
			invincible_kb[1] = 0;
		}
		if (invincible_kb[0] == 0 && invincible_kb[1] == 0&& invincible_kb[2]==0)
		{
			putimage(player_x - 50, player_y - 50, 100, 100, &img_player[1], 0, 0, NOTSRCERASE);//角色图
			putimage(player_x - 50, player_y - 50, 100, 100, &img_player[0], 0, 0, SRCINVERT);
			setlinecolor(WHITE);//玩家判定点绘制
			setfillcolor(WHITE);
			fillcircle(player_x, player_y, 2);
		}
	}
	else
	{
		if ((GetAsyncKeyState(0x4A) & 0x8000))//L
		{
			invincible_kb[2] = 1;
			enemy_life -= 1;
			setlinecolor(BLUE);//光圈
			setfillcolor(BLUE);
			//putimage(bullet_e5_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[1], 62, 144, NOTSRCERASE);
			//putimage(bullet_e5_map[x1][0], bullet_e2_map[x1][1], 18, 15, &img_player[0], 62, 144, SRCINVERT);
			circle(player_x, player_y, 35);
			circle(player_x, player_y, 37);
		}
		else
		{
			invincible_kb[2] = 0;
		}
		putimage(player_x - 50, player_y - 50, 100, 100, &img_player[1], 0, 0, NOTSRCERASE);//角色图
		putimage(player_x - 50, player_y - 50, 100, 100, &img_player[0], 0, 0, SRCINVERT);
		setlinecolor(WHITE);//玩家判定点绘制
		setfillcolor(WHITE);
		fillcircle(player_x, player_y, 2);
	}
	if (player_life == 0)
	{
		int i = 1, layer_1 = layer;
		while (i)
		{
			putimage(0, 0, &img_bg[8]);
			FlushBatchDraw();
			MOUSEMSG m;			// 定义鼠标消息
			m = GetMouseMsg();	//这个函数用于获得鼠标消息
			if (m.mkLButton)
			{
				// 鼠标按键
				if (m.x >= 27 && m.x < 128 && m.y >= 333 && m.y <= 394)
				{
					Sleep(200);
					gameover();
				}
				if (m.x >= 158 && m.x < 258 && m.y >= 333 && m.y <= 394)
				{
					Sleep(200);
					startup();
					i = 0;
					layer = 0;
				}
				if (m.x >= 287 && m.x < 388 && m.y >= 333 && m.y <= 394)
				{
					Sleep(200);
					startup();
					i = 0;
					layer = layer_1;
				}
			}
		}
	}
}

//游戏剧情
void story()//游戏剧情
{
	mciSendString(TEXT("close all"), NULL, 0, NULL);//关闭先前音乐
	if (layer == 4)
	{
		LOGFONT f;
		gettextstyle(&f);						 // 获取当前字体设置
		setbkmode(TRANSPARENT);					// 设置文字背景为透明色
		f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
		settextstyle(&f);						 // 设置字体样式
		cleardevice();//章节
		settextcolor(WHITE);
		settextstyle(64, 0, _T("宋体"));
		Sleep(1000);
		outtextxy(100, 260, _T("--第一章★启程--"));	FlushBatchDraw(); Sleep(2500);

		mciSendString(TEXT("close all"), NULL, 0, NULL);//音乐
		mciSendString(TEXT(" open music\\拔剑声.wav alias C"), NULL, 0, NULL);
		mciSendString(TEXT("play  C"), NULL, 0, NULL);
		Sleep(2000);
		cleardevice();//剧情
		settextcolor(BLACK);
		settextstyle(30, 0, _T("楷体"));
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		dialogue(2);outtextxy(30, 500, _T("：你是谁？")); 			FlushBatchDraw(); get(); 
		dialogue(1);outtextxy(30, 500, _T("：我是可乐")); 			FlushBatchDraw(); get();  
		dialogue(1);outtextxy(30, 500, _T("可乐：我在找一个东西")); FlushBatchDraw(); get(); 
		dialogue(2);outtextxy(30, 500, _T("：什么东西？ ")); 		FlushBatchDraw(); get(); 
		dialogue(1);outtextxy(30, 500, _T("可乐：不能告诉你 ")); 	FlushBatchDraw();  get(); 
		dialogue(2);outtextxy(30, 500, _T("：小心后面！！！ "));	FlushBatchDraw(); get();

		mciSendString(TEXT("close all"), NULL, 0, NULL);
		mciSendString(TEXT(" open music\\死机之歌.wav alias C1"), NULL, 0, NULL);
		mciSendString(TEXT("play  C1"), NULL, 0, NULL);
	}
	if (layer == 5)
	{
		LOGFONT f;
		gettextstyle(&f);						 // 获取当前字体设置
		setbkmode(TRANSPARENT);					// 设置文字背景为透明色
		f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
		settextstyle(&f);						 // 设置字体样式

		cleardevice();//章节
		settextcolor(WHITE);
		settextstyle(64, 0, _T("宋体"));
		Sleep(1000);
		outtextxy(100, 260, _T("--第二章★前进！！--"));	FlushBatchDraw(); Sleep(2500);

		mciSendString(TEXT("close all"), NULL, 0, NULL);//音乐
		mciSendString(TEXT(" open music\\诡异笛声.wav alias C"), NULL, 0, NULL);
		mciSendString(TEXT("play  C"), NULL, 0, NULL);
		Sleep(2000);
		cleardevice();//剧情
		settextcolor(BLACK);
		settextstyle(30, 0, _T("楷体"));
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		dialogue(2); outtextxy(30, 500, _T("：(收刀)呼.....")); 	FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：这些人是谁")); 		FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：你别管")); 		FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：我都被你牵连了！")); FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：沉默 ")); 		FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：他们是来阻止我的 ")); FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：阻止你？")); 		FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：组织我找到那个东西")); FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：该死")); 		FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：他们又来了")); 	FlushBatchDraw(); get();

		mciSendString(TEXT("close all"), NULL, 0, NULL);
		mciSendString(TEXT(" open music\\NothingToFear.wav alias C2"), NULL, 0, NULL);
		mciSendString(TEXT("play  C2"), NULL, 0, NULL);
	}
	if (layer == 6)
	{
		LOGFONT f;
		gettextstyle(&f);						 // 获取当前字体设置
		setbkmode(TRANSPARENT);					// 设置文字背景为透明色
		f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
		settextstyle(&f);						 // 设置字体样式

		cleardevice();//章节
		settextcolor(WHITE);
		settextstyle(50, 0, _T("宋体"));
		Sleep(1000);
		outtextxy(100, 260, _T("--第三章★混沌--"));	FlushBatchDraw(); Sleep(2500);

		mciSendString(TEXT("close all"), NULL, 0, NULL);
		mciSendString(TEXT(" open music\\诡异笛声.wav alias C3"), NULL, 0, NULL);
		mciSendString(TEXT("play  C3"), NULL, 0, NULL);
		Sleep(2000);
		cleardevice();//剧情
		settextcolor(BLACK);
		settextstyle(30, 0, _T("楷体"));
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		dialogue(2); outtextxy(30, 500, _T("：我们不能停下来")); 	FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：我跑不动了")); 	FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：你别管我了")); 	FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：快走！")); 		FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：沉默 ")); 			FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：我想走也走不了了 ")); FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：那里有亮光")); 	FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：估计另有出路")); 	FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：没有出路了")); 	FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：这是宿命")); 	FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：去你*的宿命")); 		FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：赶紧给我走")); 		FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：我哪都不去了"));	FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：行")); 				FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：啊（被那人背上）")); FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("。。。。")); 			FlushBatchDraw(); get();
		mciSendString(TEXT("close all"), NULL, 0, NULL);
		mciSendString(TEXT(" open music\\Unity.wav alias C3"), NULL, 0, NULL);
		mciSendString(TEXT("play  C3"), NULL, 0, NULL);
	}
	if (layer == 7)
	{
		LOGFONT f;
		gettextstyle(&f);						 // 获取当前字体设置
		setbkmode(TRANSPARENT);					// 设置文字背景为透明色
		f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
		settextstyle(&f);						 // 设置字体样式

		cleardevice();//章节
		settextcolor(WHITE);
		settextstyle(64, 0, _T("宋体"));
		Sleep(1000);
		outtextxy(100, 260, _T("--最终章★最终章--"));		FlushBatchDraw(); Sleep(2000);

		mciSendString(TEXT("close all"), NULL, 0, NULL);//音乐
		mciSendString(TEXT(" open music\\黑暗森林.wav alias C"), NULL, 0, NULL);
		mciSendString(TEXT("play  C"), NULL, 0, NULL);
		Sleep(2000);
		cleardevice();//剧情
		settextcolor(BLACK);
		settextstyle(30, 0, _T("楷体"));
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		dialogue(2); outtextxy(30, 500, _T("：被这些奇怪的东西留在这里")); 		FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：这就是我的结局了吗")); 				FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：本不该是")); 					FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：你该和我好好解释一下外面的东西")); 	FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：他们其实是我的食梦者")); 		FlushBatchDraw(); get();
		dialogue(2); outtextxy(30, 500, _T("：食梦者？ ")); 						FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：你不知道？ ")); 					FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：也是，你是那里来的人")); 		FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：这个世界每个人都在寻找梦想")); 	FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：每天")); 						FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：每一秒")); 						FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：可是有一种东西叫食梦者")); 		FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：被盯上只有一个下场")); 			FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：找到梦想")); 					FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：或者留在这里")); 				FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：听说有一个世界的人")); 			FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：他们会来帮助我们")); 			FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：找到我们的梦想")); 				FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：你或许")); 						FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：就是吧")); 						FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：但你的实力")); 					FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：还远远不够")); 					FlushBatchDraw(); get();
		dialogue(1); outtextxy(30, 500, _T("可乐：来阻止这些人！！")); 			FlushBatchDraw(); get();

		//mciSendString(TEXT("close all"), NULL, 0, NULL);
		//mciSendString(TEXT(" open music\\seve.wav alias C4"), NULL, 0, NULL);
		//mciSendString(TEXT("play  C4"), NULL, 0, NULL);
	}
}

//游戏暂停
void stop()
{
	putimage(185, 181, 434, 327, &img_png[1], 185, 181, NOTSRCERASE);
	putimage(185, 181, 434, 327, &img_png[0], 185, 181, SRCINVERT);
	FlushBatchDraw();
	int i = 1;
	while (i)
	{
		MOUSEMSG m;			// 定义鼠标消息
		m = GetMouseMsg();	//这个函数用于获得鼠标消息
		if (m.mkLButton)
		{
			// 鼠标按键
			if (m.x >= 193 && m.x < 315 && m.y >= 342 && m.y <= 394)
			{
				Sleep(200);
				gameover();
			}
			if (m.x >= 336 && m.x < 457 && m.y >= 342 && m.y <= 394)
			{
				Sleep(200);
				startup();
				i = 0;
				layer = 0;
			}
			if (m.x >= 473 && m.x < 595 && m.y >= 342 && m.y <= 394)
			{
				Sleep(200);
				i = 0;
			}
		}
	}
}

//人物对话
void dialogue(int i)
{
	if (i == 1)
	{
		cleardevice();
		putimage(0, 182, 290, 300, &img_png[3], 0, 182, NOTSRCERASE);//对话图——可乐
		putimage(0, 182, 290, 300, &img_png[2], 0, 182, SRCINVERT);
		putimage(0, 482, 800, 118, &img_png[3], 0, 482, NOTSRCERASE); //对话图对话框
		putimage(0, 482, 800, 228, &img_png[2], 0, 482, SRCINVERT);
	}
	if (i == 2)
	{
		cleardevice();
		putimage(0, 482, 800, 118, &img_png[3], 0, 482, NOTSRCERASE); //对话图对话框
		putimage(0, 482, 800, 228, &img_png[2], 0, 482, SRCINVERT);
	}
	if (i == 3)
	{
		cleardevice();
		putimage(400, 182, 400, 300, &img_png[3], 400, 182, NOTSRCERASE);//对话图——师傅
		putimage(400, 182, 400, 300, &img_png[2], 400, 182, SRCINVERT);
		putimage(0, 482, 800, 118, &img_png[3], 0, 482, NOTSRCERASE); //对话图对话框
		putimage(0, 482, 800, 228, &img_png[2], 0, 482, SRCINVERT);
	}
}

//对话任意键
char get()
{
	int k;
	while (!(k = _getch()));
	return k;
}

//游戏结束
void gameover()	// 游戏结束、后续处理
{
	mciSendString(TEXT("close all"), NULL, 0, NULL);//音乐
	EndBatchDraw();
	closegraph();
	exit(0);
}