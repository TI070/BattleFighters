#pragma once

#include <random>

class UsefulFunction {

public:

	void Delay() {};

	static float GetRandomRange(float min, float max) {
		int rnd = rand() % 1001;

		float range = max - min;

		return min + range * (rnd/1000.0f);
	}

	//��P�����ɑ΂��đ�Q�����̐U�ꕝ�������������l��Ԃ�
	static float GenerateSwingWidth(float baseNum, float swingNum) {
		float sSwing = baseNum * swingNum;
		int r = rand() % 201 - 100;
		float sNum = r / 100.0f;
		sSwing *= sNum;
		return baseNum + sSwing;
	}



};