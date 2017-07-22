/******************************************************************************
 Cambridge Silicon Radio Limited, a CSR plc group company iAP Application

 Copyright (c) 2012  Cambridge Silicon Radio Limited, a CSR plc group
 company.

 All rights reserved.

 This Software is protected by United Kingdom copyright laws and international
 treaties.  You may not reverse engineer, decompile or disassemble this
 Software.

 WARNING:
 This Software contains Cambridge Silicon Radio Limited's confidential and
 proprietary information. UNAUTHORIZED COPYING, USE, MODIFICATION,
 DISTRIBUTION, PUBLICATION, TRANSFER, SALE, RENTAL, REPACKAGING, REASSEMBLING
 OR DISCLOSURE OF THE WHOLE OR ANY PART OF THE SOFTWARE IS PROHIBITED AND MAY
 RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this Software without
 Cambridge Silicon Radio Limited's express written permission.   Use of any
 portion of the contents of this Software is subject to and restricted by your
 signed written agreement with Cambridge Silicon Radio Limited.
******************************************************************************/
#pragma once
#include "../../stdafx.h"

namespace iAP{

class ShuffleMode{
public:
	ShuffleMode();
	~ShuffleMode();
	static ShuffleMode ShuffleOff();
	static ShuffleMode ShuffleTracks();
	static ShuffleMode ShuffleAlbums();
	static ShuffleMode create(UINT8 shuffleMode);
	UINT8 get() const;
	BOOL operator==(const ShuffleMode& other) const;
private:
	explicit ShuffleMode(UINT8 shuffleMode);
	UINT8 shuffleMode_;
};

}//namespace iAP