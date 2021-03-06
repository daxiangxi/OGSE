
#include <math.h>
#include <algorithm>

#include "GeoEnvelope.h"
#include "GeoCoordinate.h"

GeoEnvelope::GeoEnvelope()
{
	minX = 0;
	minY = -2;
	maxX = 0;
	maxY = -2;
}

//带参数的构造函数
GeoEnvelope::GeoEnvelope(double minX,double maxX,double minY,double maxY)
{
	//先要比较坐标大小，然后再进行判断
	if (minX > maxX)
	{
		this->minX = maxX;
		this->maxX = minX;
	}
	else
	{
		this->minX = minX;
		this->maxX = maxX;
	}

	if (minY > maxY)
	{
		this->minY = maxY;
		this->maxY = minY;
	}
	else
	{
		this->minY = minY;
		this->maxY = maxY;
	}
}

//拷贝构造函数
GeoEnvelope::GeoEnvelope(const GeoEnvelope& envelope)
{
	this->minX = envelope.minX;
	this->maxX = envelope.maxX;
	this->minY = envelope.minY;
	this->maxY = envelope.maxY;
}

GeoEnvelope &GeoEnvelope::operator =(const GeoEnvelope& other)
{
	if ( &other != this ) // 自检测
	{
		minX = other.minX;
		maxX = other.maxX;
		minY = other.minY;
		maxY = other.maxY;
	}
	return *this;
}

//用两个坐标点初始化
GeoEnvelope::GeoEnvelope(GeoCoordinate *coord1,GeoCoordinate *coord2)
{
	if (coord1->x < coord2->x)
	{
		minX = coord1->x;
		maxX = coord2->x;
	}
	else
	{
		minX = coord2->x;
		maxX = coord1->x;
	}

	if (coord1->y < coord2->y)
	{
		minY = coord1->y;
		maxY = coord2->y;
	}
	else
	{
		minY = coord2->y;
		maxY = coord1->y;
	}
}

//析构函数
GeoEnvelope::~GeoEnvelope(void)
{
}

void GeoEnvelope::Init(double x1,double x2,double y1,double y2)
{
	if (x1 > x2)
	{
		minX = x2;
		maxX = x1;
	}
	else
	{
		minX = x1;
		maxX = x2;
	}

	if (y1 > y2)
	{
		minY = y2;
		maxY = y1;
	}
	else
	{
		minY = y1;
		maxY = y2;
	}
}

bool GeoEnvelope::Intersects(const GeoEnvelope *otherEvp) const
{
	//上下左右四个方向
	//if (maxX < otherEvp.minX || minX > otherEvp.maxX ||
	//	maxY < otherEvp.minY || minY > otherEvp.maxY)
	//{
	//	return 0;
	//}

	////四个对角方向
	//if (maxX < otherEvp.minX && maxY < otherEvp.minY)
	//{
	//	return 0;
	//}
	//if (maxX < otherEvp.minX && minY > otherEvp.maxY)
	//{
	//	return 0;
	//}

	////在矩形内
	//if (minX >= otherEvp.minX && maxX <= otherEvp.maxX &&
	//	minY >= otherEvp.minY && maxY <= otherEvp.maxY)
	//{
	//	return 1;
	//}
	//
	//return 1;	//相交

	if (NULL == otherEvp)
	{
		return false;
	}

	if ( IsNull() || otherEvp->IsNull() ) 
	{
		return false;
	}

	return !(otherEvp->minX > maxX ||
		otherEvp->maxX < minX ||
		otherEvp->minY > maxY ||
		otherEvp->maxY < minY);
}

bool GeoEnvelope::Intersects(const GeoEnvelope &env) const
{
	return Intersects(&env);
}

void GeoEnvelope::ExpandToInclude(double x, double y)
{
	if (IsNull())
	{
		minX = x;
		maxX = x;
		minY = y;
		maxY = y;
	}

	else 
	{
		if (x < minX) 
		{
			minX = x;
		}
		if (x > maxX) 
		{
			maxX = x;
		}
		if (y < minY) 
		{
			minY = y;
		}
		if (y > maxY) 
		{
			maxY = y;
		}
	}
}

void GeoEnvelope::ExpandToInclude(const GeoCoordinate &pt)
{
	ExpandToInclude(pt.x,pt.y);
}

void GeoEnvelope::ExpandToInclude(const GeoEnvelope *other)
{
	if (NULL == other)
	{
		return;
	}
	if (other->IsNull())
	{
		return;
	}

	if (IsNull())
	{
		minX = other->minX;
		maxX = other->maxX;
		minY = other->minY;
		maxY = other->maxY;
	}

	else
	{
		if (other->minX < minX)
		{
			minX = other->minX;
		}
		if (other->maxX > maxX)
		{
			maxX = other->maxX;
		}
		if (other->minY < minY)
		{
			minY = other->minY;
		}
		if (other->maxY > maxY)
		{
			maxY = other->maxY;
		}
	}
}

void GeoEnvelope::ExpandToInclude(const GeoEnvelope &other)
{
	ExpandToInclude(&other);
}

// 判断矩形是否为空
/*inline*/ bool GeoEnvelope::IsNull(void) const
{
	if (maxX < minX || maxY < minY)
	{
		return true;
	} 
	return false;
}

// 获取最小外包矩形的宽度
double GeoEnvelope::GetWidth(void) const
{
	if (IsNull())
	{
		return 0;
	}
	return fabs(maxX - minX);
}

// 获取最小外界矩形的高度
double GeoEnvelope::GetHeight(void) const
{
	if (IsNull())
	{
		return 0;
	}
	return fabs(maxY - minY);
}

// 获得矩形的中心点坐标
bool GeoEnvelope::Center(GeoCoordinate &coord) const
{
	if (IsNull())
	{
		return false;
	}

	coord.x = minX + GetWidth() / 2.0;
	coord.y = minY + GetHeight() / 2.0;

	return true;
}

bool GeoEnvelope::Center(double &x,double &y) const
{
	if (IsNull())
	{
		return false;
	}

	x = minX + GetWidth() / 2.0;
	y = minY + GetHeight() / 2.0;

	return true;
}

//测试是否包含另一个MBR
bool GeoEnvelope::Contains(const GeoEnvelope &env) const
{
	if (IsNull() || env.IsNull())
	{
		return false;
	}
	return env.minX > minX &&
		env.maxX < maxX &&
		env.minY > minY &&
		env.maxY < maxY;
}

//判断一个点是否在该矩形中
bool GeoEnvelope::Contains(const GeoCoordinate &pt) const
{
	if (IsNull())
	{
		return false;
	}

	if (pt.x > minX && pt.x < maxX && pt.y > minY && pt.y < maxY)
	{
		return 1;
	}
	return 0;
}

//在矩形外返回0，否则返回1
bool GeoEnvelope::IsPointInRect(double x,double y) const
{
	if (IsNull())
	{
		return false;
	}

	if (x > minX && x < maxX && y > minY && y < maxY)
	{
		return 1;
	}
	return 0;
}

//计算两个矩形相交的部分
bool GeoEnvelope::Intersection(const GeoEnvelope& env,GeoEnvelope &envResult) const
{
	if (IsNull() || env.IsNull() || ! Intersects(env))
	{
		return false;
	}

	double intMinX = minX > env.minX ? minX : env.minX;
	double intMinY = minY > env.minY ? minY : env.minY;
	double intMaxX = maxX < env.maxX ? maxX : env.maxX;
	double intMaxY = maxY < env.maxY ? maxY : env.maxY;
	envResult.Init(intMinX, intMaxX, intMinY, intMaxY);

	return true;
}

//计算到另一个MBR的距离
double GeoEnvelope::DistanceTo(GeoEnvelope &env) const
{
	//如果相交，距离则为0
	if (Intersects(env))
	{
		return 0;
	}
	double dx = 0;
	if (maxX < env.minX)
	{
		dx = env.minX - maxX;
	}
	if (minX > env.maxX)
	{
		dx = minX - env.maxX;
	}

	double dy = 0;
	if (maxY < env.minY)
	{
		dy = env.minY - maxY;
	}
	if (minY > env.maxY)
	{
		dy = minY - env.maxY;
	}

	//如果其中之一为0，则计算水平或者垂直距离
	if (0.0 == dx)
	{
		return dy;
	}
	if (0.0 == dy)
	{
		return dx;
	}
	return sqrt(dx*dx + dy*dy);
}

//计算面积
double GeoEnvelope::GetArea() const
{
	if (IsNull())
	{
		return 0;
	}
	return GetHeight()*GetWidth();
}

//计算周长
double GeoEnvelope::Perimeter() const
{
	if (IsNull())
	{
		return 0;
	}
	return GetWidth()*2 + GetHeight()*2;
}

void GeoEnvelope::Translate(double transX, double transY)
{
	if (IsNull())
	{
		return;
	}
	minX += transX;
	maxX += transX;
	minY += transY;
	maxY += transY;
}

//测试是否包含
bool GeoEnvelope::Contains(double x, double y) const
{
	if (IsNull()) 
		return false;

	return x >= minX &&
		x <= maxX &&
		y >= minY &&
		y <= maxY;
}

//判断p1,p2构成的矩形和q1,q2构成的矩形是否相交
bool GeoEnvelope::Intersects(const GeoCoordinate &p1, const GeoCoordinate &p2, const GeoCoordinate &q1, const GeoCoordinate &q2)
{
	double minq = min(q1.x, q2.x);
	double maxq = max(q1.x, q2.x);
	double minp = min(p1.x, p2.x);
	double maxp = max(p1.x, p2.x);

	if( minp >= maxq )
		return false;
	if( maxp <= minq )
		return false;

	minq = min(q1.y, q2.y);
	maxq = max(q1.y, q2.y);
	minp = min(p1.y, p2.y);
	maxp = max(p1.y, p2.y);

	if( minp >= maxq )
		return false;
	if( maxp <= minq )
		return false;
	return true;
}
