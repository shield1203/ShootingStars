
 #pragma once
// ����â ������ --------------------------------------------------------------------------------------
enum MY_WINDOW_SIZE { WIN_LEFT = 0, WIN_TOP = 0, WIN_RIGHT = 1699, WIN_BOTTOM = 799 }; 

// Ű �Է� ����----------------------------------------------------------------------------------------
enum KEY_VALUE { KEY_N = 0x4E, KEY_B = 0x42, KEY_V = 0x56 };

// �������� ���� �ܰ� ---------------------------------------------------------------------------------
enum GAME_STEP { STEP_MAIN, STEP_PLAY, STEP_RANK, STEP_EXIT };

// ���� ----------------------------------------------------------------------------------------------
enum MAIN_SEL { SEL_PLAY, SEL_RANK, SEL_EXIT };

// ���ӽ��� -------------------------------------------------------------------------------------------
enum PLAY_STEP { STEP_GO, STEP_STOP, STEP_SUCCESS, STEP_FAIL, STEP_RECORD };

//enum STAGE_STEP { STEP_0, STEP_1, STEP_2, STEP_3, STEP_4 };

// ��� �ӵ� ------------------------------------------------------------------------------------------

// �÷��̾� ���� --------------------------------------------------------------------------------------
enum PLAYER_STATE { PLAYER_IDLE = 0, PLAYER_RIGHT = 256, PLAYER_LEFT = 512, PLAYER_ATTACK = 255, PLAYER_DIE = 768, PLAYER_SKILL = 1, PLAYER_RESPAWN = 1024 };

enum PLAYER_MOVE { MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN, MOVE_SPEED = 5 };

enum PLAYER_ANI { P_ANI_IDLE = 256, P_ANI_DIE = 1280, P_ANI_RESPAWN = 768 };

// �÷��̾� ����Ʈ ---------------------------------------------------------------------------------------
enum EFFECT_STATE { EFFECT_BOOSTER = 256, EFFECT_ATTACK = 0, EFFECT_SKILL = 512};

// ������
enum ITEM_KIND { LIFE_UP = 30, SKILL_UP = 0, POWER_UP = 60 };

// ���� ����
enum WEAPON { LV1 = 0, LV2 = 30, LV3 = 60 };

// Enemy State - ���¿� ���� ���������� �ٸ���
enum BOSS_PATTERN { TYPE_0 = 0 ,TYPE_A = 768, TYPE_B = 512, TYPE_C = 256, TYPE_D = 513, TYPE_E, TYPE_F };

enum ENEMY_TYPE { NOMAL_NECO, GRAY_NECO = 50, MECA_GIRL = 100 };

enum ENEMY_PATTERN { TYPE_1, TYPE_2, TYPE_3, TYPE_4, TYPE_5, TYPE_6, TYPE_7, TYPE_8, TYPE_9};

enum ENEMY_MOVE { MOVE_R_L, MOVE_R_L_FS, MOVE_R_L_ST, MOVE_U_D, MOVE_U_D_FS, MOVE_U_D_ST, MOVE_D_U, MOVE_D_U_FS, MOVE_D_U_ST,
	MOVE_RU_LD, MOVE_C_R_L };

// Boss State
enum BOSS_STATE {  };

// ��ŷ 
//enum RANK_HTTP { GET_LIST, GET_INSERT };

enum RANK_STATE { RANK_WORk, RANK_DONE };

// ���Ӱ��
enum GAME_RESULT { RESULT_NOT, RESULT_GAME_OVER, RESULT_CLEAR, RESULT_BLACK };