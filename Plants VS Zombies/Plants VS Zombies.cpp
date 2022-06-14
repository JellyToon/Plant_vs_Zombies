#include <iostream>
#include <conio.h>
#include <cstring>
#include <Windows.h>
#include <ctime>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

struct Properties {
	const double delay;// = 0.1;
	int		pos;
	int		shape_size;
	bool	existence;
	char	shape[10];
	double 	cooldown_time;

	Properties(const char* Shape) : delay(0.1), pos(0),existence(false), 
		cooldown_time(0),shape_size(10) {
		strcpy(shape, Shape);
	}
	Properties(const char* Shape, double cooldown_time) : delay(0.1), pos(0), 
		shape_size(10), existence(false), cooldown_time(cooldown_time) {
		strcpy(shape, Shape);
	}
	Properties(int pos, const char *shape, double cooldown_time) : delay(0.1), pos(pos),
		shape_size(10), existence(false), cooldown_time(cooldown_time)
	{
		Setshape(shape);
	}

	int	Getpos() { return pos; }
	const char* Getshpae() { return shape; }
	void Setpos(int pos) { this->pos = pos; }
	void Setshape(const char* shape) {
		int len = strlen(shape);
		if (shape_size <= len) {
			strncpy(this->shape, shape, shape_size-1);
			this->shape[shape_size-1] = '\0';
		}
		else strcpy(this->shape, shape);
	}
	void Setexistence(bool existence) { this->existence = existence; }
	void SetCooldownTime(double time) { cooldown_time = time; }
	bool isInside(int length) {
		return pos <= (length - strlen(shape)-1);
	}
	void moveRight(int move) { this->pos+= move; }
	void moveLeft(int move) { this->pos-= move; }
	void Initialization() {
		Setpos(0);
		Setexistence(false);
		SetCooldownTime(0);
	}
};
struct Coin : public Properties {

	Coin(const char* shape = "@") : Properties(shape, 20) {}

	void Setting(int pos){
		Setpos(pos);
		Setexistence(true);
		SetCooldownTime(cooldown_time);
	}
	void Initialization() {
		Properties::Initialization();
		SetCooldownTime(20);
	}
	void CoolDown() {
		if (cooldown_time <= 0) {
			existence = false;
			SetCooldownTime(20);
			return;
		}
		cooldown_time -= delay;
	}
};
Coin* findUnusedCoin(Coin coin[], int maxnum) {
	for (int i = 0; i < maxnum; i++) {
		if (coin[i].existence) continue;
		return &coin[i];
	}
	return nullptr;
}
Coin* findcoinarr(Coin coin[], int maxnum, int pos) {
	for (int i = 0; i < maxnum; i++) {
		if (coin[i].existence) continue;
		if (coin[i].pos == pos) {
			return &coin[i];
		}
	}
	return nullptr;
}
struct Bullet : public Properties{
	int		damage;
	const int Speed;
	const int FireCool;

	Bullet(const char* shape = "*") :Properties(shape), damage(1), Speed(4), FireCool(1) {}
	Bullet(int pos,const char *shape)
		: Properties("*"), damage(1), Speed(4), FireCool(1) {}
	//bool CheckZombie(bool zombie_existence) { return zombie_existence; }
	void draw(char* canvas, int maxCount) {
		if (!isInside(maxCount)) return;
		strncpy(&canvas[pos], shape, strlen(shape));
	}
	void bulletCoolDown() { 
		if (cooldown_time < delay) {
			SetCooldownTime(0);
			return;
		}
		cooldown_time -= delay; 
	}
	void Setting(int pos, int existence) {
		this->Setpos(pos);
		this->Setexistence(existence);
	}
	void initialzation() {
		this->Setpos(0);
		this->Setexistence(false);
	}

};
struct Plant_information{
	//HP 관련
	const int HP_low;
	const int HP_middle;
	const int HP_high;
	//가격관련
	const int price_25;
	const int price_50;
	const int price_100;
	//쿨타임 관련
	const double CT_short;
	const double CT_long;

	Plant_information() : HP_low(1), HP_middle(5), HP_high(72),
		price_25(25), price_50(50), price_100(100),
		CT_short(7.5), CT_long(30) { }
};
struct Plant : public Properties {
	int		HP;
	int		price;
	bool	saveExis;
	bool	Bomb;
	const int Bombdamage;
	int		Dropwidth;
	double	Coin_DropTime;
	const int DropDelay;
	const int coin_arrsize;
	const int MaxTime;
	Bullet	bullet;
	Coin	coins[7]; // 7 -> Dropwidth

	Plant(const char* shape = "") : Properties(shape), HP(0), price(0), saveExis(false),
		Bomb(false), Bombdamage(120), Dropwidth(7), coin_arrsize(7), DropDelay(5), MaxTime(5) {
		SetDropTime(rand() % MaxTime + 1);
	}
	Plant(const char* shape, int price, double cooldown_time, int HP) : 
		Properties(shape, cooldown_time), HP(HP), price(price), saveExis(false), 
		Bomb(false), Bombdamage(120), Dropwidth(7), coin_arrsize(7), DropDelay(5), MaxTime(5) {
		SetDropTime(rand() % MaxTime + 1);
	}
	Plant(int HP, char* shape, int price) : Properties(shape), HP(HP), price(price), 
		saveExis(false), Bomb(false), Bombdamage(120), Dropwidth(7), coin_arrsize(7), DropDelay(5), MaxTime(5) {
		SetDropTime(rand() % MaxTime + 1);
	}

	void SetHP(int hp) { this->HP = hp; }
	void SetSaveExis(bool saveExis) { this->saveExis = saveExis; }
	void Setprice(int price) { this->price = price; }
	void SetBomb(bool Bomb) { this->Bomb = Bomb; }
	void DropCoolDown() { Coin_DropTime -= delay; }
	void CoolDown() {
		if (cooldown_time > delay) {
			cooldown_time -= delay;
			return;
		}
		existence = false;
		cooldown_time = 0;
	}
	void Setting(const char* shape, int price, int hp, int cooldown_time) {
		this->Setshape(shape);
		this->Setprice(price);
		this->SetCooldownTime(cooldown_time);
		this->SetHP(hp);
		this->SetSaveExis(true);
		this->SetCooldownTime(cooldown_time);
	}
	void Setting(Plant plant) {
		this->Setshape(plant.shape);
		this->Setprice(plant.price);
		this->SetCooldownTime(plant.cooldown_time);
		this->SetHP(plant.HP);
		this->SetSaveExis(true);
	}
	void SetDropTime(int DropTime) { Coin_DropTime = DropTime; }
	void Fire() {
		bullet.Setting((this->pos+1), true);
		bullet.SetCooldownTime(bullet.FireCool);
	}
	void Die() {
		Properties::Initialization();
		SetHP(0);
		SetBomb(false);
		if (strcmp(shape, "&") == 0) {
			for (int i = 0; i < coin_arrsize; i++) coins[i].Initialization();
		}
	}
	void UpdateBullet(const int maxCount) {
		if (!bullet.existence&&bullet.cooldown_time <= 0) {
			Fire();
			return;
		}
		bullet.moveRight(bullet.Speed);
		bullet.bulletCoolDown();
		if (!bullet.isInside(maxCount)) {
			bullet.initialzation();
			return;
		}
	}
	void CoinSetting (int width, char* canvas, const int roomNum){
		for (int i = 0; i < coin_arrsize; i++) {
			if (Coin_DropTime > 0) {
				DropCoolDown();
				return;
			}
			Coin* findCoin = findUnusedCoin(coins, coin_arrsize);
			int Droppos = pos + (rand() % Dropwidth - (Dropwidth / 2));
			if (Droppos < roomNum * width) Droppos = roomNum * width;
			if (canvas[Droppos] == '@') return;
			findCoin->Setting(Droppos);
			Coin_DropTime = rand() % DropDelay + 1;
		}
	}
	void UpdateCoin(){
		for (int i = 0; i < coin_arrsize; i++) {
			if (!coins[i].existence)continue;
			if (coins[i].cooldown_time <= 0) {
				coins[i].Initialization();
				continue;
			}
			coins[i].CoolDown();
		}
	}
};
Plant* findUnusedPlant(Plant plant[], int maxnum) {
	for (int i = 0; i < maxnum; i++) {
		if (plant[i].existence) continue;
		return &plant[i];
	}
	return nullptr;
}
Plant* findCoinPlant(Plant plant[], int maxnum) {
	for (int i = 0; i < maxnum; i++) {
		if (strcmp(plant[i].shape, "&") != 0) continue;
		return &plant[i];
	}
}
struct Zombie_information {
	//HP관련
	const int HP_normal;
	const int HP_high;
	//speed관련
	const double Speed_normal;
	const double Speed_fast;

	Zombie_information() : HP_normal(10), HP_high(28),
		Speed_normal(0.2),Speed_fast(0.1)
	{}
};
struct Zombie : public Properties{
	int		HP;
	int		damage;
	bool	savemove;
	double	attackDelay;
	double	savecoolTime;
	Zombie(const char* shape = "('')") : Properties(shape),HP(10),damage(1),
		savemove(false),attackDelay(10*delay),savecoolTime(0){
		SetCooldownTime(2*delay);
	}
	Zombie(const char* shape, int HP) : Properties(shape), HP(HP), damage(1), 
		savemove(false), savecoolTime(0) {}
	Zombie(const char* shape, int HP, double cooldown_time) :  Properties(shape,cooldown_time), 
		savemove(false), HP(HP), damage(1), savecoolTime(0) { }
	void SetHP(int HP) { this->HP = HP; }
	void Setdamage(int damage) { this->damage = damage; }
	void SetsavecoolTime(int savecooltime) { this->savecoolTime = savecoolTime; }
	void Setsavemove(bool savemove) { this->savemove = savemove; }
	void Setting(Zombie zombie,int length) {
		Setpos(length - strlen(zombie.shape)-1);
		Setshape(zombie.shape);
		SetHP(zombie.HP);
		Setexistence(true);
		Setdamage(1);
		SetCooldownTime(2*delay);
		SetsavecoolTime(zombie.cooldown_time);
		Setsavemove(false);
	}
	void Initialization() {
		Properties::Initialization();
		SetsavecoolTime(0);
		Setsavemove(false);
		SetHP(0);
	}
	void ZombieCoolDown() { cooldown_time -= delay; }
	void Zombiemove() {
		if (cooldown_time >= 0) {
			ZombieCoolDown();
			return;
		}
		pos--; 
		SetCooldownTime(savecoolTime);
	}
	void UpdateZombie(Plant* plant) {
		if (HP <= 0) {
			Die();
			return;
		}
		if (meetPlant(plant)) {
			AttackPlant(plant);
			cooldown_time = savecoolTime;
			return;
		}
	}	
	bool meetPlant(Plant* plant) {
		if (!plant->existence) return false;
		if (plant->pos + 1 >= pos &&pos+strlen(shape)>=plant->pos ) return true;
		return false;
	}
	void AttackPlant(Plant* plant) {
		if (0 < attackDelay) attackDelay -= delay;
		else {
			attackDelay = 10 * delay;
			plant->HP--;
		}
	}
	void AttackedBullet(Bullet* bullet) {
		if (!bullet->existence) return;
		if (pos <= bullet->pos&&bullet->pos <= pos + strlen(shape)) {
			HP -= bullet->damage;
			bullet->Setting(0, false);
		}
	}
	void BOMB(Plant* plant, int maxnum, int width) {
		if (plant->pos + 1 == pos) plant->SetBomb(true);
		if (!plant->Bomb) return;
		for (int i = 0; i < maxnum; i++) {
			if (!existence) continue;
			if (plant->pos < pos && pos < plant->pos + width) {
				HP-= plant->Bombdamage;
				if (HP < 0)
					Die();
			}
		}
	}
	void Die() {
		this->Setpos(0);
		this->Setexistence(false);
		this->SetCooldownTime(2*delay);
		this->Setdamage(1);
	}
};
Zombie* findUnusedZombie(Zombie zombie[], int maxnum) {
	for (int i = 0; i < maxnum; i++) {
		if (zombie[i].existence) continue;
		return &zombie[i];
	}
	return nullptr;
}
struct Cursor : public Properties {
	Cursor(int pos, const char* shape) : Properties(pos, shape, 2*delay) { existence = true; }
	void Setpos(int position) { pos = position; }
	void Flicker() { 
		if (0 < cooldown_time) {
			cooldown_time -= delay;
			return;
		}
		existence = !existence;
		cooldown_time = 2*delay;
	}
};
struct UI {
	const int maxCount;
	const int PlantUINum;
	const int cursorNum;
	const int zombieNum;
	const int width;
	char canvas[80];
	int Totalcoin;
	int zombietype;
	Plant_information P_info;
	Zombie_information Z_info;
	Plant plantUI[4] ={ 
		{"%", P_info.price_100, P_info.CT_short,P_info.HP_middle},
		{"&", P_info.price_50, P_info.CT_short, P_info.HP_middle},
		{"/", P_info.price_50, P_info.CT_long, P_info.HP_high},
		{"_", P_info.price_25, P_info.CT_long, P_info.HP_low} };
	Cursor cursor[2] = { {5*width-1,"<"},{9*width-1,">"} }; // cursor[0] = Uicursor, cursor[1] = playcursor
	Zombie zombie[2] = { 
		{"('_')", Z_info.HP_normal, Z_info.Speed_normal},
		{"('A')", Z_info.HP_high, Z_info.Speed_fast} };
	UI() : maxCount(80), PlantUINum(4), cursorNum(2), zombieNum(2), 
		Totalcoin(50), width(5), zombietype(0){
		for (int i = 0; i < PlantUINum; i++) {
			plantUI[i].pos = (i + 1) * width;
			plantUI[i].existence = false;
		}
	}
	void clearcanvas() {
		memset(canvas, ' ', maxCount-1);
		canvas[maxCount - 1] = { NULL };
	}
	void Drawcursor() {
		for (int i = 0; i < cursorNum; i++) {
			if (!cursor[i].existence) continue;
			strncpy(&canvas[cursor[i].pos], cursor[i].shape, strlen(cursor[i].shape));
		}
	}
	void Setcanvas() {
		for (int i = 0; i < maxCount; i++) {
			if (i % width == width-1 && i < maxCount - 1) canvas[i] = '|';
			else if (6*width <= i && i < 7*width) canvas[i] = '|';
			else continue;
		}
	}
	void FlickerCursor() {
		for (int i = 0; i < cursorNum; i++) cursor[i].Flicker();
	}
	void UIcursorRightmove() {
		if (cursor[0].pos > width * 5) return;
		int Pos = cursor[0].pos / 5;
		cursor[0].moveRight(width);
		if (Pos < 0 && PlantUINum < Pos) return;
		if (plantUI[Pos].existence) UIcursorRightmove();
	}
	void UIcursorLeftmove() {
		if (width > cursor[0].pos) return;
		int Pos = cursor[0].pos / 5;
		cursor[0].moveLeft(width);
		if (Pos < 0 && PlantUINum < Pos) return;
		if (plantUI[Pos-2].existence) UIcursorLeftmove();

	}
	void PlaycursorRightmove() {

		if (cursor[1].pos < (maxCount - width - 1)) cursor[1].moveRight(1);
	}
	void PlaycursorLeftmove() {
		if (width * 7 - 1 < cursor[1].pos) cursor[1].moveLeft(1);
	}
	void getcoin(Plant* plant) {
		for (int i = 0; i < plant->coin_arrsize; i++) {
			if (!plant->coins[i].existence) continue;
			if (plant->coins[i].pos != cursor[1].pos) continue;
			Totalcoin += 25;
			plant->coins[i].Initialization();
		}
	}
	void BuyPlant(int price) {
		if (Totalcoin < price) return;
		Totalcoin -= price;
		UIcursorRightmove();
	}
	void plantmenu() {
		canvas[0] = '$';
		char data[10];
		sprintf(data, "%3d", Totalcoin);
		strncpy(&canvas[1], data, strlen(data));
		for (int i = 0; i < PlantUINum; i++) {
			if (plantUI[i].existence) {
				sprintf(data, "%2.1f", plantUI[i].cooldown_time);
				strncpy(&canvas[plantUI[i].pos], data, strlen(data));
				continue;
			}
			strncpy(&canvas[plantUI[i].pos], plantUI[i].shape, strlen(plantUI[i].shape));
			sprintf(data, "%3d", plantUI[i].price);
			strncpy(&canvas[plantUI[i].pos + 1], data, strlen(data));
		}
	}
	void plantSetting(Plant* plant) {

		int plant_num = cursor[0].pos / width;
		int arr_num = plant_num - 1;
		switch (plant_num) {
		case 1:
			if (plantUI[arr_num].existence)break;
			if (Totalcoin < plantUI[arr_num].price) break;
			BuyPlant(plantUI[arr_num].price);
			plant->Setting(plantUI[arr_num]);
			plantUI[plant_num - 1].SetCooldownTime(7.5);
			plantUI[plant_num - 1].Setexistence(true);
			break;
		case 2:
			if (plantUI[arr_num].existence) break;
			if (Totalcoin < plantUI[arr_num].price) break;
			BuyPlant(plantUI[arr_num].price);
			plant->Setting(plantUI[arr_num]);
			plantUI[plant_num - 1].SetCooldownTime(7.5);
			plantUI[plant_num - 1].Setexistence(true);
			break;
		case 3:
			if (plantUI[arr_num].existence) break;
			if (Totalcoin < plantUI[arr_num].price) break;
			BuyPlant(plantUI[arr_num].price);
			plant->Setting(plantUI[arr_num]);
			plantUI[plant_num - 1].SetCooldownTime(30);
			plantUI[plant_num - 1].Setexistence(true);
			break;
		case 4:
			if (plantUI[arr_num].existence) break;
			if (Totalcoin < plantUI[arr_num].price) break;
			BuyPlant(plantUI[arr_num].price);
			plant->Setting(plantUI[arr_num]);
			plantUI[plant_num - 1].SetCooldownTime(30);
			plantUI[plant_num - 1].Setexistence(true);
			break;
		}
	}
	void plantPosSet(Plant* plant) {
		plant->Setpos((cursor[1].pos / width)*width);
		if (canvas[plant->pos] != ' ') return;
		plant->Setexistence(true);
		plant->SetSaveExis(false);
	}
	bool CheckUsed(int pos) {
		if (canvas[pos] == ' ')
			return false;
		return true;
	}
	void render() {
		cout << "\r" << canvas;
	}
	void DrawPlant(Plant* plant) {
		if (plant->HP < 0) {
			plant->Die();
			return;
		}
		if (!plant->existence) return;
		strncpy(&canvas[plant->pos], plant->shape, strlen(plant->shape));
	}
	void BasicDraw() {
		clearcanvas();
		Setcanvas();
		for (int i = 0; i < PlantUINum; i++) plantUI[i].CoolDown();
		plantmenu();
		FlickerCursor();
	}
	void DrawBullet(Plant* plant) {
		plant->UpdateBullet(maxCount);
		if (!plant->bullet.existence) return;
		strncpy(&canvas[plant->bullet.pos], plant->bullet.shape,
			strlen(plant->bullet.shape));
	}
	void zombieSetting(Zombie* zombie) {
		if (zombietype == 9) {
			zombie->Setting(this->zombie[1], maxCount);
		}
		else {
			zombie->Setting(this->zombie[0], maxCount);
		}
		zombietype = time(NULL) % 10;
	}
	void DrawZombie(Zombie* zombie) {
		if (!zombie->isInside(maxCount)) return;
		if (!zombie->existence) return;
		strncpy(&canvas[zombie->pos], zombie->shape, strlen(zombie->shape));
	}
	int FindFrontZombie(Zombie zombie1, Zombie zombie2, int arrNum1, int arrNum2) {
		if (zombie1.pos <= zombie2.pos) return arrNum1;
		return arrNum2;
	}
	void DrawCoin(Plant* plant) {
		for (int i = 0; i < plant->coin_arrsize; i++) {
			if (!plant->coins[i].existence) continue;
			strncpy(&canvas[plant->coins[i].pos], plant->coins[i].shape,
				strlen(plant->coins[i].shape));
		}
	}
};
int main()
{
	srand(time(NULL));
	const int object_maxnum = 10;
	const int StartPlayzon = 7;
	const double delay = 0.1;
	Plant plants[object_maxnum];
	Zombie zombies[object_maxnum];
	Plant* plant = nullptr;
	Zombie* zombie = nullptr;
	UI ui;
	double zombieCoolTime = 10;			// 초기 좀비 출연 시간
	double Coin_CoolTime = rand()%5 + 1; // 코인 쿨타임 : 1~5초
	int	 Life = 5; // 목숨 5
	cout << "코인 : @, 콩슈터 : %, 해바라기 : &, 지뢰 : _, 호두 : /, 좀비 : ('_'), 콘좀비 : ('A')" << endl;
	
	zombie = findUnusedZombie(zombies, object_maxnum);

	while (true) 
	{
		ui.BasicDraw();
		if (zombie == nullptr) {
			zombie = findUnusedZombie(zombies, object_maxnum);
		}
		if (zombieCoolTime <= 0) {
			ui.zombieSetting(zombie);
			zombie = nullptr;
			zombieCoolTime = (double)((time(NULL) % 6) + 10);
		}
		for (int i = 0; i < object_maxnum; i++) {
			ui.DrawPlant(&plants[i]);
			ui.DrawZombie(&zombies[i]);
			zombies[i].Zombiemove();
			if (zombies[i].pos == ui.width * StartPlayzon) {
				Life--;
				zombies[i].Initialization();
			}
		}
		int arrNum = 0;
		for (int i = 1; i < object_maxnum; i++) {
			arrNum = ui.FindFrontZombie(zombies[arrNum], zombies[i], arrNum, i);
		}
		for (int i = 0; i < object_maxnum; i++) {
			if (!plants[i].existence) continue;
			ui.DrawCoin(&plants[i]);
			plants[i].UpdateCoin();
			for (int j = 0; j < object_maxnum; j++) {
				if (!zombies[j].existence) continue;
				zombies[j].AttackedBullet(&plants[i].bullet);
				zombies[j].UpdateZombie(&plants[i]);
				if (strcmp(plants[i].shape, "_") == 0) {
					zombies[arrNum].BOMB(&plants[i], object_maxnum, ui.width);
					if(arrNum!=j)
						zombies[j].BOMB(&plants[i], object_maxnum, ui.width);
				}
			}
			if (plants[i].Bomb) {
				plants[i].Die();
				continue;
			}
			if (strcmp(plants[i].shape, "&") == 0) {
				plants[i].CoinSetting(ui.width, ui.canvas, StartPlayzon);
				continue;
			}
			if (strcmp(plants[i].shape, "%") == 0) {
				ui.DrawBullet(&plants[i]);
				continue;
			}
		}
		if (_kbhit()) {
			int key = _getch();
			int arr_num = 0;

			if (key == 224) {
				key = _getch();
				switch (key) {
				case 75:
					ui.PlaycursorLeftmove();
					break;
				case 77:
					ui.PlaycursorRightmove();
					break;
				}
			}
			else {
				switch (key) {
				case 'a':
					ui.UIcursorLeftmove();
					break;
				case 'd':
					ui.UIcursorRightmove();
					break;
				case 's':
					for (int i = 0; i < object_maxnum; i++) {
						ui.getcoin(&plants[i]);
					}
					break;
				case ' ':
					if (plant != nullptr) break;
					plant = findUnusedPlant(plants, object_maxnum);
					if (plant == nullptr) break;
					ui.plantSetting(plant);
					break;
				case 13:
					if (plant == nullptr) break;
					ui.plantPosSet(plant);
					if(plant->existence) plant = nullptr;
					break;
				}
			}
		}
		zombieCoolTime -= delay;
		ui.Drawcursor();
		ui.render();
		Sleep(delay*1000);
		if (Life == 0)
			break;
	}
	cout << endl << "패배하셨습니다.....";
	return 0;
}