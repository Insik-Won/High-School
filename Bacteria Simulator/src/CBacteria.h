#pragma once
class CBacteria
{
public:
	CBacteria(double fatality = 0, bool dead = false, bool MutateFlag = false);
	CBacteria(const CBacteria& bacteria);
	virtual ~CBacteria() = default;

	CBacteria& operator=(const CBacteria& bacteria);
	// �⺻ ������, ���� ������, �̵� ���� ������ �����ε�

private:
	double m_fatality;
	double m_health;
	bool m_bDead;
	bool m_bMutateFlag;
	// ������ ���� ��ġ�� ����ϴ� ����

public:
	static double mutation_rate;
	// �������� �����ϴ� ����

public:
	double fatality();
	void fatality(double fatality);
	// ġ���� get/set �Լ�

	bool IsMutate();
	void SetMutate(bool MutateFlag);
	//

	double health();
	void living(double time);
	void initiate_Health();
	// m_Health�� ���õ� �Լ�

	bool isDead();
	bool isAlive();
	void Kill();
	void Revive();
	// m_bDead�� ���̴� �Լ�

	bool try_Kill();
	void reproduce(CBacteria& bacteria);
	// �׻����� �°ų�, ������ �� �� ���̴� �Լ�
};
