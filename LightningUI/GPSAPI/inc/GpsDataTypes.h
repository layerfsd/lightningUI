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

#ifndef _GPS_DATA_TYPES_H
#define _GPS_DATA_TYPES_H

#ifdef WINDOWS_SIMULATION
   #include <stdio.h>
#endif


// this file declare the necessary data structure for the gpsapi
#if !defined (STDTYPES)
#if !defined(STDTYPE_H_INCLUDED)
#ifndef SOCGPS_BOOL
#define SOCGPS_BOOL
typedef int BOOL;
#endif
#endif
#if !defined (TRUE)
#define TRUE ((BOOL)1)
#endif

#if !defined (FALSE)
#define FALSE ((BOOL)0)
#endif

#if !defined NULL
#define NULL (void *)(0)
#endif

#endif // !defined (STDTYPES)


#define GPS_STATUS_RUNNING			1
#define GPS_STATUS_POSITION_VALID	2
#define GPS_STATUS_INTERRUPT		4

#define DR_STATUS_POSITION_VALID    256
#define MAX_PRN 32

//------------------ virtual com port number definitions ---------------------
#define DEFAULT_GPS_COM_PORT		7

#pragma pack(push, 4)

typedef struct
{
    double ElevationMask;//in degree as unit.
    int DOPMask;
    int PowerMask;
    unsigned char available;
}MaskParam;

// Position in ECEF

typedef struct
{
    double x;
    double y;
    double z;
    unsigned char available;
}ECEFPOSITION;

// Position in Latitude, Longitude, and height

typedef struct
{
    double lat;
    double lon;
    double alt;
    unsigned char available;
}LLAPOSITION;

// UTC time

typedef struct
{
    short utcYY;	// Year
    short utcMon;	// Month
    short utcDD;	// Day
    short utcHH;	// Hours
    short utcMM;	// Minutes
    short daysOfYear;	// days into a year, less than 365 (366 for leap)
    double utcSS;	// Seconds ss.ss
    long timeOfDay;	// seconds into a day (less than 86400)
    unsigned char  available;
}UTCTIME;


// Fixed point almanac

typedef struct
{
    int svid;
    int weekNo;
    unsigned long ecc;
    unsigned long toa;
    long i0;
    long OMEGADOT;
    unsigned long sqrtA;
    long OMEGA0;
    long w;
    long M0;
    long af0;
    long af1;
    int health;
    unsigned char available;
}ALMANAC;

// Ephemeris data

typedef struct
{
    short	svid;
    unsigned short weekNo;
    unsigned short acc;	// user range accuracy units: n/a, range: 0 to 15
    // Eph is good when IODE2 equals to IODE3
    unsigned char IODE2;	// issue of data - ephemeris
    unsigned char IODE3;	// issue of data - ephemeris
    double M0;		// mean anomaly at reference time
    double deltaN;  // mean motion diff. from computed value
    double ecc;		// eccentricity
    double Ek;		// calculated in SatPosSpeedECEF()
    double sqrtA;	// square root of the semi-major axis
    double OMEGA0;	// longitude of ascending node
    double i0;		// inclination angle at reference time
    double w;		// argument of perigee
    double OMEGADOT;	// rate of right ascension
    double IDOT;		// rate of inclination angle
    double Cuc;		// amplitude of the cosine correction-latitude
    double Cus;		// amplitude of the sine correction-latitude
    double Crc;     // amplitude of the cosine correction-orbit R
    double Crs; 	// amplitude of the sine correction-orbit R
    double Cic;		// amplitude of the cosine correction-inclination
    double Cis;		// amplitude of the sine correction-inclination
    // Clock parameters
    double	GroupDelay;
    double  af0;
    double  af1;
    double  af2;
    unsigned long 	aodc;	// Age of data clock - not required ??
    int health;
    unsigned long toc;		// clock data reference time in seconds
    unsigned long toe;		// reference time - ephemeris

    unsigned char		available;
    unsigned char		repeat;
    unsigned char		update;		// the ephemeris is updated since this startup.
    unsigned char FitInterval;	//Curve fit interval flag
}EPHEMERIS;

#define TYPES_DEFINED_BY_GPS

// Ionospheric parameters

typedef struct
{
    double    a0;  // 2**-30, alpha0 constant term     sec
    double    a1;  // 2**-27, alpha1 first order term  sec/semicircles
    double    a2;  // 2**-24, alpha2 second order term sec/(semic.)**2
    double    a3;  // 2**-24, alpha3 third order term  sec/(semic.)**3
    double    b0;  // 2**11, beta0  constant term     sec
    double    b1;  // 2**14, beta1  first order term  sec/simic.
    double    b2;  // 2**16, beta2  second order term sec/(semic.)**2
    double    b3;  // 2**16, beta3  third order term  sec/(semic.)**3
    unsigned char	available; // 1, availble
}IONOPARAM;

// NMEA parameters

typedef struct
{
    int gga;
    int gll;
    int gsa;
    int gsv;
    int mss;
    int rmc;
    int vtg;
    int zda;
    unsigned char available;
} NMEAParam;


typedef struct _sniff_param_mc
{
    unsigned long sniff_run_timeout;
    unsigned long s2_sniff_run_timeout;
    unsigned long sniff_sleep_timeout;
    unsigned long s2_sniff_sleep_timeout;
    unsigned long long_sniff_time;
    unsigned long s2_con;
    unsigned long s3_con;
    unsigned long reserved1;   //enable
    unsigned long reserved2;   // stime
    unsigned long reserved3;   //etime
}SNIFF_PARAM_MC, *pSNIFF_PARAM_MC;


typedef struct
{
    int     prn;        // SBAS satellite number
    double  dLongitude; // sbas satellite longitude
} SBAS_NEW_PRN;

typedef struct {
	double	_LastGoodPosTime;
	int _LastGoodPosWeekno;
	ECEFPOSITION 	_LastGoodPos;
}LastGoodPosInfo;
typedef struct {
	BOOL SvPrdEphStatus[MAX_PRN+1];
	BOOL SvPrdEphUsedStatus[MAX_PRN+1];
}GPS_IF_SAT_INFO,*PGPS_IF_SAT_INFO;
#pragma pack(pop)

#endif
