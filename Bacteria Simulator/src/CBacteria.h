#pragma once
class CBacteria
{
public:
	CBacteria(double fatality = 0, bool dead = false, bool MutateFlag = false);
	CBacteria(const CBacteria& bacteria);
	virtual ~CBacteria() = default;

	CBacteria& operator=(const CBacteria& bacteria);
	// 기본 생성자, 복사 생성자, 이동 대입 연산자 오버로딩

private:
	double m_fatality;
	double m_health;
	bool m_bDead;
	bool m_bMutateFlag;
	// 세균의 각종 수치를 기록하는 변수

public:
	static double mutation_rate;
	// 변이율을 저장하는 변수

public:
	double fatality();
	void fatality(double fatality);
	// 치사율 get/set 함수

	bool IsMutate();
	void SetMutate(bool MutateFlag);
	//

	double health();
	void living(double time);
	void initiate_Health();
	// m_Health와 관련된 함수

	bool isDead();
	bool isAlive();
	void Kill();
	void Revive();
	// m_bDead와 엮이는 함수

	bool try_Kill();
	void reproduce(CBacteria& bacteria);
	// 항생제를 맞거나, 번식을 할 때 쓰이는 함수
};

