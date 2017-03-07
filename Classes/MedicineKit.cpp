#include "MedicineKit.h"


MedicineKit::MedicineKit()
{
	goodvaule = 100;
}


MedicineKit::~MedicineKit()
{

}

bool MedicineKit::init()
{
	if (Sprite::initWithSpriteFrameName("ui/medicinekit.png"))
	{

		return true;
	}
	return false;
}

void MedicineKit::action(int minute, int exminute)
{

}