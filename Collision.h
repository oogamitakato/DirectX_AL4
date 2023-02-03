#pragma once
#include "CollisionPrimitive.h"

class Collision
{
public:
	//���ƕ��ʂ̓����蔻��
	static bool CheckSphere2Plane(const Sphere& sphere,
		const Plane& plane,DirectX::XMVECTOR* inter = nullptr);

	//�_�ƎO�p�`�̍Őڋߓ_�����߂�
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
		const Triangle& triangle, DirectX::XMVECTOR* closest);

	//���Ɩ@���t���O�p�`�̓����蔻��
	static bool CheckSphere2Triangle(const Sphere& sphere,
		const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr);
};

