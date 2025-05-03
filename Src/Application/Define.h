#pragma once
typedef struct
{
	int Cell1X;
	int Cell2X;
	int Cell3X;
	int Cell4X;

	int Cell1Y;
	int Cell2Y;
	int Cell3Y;
	int Cell4Y;
}Cell;

typedef enum
{
	Straight = 0,
	fast,
	Missile,
	Auto,
	HighPower,
	Burst,
	BurstP,
	Penetration,
	Charge,
	Fire,
	Laesr,
	Gravity,
	Homing,
	RailGun,
	Chain,
	Virus,
	Moon,
	Commet,
	Sun,
	Poizon,
	Conti,
	BulletMax,
}BulletTypeNum;

typedef enum 
{
	Title = 0,
	Game,
	Result,
	LevelUp,
	Select,
};

enum class EnemyState
{
	Move,
	Attack,
	Return,
	Dead,
};

typedef struct 
{
	int Cnt;
	int Time;

} WaponBullet;
#define Cell Cell