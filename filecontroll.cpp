
#include "filecontroll.h"
#include "fieldConfig.h"
#include "field.h"

#define END_PERIOD 0

/*
void FileControll::GameDataSave() {


	FILE *fp;

	if ((fp = fopen("system/save/save.dat", "w")) == NULL) {

		MessageBox(NULL, "�Z�[�u�f�[�^�̏������݂Ɏ��s", "�G���[", MB_OK);
		return;
	}

	////�t�B�[���h�f�[�^�ǂݍ���
	//�t�B�[���h�^�C�v
	for (int i = 0; i < FIELDTILE_MAX_X;i++) {
		for (int j = 0; j < FIELDTILE_MAX_Z; j++) {
			fprintf(fp,"%d",FieldManager::GetFieldTypeFromFieldData(j,i));
			if(j != FIELDTILE_MAX_Z - 1) fprintf(fp, ",");

		}
		fprintf(fp, "\n");
	}
	
	//////���j�b�g�f�[�^�ǂݍ���
	std::list<Unit*> pUList;
	////�����f�[�^
	//���X�g���擾
	pUList = UnitManager::GetUnitList();

	for (Unit* pUnit : pUList) {

		if (pUnit->GetUnitType() != UTYPE_BUILDING) continue;

		//��ނ��擾
		fprintf(fp, "%d,",(int)pUnit->GetUTD());

		//���W���擾
		fprintf(fp, "%f,%f,%f", pUnit->GetPosition().x, pUnit->GetPosition().y, pUnit->GetPosition().z);

		fprintf(fp, "\n");
	}
	fprintf(fp,"%d", END_PERIOD);

	fprintf(fp, "\n");

	//�L�����N�^�[�f�[�^�ǂݍ���
	for (Unit* pUnit : pUList) {

		if (pUnit->GetUnitType() != UTYPE_UNIT) continue;

		//��ނ��擾
		fprintf(fp, "%d,", (int)pUnit->GetUTD());

		//���W���擾
		fprintf(fp, "%f,%f,%f,", pUnit->GetPosition().x, pUnit->GetPosition().y, pUnit->GetPosition().z);

		//�X�e�[�^�X���擾
		fprintf(fp, "%s,%s,%d,%d,%d,%d,%d,%d", pUnit->GetName(),pUnit->GetJobName(),pUnit->GetMaxLife(), pUnit->GetLife(), pUnit->GetAtk(), pUnit->GetDef(), pUnit->GetGold(), pUnit->GetExp());


		fprintf(fp, "\n");
	}
	fprintf(fp, "%d", END_PERIOD);

	fprintf(fp, "\n");

	//�G�l�~�[�f�[�^�ǂݍ���
	std::list<Enemy*> pEList = EnemyManager::GetEnemyList();;
	for (Enemy* pEnemy : pEList) {

		//if (pEnemy->GetEnemyType() != UTYPE_UNIT) continue;

		//��ނ��擾
		fprintf(fp, "%d,", (int)pEnemy->GetEnemyType());

		//���W���擾
		fprintf(fp, "%f,%f,%f,", pEnemy->GetPosition().x, pEnemy->GetPosition().y, pEnemy->GetPosition().z);

		//�X�e�[�^�X���擾
		fprintf(fp, "%d,%d,%d,%d,%d,%d", pEnemy->GetMaxLife(), pEnemy->GetLife(), pEnemy->GetAtk(), pEnemy->GetDef(), pEnemy->GetGold(), pEnemy->GetExp());


		fprintf(fp, "\n");
	}
	fprintf(fp, "%d", END_PERIOD);
	fprintf(fp, "\n");

	//�v���C���[�p���[
	fprintf(fp, "%d", UnitManager::GetPlayer()->GetPlayerPower());

	fprintf(fp, "\n");

	//���j�b�g�̑���
	fprintf(fp, "%d", UnitManager::GetUnitNum());

	fprintf(fp, "\n");
	
	//���j�b�g�̖��O�`�F�b�N���X�g
	for (int i = 0; i < 128; i++) {
		fprintf(fp, "%d,", UnitManager::GetNameCheckList(i));

	}


	fclose(fp);


}

bool FileControll::GameDataLoad() {

	FILE *fp;
	int dummy;

	if ((fp = fopen("system/save/save.dat", "r")) == NULL) {

		MessageBox(NULL, "�Z�[�u�f�[�^������܂���", "�G���[", MB_OK);
		return false;
	}

	////�t�B�[���h�f�[�^�ǂݍ���
	//�t�B�[���h����
	int type;
	D3DXVECTOR3 s_pos;
	s_pos.y = 0;
	for (int i = 0; i < FIELDTILE_MAX_X; i++) {
		s_pos.x = i;

		for (int j = 0; j < FIELDTILE_MAX_Z; j++) {

			//�^�C�v�擾
			fscanf(fp, "%d", &type);

			s_pos.z = j;

			//����
			if (type == GRASS) FieldManager::SetFieldT<Grass>(s_pos);
			else if(type == BUILDING) FieldManager::SetFieldT<Grass>(s_pos);
			else if (type == ROAD) FieldManager::SetFieldT<Road>(s_pos);

			//,���J�b�g
			if (j != FIELDTILE_MAX_Z - 1) fscanf(fp, "%c,", &dummy);

		}
		//\n���J�b�g
		fscanf(fp, "\n", &dummy);
	}


	//////���j�b�g�f�[�^�ǂݍ���
	////�����f�[�^

	while (true) {

		//�f�[�^�ǂݍ���
		fscanf(fp, "%d",&type);

		if (type == END_PERIOD) break;

		fscanf(fp, ",%f,%f,%f", &s_pos.x, &s_pos.y, &s_pos.z);

		if (type == UTD_BUILDING) UnitManager::SetBuilding(s_pos);
		else if (type == UTD_CHURCH) UnitManager::SetCHURCH(s_pos);
		else if (type == UTD_INN) UnitManager::SetInn(s_pos);
		else if (type == UTD_WP_SHOP) UnitManager::SetWPSHOP(s_pos);
		else if (type == UTD_GATE) UnitManager::SetGate(s_pos);
		else if (type == UTD_STATUE) UnitManager::SetStatue(s_pos);

		//\n���J�b�g
		fscanf(fp, "\n", &dummy);
	}

	//\n���J�b�g
	fscanf(fp, "\n", &dummy);

	int life, mLife, atk, def, gold, exp;
	char s_name[64];
	char s_jobName[64];
	char* oName = new char;
	char* ojobName = new char;

	//�L�����N�^�[
	while (true) {

		//�f�[�^�ǂݍ���
		fscanf(fp, "%d", &type);

		if (type == END_PERIOD) break;

		fscanf(fp, ",%f,%f,%f", &s_pos.x, &s_pos.y, &s_pos.z);

		//���݉\�ӏ��̊O�ɂ����ꍇ�A�ʒu��␳����
		D3DXVECTOR3 cpos = D3DXVECTOR3(1, 0, 5);
		if (s_pos.x < 0) s_pos = cpos;
		else if (s_pos.z < 0) s_pos = cpos;


		//�p�����[�^�擾
		fscanf(fp, ",%[^,],%[^,],%d,%d,%d,%d,%d,%d",s_name,s_jobName, &mLife, &life, &atk,&def,&gold,&exp);

		//����
		Unit* pU;// = UnitManager::SetUnit(s_pos);
		UNIT_TYPE_DETAIL UTP = UTD_UNIT;
		if (strncmp(s_jobName,"�ނ�т�",8) == 0) UTP = UTD_UNIT;
		else if (strncmp(s_jobName, "����",6) == 0) UTP = UTD_SOLDIER;
		else if (strncmp(s_jobName, "�܂ق�����",12) == 0) UTP = UTD_WIZARD;
		else if (strncmp(s_jobName, "�������",8) == 0) UTP = UTD_PRIEST;


		switch (UTP)
		{
		case UTD_UNIT:
			pU = UnitManager::SetUnitT<Unit>(s_pos, "asset\\model\\unit.obj");
			break;
		case UTD_SOLDIER:
			pU = UnitManager::SetUnitT<Soldier>(s_pos, "asset\\model\\soldier.obj");
			break;
		case UTD_WIZARD:
			pU = UnitManager::SetUnitT<Wizard>(s_pos, "asset\\model\\wizard.obj");
			break;
		case UTD_PRIEST:
			pU = UnitManager::SetUnitT<Priest>(s_pos, "asset\\model\\priest.obj");
			break;


		default:
			break;
		}

		//�ʒu�̐���
		pU->PositionAdjust();
		pU->SetTargetPosAll(pU->GetPosition());

		pU->SetMaxLife(mLife);
		pU->SetLife(life);
		pU->SetAtk(atk);
		pU->SetDef(def);
		pU->SetGold(gold);
		pU->SetExp(exp);

		//strcpy(oName, s_name);
		pU->SetName(s_name);

		//\n���J�b�g
		fscanf(fp, "\n", &dummy);
	}


	//\n���J�b�g
	fscanf(fp, "\n", &dummy);

	//�G�l�~�[
	while (true) {

		//�f�[�^�ǂݍ���
		fscanf(fp, "%d", &type);

		if (type == END_PERIOD) break;

		fscanf(fp, ",%f,%f,%f", &s_pos.x, &s_pos.y, &s_pos.z);

		//���݉\�ӏ��̊O�ɂ����ꍇ�A�ʒu��␳����
		D3DXVECTOR3 cpos = D3DXVECTOR3(1, 0, 5);
		if (s_pos.x < 0) s_pos = cpos;
		else if (s_pos.z < 0) s_pos = cpos;

		//�p�����[�^�擾
		fscanf(fp, ",%d,%d,%d,%d,%d,%d", &mLife, &life, &atk, &def, &gold, &exp);

		//����
		Enemy* pE;
		if(type == ETYPE_SLIME) pE = EnemyManager::SetSlime(s_pos);
		else if (type == ETYPE_TREE) pE = EnemyManager::SetTree(s_pos);
		else if (type == ETYPE_MH) pE = EnemyManager::SetMonsterHouse(s_pos);
		else if (type == ETYPE_SLIMEKING) pE = EnemyManager::SetSlimeKing(s_pos);
		else if (type == ETYPE_MAGICMAN) pE = EnemyManager::SetMagicMan(s_pos);

		//�ʒu�̐���
		pE->PositionAdjust();
		pE->SetTargetPosShort(pE->GetPosition());
		pE->SetTargetPos(pE->GetPosition());

		pE->SetMaxLife(mLife);
		pE->SetLife(life);
		pE->SetAtk(atk);
		pE->SetDef(def);
		pE->SetGold(gold);
		pE->SetExp(exp);


		//\n���J�b�g
		fscanf(fp, "\n", &dummy);
	}


	//\n���J�b�g
	fscanf(fp, "\n", &dummy);

	//�v���C���[�p���[�ǂݍ���
	int pPow;
	fscanf(fp, "%d", &pPow);
	UnitManager::GetPlayer()->SetPlayerPower(pPow);

	//\n���J�b�g
	fscanf(fp, "\n", &dummy);

	//���j�b�g�����̓ǂݍ���
	int uNum;
	fscanf(fp, "%d", &uNum);
	UnitManager::SetUnitNum(uNum);

	//\n���J�b�g
	fscanf(fp, "\n", &dummy);

	//���j�b�g�̖��O�`�F�b�N���X�g
	for (int i = 0; i < 128; i++) {
		fscanf(fp, "%d", &uNum);
		UnitManager::SetNameCheckList(i,uNum);

	}

	fclose(fp);

	

	return true;
}
*/