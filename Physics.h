#pragma once

#include "Common.h"

class Physics
{
	HWND m_hwnd;
	BOOL* pDrawMode;
	D2D1_POINT_2F* point;

	FLOAT deltaTimeS = DELTA_TIME / 1000.0f;
	D2D1_POINT_2F velocity = D2D1::Point2F(0.0f, 0.0f);
	D2D1_POINT_2F appliedForceVector = D2D1::Point2F(0.0f, 0.0f);
	D2D1_POINT_2F frictionForceVector = D2D1::Point2F(0.0f, 0.0f);

	int scrollTime = 0;

	
	void SetFrictionForceDirection();
	void fixCollision();
	void DiscardResources();

public:
	Physics(HWND hwnd, BOOL* drawMode, D2D1_POINT_2F* pPoint) {
		m_hwnd = hwnd;
		pDrawMode = drawMode;
		point = pPoint;
	};

	void HandleCommand(UINT uMsg, WPARAM command);
	void simulateInertia();

	~Physics() {
		DiscardResources();
	};
};