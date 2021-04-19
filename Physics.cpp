#include "Physics.h"

void Physics::HandleCommand(UINT uMsg, WPARAM command)
{
	switch (uMsg)
	{

	case WM_KEYDOWN:
		switch (LOWORD(command))
		{
		case VK_UP:
			appliedForceVector.y = -APPLIED_FORCE_VALUE;
			frictionForceVector.y = +FRICTION_FORCE_VALUE;
			break;
		case VK_DOWN:
			appliedForceVector.y = +APPLIED_FORCE_VALUE;
			frictionForceVector.y = -FRICTION_FORCE_VALUE;
			break;
		case VK_LEFT:
			appliedForceVector.x = -APPLIED_FORCE_VALUE;
			frictionForceVector.x = +FRICTION_FORCE_VALUE;
			break;
		case VK_RIGHT:
			appliedForceVector.x = +APPLIED_FORCE_VALUE;
			frictionForceVector.x = -FRICTION_FORCE_VALUE;
			break;
		}
		break;

	case WM_KEYUP:
		switch (LOWORD(command))
		{
		case VK_UP:
			appliedForceVector.y = 0;
			break;
		case VK_DOWN:
			appliedForceVector.y = 0;
			break;
		case VK_LEFT:
			appliedForceVector.x = 0;
			break;
		case VK_RIGHT:
			appliedForceVector.x = 0;
			break;
		}
		SetFrictionForceDirection();
		break;

	case WM_MOUSEWHEEL:
	{
		WORD fwKeys = GET_KEYSTATE_WPARAM(command);
		short zDelta = GET_WHEEL_DELTA_WPARAM(command);
		zDelta < 0 ? scrollTime = -zDelta / 12 : scrollTime = zDelta / 12;
		if (fwKeys == MK_SHIFT)
		{
			if (zDelta > 0) {
				appliedForceVector.x = -APPLIED_FORCE_VALUE;
				frictionForceVector.x = +FRICTION_FORCE_VALUE;
			}
			else
			{
				appliedForceVector.x = +APPLIED_FORCE_VALUE;
				frictionForceVector.x = -FRICTION_FORCE_VALUE;
			}

		}
		else
		{
			if (zDelta > 0) {
				appliedForceVector.y = -APPLIED_FORCE_VALUE;
				frictionForceVector.y = +FRICTION_FORCE_VALUE;
			}
			else
			{
				appliedForceVector.y = +APPLIED_FORCE_VALUE;
				frictionForceVector.y = -FRICTION_FORCE_VALUE;
			}
		}
	}
	break;

	}
	return;
}

void Physics::SetFrictionForceDirection()
{
	if (velocity.x > 0) 
	{ frictionForceVector.x = -FRICTION_FORCE_VALUE; }
	else if (velocity.x < 0)
	{ frictionForceVector.x = +FRICTION_FORCE_VALUE; }

	if (velocity.y > 0)
	{ frictionForceVector.y = -FRICTION_FORCE_VALUE; }
	else if (velocity.y < 0)
	{ frictionForceVector.y = +FRICTION_FORCE_VALUE; }

	return;
}

void Physics::fixCollision()
{
	BOOL res = TRUE;

	RECT rc;
	GetClientRect(m_hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

	if (*pDrawMode)
	{
		if (point->y - CIRCLE_RADIUS < 0 || point->y + CIRCLE_RADIUS > size.height)
		{
			velocity.y = -velocity.y;
			frictionForceVector.y = -frictionForceVector.y;
			if (point->y - CIRCLE_RADIUS < 0) 
			{ point->y = CIRCLE_RADIUS; }
			else 
			{ point->y = size.height - CIRCLE_RADIUS; }
		}
		if (point->x - CIRCLE_RADIUS < 0 || point->x + CIRCLE_RADIUS > size.width)
		{
			velocity.x = -velocity.x;
			frictionForceVector.x = -frictionForceVector.x;
			if (point->x - CIRCLE_RADIUS < 0)
			{ point->x = CIRCLE_RADIUS; }
			else
			{ point->x = size.width - CIRCLE_RADIUS; }
		}
	}
	else
	{
		////////////////////////////////////
		UINT dpi = GetDpiForWindow(m_hwnd);							
		FLOAT fWidth = PIC_WIDTH / SCALE_SIZE;
		FLOAT fHeight = PIC_HEIGHT / SCALE_SIZE;
		////////////////////////////////////
		if (point->y - (fHeight / 2) < 0 || point->y + (fHeight / 2) > size.height)
		{
			velocity.y = -velocity.y;
			frictionForceVector.y = -frictionForceVector.y;
			if (point->y - (fHeight / 2) < 0)
			{ point->y = (fHeight / 2); }
			else
			{ point->y = size.height - (fHeight / 2); }
		}
		if (point->x - (fWidth / 2) < 0 || point->x + (fWidth / 2) > size.width)
		{
			velocity.x = -velocity.x;
			frictionForceVector.x = -frictionForceVector.x;
			if (point->x - (fWidth / 2) < 0)
			{ point->x = (fWidth / 2); }
			else
			{ point->x = size.width - (fWidth / 2);	}
		}
	}
	return;
}

void Physics::simulateInertia()
{
	if (scrollTime != 0) {
		--scrollTime;
		if (scrollTime == 0)
		{
			appliedForceVector.y = 0;
			appliedForceVector.x = 0;
			SetFrictionForceDirection();
		}
	}

	// Выполняем, если существуют проекции приложенной силы
	if (appliedForceVector.x != 0 || appliedForceVector.y != 0)
	{
		// Высчитываем проецкию на Х вектора скорости и новое положение компоненты Х середины окружности
		velocity.x = velocity.x + (appliedForceVector.x + frictionForceVector.x) * deltaTimeS;
		point->x = point->x + velocity.x * deltaTimeS + ((appliedForceVector.x + frictionForceVector.x) * deltaTimeS * deltaTimeS) / 2;

		// Высчитываем проецкию на У вектора скорости и новое положение компоненты У середины окружности
		velocity.y = velocity.y + frictionForceVector.y * deltaTimeS + appliedForceVector.y * deltaTimeS;
		point->y = point->y + velocity.y * deltaTimeS + ((frictionForceVector.y + appliedForceVector.y) * deltaTimeS * deltaTimeS) / 2;
	}
	// Если к шайбе больше не приложена сила, то шайба замедляется под действием трения
	else
	{
		// Проекция вектора на Х уменьшается до 0 под действием силы трения, которая вызывает противоположное по направлению проекции Х ускорение
		velocity.x = velocity.x + frictionForceVector.x * deltaTimeS;
		point->x = point->x + velocity.x * deltaTimeS + (frictionForceVector.x * deltaTimeS * deltaTimeS) / 2;
		if ((velocity.x > 0 && frictionForceVector.x > 0) ||
			(velocity.x < 0 && frictionForceVector.x < 0))
		{
			velocity.x = 0.0f;
			frictionForceVector.x = 0.0f;
		}

		// Проекция вектора на У уменьшается до 0 под действием силы трения, которая вызывает противоположное по направлению проекции У ускорение
		velocity.y = velocity.y + frictionForceVector.y * deltaTimeS;
		point->y = point->y + velocity.y * deltaTimeS + (frictionForceVector.y * deltaTimeS * deltaTimeS) / 2;
		if ((velocity.y > 0 && frictionForceVector.y > 0) ||
			(velocity.y < 0 && frictionForceVector.y < 0))
		{
			velocity.y = 0.0f;
			frictionForceVector.y = 0.0f;
		}
	}
	fixCollision();
	return;
}

void Physics::DiscardResources()
{
}
