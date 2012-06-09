
#include <iostream>
using namespace std;

#include "math/math_lib.h"

using namespace math;

int main()
{
	AxisAlignedBox aab;
	Math math;
	Vector2 v2;
	Vector4 v4;
	Sphere sphere;
	Ray ray;
	PlaneBoundedVolume planbv;
	Plane;

	Quaternion quat;
	Matrix3 rotMat;
	Vector3 v3(1, 1, 1);

	quat.FromAngleAxis(Radian(Math::DegreesToRadians(30)), Vector3::UNIT_Y);
	quat.ToRotationMatrix(rotMat);

	Vector3 vt1 = quat * v3;
	Vector3 vt2 = rotMat * v3;

	cout << vt1 << endl;
	cout << vt2 << endl;

	return 0;
}