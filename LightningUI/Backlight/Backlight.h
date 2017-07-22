
#pragma once
#include "CspSocBacklight.h"

#define DEBUG_BKL						0


void BKL_InitBackLight(int iBKL);
void BKL_SetBackLight(int iBKL,BOOL bJump=FALSE, BOOL bLedAnti=TRUE);
int BKL_GetBackLight();
void BKL_TurnBKLSOn();
void BKL_TurnBKLSOff();