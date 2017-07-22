/******************************************************************************
Cambridge Silicon Radio Limited, a CSR plc group company [GPS Software]

Copyright (c) [2001] ¨C [2010] Cambridge Silicon Radio Limited, a CSR plc group company.

All rights reserved.

This Software is protected by United Kingdom copyright laws and international 
treaties.  You may not reverse engineer, decompile or disassemble this 
Software.

WARNING:
This Software contains Cambridge Silicon Radio Limited¡¯s confidential and 
proprietary information. UNAUTHORIZED COPYING, USE, MODIFICATION, 
DISTRIBUTION, PUBLICATION, TRANSFER, SALE, RENTAL, REPACKAGING, REASSEMBLING 
OR DISCLOSURE OF THE WHOLE OR ANY PART OF THE SOFTWARE IS PROHIBITED AND MAY 
RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this Software without 
Cambridge Silicon Radio Limited¡¯s express written permission.   Use of any 
portion of the contents of this Software is subject to and restricted by your 
signed written agreement with Cambridge Silicon Radio Limited. 
******************************************************************************/
/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. Platform Software               *
 *                                                                         *
 *    Copyright (c) 2008 by SiRF Technology, Inc.  All rights reserved.    *
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology, Inc.'s confidential and      *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.'s  express written            *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your written agreement with          *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 ***************************************************************************/

/* File: GpsAPI.h
 * --------------
 * This file specifies the API defined by the GpsAPI.lib.  This API is designed to be used to
 * communicate data and state values between upper layer applications and the Centrality GPS
 * baseband.
 *
 */

#ifndef _GPSAPI_H_
#define _GPSAPI_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "GpsDataTypes.h"


    /* Function: StartGpsApi
     * ---------------------
     * This function must be called first in any application which attemps to use the GPS API.  It
     * opens GPS com port. All API functions are based on the handle of this com port. Before
     * a call to this function, no API functions will work.
     */
    BOOL StartGpsApi();

    /* Function: StopGpsApi
     * ---------------------
     * This function must be called first before application exit.
     */
    BOOL StopGpsApi();


// Satellite Data Functions
// ------------------------

    /* Function: GetAlmanac
     * --------------------
     * This function completes a struct ALMANAC, passed in by address, for the satellite with the
     * given svid.  The function returns TRUE if successful.
     */
    BOOL GetAlmanac(int svid, ALMANAC* almanac);


    /* Function: GetEphemeris
     * ----------------------
     * This function completes a struct EPHEMERIS, passed in by address, for the satellite with
     * the given svid.  The function returns TRUE if sucessful.
     */
    BOOL GetEphemeris(int svid, EPHEMERIS* ephemeris);


    /* Function: GetSatelliteCN0Ratio
     * ------------------------------
     * This function returns the ratio of carrier and noise of the specified satellite.
     */
    unsigned char GetSatelliteCN0Ratio(int svid);


    /* Function: GetSatelliteAzimuth
     * ------------------------------
     * This function returns the azimuth of the specified satellite.
     */
    int GetSatelliteAzimuth(int svid);


    /* Function: GetSatelliteElevation
     * ------------------------------
     * This function returns the elevation of the specified satellite (in degrees).
     */
    int GetSatelliteElevation(int svid);


    /* Function: GetSatelliteECEF
     * --------------------------
     * This function completes a struct ECEFPOSITION, passed in by address, for the satellite
     * with the specified svid.  The function returns TRUE if successful.
     */
    BOOL GetSatelliteECEF(int svid, ECEFPOSITION* ecefpos);


    /* Function: GetChannelSat
     * --------------------------
     * This function returns the satellite PRN of the corresponding channel
     */
    int GetChannelSat(short channel);//By Qiu Zongde 04/08/03


    /* Function: GetChannelState
     * -----------------------
     * This function returns the tracking status of the satellite with the specified channel.
     * The format of the returned char willbe as follows:
     * 	Bit 0: bit sync has been obtained
     * 	Bit 1: fine track status has been obtained
     * 	Bit 2: acquisition has been secure
     * 	Bit 3: trickle power flag is being used
     * 	Bit 4: bit deduction is being used
     */
    unsigned short GetChannelState(short channel);//By Qiu Zongde 04/08/03


    /* Function: GetChannelPower
     * --------------------------
     * This function returns the signal power of the corresponding channel
     */
    unsigned long GetChannelPower(short channel);//By Qiu Zongde 04/08/03


    /* Function: GetIonoData
     * ------------------------------
     * This function completes a struct IONOPARAM, passed in by address, for the satellite with
     * the given svid.  The function returns TRUE if successful.
     */
    BOOL GetIonoData(IONOPARAM* ionoparam);


    /* Function: GetPosition
     * ---------------------
     * This function completes a struct LAAPOSITION, passed in by address, for the satellite
     * with the specified svid.  The function returns TRUE if successful.
     */
    BOOL GetPosition(LLAPOSITION* position);


// DOP functions
// -----------------------

    /* Function: GetGDOP
     * -----------------
     * This function returns the geometric dilution of precistion of the receiver.
     */
    double GetGDOP();


    /* Function: GetPDOP
     * -----------------
     * This function returns the positional dilution of precistion of the receiver.
     */
    double GetPDOP();


    /* Function: GetHDOP
     * -----------------
     * This function returns the horizontal dilution of precistion of the receiver.
     */
    double GetHDOP();


    /* Function: GetVDOP
     * -----------------
     * This function returns the vertical dilution of precistion of the receiver.
     */
    double GetVDOP();

    /* Function: GetTDOP
     * -----------------
     * This function returns the dilution of precistion of time of the receiver.
     */
    double GetTDOP();


    /* Function: GetCurrentUTC
     * -------------------------
     * This function completes a struct UTCTIME, passed in by address.  The function
     * returns TRUE if successful.  If this is called prior to the GPS system's successful
     * calculation of GPS UTC, the system time (e.g. the time of the WinCE system) will
     * be returned.  Once the actual GPS UTC time is determined, this function will
     * return the GPS UTC time.  Because this function is suseptible to system latency,
     * it is only accurate to the nearest second.
     */
    BOOL GetCurrentUTC(UTCTIME* time);


    /* Function: GetGroundSpeed
     * ------------------------
     * This function returns the ground speed of the receiver in knots.
     */
    double GetGroundSpeed();


    /* Function: GetMaskParams
     * -----------------------
     * This function gets the masking parameters including DOP, Elevation, and Power masks
     * and completes the maskparam passed in by address.
     */
    BOOL GetMaskParams(MaskParam* maskparam);


    /* Function: GetCrystalOffset
     * --------------------
     * This function returns the receiver's crystal offset in Hz.
     */
    double GetCrystalOffset();


    /* Function: GetNMEAType
     * ----------------------
     * This function gets the output characteristics of the GPS receiver NMEA messages.  This
     * function will return TRUE upon success.
     */
    BOOL GetNMEAType(NMEAParam *nmea);


    /* Function: GetTemperature
     * ----------------------
     * This function gets the temperature from the temperature sensor.
     */
    double GetTemperature();


    /* Function: GetReceiverStatus
     * ------------------------
     * This function returns the current current status of the GPS.
     */
    unsigned long GetReceiverStatus();


    /* Function: SetAlmanac
     * --------------------
     * This function sets the almanac data for a satellite. The function returns TRUE if successful.
     */
    BOOL SetAlmanac(ALMANAC *newalmanac);


    /* Function: SetEphemeris
     * ----------------------
     * This function sets the ephemeris data for a satellite.  This can be used
     * to expedite TTFF.
     */
    BOOL SetEphemeris(EPHEMERIS* newephem);


    /* Function: SetPosition
     * ---------------------
     * This function sets the position of the receiver in terms of latitude, longitude,
     * and altitude.  These values are passed in using an LAAPOSITION struct.  In order
     * for these parameters to get used by the system, the user must set the "available"
     * field to TRUE.  The function returns TRUE if successful.
     */
    BOOL SetPosition(LLAPOSITION* position);


    /* Function: SetReceiverUTC
     * -------------------------
     * This function sets the UTC time of the system using a UTCTIME struct.  When the
     * GPS system is started, it will use the system time as the assumed time.  After
     * the GPS UTC time is determined, this time will be used for further GPS calculations.
     * Calling this function after a position fix has been attained will only have teh effect
     * of changing the system (e.g. WinCE) time.  The GPS time will not be effected.  This
     * funtions returns TRUE if successful.
     */
    BOOL SetReceiverUTC(UTCTIME* time);


    /* Function: SetMaskParams
     * -----------------------
     * This function sets the masking parameters including DOP, Elevation, and Power masks
     * using a MaskParam struct.  In order for these parameters to get used by the system,
     * the user must set the "available" field to TRUE.  This function will return TRUE upon success.
     */
    BOOL SetMasks(MaskParam* masks);


    /* Function: SetNMEAType
     * ----------------------
     * This function sets the output characteristics of the GPS receiver NMEA messages using the
     * NMEAPARAM structure.  The user must specify the frequency (in seconds per message) for each
     * NMEA message in the struct.  In order for these parameters to get used by the system, the
     * user must set the "available" field to TRUE.  This function will return TRUE upon success.
     */
    BOOL SetNMEAType(NMEAParam *nmea);


    /* Function: GPSEnableWAAS
     * ---------------------
     * This function enable or disable the WAAS.  The function returns TRUE if successful.
     */
//BOOL GPSEnableWAAS(BOOL Enable);


    /* Function: SetSBASType
     * ----------------------
     * This function set the SBAS type:
     * sbastype:
     * 0 -- NONE
     * 1 -- WAAS Auto Selection
     * 2 -- EGNOS Auto Selection
     * 3 -- ESTB Auto Selection
     * 4 -- MSAS Auto Selection
     * >4 -- SBAS SVID
     */
    BOOL SetSBASType(int sbastype);
    /* Function: SetNewSBASSat
     * ----------------------
     * This function enable the extension of future SBAS satellites.
    */
    BOOL SetNewSBASSat(SBAS_NEW_PRN newSbasPrn);

    /* Function: SetChannelNumber
     * ---------------------
     * This function sets the channel number of hot/warm/cold start to GPS
     * The valid range to set is [1, 32]
     */
    BOOL SetChannelNumber(int chnumber);

    /* Function: GetChannelNumber
     * ---------------------
     * This function gets the channel number from GPS regarding the start type
     * iscoldstart:
     * true -- the channel number of cold start
     * flase -- the channel number of hot/warm start
     */
    int GetChannelNumber(BOOL iscoldstart);
    /* Function: GetValidEphNumber
     * --------------------------
     * This function get current valid satellite ephemeris number
     */
    int GetValidEphNumber(double ValidEphTimeDiff);
    /* Function: GetUpdatedEphNumber
     * --------------------------
     * This function get satellite number which ephemeris is updated.
     */
    int GetUpdatedEphNumber();
    /* Function: GetBitSyncNumber
     * --------------------------
     * This function get current bit sync satellite number
     */
    int GetBitSyncNumber();

    /* Function: GetUsedSatNumber
     * -------------------------
     * This function return the number of the satellites used in PVT
     */
    int GetUsedSatNumber();



    /* Function: SetDspMipsPercent
     * ---------------------
     * This function sets the maximum allowed dsp mips percentage to GPS
     * The valid range to set is (0, 100]
     */
    BOOL SetDspMipsPercent(int dspmipspercent);
    /* Function: SetNMEARate
     * ---------------------
     * This function set NMEA output rate, [1,5]Hz is accepted, other is invalid currently.
     */
    BOOL SetNMEARate(int NMEARate);


    /* Function: GetDspMipsPercent
     * ---------------------
     * This function gets the maximum allowed dsp mips percentage from GPS
     * Return value x, which range is (0, 100] and it means x% of maximum of dsp misp is allowed
     */
    int GetDspMipsPercent();

    /* Function: GetMaxDspMips
     * ---------------------
     * This function gets the maximum allowed dsp mips from GPS
     * Return value x, which means the default maximum dsp mips is (x/100) MHz
     */
    int GetMaxDspMips();
    /* Function: SetStaticConstraint
     * ---------------------
     * This function enables the detection of stop/move status and apply it to the PVT result
     * If this function is enabled, if stop status is detected, the output position will fix
     * to a static position until the movement of the receiver is detected.
     * This function will return TRUE upon success.
     */
    BOOL SetStaticConstraint(BOOL bConstraint);

    /* Function: GPSHotStart
     * ---------------------
     * This function starts up the GPS baseband in Hot Start mode.  This function takes as its
     * only parameter a COM port which has been opened by the application where NMEA data is
     * expected.  If NULL is passed in as the HANDLE, GPS will start, but no NMEA information
     * will be exported.  The Centrality GPS virtual COM port is COM7.  It is always possible to
     * retrieve GPS NMEA information through COM7.  The handle specified here is meant to be used for
     * ports other than COM7.  This function will return TRUE upon success.
     */
    BOOL GPSHotStart();

    /* Function: GPSWarmStart
     * ----------------------
     * This function starts up the GPS baseband in Warm Start mode.  This function takes as its
     * only parameter a COM port which has been opened by the application where NMEA data is
     * expected.  If NULL is passed in as the HANDLE, GPS will start, but no NMEA information
     * will be exported.  The Centrality GPS virtual COM port is COM7.  It is always possible to
     * retrieve GPS NMEA information through COM7.  The handle specified here is meant to be used for
     * ports other than COM7.  This function will return TRUE upon success.
     */
    BOOL GPSWarmStart();


    /* Function: GPSColdStart
     * ----------------------
     * This function starts up the GPS baseband in Cold Start mode.  This function takes as its
     * only parameter a COM port which has been opened by the application where NMEA data is
     * expected.  If NULL is passed in as the HANDLE, GPS will start, but no NMEA information
     * will be exported.  The Centrality GPS virtual COM port is COM7.  It is always possible to
     * retrieve GPS NMEA information through COM7.  The handle specified here is meant to be used for
     * ports other than COM7.  This function will return TRUE upon success.
     */
    BOOL GPSColdStart();


    /* Function: GPSFlexibleHotStart
     * ---------------------
     * This function is the same as hot start with the exception that it provide flexible control
     * to the frequency search range and first timeout time to switch satellites.
     */
    BOOL GPSFlexibleHotStart(int SearchRange, int Timeout, int StartType);

    /* Function: GPSRealShutdown
     * ---------------------
     * This function shuts down the GPS receiver by real stop.  The function returns TRUE if successful.
     */
    BOOL GPSRealShutdown();

    /* Function: GPSShutdown
     * ---------------------
     * This function shuts down the GPS receiver.  The function returns TRUE if successful.
     */
    BOOL GPSShutdown();


    /* Function: GPSReset
     * ------------------
     * This function resets the GPS receiver.  Tracking will cease and restart with the default parameters.
     * This call is functionally equivalent to closing the COM port from a previous start (or calling
     * GPSShutdown) and then calling GPSHotStart() from this API.  If failure occurs, NULL is returned.
     */
    BOOL GPSReset();


    /* Function: AllChSearchOneSV
     * --------------------------
     * This function sets the GPS baseband into a special mode where all channels are used to track one
     * specific satellite.  The function returns TRUE if successful.
     */
    BOOL AllChSearchOneSV(int SV);


    /* Function: CheckParameter
     * --------------------------
     * This function check the existance and validation of the parameters saved, return TRUE if exist
     * and valid, FALSE otherwise
     */
    BOOL CheckParameter();


    /* Function: RemoveParameter
     * --------------------------
     * This function remove the saved parameters or set the flag as invalid, return TRUE on success or
     * parameter does not exist, FALSE otherwise
     */
    BOOL RemoveParameter();


    /* Function: SaveParam
     * --------------------------
     * This function will save parameters into nandflash, return TRUE on success, FALSE otherwise
     */
    BOOL SaveParam();

    /* Function: EnableTestParamSave
     * --------------------------
     * This function enables or disables test param file save
     */
    BOOL EnableTestParamSave(BOOL Enable);

    /* Function: GetGpsVersion
     * --------------------------
     * This function return the gps version message in pGpsVersion(200bytes), see "V6.0-B113MR1a bn0.4"
     */

    BOOL GetGpsVersion(char* pGpsVersion);

    /* Function: GetAlmanacAge
     * --------------------------
     * This function returns the age of Almanac from current time in unit of week.
     * The age of Amanac is calculated from the nearest Almanac time
     * No weekno wrap around case has been handeled yet.
     */
    long GetAlmanacAge();

    /* Function: GetParamAge
     * --------------------------
     * This function returns the age of parameter from current time in unit of second.
     */
    long GetParamAge();

    /* Function: GetFreqOffsetQuality
     * --------------------------
     * This function returns the quality of frequent offset calculation
     * return value:
     * 0 -- quality not available
     * 1 -- quality excellent (exact match)
     * 2 -- quality good (confidence in interpolation)
     * 3 -- quality fair (less confidence in interpolation/extrapolation)
     * 4 -- quality bad  (no confidence in interpolation/extrapolation)
     */
    long GetFreqOffsetQuality(long tempurature);

    /* Function: SetTcxoXoPpmSetting
     * --------------------------
     * This function set TCXO/XO ppm setting in firmware.
     */
    BOOL SetTcxoXoPpmSetting(double TcxoXoPpmSetting);

    /* Function: GetValidEphNumber
     * --------------------------
     * This function get current valid satellite ephemeris number
     */
    int GetValidEphNumber(double ValidEphTimeDiff);

    /* Function: GetUpdatedEphNumber
     * --------------------------
     * This function get satellite number which ephemeris is updated.
     */
    int GetUpdatedEphNumber();


    /* Function: SetMultiCopyEphDecode
     * --------------------------
     * This function set multicopy eph decode mode. 0: not to use.
     */
    BOOL SetMultiCopyEphDecode(long EphDecodeMode);

    /* Function: EnableBKLogData
     * --------------------------
     * This function set enable/disable background data loging function.
     */
    BOOL EnableBKLogData(BOOL bEnable, char *pBacksaveFolder);

    /* Function: GetRcvrPwrConsumptionMode
     * ------------------------
     * This function returns the current receiver power consumption mode.
     * 1: high power consumption mode, match filter available.
     * 0: low power consumption mode, match filter unavailable.
     */
    unsigned long GetRcvrPwrConsumptionMode();

    /* Function: SetRcvrPwrConsumptionMode
     * --------------------
     * This function sets the receiver power consumption mode. If pwr_consumption_mode is 1, receiver will switch to
     * high powr consumption mode, match filter available.
     * The function returns TRUE if successful.
     */
    BOOL SetRcvrPwrConsumptionMode(BOOL pwr_consumption_mode);

    /* Function: EnableFastHotStart
     * --------------------
     * This function enable/disable FastHotStart. If bEnable is 0, FastHotStart will be disabled, RF will be power off
     * and the strategy for FastHotStart will be disabled corresponding. By now, only disable function is supported.
     * The function returns TRUE if successful.
     */
    BOOL EnableFastHotStart(BOOL bEnable);

    /* Function: InstantFixSGEEEnable
     * --------------------------
     * This function is to notify disable/enable/update InstantFixI by applications.
     */
    BOOL  InstantFixSGEEEnable(int OPTYPE);

    /* Function: InstantFIxCGEEEnable
     * --------------------------
     * This function is to disable/enable/update InstantFixII by applications.
     */
    BOOL InstantFixCGEEEnable(int OPTYPE_II);

    /* Function: InstantFixCGEETest
     * --------------------------
     * This function is to help debug InstantFixII by applications.
     */
    BOOL InstantFixCGEETest(int OPTYPE_II_test);

    /* Function: InstantFixSGEEClkUpdate
     * --------------------------
     * This function is to disable\enable\enable&use_until_update InstantFixI by applications.
     */
    BOOL InstantFixSGEEClkUpdate(int OPTYPE_Clk_Update);

    /* Function: IFStatus
     * --------------------------
     * This function is to check InstantFixI\InstantFixII status by applications.
     */
    unsigned short IFStatus();

    /* Function: DMAEnable
     * --------------------------
     * This function is to disable\enable\enable DMA Mode by applications.
     */
    BOOL DMAEnable(int DMA_Enable);
    
    /* Function: MiniGpsEnable
     * --------------------------
     * This function is to enable\disable Ephemeris downloader and InstantFixII feature of Mini-Gps by applications.
     */
    BOOL MiniGpsEnable(int MGEphDldEnable, int MGInstantFixIIEnable);
    
    /* Function: GetMiniGpsSetting
     * --------------------------
     * This function will get the Ephemeris downloader and InstantFixII configuration in GPS driver. Bit0 indicate 
     * Ephemeris downloader enable/disable, while bit1 indicate InstantFixII enable/disable.
     */
    int GetMiniGpsSetting();

    /* Function: SetFactoryStaus
     * --------------------------
     * This function is to set device to factory status, bin file in residentflash and data in exchangable data buffer
     * will be cleared according to the setting.
     */
    BOOL SetFactoryStaus(int BinFileClean, int ExchgBuffClean);
    
    /* Function: GetIFSatInfo
     * --------------------------
     * This function will get the InstantFixII Server/InstantFixII Client ephemeris information:
     * one of them indicate whether the predicated ephemeris is available; while the other one indicate whether the 
     * predicated ephemeris is used for fix position.
     */
    BOOL GetIFSatInfo(GPS_IF_SAT_INFO *pGpsIFSatInfo,int OPTYPE_IF);

	  /* Function: SetGloRunningMode
     * --------------------------
     * This function will set the glonass running mode:
     * TRUE: only run glonass 
     * FALSE: run GPS + Glonass mixed mode.
     */
    BOOL SetGnssRunningMode(short GnssMode);

#if ENABLE_SINS
/* Function: EnableDrModule
 * --------------------------
 * This function is to provide the interface to enable/disable DR module.
 */
    BOOL EnableDrModule(BOOL bEnable);

/* Function: ResetDrModule
 * --------------------------
 * This function is to provide the interface to reset DR module.
 * DR will be forced to do initialization again.
 * For in-dash version, after this interface is called, the higher application must wait a moment until the lower 3 bits of return value of DR_GetStatus is greater than 1.
 */
    BOOL ResetDrModule();

/* Function: GetDrStatus
 * --------------------------
 *  This function is to provide the interface to retrieve DR module status.
 */
    unsigned int GetDrStatus();
    unsigned int OnExternalImuEvent(DWORD code, DWORD status);
	BOOL DrApiFunction(void *pBufIn,  unsigned int LenIn, void *pBufOut, unsigned int LenOut);
#endif
#ifdef __cplusplus
}

#endif

#endif // _GPSAPI_H_
