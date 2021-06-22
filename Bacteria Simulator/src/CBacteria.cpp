#include "pch.h"
#include "CBacteria.h"

CBacteria::CBacteria(double fatality, bool dead, bool MutateFlag) {
	srand(time(NULL));
	m_fatality = fatality;
	m_bMutateFlag = MutateFlag;
	if (m_bMutateFlag == true) {
		m_health = 50 + 50 * m_fatality;
	}
	else {
		m_health = 100;
	}
	m_bDead = dead;	
} // ���� �ʱ�ȭ

CBacteria::CBacteria(const CBacteria& bacteria) {
	m_health = bacteria.m_health;
	m_fatality = bacteria.m_fatality;
	m_bDead = bacteria.m_bDead;
	m_bMutateFlag = bacteria.m_bMutateFlag;
} // ���� ���� �ʱ�ȭ

double CBacteria::mutation_rate = 0.2;

CBacteria& CBacteria::operator=(const CBacteria& bacteria) {
	m_health = bacteria.m_health;
	m_fatality = bacteria.m_fatality;
	m_bDead = bacteria.m_bDead;
	m_bMutateFlag = bacteria.m_bMutateFlag;
	return *this;
} // �̵� ���� �ʱ�ȭ

double CBacteria::fatality() {
	return m_fatality;
}
void CBacteria::fatality(double fatality) {
	m_fatality = fatality;
}
// ġ���� ���� �Լ�

bool CBacteria::IsMutate() {
	return m_bMutateFlag;
}
void CBacteria::SetMutate(bool MutateFlag) {
	m_bMutateFlag = MutateFlag;
}
//

double CBacteria::health() {
	return m_health;
}
void CBacteria::living(double time) {
	m_health += time;
}
void CBacteria::initiate_Health() {
	if (m_bMutateFlag == true) {
		m_health = 50 + 50 * m_fatality;
	}
	else {
		m_health = 110 * (1 - rand() / (double)RAND_MAX / 5);
	}
}
// ���� ���� �Լ�

bool CBacteria::isDead() {
	return m_bDead;
}
bool CBacteria::isAlive() {
	return !m_bDead;
}
void CBacteria::Kill() {
	m_bDead = true;
}
void CBacteria::Revive() {
	m_bDead = false;
}
// ��� ���� ���� �Լ�

bool CBacteria::try_Kill() {
	if (m_bDead == true) {
		return true;
	}

	if (m_fatality > 95) {
		m_bDead = true;
		return true;
	}

	double num =  rand() / (double)RAND_MAX - 0.1;
	num = max(0.0, num);
	if (num < m_fatality) {
		m_bDead = true;
		return true;
	}
	else {
		m_bDead = false;
		return false;
	}
}
// �׻����� �¾��� �� ������ ������ �Ǻ��ϴ� �Լ�.

void CBacteria::reproduce(CBacteria& bacteria) {
	double num = (double)rand() / RAND_MAX - 0.6;
	if (m_bMutateFlag == true) {
		bacteria.m_fatality = m_fatality + num * (mutation_rate);
		bacteria.m_fatality = min(1.0, bacteria.m_fatality);
		bacteria.m_fatality = max(0.0, bacteria.m_fatality);
	}
	else {
		bacteria.m_fatality = m_fatality;
	}
	bacteria.m_health = 50 + 50 * bacteria.m_fatality;
	bacteria.m_bDead = m_bDead;
	bacteria.m_bMutateFlag = m_bMutateFlag;
}
// �����ϴ� �Լ�