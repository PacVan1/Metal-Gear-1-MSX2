#include "precomp.h"
#include "SoldierTypes.h"

#include "Soldiers.h" 

SoldierTypes::SoldierTypes()
{
	mTypeData[SOLDIER_TYPES_GRAY].mPalette		= ColorPalette8("assets/color_palettes/gray_soldier.cpalette");
	mTypeData[SOLDIER_TYPES_GRAY].mAlertLevel	= ALERT_LEVELS_LOW;
	mTypeData[SOLDIER_TYPES_GRAY].mSpeed		= 0.08f;
	mTypeData[SOLDIER_TYPES_GRAY].mMaxHealth	= 3;

	mTypeData[SOLDIER_TYPES_RED ].mPalette		= ColorPalette8("assets/color_palettes/damaged.cpalette");
	mTypeData[SOLDIER_TYPES_RED ].mAlertLevel	= ALERT_LEVELS_HIGH;
	mTypeData[SOLDIER_TYPES_RED ].mSpeed		= 0.10f;
	mTypeData[SOLDIER_TYPES_RED ].mMaxHealth	= 4;
}
