#include "pch.h"
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <string>
#include <setjmp.h>
#pragma comment(lib,"Winmm.lib")
#define  ipn 23				//碰撞区数量
#define  mon 9				//小怪物数量
#define  space_height 80	//跳跃高度
#define  qipao 19			//气泡数量
using namespace std;
double z_x = 0;			    //偏移量
int sleep = 70;			//贴图切换间隔
int page = 0;			//贴图切换次数
int ii = 0;


class Player
{
public:
	int player_x, player_y;		//角色坐标
	int player_h;				//角色高度
	int player_r;				//角色宽度
	int player_direction;			//角色方向 1为右 0为左
	int player_imgx ,player_imgy;	//角色行走图坐标
	bool space;						//跳跃状态是否开启
	bool kyt ;						//脚下是否有实体
	bool wall;						//左右是否有实体
	bool skill_j ;					//j技能是否释放
	bool skill_k ;					//k技能是否释放
	int  hhh;;						//跳跃高度	
	Player()
	{
		player_x = 50;
		player_y = 400;
		player_h = 67;
		player_r = 65;
		player_imgx = 0;
		player_imgy = 0;
		player_direction = 1;
		space = false;
		kyt = true;
		wall = false;
		skill_k = false;
		skill_j = false;
		hhh = space_height;
	}
};

int blood = 24;			//角色血量	
bool dead = false;//是否死亡
int back_imgx = 0, back_imgy = 0;		//背景图坐标
char input;
bool zanting = false;		//暂停界面打开与否
bool skill_null = false;	//技能是否放空
int  skill_jx = 0;		    //j技能贴图坐标
int  skill_kx = 0;			//k技能贴图坐标
int  skill_time = 0;		//技能计时

int sum_d = 0;	//'d'键输入次数
int sum_a = 0;  //'a'键输入次数


double monster1_x[mon] = {1068,3618,1349,2726,2826,2551,3155,2253,3786};
double monster1_y[mon] = { 378,324,316 ,21,21,298,343,315,338};
int monster1_live[mon] = {1,1,1,1,1,1,1,1,1};//怪物 1,2活 3死 4死亡叫声
bool monster_switch[mon] = { false };
bool msdm = false;	//怪物死亡音效是否开启
double boss1_x = 3416;//boss1坐标
double boss1_y = 303;
int boss1_xue = 2000;//boss1血量
double boss1_imgx=0, boss1_imgy=0;//boss1贴图坐标
int boss_muka = 1;//boss朝向，1为右，0为左
int boss_time = 0;
int boss1_live = 1;	//boss1状态 1,2活,3死

int qipao_x[qipao] = {467,401,599,634,2311,2448,2586,2931,2793,\
					2725,2999,3258,3321,3361,3928,1536,1711,1922,2061};
int qipao_y[qipao] = {535,567,537,569, 538, 538, 569, 536, 571,\
					537, 537, 533,  528, 563, 539, 542, 565, 542, 540};
int monster1img_x = 0, monster1img_y = 0;

int time_qipao = 0;//气泡计时
int ip_left_x[ipn]   = {   0 ,  430, 798  , 465 ,\
						 199 ,    0, 729  , 660 ,\
						 1195, 1264, 1529 , 1497,\
						 1764, 1765, 1697 , 1963,\
						 2099, 2166, 2306 , 2719,\
						 3098, 3406, 3300 };
						//碰撞区块左上角坐标
int ip_left_y[ipn]   = { 470 , 372 ,  168 , 171 ,\
						 169 , 436 ,  302 , 434 ,\
						 434 , 368 ,  200 , 335 ,\
						 204 , 305 ,  436 , 303 ,\
						 369 , 236 ,  368,  402,\
						 402 , 403 ,  350};

int ip_right_x[ipn]  = { 369 ,  597, 1195 , 531  ,\
						 432 ,  131,  898 , 1127 ,\
						 1395, 1396, 1662 , 1596 ,\
						 1798, 1863, 1795 , 2030 ,\
						 2307, 2309, 2606 , 3098 ,\
						 3233, 3921, 3336};
						//碰撞区块右下角坐标
int ip_right_y[ipn]  = { 600 , 432 , 199  , 198 ,\
						 236 , 437 , 303  , 599 ,\
						 599 , 440 , 233  , 366 ,\
						 234 , 335 , 466  , 334 ,\
						 599 , 245 , 384  , 414 ,\
						 599 , 599 , 381};
MOUSEMSG m;
LOGFONT zt;
IMAGE biaoti;	//标题背景
IMAGE xstartt, xstartt_mask;	//开始按钮
IMAGE startt, startt_mask;	   //开始按钮选中
IMAGE exitt, exitt_mask;	//退出按钮
IMAGE xexitt, xexitt_mask;	//退出按钮选中
IMAGE player , player_mask;	//人物及遮罩
IMAGE xue,xue_mask;//人物血量
IMAGE monster1, monster1_mask;//怪物一
IMAGE ms1, ms1_mask;		 //怪物一，死
IMAGE boss1, boss1_mask;	//boos1相关
IMAGE boos1s, boos1s_mask;	//	boos1死
IMAGE skill_j_right, skill_j_right_mask; // J技能 右 释放贴图及遮罩110*70
IMAGE skill_j_left,  skill_j_left_mask;	 // J技能 左 释放贴图及遮罩110*70
IMAGE skill_k_right, skill_k_right_mask; // k技能 右 释放贴图及遮罩100*100
IMAGE skill_k_left,  skill_k_left_mask;	//  k技能 左 释放贴图及遮罩100*100
IMAGE back;					//背景
IMAGE qipaoimg;				//气泡
IMAGE tou, tou_mask;//头像及遮罩
IMAGE deadimg, deadimg_mask;//死亡界面
IMAGE stop, stop_mask;	//暂停界面
IMAGE XComtinue;	   //暂停界面选中
IMAGE XBack;//选中暂停
IMAGE zimu;//字幕背景
IMAGE background;//剧情背景 黑
IMAGE skipb;//快进按钮
IMAGE endd;
string plot[6] = {  "我为什么要这么做...？","哈尔希洛回过神来",\
					"发现自己身处在黑暗当中","他完全不知道自己人在何处 也不明白这个地方是哪里","没有人知道未来会遇见什么...",\
					"这一切，就是从灰烬之中所诞生的冒险谭"};
string eee[3] = { "少女明白了一切","梦该醒了","END " };
void BGM()
{
	mciSendString("open F://CloudMusic//Cagayake!GIRLS.mp3 alias mc", NULL, 0, NULL);
	mciSendString("play mc repeat", NULL, 0, NULL);
}
void Start()//初始化
{
	initgraph(1000, 600);
	
	loadimage(&skipb, "image//skip.png");

	loadimage(&endd, "image//end.png");

	loadimage(&boos1s, "image//boos1s.bmp");
	loadimage(&boos1s_mask, "image//boos1s_mask.bmp");

	loadimage(&boss1, "image//boss1.bmp");
	loadimage(&boss1_mask, "image//boss1_mask.bmp");
	
	loadimage(&biaoti, "image//标题界面.png");

	loadimage(&background, "image//剧情1.png");
	loadimage(&zimu, "image//zimu.png");


	loadimage(&deadimg, "image//dead.bmp");
	loadimage(&deadimg_mask, "image//dead_mask.bmp");

	loadimage(&stop, "image//stop.bmp");
	loadimage(&stop_mask, "image//stop_mask.bmp");

	loadimage(&xstartt, "image//xstart.bmp");
	loadimage(&xstartt_mask, "image//xstart_mask.bmp");
	loadimage(&xexitt, "image//xexit.bmp");
	loadimage(&xexitt_mask, "image//xexit_mask.bmp");

	loadimage(&startt, "image//start.bmp");
	loadimage(&startt_mask, "image//start_mask.bmp");
	loadimage(&exitt, "image//exit.bmp");
	loadimage(&exitt_mask, "image//exit_mask.bmp");


	loadimage(&back, "image//back.bmp");
	loadimage(&qipaoimg, "image//qipao.bmp");

	loadimage(&tou,      "image//touxiang.bmp");
	loadimage(&tou_mask, "image//touxiang_mask.bmp");

	loadimage(&monster1,      "image//monster1.bmp");
	loadimage(&monster1_mask, "image//monster1_mask.bmp");
	loadimage(&ms1,		 "image//m1死.bmp");
	loadimage(&ms1_mask, "image//m1死_mask.bmp");

	loadimage(&xue, "image//xue.bmp");
	loadimage(&xue_mask, "image//xue_mask.bmp");

	loadimage(&player,      "image//player.bmp");
	loadimage(&player_mask, "image//player_mask.bmp");

	loadimage(&skill_j_right,      "image//j_right.bmp");
	loadimage(&skill_j_right_mask, "image//j_right_mask.bmp");
	loadimage(&skill_j_left,       "image//j_left.bmp");
	loadimage(&skill_j_left_mask,  "image//j_left_mask.bmp");
	loadimage(&skill_k_right,      "image//k_right.bmp");
	loadimage(&skill_k_right_mask, "image//k_right_mask.bmp");
	loadimage(&skill_k_left,       "image//k_left.bmp");
	loadimage(&skill_k_left_mask,  "image//k_left_mask.bmp");

	loadimage(&XComtinue,	"image//XComtinue.png");
	loadimage(&XBack,		"image//XBack.png");
	BeginBatchDraw();
	
	
}
void MonsterImpact()//怪物碰撞
{
	
	for(int j=0;j<mon;j++)
	{
		for (int i = 0; i < ipn; i++)
		{
			//上下检测
			if (((monster1_x[j] < ip_right_x[i] ) || (monster1_x[j] + 48 < ip_right_x[i] )) && \
				((monster1_x[j] > ip_left_x[i] ) || (monster1_x[j] + 48 > ip_left_x[i] )))
			{
				if (abs((monster1_y[j] + 39) - ip_left_y[i]) <= 1)//脚下有实体
				{
					monster1_y[j] = ip_left_y[i] - 39;
				}
			}
		}
	}
	for(int i=0;i<mon;i++)
	{
		monster1_y[i]++;
	}
	
	
}

Player PlayerImpact(Player p)//人物碰撞检测
{
	p.wall = false;
	//边界检测
	if (p.player_x <= 0)
		p.player_x = 0;
	if (p.player_y > 600)
		 blood = 0;
	for(int i=0 ; i< ipn ; i++)
	{
		//上下检测
		if ( ((p.player_x < ip_right_x[i] + z_x) || (p.player_x+ p.player_r < ip_right_x[i] + z_x)) &&\
			 ((p.player_x > ip_left_x[i]  + z_x) || (p.player_x+ p.player_r > ip_left_x[i]  + z_x))   )
		{
			if (abs((p.player_y + p.player_h) - ip_left_y[i]) <= 1)//脚下有实体
			{
				p.player_y = ip_left_y[i] - p.player_h;
				p.kyt = true;
			}
			if (abs(p.player_y    -    ip_right_y[i]) <= 1)//头顶有实体
				p.player_y = ip_right_y[i] + 1;
			
		}
	}
	p.player_y++;//重力
	return p;
}
void Chongzhi()//重置函数 
{
	zanting = false;
	dead = false;
	blood = 24;
	back_imgx = 0;
	back_imgy = 0;
	z_x = 0;
	ii = 0;
}
bool UI()
{
	if(boss1_live != 3)
	{
		putimage(0, 0,&tou_mask,SRCAND);//角色头像
		putimage(0, 0,&tou     ,SRCPAINT);
		for(int i=1;i<=blood;i++)
		{
			putimage(70 + i * 10, 19, &xue_mask, SRCAND);//角色血量
			putimage(70 + i * 10, 19, &xue     , SRCPAINT);//角色血量
		}
	}
	//for (int i = 1; i <= 10; i++)
	//{
	//	putimage(50 + i * 25, 10, &xin_off_mask, SRCAND);//角色血量
	//	putimage(50 + i * 25, 10, &xin_off, SRCPAINT);
	//}
	if (zanting==true)
	{
		putimage(250, 80, &stop_mask, SRCAND);//暂停界面
		putimage(250, 80, &stop, SRCPAINT);
		if (MouseHit())
			m = GetMouseMsg(); //获取鼠标消息
		if (m.x > 356 && m.x < 635 && m.y>199 && m.y < 275) //继续按钮 高亮 
		{
			putimage(356, 194, &XComtinue);
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				zanting = false;
				PlaySound("music//选定.wav", NULL, SND_FILENAME | SND_ASYNC);
				return false;
			}	
		}
		if (m.x > 357 && m.x < 637 && m.y>344 && m.y < 423) //返回按钮 高亮
		{
			putimage(357, 340, &XBack);
			if (m.uMsg == WM_LBUTTONDOWN)
			{	
				PlaySound("music//选定.wav", NULL, SND_FILENAME | SND_ASYNC);
				Chongzhi();
				return true;
			}
		}
		return false;
	}
	if(dead == true)//死亡界面
	{
		if(ii==0)
		{
			PlaySound("music//dead.wav", NULL, SND_FILENAME | SND_ASYNC);
			ii++;
		}
		putimage(250, 80, &deadimg_mask, SRCAND);//暂停界面
		putimage(250, 80, &deadimg, SRCPAINT);
		if (MouseHit())
			m = GetMouseMsg(); //获取鼠标消息
		if (m.x > 357 && m.x < 637 && m.y>344 && m.y < 423) //返回按钮 高亮
		{
			putimage(357, 340, &XBack);
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				PlaySound("music//选定.wav", NULL, SND_FILENAME | SND_ASYNC);
				Chongzhi();


				return true;
			}
		}
		return false;
	}

}

bool Collision(Player p, int x, int y, int height,int width)
{
	if ((abs(x + z_x - p.player_x + p.player_r) <= 1 && abs(x + z_x - p.player_x + p.player_r) > 0)||\
		(abs(x + z_x+ width - p.player_x) <= 1 && abs(x + z_x+ width - p.player_x) > 0)&&\
		 abs(p.player_y - y) <= height)//撞到玩家
		return true;
	return false;
}
Player UpMonster(Player p)//怪物更新
{
	setfillcolor(RED);
	solidrectangle(boss1_x + z_x - 50, boss1_y - 20, boss1_x + z_x -50 + boss1_xue/10, boss1_y - 10);
	//boos1检查
	if(boss1_live == 1)
	{
		putimage(boss1_x +z_x, boss1_y, 100, 100, &boss1_mask,        boss1_imgx, boss1_imgy, SRCAND);
		putimage(boss1_x +z_x, boss1_y, 100, 100, &boss1, boss1_imgx, boss1_imgy, SRCPAINT);
	}
	else
	{
		putimage(boss1_x + z_x, boss1_y,  &boos1s_mask,  SRCAND);
		putimage(boss1_x + z_x, boss1_y,  &boos1s,  SRCPAINT);
	}
	if (boss1_live == 3)
	{
		Sleep(1000);
		putimage(0, 0, &endd);
		
		outtextxy(500 - (eee[0].length() * 18 / 2), 200, eee[0].c_str());
		outtextxy(500 - (eee[1].length() * 18 / 2), 250, eee[1].c_str());
		outtextxy(500 - (eee[2].length() * 18 / 2), 320, eee[2].c_str());
		Sleep(200);
			//putimage(0, 0, &endd);
		
		
		
	}
	if (Collision(p, (int)boss1_x, (int)boss1_y, 60,60) && boss1_live == 1)	//碰撞
	{
		
			blood -= 1;
	}
	if ((p.skill_j == true || p.skill_k == true) && abs(boss1_x+50+z_x-p.player_x+p.player_r/2)<=50 &&\
		abs(boss1_x + 50 + z_x - p.player_x + p.player_r / 2)>0)
	{
		boss1_xue -= 200;
		if (boss1_xue < 0)
			boss1_xue = 0;
		if (boss_muka == 1 && p.player_direction == 0 && boss1_live == 1)
			boss1_x -= 1;
		if (boss_muka == 0 && p.player_direction == 1 && boss1_live == 1)
			boss1_x += 1;
		if (boss_muka == p.player_direction && boss_muka == 0 && boss1_live == 1)
			boss1_x -= 1;
		if (boss_muka == p.player_direction && boss_muka == 1 && boss1_live == 1)
			boss1_x += 1;

	}
	
	if (boss1_xue == 0)
	{
		//boss死亡音效
		boss1_live = 3;
		boss1_xue = -1;
	}

	if (boss1_imgx < 1100 && boss_time==10 && boss1_live == 1)
		boss1_imgx += 100;
	if (boss1_imgx == 1100)
		boss1_imgx = 0;
	boss_muka == 1 ? boss1_imgy = 0 : boss1_imgy = 100;
	boss_time == 80 ? boss_time = 0 : boss_time++;
	if(boss1_x<=3817 && boss1_x>=3408 && boss1_live == 1)
	{
		if(boss_muka == 1)
		{
			boss1_x += 0.2;

		}
		if (boss_muka == 0)
		{
			boss1_x -= 0.2;

		}
		if (abs(boss1_x - 3817)<=2)
			boss_muka = 0;
		if (abs(boss1_x - 3408)<=2)
			boss_muka = 1;
	}
	//小怪检查
	for (int i = 0; i < mon; i++)
	{	
		if (monster1_live[i] == 2 && boss1_live != 3)
		{	
			putimage(monster1_x[i] + z_x, monster1_y[i], 48, 39, &monster1_mask, monster1img_x, monster1img_y, SRCAND);
			putimage(monster1_x[i] + z_x, monster1_y[i], 48, 39, &monster1, monster1img_x, monster1img_y, SRCPAINT);
		}
		if(abs((monster1_x[i] + z_x - p.player_x)<=500))//怪物看见玩家
		{
			monster_switch[i] = true;
			

		}
		if (monster_switch[i] == true)
		{
			if(monster1_live[i]==1)//1
			{
				PlaySound("music//小怪叫.wav", NULL, SND_FILENAME | SND_ASYNC);
				monster1_live[i]=2;
			}
			if(monster1_live[i] ==2)
				monster1_x[i]-=0.1;
			monster1img_y = 0;
			monster1img_x += 48;
			if (monster1img_x >= 144)
				monster1img_x = 0;
		}

		if (Collision(p, (int)monster1_x[i], (int)monster1_y[i], 50,48) && monster1_live[i] == 2)//怪物碰到玩家
			blood -= 1;

		//击杀
		if (abs(monster1_x[i] + z_x - (p.player_x + p.player_r)) <= 65 &&\
			(p.skill_j == true || p.skill_k == true ) && monster1_live[i] == 2)
		{
			monster1_live[i] = 4;
		}
		if(monster1_live[i] == 4)
		{
			msdm = true;
			monster1_live[i] = 3;
		}
		if(monster1_live[i]==3 && boss1_live != 3)
		{ 
		putimage(monster1_x[i] + z_x, monster1_y[i], &ms1_mask, SRCAND);
		putimage(monster1_x[i] + z_x, monster1_y[i], &ms1, SRCPAINT);
		}
	}
	return p;
}

Player UpInput(Player p)//输入有关更新
{
	if (GetAsyncKeyState(0x44) & 0x8000 && zanting==false && dead==false)//d
	{
		p.player_direction = 1;
		if(sum_d == 35)
		{
			p.player_x+=3;
			sum_d = 0;
			p.player_imgy = 0;
			p.player_imgx < 455 ? p.player_imgx += p.player_r : p.player_imgx = p.player_r;
		}
		sum_d++;
		if (p.wall == false)
			z_x -= 0.3;
	}
	if (GetAsyncKeyState(0x41) & 0x8000 && zanting == false&& dead==false)//a
	{
		p.player_direction = 0;
		if (sum_a == 35)
		{
			p.player_x-=3;
			sum_a = 0;
			p.player_imgy = p.player_h;
			p.player_imgx < 455 ? p.player_imgx += p.player_r : p.player_imgx = p.player_r;
		}
		sum_a++;
		if(back_imgx + z_x <=0)
			z_x+=0.3;
	}
	if (_kbhit())	//如果有输入
	{
		input = _getch();
		if((input==' ') && (p.space == false) && (p.kyt == true))//当输入为空格，跳跃状态未开启，脚下为实体时
			p.space = true;		//开启跳跃状态
		if(input=='&')//彩蛋
			blood += 20;
		if (input == 0X1B)//暂停
			if (zanting == false)
				zanting = true;
			else
				zanting = false;
	}
	if (GetAsyncKeyState(0x4A) & 0x8000 && p.skill_j == false && p.skill_k == false && zanting == false && dead == false)//技能j
	{
		p.skill_j = true;
		skill_time = 0;
	}
	if (GetAsyncKeyState(0x4B) & 0x8000 && p.skill_k == false && p.skill_j == false && zanting == false && dead == false)//技能k
	{
		p.skill_k = true;
		skill_time = 0;
	}
	if (!(GetAsyncKeyState(0x41) & 0x8000) &&\
		!(GetAsyncKeyState(0x44) & 0x8000) &&\
		!(GetAsyncKeyState(0x4A) & 0x8000) &&\
		!(GetAsyncKeyState(0x4B) & 0x8000))
		p.player_imgx = 0;
	return p;
}
Player Skill_Gif(Player p)
{
	if(p.skill_j==true)
	{
		//j技能右释放动画
		if (skill_time > sleep*page && skill_time < sleep*(page + 1) && p.player_direction == 1)
		{
			skill_jx = 110 * page;
			putimage(p.player_x, p.player_y, 110, 70, &skill_j_right_mask, skill_jx, 0, SRCAND);
			putimage(p.player_x, p.player_y, 110, 70, &skill_j_right     , skill_jx, 0, SRCPAINT);
		}
	
		//j技能左释放动画
		if (skill_time > sleep*page && skill_time < sleep*(page + 1) && p.player_direction == 0)
		{
			skill_jx = 110 * page;
			putimage(p.player_x, p.player_y, 110, 70, &skill_j_left_mask, skill_jx, 0, SRCAND);
			putimage(p.player_x, p.player_y, 110, 70, &skill_j_left     , skill_jx, 0, SRCPAINT);
		}
	}
	if(p.skill_k==true)
	{

		//k技能右释放动画
		if (skill_time > sleep*page && skill_time < sleep*(page + 1) && p.player_direction == 1)
		{
			skill_kx = 100 * page;
			putimage(p.player_x, p.player_y-30, 100, 100, &skill_k_right_mask, skill_kx, 0, SRCAND);
			putimage(p.player_x, p.player_y-30, 100, 100, &skill_k_right, skill_kx, 0, SRCPAINT);
		}
		//k技能左释放动画
		if (skill_time > sleep*page && skill_time < sleep*(page + 1) && p.player_direction == 0)
		{
			skill_kx = 100 * page;
			putimage(p.player_x, p.player_y-30, 100, 100, &skill_k_left_mask, skill_kx, 0, SRCAND);
			putimage(p.player_x, p.player_y-30, 100, 100, &skill_k_left, skill_kx, 0, SRCPAINT);
		}
	}


	if (skill_time == sleep * (page + 1))
		page++;
	if(skill_time == 2)
	{
		if (p.skill_j == true)
		{
			if (msdm == false)
				PlaySound("music//shua.wav", NULL, SND_FILENAME | SND_ASYNC);
			else
			{
				PlaySound("music//击中.wav", NULL, SND_FILENAME | SND_ASYNC);
				msdm = false;
			}
		}
		if (p.skill_k == true)
		{
			if (msdm == false)
				PlaySound("music//挥棒大.wav", NULL, SND_FILENAME | SND_ASYNC);
			else
			{
				PlaySound("music//击中.wav", NULL, SND_FILENAME | SND_ASYNC);
				msdm = false;
			}
		}
	}
	if (skill_kx >= 500)
	{
		skill_kx = 0;
		p.skill_k = false;
		page = 0;
	}
	
	if (skill_jx >= 550)
	{
		skill_jx = 0;
		p.skill_j = false;
		page = 0;
	}
	if(p.skill_k==true || p.skill_j==true)
		skill_time++;
	return p;
}
void Gif()
{
	
	if (time_qipao > 250 && time_qipao < 500 && boss1_live !=3)
		for(int i=0;i<qipao;i++)
			putimage(qipao_x[i]+z_x, qipao_y[i], &qipaoimg);
	if(time_qipao == 500)
		time_qipao = 0;
	time_qipao++;
}
Player UpPlayer(Player p)//玩家角色更新
{
	putimage(back_imgx + z_x , back_imgy, &back);//背景
	if((p.skill_j==false )&& (p.skill_k==false))//角色行走图
	{	
		putimage(p.player_x, p.player_y, p.player_r, p.player_h, &player_mask, p.player_imgx, p.player_imgy, SRCAND);
		putimage(p.player_x, p.player_y, p.player_r, p.player_h, &player     , p.player_imgx, p.player_imgy, SRCPAINT);
	}
	p = Skill_Gif(p);
	if ((p.space == true) && (p.hhh < space_height))	//如果跳跃状态开启
	{
		p.kyt = false;
		p.player_y -= 3;
		p.hhh++;
		if(p.hhh==2)
			PlaySound("music//hei.wav", NULL, SND_FILENAME | SND_ASYNC);
	}
	if (p.hhh == space_height)		//跳跃已达最大高度
	{
		p.space = false;
		p.hhh = 0;
	}
	if (blood <= 0)
		dead = true;
	return p;
}
void ziti()
{
	zt.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	gettextstyle(&zt);                     // 获取当前字体设置
	zt.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(zt.lfFaceName, "微软雅黑");    // 设置字体为“黑体”
	settextstyle(&zt);                     // 设置字体样式
	setbkmode(TRANSPARENT);			//设置字体背景透明
	settextcolor(WHITE);			//设置字体白色
}
void Animei()
{
	ziti();
	int i = 0;
	int ttime = 0;
	while(1)
	{	
		putimage(0, 0, &background);
		outtextxy(500-(plot[i].length()*18/2),240, plot[i].c_str());


		if (i == 6)
			break;
		if (MouseHit())
			m = GetMouseMsg(); //获取鼠标消息
		if (m.x >= 829 && m.x <= 985 && m.y >= 503 && m.y <= 563) //选中skip
		{
			putimage(829, 503, &skipb);
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				PlaySound("music//选定.wav", NULL, SND_FILENAME | SND_ASYNC);
				Sleep(200);
				break;
			}
		}
		if(ttime==1000)
		{
			i++;
			ttime = 0;
		}
		ttime++;
		//Sleep(3000);
		
		FlushBatchDraw();
	}
}

void index()//标题界面
{
	//mciSendString("open music//标题bgm.mp3 alias mc", NULL, 0, NULL);
	//mciSendString("play mc repeat", NULL, 0, NULL);
	while(1)
	{
		putimage(0, 0, &biaoti);
		putimage(20, 350, &startt_mask, SRCAND);
		putimage(20, 350, &startt,		 SRCPAINT);

		putimage(20, 460, &exitt_mask, SRCAND);
		putimage(20, 460, &exitt,	   SRCPAINT);

		if (MouseHit())
			m = GetMouseMsg(); //获取鼠标消息
		if (m.x >= 20 && m.x <= 300 && m.y>=350 && m.y<=430) //选中开始
		{
			putimage(20, 350, &xstartt_mask, SRCAND);
			putimage(20, 350, &xstartt, SRCPAINT);
			if(m.uMsg == WM_LBUTTONDOWN)
			{	
				PlaySound("music//选定.wav", NULL, SND_FILENAME | SND_ASYNC);
				break;
			}
		}
		if(m.x > 20 && m.x<280 && m.y>460 && m.y<560)
		{
			putimage(20 , 460, &xexitt_mask, SRCAND);
			putimage(20 , 460, &xexitt, SRCPAINT);
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				PlaySound("music//选定.wav", NULL, SND_FILENAME | SND_ASYNC);
				Sleep(1);
				//退出
				HWND hwnd = GetHWnd();
				SendMessage(hwnd, WM_CLOSE, 0, 0);
				DestroyWindow(hwnd);
				system("pause");
			}
		}
		FlushBatchDraw();
	}
}
int main()
{
	
	S:Start();
	index();//主界面
	BGM();
	Sleep(300);
	Animei();
	Player p1;
	while(1)
	{
		p1 = UpPlayer(p1);
		p1 = UpMonster(p1);
		p1 = UpInput(p1);
		p1 = PlayerImpact(p1);
		MonsterImpact();
		Gif();
		if (UI()) goto S;
		FlushBatchDraw();
	}
	EndBatchDraw();
	return 0;
}
//BeginBatchDraw();循环前
//FlushBatchDraw();循环中
//EndBatchDraw();