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
#include "string"

namespace iAP{

class BrowserListener;

enum RootRecordID{
	ROOT_RECORD_ID_AUDIO		= 0x00,
	ROOT_RECORD_ID_VIDEO		= 0x01,
	ROOT_RECORD_ID_TOTAL	= 0x02,
};

enum TopLevelRecordID{
	TOP_LEVEL_RECORD_ID_PLAYLIST	= 0x00,
	TOP_LEVEL_RECORD_ID_ARTIST		= 0x01,
	TOP_LEVEL_RECORD_ID_ALBUM		= 0x02,
	TOP_LEVEL_RECORD_ID_GENRE		= 0x03,
	TOP_LEVEL_RECORD_ID_TRACK		= 0x04,
	TOP_LEVEL_RECORD_ID_COMPOSER		= 0x05,
	TOP_LEVEL_RECORD_ID_AUDIOBOOK	= 0x06,
	TOP_LEVEL_RECORD_ID_PODCAST	= 0x07,
	TOP_LEVEL_RECORD_ID_TOTAL		= 0x08,
	TOP_LEVEL_RECORD_ID_INVALID		= 0xFF,
};

class iBrowser{
public:
	iBrowser();
	virtual ~iBrowser();
	/**
	Start browser, let iPod to be prepared to browsing
	The status will be returned by BrowserListener::onExtendedUIModeReady with parameter extended UI mode availabe or not.
	*/
	virtual void start()=0;
	/**
	Register the browser listener, GUI need to listen the browse result of each open/up
	@param listener pointer to the BrowserListener object
	*/
	virtual void registerListener( BrowserListener* listener)=0;
	/**
	Go into one item, display the child list if there is
	The status will be returned by BrowserListener::onOpenFinished with parameter total number of records in current item.
	@param index the item id, which is relative to the first item, start from 0
	*/
	virtual void open(UINT32 index)=0;
	/**
	Go up to parent item, display the item list parent belongs to
	The status will be returned by BrowserListener::onOpenFinished with parameter total number of records in parent item.
	*/
	virtual void up()=0;
	/**
	Go to root, display the item list in the root
	The status will be returned by BrowserListener::onOpenFinished with parameter total number of records in root item.
	*/
	virtual void root()=0;
	/**
	Get the item's name in current folder
	The status will be returned by BrowserListener::onRecordReady with parameter record index and record name or id.
	@param startIndex start item id which user wants to get, starts from 0
	@param count item count which user wants to get, it should be larger or equal to 1, default is 1
	*/
	virtual void getRecords(UINT32 startIndex, UINT32 count=1)=0;
	/**
	Check if it is root or not, self defined
	@return BOOL
	*/
	virtual BOOL isRoot()=0;
	/**
	Check if it is top level, it is the child of root, TopLevel is from iAP spec categary
	@return BOOL
	*/
	virtual BOOL isTopLevel()=0;
	/**
	Check if it is playable item or not, it is the last level can be browsed
	@return BOOL
	*/
	virtual BOOL isPlayable()=0;
	/**
	Check if it is track or not, Track is from iAP spec categary
	@return BOOL
	*/
	virtual BOOL isTrack()=0;
	/**
	Get extended UI mode available variable's value, use to check if device is ready to browse or not
	@return BOOL
	*/
	virtual BOOL getExtendedUIModeAvailable()=0;
	/**
	Get current hierarchy type, which will be Audio or Video
	@return 0 for root, 1 for audio, 2 for video.
	*/
	virtual UINT8 getHierarchyType()=0;
	/**
	Get current menu level, which identify the browse track according to the categary grade
	Currently it is just used to verify in test cases
	@return Self defined menu level
	*/
	virtual UINT8 getMenuLevel()=0;
	/**
	Get current menu depth, which identify the browse track according to the categary grade
	Currently it is just used to verify in test cases
	@return Self defined menu depth
	*/
	virtual UINT8 getMenuDepth()=0;
	/**
	Request APPLE device to run a named application with applicationID.
	@return BOOL
	*/
	virtual BOOL launchApplication(std::string& applicationID, BOOL bAlert)=0;
	/**
	Request APPLE device to run a named application with applicationID.
	@return BOOL
	*/
	virtual void getCurrentApplicationName()=0;
};

class BrowserListener{
public:
	BrowserListener(void){};
	virtual ~BrowserListener(void){};
	/**
	Notify GUI the record is returned by iPod
	@param index the record index
	@param record record name
	*/
	virtual void onRecordReady(UINT32 index, std::string record)=0;
	/**
	Notify GUI the record is returned by iPod
	@param index the record index
	@param record record id: RootRecordID or TopLevelRecordID
	*/
	virtual void onRecordReady(UINT32 index, RootRecordID recordId)=0;
	/**
	Notify GUI the record is returned by iPod
	@param index the record index
	@param record record id: RootRecordID or TopLevelRecordID
	*/
	virtual void onRecordReady(UINT32 index, TopLevelRecordID recordId)=0;
	/**
	Notify GUI the total number of records in the current display folder
	@param numberofRecords total number of records
	*/
	virtual void onOpenFinished(UINT32 numberofRecords)=0;
	/**
	Notify GUI the extended ui mode available or not
	@param isExtendedUIModeAvailable TRUE or FALSE
	*/
	virtual void onExtendedUIModeReady(BOOL isExtendedUIModeAvailable)=0;
	/**
	Notify GUI the application status
	@param isApplicationLaunched TRUE or FALSE
	*/
	virtual void onApplicationLaunched(BOOL isApplicationLaunched)=0;
};

}//namespace iAP