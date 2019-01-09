//---------------------------------------------------------------------------
#ifndef EnemiesManagerH
#define EnemiesManagerH

#include <vector>
#include <windows.h>
#include "Sprite.h"
#include "Config.h"
#include "Enemy.h"

using namespace std;

//---------------------------------------------------------------------------
class TEnemiesManager
{
    vector<Sprite *> SimpleEnemyGfx;
    vector<Sprite *> CleverEnemyGfx;
    vector<Sprite *> RobotEnemyGfx;
    vector<Sprite *> ArmedEnemyGfx;
    Sprite *CraxGfx;

    vector<Enemy *> Enemies;
//    vector<int> ResurrectTimes;
 //   vector<bool> WasValid;

    Enemy *CreateNewEnemy();
    Enemy *CreateEmptyEnemyObject();
    CRITICAL_SECTION EnemyCS;

    public:
        TEnemiesManager();
        ~TEnemiesManager();
        void ReInit();
        void UpdateEnemiesList();
        void ProcessGrenadeExplosion();
        Enemy* EnemyHitsJoe();
        void ShowEnemies();
        void RemoveCrax();

        void SaveState(FILE *fp);
        void LoadState(FILE *fp);
};
//---------------------------------------------------------------------------
TEnemiesManager& EnemiesManager();
//---------------------------------------------------------------------------
#endif
