#ifndef __FEATURE_DEFAULT_H__
#define __FEATURE_DEFAULT_H__

//����Ĭ��ƽ̨����
#ifndef CVTE_MODEL_TYPE
#define CVTE_MODEL_TYPE	CVTE_MODEL_TYPE_U701P_C
#endif

//����Ĭ����ʾ����
#ifndef CVTE_DEF_DISPLAY_TYPE
#define CVTE_DEF_DISPLAY_TYPE CVTE_DISPLAY_TYPE_U701P
#endif

//����Ĭ�����LED������
#ifndef CVTE_DEF_LED_TYPE
#define CVTE_DEF_LED_TYPE CVTE_LED_TYPE_U701P
#endif

//����Ĭ�ϴ�������
#ifndef CVTE_DEF_TOUCH_TYPE
#define CVTE_DEF_TOUCH_TYPE CVTE_TOUCH_TYPE_U701P
#endif

//����Ĭ����ƵIC
#ifndef CVTE_DEF_AUDIO_TYPE
#define CVTE_DEF_AUDIO_TYPE CVTE_AUDIO_TYPE_ST7418
#endif

//����Ĭ�ϵ�UI���
#ifndef CVTE_DEF_UI_STYLE
#define CVTE_DEF_UI_STYLE	CVTE_UI_STYLE_LIGHTNINGUI
#endif

////////////////////////////////////////////////////////////////////////////////
// RADIO OPTIONS
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//EUROPE
//ŷ������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_EUROPE_FM_FREQ_MIN
#define CVTE_DEF_RADIO_EUROPE_FM_FREQ_MIN				87500000    //Hz
#endif

//ŷ������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_EUROPE_FM_FREQ_MAX
#define CVTE_DEF_RADIO_EUROPE_FM_FREQ_MAX				108000000   //Hz
#endif

//ŷ������FM����Ƶ��
#ifndef CVTE_DEF_RADIO_EUROPE_FM_STEP
#define CVTE_DEF_RADIO_EUROPE_FM_STEP					5          // KHz
#endif

//ŷ������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_EUROPE_AM_FREQ_MIN
#define CVTE_DEF_RADIO_EUROPE_AM_FREQ_MIN				522000
#endif

//ŷ������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_EUROPE_AM_FREQ_MAX
#define CVTE_DEF_RADIO_EUROPE_AM_FREQ_MAX				1620000
#endif

//ŷ������AM����Ƶ��
#ifndef CVTE_DEF_RADIO_EUROPE_AM_STEP
#define CVTE_DEF_RADIO_EUROPE_AM_STEP					9          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//U.S.A 1
//����1����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_USA_FM_FREQ_MIN
#define CVTE_DEF_RADIO_USA_FM_FREQ_MIN					87500000    //Hz
#endif

//����1����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_USA_FM_FREQ_MAX
#define CVTE_DEF_RADIO_USA_FM_FREQ_MAX					107900000   //Hz
#endif

//����1����FM����Ƶ��
#ifndef CVTE_DEF_RADIO_USA_FM_STEP
#define CVTE_DEF_RADIO_USA_FM_STEP						10          // KHz
#endif

//����1����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_USA_AM_FREQ_MIN
#define CVTE_DEF_RADIO_USA_AM_FREQ_MIN					530000
#endif

//����1����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_USA_AM_FREQ_MAX
#define CVTE_DEF_RADIO_USA_AM_FREQ_MAX					1710000
#endif

//����1����AM����Ƶ��
#ifndef CVTE_DEF_RADIO_USA_AM_STEP
#define CVTE_DEF_RADIO_USA_AM_STEP						10          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//U.S.A 2
//����2����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_USA2_FM_FREQ_MIN
#define CVTE_DEF_RADIO_USA2_FM_FREQ_MIN					87500000    //Hz
#endif

//����2����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_USA2_FM_FREQ_MAX
#define CVTE_DEF_RADIO_USA2_FM_FREQ_MAX					107900000   //Hz
#endif

//����2����FM����Ƶ��
#ifndef CVTE_DEF_RADIO_USA2_FM_STEP
#define CVTE_DEF_RADIO_USA2_FM_STEP						20          // KHz
#endif

//����2����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_USA2_AM_FREQ_MIN
#define CVTE_DEF_RADIO_USA2_AM_FREQ_MIN					530000
#endif

//����2����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_USA2_AM_FREQ_MAX
#define CVTE_DEF_RADIO_USA2_AM_FREQ_MAX					1710000
#endif

//����2����AM����Ƶ��
#ifndef CVTE_DEF_RADIO_USA2_AM_STEP
#define CVTE_DEF_RADIO_USA2_AM_STEP						10          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//EAST ERNEUROPE	��ӦMCU OIRT
//��ŷ����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MIN
#define CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MIN		65000000    //Hz
#endif

//��ŷ����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MAX
#define CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_FREQ_MAX		74000000   //Hz
#endif

//��ŷ����FM����Ƶ��
#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_STEP
#define CVTE_DEF_RADIO_EAST_ERNEUROPE_FM_STEP			1          // KHz
#endif

//��ŷ����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MIN
#define CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MIN		522000
#endif

//��ŷ����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MAX
#define CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_FREQ_MAX		1629000
#endif

//��ŷ����AM����Ƶ��
#ifndef CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_STEP
#define CVTE_DEF_RADIO_EAST_ERNEUROPE_AM_STEP			9          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//JAPAN
//�ձ�����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_JAPAN_FM_FREQ_MIN
#define CVTE_DEF_RADIO_JAPAN_FM_FREQ_MIN				76000000    //Hz
#endif

//�ձ�����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_JAPAN_FM_FREQ_MAX
#define CVTE_DEF_RADIO_JAPAN_FM_FREQ_MAX				90000000   //Hz
#endif

//�ձ�����FM����Ƶ��
#ifndef CVTE_DEF_RADIO_JAPAN_FM_STEP
#define CVTE_DEF_RADIO_JAPAN_FM_STEP					10          // KHz
#endif

//�ձ�����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_JAPAN_AM_FREQ_MIN
#define CVTE_DEF_RADIO_JAPAN_AM_FREQ_MIN				522000
#endif

//�ձ�����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_JAPAN_AM_FREQ_MAX
#define CVTE_DEF_RADIO_JAPAN_AM_FREQ_MAX				1629000
#endif

//�ձ�����AM����Ƶ��
#ifndef CVTE_DEF_RADIO_JAPAN_AM_STEP
#define CVTE_DEF_RADIO_JAPAN_AM_STEP					9          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//KOREAN
//��������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_KOREAN_FM_FREQ_MIN
#define CVTE_DEF_RADIO_KOREAN_FM_FREQ_MIN				87500000    //Hz
#endif

//��������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_KOREAN_FM_FREQ_MAX
#define CVTE_DEF_RADIO_KOREAN_FM_FREQ_MAX				108000000   //Hz
#endif

//��������FM����Ƶ��
#ifndef CVTE_DEF_RADIO_KOREAN_FM_STEP
#define CVTE_DEF_RADIO_KOREAN_FM_STEP					10          // KHz
#endif

//��������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_KOREAN_AM_FREQ_MIN
#define CVTE_DEF_RADIO_KOREAN_AM_FREQ_MIN				531000
#endif

//��������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_KOREAN_AM_FREQ_MAX
#define CVTE_DEF_RADIO_KOREAN_AM_FREQ_MAX				1629000
#endif

//��������AM����Ƶ��
#ifndef CVTE_DEF_RADIO_KOREAN_AM_STEP
#define CVTE_DEF_RADIO_KOREAN_AM_STEP					9          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//SOUTHAMERICA
//����������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MIN
#define CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MIN			87500000    //Hz
#endif

//����������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MAX
#define CVTE_DEF_RADIO_SOUTHAMERICA_FM_FREQ_MAX			108000000   //Hz
#endif

//����������FM����Ƶ��
#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_FM_STEP
#define CVTE_DEF_RADIO_SOUTHAMERICA_FM_STEP				10          // KHz
#endif

//����������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MIN
#define CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MIN			520000
#endif

//����������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MAX
#define CVTE_DEF_RADIO_SOUTHAMERICA_AM_FREQ_MAX			1600000
#endif

//����������AM����Ƶ��
#ifndef CVTE_DEF_RADIO_SOUTHAMERICA_AM_STEP
#define CVTE_DEF_RADIO_SOUTHAMERICA_AM_STEP				5          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//TAIWANG 
//̨������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MIN
#define CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MIN				87500000    //Hz
#endif

//̨������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MAX
#define CVTE_DEF_RADIO_TAIWANG_FM_FREQ_MAX				108000000   //Hz
#endif

//̨������FM����Ƶ��
#ifndef CVTE_DEF_RADIO_TAIWANG_FM_STEP
#define CVTE_DEF_RADIO_TAIWANG_FM_STEP					10          // KHz
#endif

//̨������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MIN
#define CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MIN				531000
#endif

//̨������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MAX
#define CVTE_DEF_RADIO_TAIWANG_AM_FREQ_MAX				1629000
#endif

//̨������AM����Ƶ��
#ifndef CVTE_DEF_RADIO_TAIWANG_AM_STEP
#define CVTE_DEF_RADIO_TAIWANG_AM_STEP					9          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//ARABIA AUSTRALIA
//������/�Ĵ���������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MIN
#define CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MIN		87500000    //Hz
#endif

//������/�Ĵ���������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MAX
#define CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_FREQ_MAX		108000000   //Hz
#endif

//������/�Ĵ���������FM����Ƶ��
#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_STEP
#define CVTE_DEF_RADIO_ARABIA_AUSTRALIA_FM_STEP			10          // KHz
#endif

//������/�Ĵ���������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MIN
#define CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MIN		531000
#endif

//������/�Ĵ���������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MAX
#define CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_FREQ_MAX		1602000
#endif

//������/�Ĵ���������AM����Ƶ��
#ifndef CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_STEP
#define CVTE_DEF_RADIO_ARABIA_AUSTRALIA_AM_STEP			9          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//LATIN AMERICA
//������������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MIN
#define CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MIN		87500000    //Hz
#endif

//������������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MAX
#define CVTE_DEF_RADIO_LATIN_AMERICA_FM_FREQ_MAX		108000000   //Hz
#endif

//������������FM����Ƶ��
#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_FM_STEP
#define CVTE_DEF_RADIO_LATIN_AMERICA_FM_STEP			10          // KHz
#endif

//������������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MIN
#define CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MIN		520000
#endif

//������������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MAX
#define CVTE_DEF_RADIO_LATIN_AMERICA_AM_FREQ_MAX		1620000
#endif

//������������AM����Ƶ��
#ifndef CVTE_DEF_RADIO_LATIN_AMERICA_AM_STEP
#define CVTE_DEF_RADIO_LATIN_AMERICA_AM_STEP			5          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//MIDDLE_EAST
//�ж�����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MIN
#define CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MIN			87500000    //Hz
#endif

//�ж�����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MAX
#define CVTE_DEF_RADIO_MIDDLE_EAST_FM_FREQ_MAX			108000000   //Hz
#endif

//�ж�����FM����Ƶ��
#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_FM_STEP
#define CVTE_DEF_RADIO_MIDDLE_EAST_FM_STEP				10          // KHz
#endif

//�ж�����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MIN
#define CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MIN			531000
#endif

//�ж�����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MAX
#define CVTE_DEF_RADIO_MIDDLE_EAST_AM_FREQ_MAX			1602000
#endif

//�ж�����AM����Ƶ��
#ifndef CVTE_DEF_RADIO_MIDDLE_EAST_AM_STEP
#define CVTE_DEF_RADIO_MIDDLE_EAST_AM_STEP				9          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//AUST
//���ٵ۹�����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_AUST_FM_FREQ_MIN
#define CVTE_DEF_RADIO_AUST_FM_FREQ_MIN					87500000    //Hz
#endif

//���ٵ۹�����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_AUST_FM_FREQ_MAX
#define CVTE_DEF_RADIO_AUST_FM_FREQ_MAX					108000000   //Hz
#endif

//���ٵ۹�����FM����Ƶ��
#ifndef CVTE_DEF_RADIO_AUST_FM_STEP
#define CVTE_DEF_RADIO_AUST_FM_STEP						10          // KHz
#endif

//���ٵ۹�����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_AUST_AM_FREQ_MIN
#define CVTE_DEF_RADIO_AUST_AM_FREQ_MIN					522000
#endif

//���ٵ۹�����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_AUST_AM_FREQ_MAX
#define CVTE_DEF_RADIO_AUST_AM_FREQ_MAX					1710000
#endif

//���ٵ۹�����AM����Ƶ��
#ifndef CVTE_DEF_RADIO_AUST_AM_STEP
#define CVTE_DEF_RADIO_AUST_AM_STEP						9          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//BRAZIL
//��������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MIN
#define CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MIN				87500000    //Hz
#endif

//��������FM���Ƶ��
#ifndef CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MAX
#define CVTE_DEF_RADIO_BRAZIL_FM_FREQ_MAX				108000000   //Hz
#endif

//��������FM����Ƶ��
#ifndef CVTE_DEF_RADIO_BRAZIL_FM_STEP
#define CVTE_DEF_RADIO_BRAZIL_FM_STEP					10          // KHz
#endif

//��������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MIN
#define CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MIN				520000
#endif

//��������AM���Ƶ��
#ifndef CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MAX
#define CVTE_DEF_RADIO_BRAZIL_AM_FREQ_MAX				1620000
#endif

//��������AM����Ƶ��
#ifndef CVTE_DEF_RADIO_BRAZIL_AM_STEP
#define CVTE_DEF_RADIO_BRAZIL_AM_STEP					10          // KHz
#endif

///////////////////////////////////////////////////////////////////////////////
//RUSSIA
//����˹����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MIN
#define CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MIN				65000000    //Hz
#endif

//����˹����FM���Ƶ��
#ifndef CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MAX
#define CVTE_DEF_RADIO_RUSSIA_FM_FREQ_MAX				108000000   //Hz
#endif

//����˹����FM����Ƶ��
#ifndef CVTE_DEF_RADIO_RUSSIA_FM_STEP
#define CVTE_DEF_RADIO_RUSSIA_FM_STEP					10          // KHz
#endif

//����˹����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MIN
#define CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MIN				522000
#endif

//����˹����AM���Ƶ��
#ifndef CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MAX
#define CVTE_DEF_RADIO_RUSSIA_AM_FREQ_MAX				1620000
#endif

//����˹����AM����Ƶ��
#ifndef CVTE_DEF_RADIO_RUSSIA_AM_STEP
#define CVTE_DEF_RADIO_RUSSIA_AM_STEP					9          // KHz
#endif

/////////////////////////////////////////////////////////////////////////////////
// MODULE ENABLE
/////////////////////////////////////////////////////////////////////////////////
//carplay���ܿ���
#ifndef CVTE_EN_CARPLAY
#define CVTE_EN_CARPLAY			CVTE_ENABLE
#endif

//ipod���ܿ���
#ifndef CVTE_EN_IPOD
#define CVTE_EN_IPOD			CVTE_ENABLE
#endif

//���LED�ƿ���
#ifndef CVTE_EN_LED
#define CVTE_EN_LED				CVTE_ENABLE
#endif

//�������ܿ���
#ifndef CVTE_EN_RADIO
#define CVTE_EN_RADIO			CVTE_ENABLE
#endif

//�������ܿ���
#ifndef CVTE_EN_EASYCONNECT
#define CVTE_EN_EASYCONNECT		CVTE_ENABLE
#endif

//�����绰���ܿ���
#ifndef CVTE_EN_BT_PHONE
#define CVTE_EN_BT_PHONE		CVTE_ENABLE
#endif

//�������ֹ��ܿ���
#ifndef CVTE_EN_BT_A2DP
#define CVTE_EN_BT_A2DP			CVTE_ENABLE
#endif

//�������ܿ���
#ifndef CVTE_EN_NAVI
#define CVTE_EN_NAVI			CVTE_ENABLE
#endif

//USB���ܿ���
#ifndef CVTE_EN_USB
#define CVTE_EN_USB				CVTE_ENABLE
#endif

//SD�����ܿ���
#ifndef CVTE_EN_SD
#define CVTE_EN_SD				CVTE_ENABLE
#endif

//DVD��Ƭ���ܿ���
#ifndef CVTE_EN_DVD
#define CVTE_EN_DVD				CVTE_ENABLE
#endif

//DTV���ֵ��ӹ��ܿ���
#ifndef CVTE_EN_DTV
#define CVTE_EN_DTV				CVTE_ENABLE
#endif

//AUX��Ƶ���빦�ܿ���
#ifndef CVTE_EN_AUX
#define CVTE_EN_AUX				CVTE_ENABLE
#endif

#ifndef CVTE_EN_ISDB
#define CVTE_EN_ISDB				CVTE_ENABLE
#endif

//RDS���ߵ�����ϵͳ���ܿ��أ���Ҫ��ŷ��
#ifndef CVTE_EN_RDS
#define CVTE_EN_RDS				CVTE_ENABLE
#endif

//RBDS���ߵ�㲥����ϵͳ���ܿ��أ���Ҫ������
#ifndef CVTE_EN_RBDS
#define CVTE_EN_RBDS			CVTE_ENABLE
#endif

//RDS_TMC RDSӦ��֮һ��ͨ��ϢƵ�����ܿ���
#ifndef CVTE_EN_RDS_TMC
#define CVTE_EN_RDS_TMC			CVTE_ENABLE
#endif

//DAB������Ƶ�㲥���ܿ���
#ifndef CVTE_EN_DAB
#define CVTE_EN_DAB				CVTE_ENABLE
#endif

//����ͷ���ܿ���
#ifndef CVTE_EN_CAMERA
#define CVTE_EN_CAMERA          CVTE_ENABLE
#endif

//DSP�����źŴ����ܿ���
#ifndef CVTE_EN_DSP
#define CVTE_EN_DSP				CVTE_DISABLE
#endif

//android auto���ܿ���
#ifndef CVTE_EN_ANDROID_AUTO
#define CVTE_EN_ANDROID_AUTO	CVTE_DISABLE
#endif

//weblink���ܿ���
#ifndef CVTE_EN_WEBLINK
#define CVTE_EN_WEBLINK			CVTE_DISABLE
#endif

//phonelink���ܿ���
#ifndef CVTE_EN_PHONELINK
#define CVTE_EN_PHONELINK		CVTE_DISABLE
#endif
/////////////////////////////////////////////////////////////////////////////////
// FUNCTION ENABLE
/////////////////////////////////////////////////////////////////////////////////
//����/ȡ������logo
#ifndef CVTE_EN_IDLE_LOGO
#define CVTE_EN_IDLE_LOGO						CVTE_ENABLE
#endif

//���������Զ����ڿ���
#ifndef CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT        //MINSHENG ENTERFRONT Need set Enable
#define CVTE_EN_REAR_VOL_AUTO_ADJUSTMENT		CVTE_DISABLE
#endif

//�ɰ汾����UI���
#ifndef CVTE_EN_STEER_WHEEL_OLD_UI_STYLE        //MINSHENG ENTERFRONT Need set Enable
#define CVTE_EN_STEER_WHEEL_OLD_UI_STYLE		CVTE_DISABLE
#endif

//����ҳ��Drop Bar
#ifndef CVTE_EN_NAVI_DROP_BAR
#define CVTE_EN_NAVI_DROP_BAR					CVTE_DISABLE
#endif

//UI������������ֻ֧����Ƶ���ţ�
#ifndef CVTE_EN_DYNAMIC_LOGO
#define CVTE_EN_DYNAMIC_LOGO					CVTE_DISABLE
#endif

//ʹ��KEY�ܴ���,ͳһ������尴����can�źŵ�
#ifndef CVTE_EN_KEY_PROXY
#define CVTE_EN_KEY_PROXY						CVTE_ENABLE
#endif

//��������
#ifndef CVTE_EN_PACKAGE_UPDATE
#define CVTE_EN_PACKAGE_UPDATE					CVTE_ENABLE
#endif

//��ACC���Զ�������ҳ��
#ifndef CVTE_EN_AUTO_ENTER_HOME_PAGE_AFTER_ACCON
#define CVTE_EN_AUTO_ENTER_HOME_PAGE_AFTER_ACCON	CVTE_DISABLE
#endif

//��Ƶ����Subwoofer�����ڿ��ƿ���
#ifndef CVTE_EN_AUDIO_SUBWOOFER
#define CVTE_EN_AUDIO_SUBWOOFER					CVTE_ENABLE
#endif

//��Ƶ����Subwoofer������Ƶ�ʿ��ƿ���
#ifndef CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST
#define CVTE_EN_AUDIO_SUBWOOFER_FREQ_ADJUST		CVTE_DISABLE
#endif

//��Ƶ�����ڲ�����ʹ�ܿ���
#ifndef CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL
#define CVTE_EN_AUDIO_INTERNAL_AMP_CONTROL		CVTE_DISABLE
#endif

//��ACC�Զ���������ҳ��
#ifndef CVTE_EN_ACCON_WARNING
#define CVTE_EN_ACCON_WARNING					CVTE_DISABLE
#endif

//�߿���ʹ�ܿ���
#ifndef CVTE_EN_BORDER_TOUCH				
#define CVTE_EN_BORDER_TOUCH					CVTE_ENABLE
#endif

//��ҳͼ�궯������
#ifndef CVTE_EN_HOME_WIDGET_ANIM
#define CVTE_EN_HOME_WIDGET_ANIM				CVTE_ENABLE
#endif

//Carplay/iPod����ͼ����л�����
#ifndef CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON	
#define CVTE_EN_CARPLAY_IPOD_OVERLAP_ICON		CVTE_ENABLE
#endif

//UI�����ȼ�ʹ�ܿ���
#ifndef CVTE_EN_LIGHTNINGUI_HIGH_PRIORITY
#define CVTE_EN_LIGHTNINGUI_HIGH_PRIORITY		CVTE_DISABLE
#endif

//������У׼����̨
#ifndef CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE
#define CVTE_EN_STEER_WHEEL_CALIBRATE_CONSOLE	CVTE_ENABLE
#endif

//������DABͼ��ֿ�
#ifndef CVTE_EN_RADIO_DEPART_DAB_UI_STYLE
#define CVTE_EN_RADIO_DEPART_DAB_UI_STYLE		CVTE_DISABLE
#endif

//OS�Ŀ�������
#ifndef CVTE_EN_OS_DYNAMIC_LOGO
#define CVTE_EN_OS_DYNAMIC_LOGO					CVTE_DISABLE
#endif

//Home��ҳ���Ͻǿͻ�logo
#ifndef CVTE_CUSTOMER_STATUS_BAR_LOGO
#define CVTE_CUSTOMER_STATUS_BAR_LOGO			CVTE_DISABLE
#endif

//��������ʶ��
#ifndef CVTE_EN_GESTURE_AIR
#define CVTE_EN_GESTURE_AIR						CVTE_DISABLE
#endif

//����ͨ����¼���ص����û����
#ifndef CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT
#define CVTE_EN_BT_CALLLOG_DOWNLOAD_PERMIT		CVTE_DISABLE
#endif

//����绰��������
#ifndef SAVE_PHONEBOOK_TO_LOCAL
#define SAVE_PHONEBOOK_TO_LOCAL					CVTE_DISABLE
#endif

//��������뵯����ʾ
#ifndef CVTE_EN_BT_PAIRED_CODE_PERMIT
#define CVTE_EN_BT_PAIRED_CODE_PERMIT			CVTE_DISABLE
#endif

//CarPlay LimitUI���� 
#ifndef CVTE_EN_CARPLAY_LIMIT_UI
#define CVTE_EN_CARPLAY_LIMIT_UI				CVTE_ENABLE
#endif

//Enable CarPlay Night mode setting
#ifndef CVTE_EN_CARPLAY_NIGHTMODE
#define CVTE_EN_CARPLAY_NIGHTMODE				CVTE_ENABLE
#endif

//����������������
#ifndef CVTE_EN_BT_PHONECALL_MEDIA_RING
#define CVTE_EN_BT_PHONECALL_MEDIA_RING			CVTE_DISABLE
#endif

//�����绰������First name first/Last name first
#ifndef CVTE_EN_BT_VCARD_NAME_ORDER
#define CVTE_EN_BT_VCARD_NAME_ORDER				CVTE_DISABLE
#endif

//FULL keyboard search:ȫ��������
#ifndef CVTE_EN_SEARCH_USE_FULL_KEYBOARD
#define CVTE_EN_SEARCH_USE_FULL_KEYBOARD		CVTE_DISABLE
#endif

//slide gesture play control: NORMAL = from LEFT to right will play previous one.
//�������Ʋ��ſ���
#ifndef CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL
#define CVTE_EN_GESTURE_PLAY_CONTROL_NORMAL		CVTE_DISABLE
#endif

//AF stop level
//RDS�Զ�ѡֹ̨ͣ�ȼ�
#ifndef CVTE_EN_RDS_AF_STOP_LEVEL
#define CVTE_EN_RDS_AF_STOP_LEVEL				CVTE_DISABLE
#endif

//��ý��ҳ���°γ�U���Զ�������ҳ
#ifndef CVTE_EN_UNDER_MEDIA_PLAYER_PULLOUT_UDISK_AUTO_RETURN_HOME
#define CVTE_EN_UNDER_MEDIA_PLAYER_PULLOUT_UDISK_AUTO_RETURN_HOME	CVTE_DISABLE
#endif

//�û���������/������̬logo
#ifndef CVTE_EN_USER_UPDATE_LOGO
#define CVTE_EN_USER_UPDATE_LOGO				CVTE_ENABLE
#endif

//LED��Siri������Ч
#ifndef CVTE_EN_LED_SIRI_ANIM				
#define CVTE_EN_LED_SIRI_ANIM					CVTE_DISABLE
#endif

//�Ƿ���ʾ���ص�ѹֵ������ֵ
#ifndef CVTE_EN_SHOW_STW_VOLTAGE_VALUE
#define CVTE_EN_SHOW_STW_VOLTAGE_VALUE			CVTE_DISABLE
#endif

//DCPר�ó��˿�/CDP������ж˿ڳ�����
#ifndef CVTE_EN_CDP_DCP_CONTROL
#define CVTE_EN_CDP_DCP_CONTROL					CVTE_DISABLE
#endif

//ʹ�ÿ���չMCU����Э��
#ifndef CVTE_EN_EXTENSIBLE_MCU_PROTOCOL
#define CVTE_EN_EXTENSIBLE_MCU_PROTOCOL			CVTE_DISABLE
#endif

//ʹ��Weblink/Android auto ��ѡһ�л�����
#ifndef CVTE_EN_COMBINE_ANDROID_LINK
#define CVTE_EN_COMBINE_ANDROID_LINK			CVTE_DISABLE
#endif

////////////////////////////////////////////////////////////////////////////////
// CONFIG OPTIONS
///////////////////////////////////////////////////////////////////////////////
//Ĭ��֧����Ƶ��ʽ
#ifndef CVTE_DEF_AUDIO_SUPPORT_FORMAT
#define CVTE_DEF_AUDIO_SUPPORT_FORMAT	(CVTE_AUDIO_SUPPORT_FORMAT_TYPE_WAV \
										|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_MP3 \
										|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_M4A \
										|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_WMA \
										|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_APE \
										|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_FLA \
										|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_FLAC \
										|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_OGG \
										|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_AMR \
										|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_MP2 \
										|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_M4R \
										|CVTE_AUDIO_SUPPORT_FORMAT_TYPE_RA )
#endif

//��Ƶ����֧��DIVX���ܿ���
#ifndef CVTE_EN_VIDEO_DECODE_SUPPORT_DIVX
#define CVTE_EN_VIDEO_DECODE_SUPPORT_DIVX	CVTE_ENABLE
#endif

//��Ƶ����֧��XVID���ܿ���
#ifndef CVTE_EN_VIDEO_DECODE_SUPPORT_XVID
#define CVTE_EN_VIDEO_DECODE_SUPPORT_XVID	CVTE_ENABLE
#endif

//��Ƶ����֧��H264���ܿ���
#ifndef CVTE_EN_VIDEO_DECODE_SUPPORT_H264
#define CVTE_EN_VIDEO_DECODE_SUPPORT_H264	CVTE_ENABLE
#endif

//Ĭ��֧����Ƶ��ʽ
#ifndef CVTE_DEF_VIDEO_SUPPORT_FORMAT
#define CVTE_DEF_VIDEO_SUPPORT_FORMAT	(CVTE_VIDEO_SUPPORT_FORMAT_TYPE_AVI \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MPG \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MPEG \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_VOB \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_TS \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_TP \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_OGM \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MKV \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_RM \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_RMVB \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MP4 \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_M4V \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_MOV \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_3GP \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_ASF \
										|CVTE_VIDEO_SUPPORT_FORMAT_TYPE_WMV )
#endif

//Ĭ��֧����������
#ifndef CVTE_DEF_LANGUAGE_SUPPORT
#define CVTE_DEF_LANGUAGE_SUPPORT		(CVTE_LANGUAGE_SUPPORT_ENGLISH \
										|CVTE_LANGUAGE_SUPPORT_CHINESE_TRA \
										|CVTE_LANGUAGE_SUPPORT_CHINESE_SIM \
										|CVTE_LANGUAGE_SUPPORT_KOREAN \
										|CVTE_LANGUAGE_SUPPORT_GERMAN \
										|CVTE_LANGUAGE_SUPPORT_SPANISH \
										|CVTE_LANGUAGE_SUPPORT_ITALIAN \
										|CVTE_LANGUAGE_SUPPORT_FRENCH \
										|CVTE_LANGUAGE_SUPPORT_RUSSIAN \
										|CVTE_LANGUAGE_SUPPORT_PERSIAN \
										|CVTE_LANGUAGE_SUPPORT_GREEK)
#endif

//Ĭ��֧����������
#ifndef CVTE_DEF_RADIO_REGION_SUPPORT
#define CVTE_DEF_RADIO_REGION_SUPPORT	(CVTE_RADIO_REGION_SUPPORT_EUROPE \
									|CVTE_RADIO_REGION_SUPPORT_USA \
									|CVTE_RADIO_REGION_SUPPORT_EAST_ERNEUROPE \
									|CVTE_RADIO_REGION_SUPPORT_JAPAN \
									|CVTE_RADIO_REGION_SUPPORT_EUROPE_EASTERNEUROPE \
									|CVTE_RADIO_REGION_SUPPORT_KOREAN \
									|CVTE_RADIO_REGION_SUPPORT_TAIWANG \
									|CVTE_RADIO_REGION_SUPPORT_USA2	 \
									|CVTE_RADIO_REGION_SUPPORT_SOUTHAMERICA \
									|CVTE_RADIO_REGION_SUPPORT_ARABIA_AUSTRALIA \
									|CVTE_RADIO_REGION_SUPPORT_LATIN_AMERICA \
									|CVTE_RADIO_REGION_SUPPORT_MIDDLE_EAST \
									|CVTE_RADIO_REGION_SUPPORT_AUST	 \
									|CVTE_RADIO_REGION_SUPPORT_BRAZIL \
									|CVTE_RADIO_REGION_SUPPORT_EASTERNEUROPE_EUROPE)
#endif

//Ĭ��carplayͼ������
#ifndef CVTE_DEF_CARPLAY_ICON_LABEL
#define CVTE_DEF_CARPLAY_ICON_LABEL	"Vehicle"
#endif

//Ĭ�ϳ�����ʾ����
#ifndef CVTE_DEF_VEHICLE_DISPLAY_NAME
#define CVTE_DEF_VEHICLE_DISPLAY_NAME		TEXT("CarPlay HU")
#endif

//Ĭ����������ʾ����
#ifndef CVTE_DEF_MANUFACTURER_DISPLAY_NAME
#define CVTE_DEF_MANUFACTURER_DISPLAY_NAME	"CarPlay"
#endif

//Ĭ���ͺ���ʾ����
#ifndef CVTE_DEF_MODEL_DISPLAY_NAME
#define CVTE_DEF_MODEL_DISPLAY_NAME			"U701P"
#endif
/******************************************************************************/
/************************** CVTE ��Ļ�������� *******************************/
/******************************************************************************/
//Ĭ����Ļ���
#ifndef CVTE_DEF_SCREEN_VIDEOWIDTH
#define CVTE_DEF_SCREEN_VIDEOWIDTH		800
#endif

//Ĭ����Ļ�߶�
#ifndef CVTE_DEF_SCREEN_VIDEOHEIGHT
#define CVTE_DEF_SCREEN_VIDEOHEIGHT		480
#endif

//Ĭ��carply��Ļ��ʾ���
#ifndef CVTE_DEF_CARPLAY_SCREEN_VIDEOWIDTH
#define CVTE_DEF_CARPLAY_SCREEN_VIDEOWIDTH		CVTE_DEF_SCREEN_VIDEOWIDTH
#endif

//Ĭ��carplay��Ļ��ʾ�߶�
#ifndef CVTE_DEF_CARPLAY_SCREEN_VIDEOHEIGHT
#define CVTE_DEF_CARPLAY_SCREEN_VIDEOHEIGHT		CVTE_DEF_SCREEN_VIDEOHEIGHT
#endif

//Ĭ��android auto��ʾ���
#ifndef CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOWIDTH
#define CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOWIDTH		CVTE_DEF_SCREEN_VIDEOWIDTH
#endif

//Ĭ��andorid auto��ʾ�߶�
#ifndef CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOHEIGHT
#define CVTE_DEF_ANDROIDAUTO_SCREEN_VIDEOHEIGHT		CVTE_DEF_SCREEN_VIDEOHEIGHT
#endif

//Ĭ����Ļʵ��������
#ifndef CVTE_DEF_SCREEN_PHYSICALWIDTH
#define CVTE_DEF_SCREEN_PHYSICALWIDTH	CVTE_SCREEN_PHYSICALWIDTH_U701P
#endif

//Ĭ����Ļʵ������߶�
#ifndef CVTE_DEF_SCREEN_PHYSICALHEIGHT
#define CVTE_DEF_SCREEN_PHYSICALHEIGHT	CVTE_SCREEN_PHYSICALHEIGHT_U701P
#endif

/******************************************************************************/
/************************** CVTE ��ҳwidget���� *******************************/
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//             HOME WIDGET OPTIONS ��ҳWidget��������                        //
///////////////////////////////////////////////////////////////////////////////
//Ĭ����ҳ�ؼ�֮���Ե���
#ifndef CVTE_DEF_HOME_WIDGET_MARGIN_WIDTH
#define CVTE_DEF_HOME_WIDGET_MARGIN_WIDTH		20
#endif

//Ĭ����ҳ�ؼ�֮���Ե�߶�
#ifndef CVTE_DEF_HOME_WIDGET_MARGIN_HEIGHT
#define CVTE_DEF_HOME_WIDGET_MARGIN_HEIGHT		14
#endif

//Ĭ����ҳ�ؼ�״̬���߶�
#ifndef CVTE_DEF_HOME_WIDGET_STATUS_BAR_HEIGHT
#define CVTE_DEF_HOME_WIDGET_STATUS_BAR_HEIGHT	46
#endif

//Ĭ����ҳ�ؼ���ȵ�λ��ÿ��UI����������λ�ı���
#ifndef CVTE_DEF_HOME_WIDGET_UNIT_CX		//130
#define CVTE_DEF_HOME_WIDGET_UNIT_CX		((CVTE_DEF_SCREEN_VIDEOWIDTH-CVTE_DEF_HOME_WIDGET_MARGIN_WIDTH)/6)
#endif

//Ĭ����ҳ�ؼ��߶ȵ�λ��ÿ��UI�߶��������λ�ı���
#ifndef CVTE_DEF_HOME_WIDGET_UNIT_CY		//140
#define CVTE_DEF_HOME_WIDGET_UNIT_CY		(CVTE_DEF_SCREEN_VIDEOHEIGHT-CVTE_DEF_HOME_WIDGET_MARGIN_HEIGHT-CVTE_DEF_HOME_WIDGET_STATUS_BAR_HEIGHT)/3
#endif

//Ĭ����ҳ�ؼ���X���꣬����ĻΪ����ԭ�㣬ÿ��UI���Լ�������ΪPX��PY
#ifndef CVTE_DEF_HOME_WIDGET_UINT_PX		//-260
#define CVTE_DEF_HOME_WIDGET_UINT_PX		(-(CVTE_DEF_SCREEN_VIDEOWIDTH/2)+CVTE_DEF_HOME_WIDGET_MARGIN_WIDTH/2+CVTE_DEF_HOME_WIDGET_UNIT_CX)
#endif

//Ĭ����ҳ�ؼ���Y���꣬����ĻΪ����ԭ�㣬ÿ��UI���Լ�������ΪPX��PY
#ifndef CVTE_DEF_HOME_WIDGET_UINT_PY		//47
#define CVTE_DEF_HOME_WIDGET_UINT_PY		(CVTE_DEF_SCREEN_VIDEOHEIGHT/2-CVTE_DEF_HOME_WIDGET_STATUS_BAR_HEIGHT-CVTE_DEF_HOME_WIDGET_MARGIN_HEIGHT/2-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#endif

//Ĭ����Ļ���һ������Դﵽ�������
#ifndef CVTE_DEF_HOME_WIDGET_WIDTH
#define CVTE_DEF_HOME_WIDGET_WIDTH			(CVTE_DEF_SCREEN_VIDEOWIDTH+CVTE_DEF_HOME_WIDGET_UNIT_CX*4)
#endif

////////////////////////////////////////////////////////////////////////////////
// HOME WIDGET MEMBERS OPTIONS
///////////////////////////////////////////////////////////////////////////////
/**
* MEDIA WIDGET
*/
//��ý��ؼ����
#ifndef CVTE_DEF_MEDIA_WIDGET_CX
#define CVTE_DEF_MEDIA_WIDGET_CX			(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#endif

//��ý��ؼ��߶�
#ifndef CVTE_DEF_MEDIA_WIDGET_CY
#define CVTE_DEF_MEDIA_WIDGET_CY			(CVTE_DEF_HOME_WIDGET_UNIT_CY*2)
#endif

//��ý��ؼ�ԭ��X����
#ifndef CVTE_DEF_MEDIA_WIDGET_PX
#define CVTE_DEF_MEDIA_WIDGET_PX			CVTE_DEF_HOME_WIDGET_UINT_PX
#endif

//��ý��ؼ�ԭ��Y����
#ifndef CVTE_DEF_MEDIA_WIDGET_PY
#define CVTE_DEF_MEDIA_WIDGET_PY			CVTE_DEF_HOME_WIDGET_UINT_PY
#endif

//��ý��ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_MEDIA_WIDGET_VISIBLE
#define CVTE_DEF_MEDIA_WIDGET_VISIBLE		1
#endif

/**
* RADIO WIDGET
*/
//�����ؼ����
#ifndef CVTE_DEF_RADIO_WIDGET_CX
#define CVTE_DEF_RADIO_WIDGET_CX			(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#endif

//�����ؼ��߶�
#ifndef CVTE_DEF_RADIO_WIDGET_CY
#define CVTE_DEF_RADIO_WIDGET_CY			(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//�����ؼ�ԭ��X����
#ifndef CVTE_DEF_RADIO_WIDGET_PX
#define CVTE_DEF_RADIO_WIDGET_PX			CVTE_DEF_HOME_WIDGET_UINT_PX
#endif

//�����ؼ�ԭ��Y����
#ifndef CVTE_DEF_RADIO_WIDGET_PY
#define CVTE_DEF_RADIO_WIDGET_PY			(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#endif

//�����ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_RADIO_WIDGET_VISIBLE
#define CVTE_DEF_RADIO_WIDGET_VISIBLE		1
#endif

/**
* CARPLAY WIDGET
*/
//carplay�ؼ����
#ifndef CVTE_DEF_CARPLAY_WIDGET_CX
#define CVTE_DEF_CARPLAY_WIDGET_CX			(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#endif

//carplay�ؼ��߶�
#ifndef CVTE_DEF_CARPLAY_WIDGET_CY
#define CVTE_DEF_CARPLAY_WIDGET_CY			(CVTE_DEF_HOME_WIDGET_UNIT_CY*2)
#endif

//carplay�ؼ�ԭ��X����
#ifndef CVTE_DEF_CARPLAY_WIDGET_PX
#define CVTE_DEF_CARPLAY_WIDGET_PX			(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#endif

//carplay�ؼ�ԭ��Y����
#ifndef CVTE_DEF_CARPLAY_WIDGET_PY
#define CVTE_DEF_CARPLAY_WIDGET_PY			CVTE_DEF_HOME_WIDGET_UINT_PY
#endif

//carplay�ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_CARPLAY_WIDGET_VISIBLE
#define CVTE_DEF_CARPLAY_WIDGET_VISIBLE		1
#endif

/**
* IPOD WIDGET
*/
//ipod�ؼ����
#ifndef CVTE_DEF_IPOD_WIDGET_CX
#define CVTE_DEF_IPOD_WIDGET_CX				(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#endif

//ipod�ؼ��߶�
#ifndef CVTE_DEF_IPOD_WIDGET_CY
#define CVTE_DEF_IPOD_WIDGET_CY				(CVTE_DEF_HOME_WIDGET_UNIT_CY*2)
#endif

//ipod�ؼ�ԭ��X����
#ifndef CVTE_DEF_IPOD_WIDGET_PX
#define CVTE_DEF_IPOD_WIDGET_PX				(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*6)
#endif

//ipod�ؼ�ԭ��Y����
#ifndef CVTE_DEF_IPOD_WIDGET_PY
#define CVTE_DEF_IPOD_WIDGET_PY				(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#endif

//ipod�ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_IPOD_WIDGET_VISIBLE
#define CVTE_DEF_IPOD_WIDGET_VISIBLE		1
#endif


/**
* EASYCONNECT WIDGET
*/
//�����ؼ����
#ifndef CVTE_DEF_ANDROID_LINK_WIDGET_CX
#define CVTE_DEF_ANDROID_LINK_WIDGET_CX		(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#endif

//�����ؼ��߶�
#ifndef CVTE_DEF_ANDROID_LINK_WIDGET_CY
#define CVTE_DEF_ANDROID_LINK_WIDGET_CY		(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//�����ؼ�ԭ��X����
#ifndef CVTE_DEF_ANDROID_LINK_WIDGET_PX
#define CVTE_DEF_ANDROID_LINK_WIDGET_PX		(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#endif

//�����ؼ�ԭ��Y����
#ifndef CVTE_DEF_ANDROID_LINK_WIDGET_PY
#define CVTE_DEF_ANDROID_LINK_WIDGET_PY		(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#endif

//�����ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_ANDROID_LINK_WIDGET_VISIBLE
#define CVTE_DEF_ANDROID_LINK_WIDGET_VISIBLE	1
#endif

/**
* BT PHONE WIDGET
*/
//�����绰�ؼ����
#ifndef CVTE_DEF_BT_PHONE_WIDGET_CX
#define CVTE_DEF_BT_PHONE_WIDGET_CX			(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#endif

//�����绰�ؼ��߶�
#ifndef CVTE_DEF_BT_PHONE_WIDGET_CY
#define CVTE_DEF_BT_PHONE_WIDGET_CY			(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//�����绰�ؼ�ԭ��X����
#ifndef CVTE_DEF_BT_PHONE_WIDGET_PX
#define CVTE_DEF_BT_PHONE_WIDGET_PX			(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4)
#endif

//�����绰�ؼ�ԭ��Y����
#ifndef CVTE_DEF_BT_PHONE_WIDGET_PY
#define CVTE_DEF_BT_PHONE_WIDGET_PY			(CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#endif

//�����绰�ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_BT_PHONE_WIDGET_VISIBLE
#define CVTE_DEF_BT_PHONE_WIDGET_VISIBLE    1
#endif

/**
* NAVI WIDGET
*/
//�����ؼ����
#ifndef CVTE_DEF_NAVI_WIDGET_CX
#define CVTE_DEF_NAVI_WIDGET_CX				(CVTE_DEF_HOME_WIDGET_UNIT_CX*2)
#endif

//�����ؼ��߶�
#ifndef CVTE_DEF_NAVI_WIDGET_CY
#define CVTE_DEF_NAVI_WIDGET_CY				(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//�����ؼ�ԭ��X����
#ifndef CVTE_DEF_NAVI_WIDGET_PX
#define CVTE_DEF_NAVI_WIDGET_PX				(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4)
#endif

//�����ؼ�ԭ��Y����
#ifndef CVTE_DEF_NAVI_WIDGET_PY
#define CVTE_DEF_NAVI_WIDGET_PY				(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#endif

//�����ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_NAVI_WIDGET_VISIBLE
#define CVTE_DEF_NAVI_WIDGET_VISIBLE		1
#endif

/**
* BT A2DP WIDGET
*/
//�������ֿؼ����
#ifndef CVTE_DEF_BT_A2DP_WIDGET_CX
#define CVTE_DEF_BT_A2DP_WIDGET_CX			(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#endif

//�������ֿؼ��߶�
#ifndef CVTE_DEF_BT_A2DP_WIDGET_CY
#define CVTE_DEF_BT_A2DP_WIDGET_CY			(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//�������ֿؼ�ԭ��X����
#ifndef CVTE_DEF_BT_A2DP_WIDGET_PX
#define CVTE_DEF_BT_A2DP_WIDGET_PX			(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4-CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#endif

//�������ֿؼ�ԭ��Y����
#ifndef CVTE_DEF_BT_A2DP_WIDGET_PY
#define CVTE_DEF_BT_A2DP_WIDGET_PY			(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#endif

//�������ֿؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_BT_A2DP_WIDGET_VISIBLE
#define CVTE_DEF_BT_A2DP_WIDGET_VISIBLE		1
#endif

/**
* USB WIDGET
*/
//usb�ؼ����
#ifndef CVTE_DEF_USB_WIDGET_CX
#define CVTE_DEF_USB_WIDGET_CX				(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#endif

//usb�ؼ��߶�
#ifndef CVTE_DEF_USB_WIDGET_CY
#define CVTE_DEF_USB_WIDGET_CY				(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//usb�ؼ�ԭ��X����
#ifndef CVTE_DEF_USB_WIDGET_PX
#define CVTE_DEF_USB_WIDGET_PX				(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#endif

//usb�ؼ�ԭ��Y����
#ifndef CVTE_DEF_USB_WIDGET_PY
#define CVTE_DEF_USB_WIDGET_PY				(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#endif

//usb�ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_USB_WIDGET_VISIBLE
#define CVTE_DEF_USB_WIDGET_VISIBLE			1
#endif

/**
* DVD WIDGET
*/
//DVD�ؼ����
#ifndef CVTE_DEF_DVD_WIDGET_CX
#define CVTE_DEF_DVD_WIDGET_CX				(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#endif

//DVD�ؼ��߶�
#ifndef CVTE_DEF_DVD_WIDGET_CY
#define CVTE_DEF_DVD_WIDGET_CY				(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//DVD�ؼ�ԭ��X����
#ifndef CVTE_DEF_DVD_WIDGET_PX
#define CVTE_DEF_DVD_WIDGET_PX				(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX*2+CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#endif

//DVD�ؼ�ԭ��Y����
#ifndef CVTE_DEF_DVD_WIDGET_PY
#define CVTE_DEF_DVD_WIDGET_PY				(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2+CVTE_DEF_HOME_WIDGET_UNIT_CY)
#endif

//DVD�ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_DVD_WIDGET_VISIBLE
#define CVTE_DEF_DVD_WIDGET_VISIBLE			1
#endif

/**
* DTV WIDGET
*/
//DTV�ؼ����
#ifndef CVTE_DEF_DTV_WIDGET_CX
#define CVTE_DEF_DTV_WIDGET_CX				(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#endif

//DTV�ؼ��߶�
#ifndef CVTE_DEF_DTV_WIDGET_CY
#define CVTE_DEF_DTV_WIDGET_CY				(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//DTV�ؼ�ԭ��X����
#ifndef CVTE_DEF_DTV_WIDGET_PX
#define CVTE_DEF_DTV_WIDGET_PX				(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*4+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#endif

//DTV�ؼ�ԭ��Y����
#ifndef CVTE_DEF_DTV_WIDGET_PY
#define CVTE_DEF_DTV_WIDGET_PY				(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2+CVTE_DEF_HOME_WIDGET_UNIT_CY)
#endif

//DTV�ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_DTV_WIDGET_VISIBLE
#define CVTE_DEF_DTV_WIDGET_VISIBLE			1
#endif

/**
* AUX WIDGET
*/
//AUX�ؼ����
#ifndef CVTE_DEF_AUX_WIDGET_CX
#define CVTE_DEF_AUX_WIDGET_CX				(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#endif

//AUX�ؼ��߶�
#ifndef CVTE_DEF_AUX_WIDGET_CY
#define CVTE_DEF_AUX_WIDGET_CY				(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//AUX�ؼ�ԭ��X����
#ifndef CVTE_DEF_AUX_WIDGET_PX
#define CVTE_DEF_AUX_WIDGET_PX				(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*6+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2)
#endif

//AUX�ؼ�ԭ��Y����
#ifndef CVTE_DEF_AUX_WIDGET_PY
#define CVTE_DEF_AUX_WIDGET_PY				(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#endif

//AUX�ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_AUX_WIDGET_VISIBLE
#define CVTE_DEF_AUX_WIDGET_VISIBLE			1
#endif

/**
* DAB WIDGET
*/
//DAB�ؼ����
#ifndef CVTE_DEF_DAB_WIDGET_CX
#define CVTE_DEF_DAB_WIDGET_CX				(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#endif

//DAB�ؼ��߶�
#ifndef CVTE_DEF_DAB_WIDGET_CY
#define CVTE_DEF_DAB_WIDGET_CY				(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//DAB�ؼ�ԭ��X����
#ifndef CVTE_DEF_DAB_WIDGET_PX
#define CVTE_DEF_DAB_WIDGET_PX				(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*6+CVTE_DEF_HOME_WIDGET_UNIT_CX*2+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#endif

//DAB�ؼ�ԭ��Y����
#ifndef CVTE_DEF_DAB_WIDGET_PY
#define CVTE_DEF_DAB_WIDGET_PY				(CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#endif

//DAB�ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_DAB_WIDGET_VISIBLE
#define CVTE_DEF_DAB_WIDGET_VISIBLE			1
#endif

/**
* CAMERA WIDGET
*/
//����ͷ�ؼ����
#ifndef CVTE_DEF_CAMERA_WIDGET_CX
#define CVTE_DEF_CAMERA_WIDGET_CX			(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#endif

//����ͷ�ؼ��߶�
#ifndef CVTE_DEF_CAMERA_WIDGET_CY
#define CVTE_DEF_CAMERA_WIDGET_CY			(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//����ͷ�ؼ�ԭ��X����
#ifndef CVTE_DEF_CAMERA_WIDGET_PX
#define CVTE_DEF_CAMERA_WIDGET_PX			(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*6+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#endif

//����ͷ�ؼ�ԭ��Y����
#ifndef CVTE_DEF_CAMERA_WIDGET_PY
#define CVTE_DEF_CAMERA_WIDGET_PY			(CVTE_DEF_HOME_WIDGET_UINT_PY+CVTE_DEF_HOME_WIDGET_UNIT_CY/2)
#endif

//����ͷ�ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_CAMERA_WIDGET_VISIBLE
#define CVTE_DEF_CAMERA_WIDGET_VISIBLE		1
#endif

/**
* SETTING WIDGET
*/
//���ÿؼ����
#ifndef CVTE_DEF_SETTING_WIDGET_CX
#define CVTE_DEF_SETTING_WIDGET_CX			(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#endif	

//���ÿؼ��߶�
#ifndef CVTE_DEF_SETTING_WIDGET_CY
#define CVTE_DEF_SETTING_WIDGET_CY			(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//���ÿؼ�ԭ��X����
#ifndef CVTE_DEF_SETTING_WIDGET_PX
#define CVTE_DEF_SETTING_WIDGET_PX			(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*6+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#endif

//���ÿؼ�ԭ��Y����
#ifndef CVTE_DEF_SETTING_WIDGET_PY
#define CVTE_DEF_SETTING_WIDGET_PY			(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#endif

//���ÿؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_SETTING_WIDGET_VISIBLE
#define CVTE_DEF_SETTING_WIDGET_VISIBLE		1
#endif

/**
* DSP WIDGET
*/
//DSP�ؼ����
#ifndef CVTE_DEF_DSP_WIDGET_CX
#define CVTE_DEF_DSP_WIDGET_CX			(CVTE_DEF_HOME_WIDGET_UNIT_CX*1)
#endif	

//DSP�ؼ��߶�
#ifndef CVTE_DEF_DSP_WIDGET_CY
#define CVTE_DEF_DSP_WIDGET_CY			(CVTE_DEF_HOME_WIDGET_UNIT_CY*1)
#endif

//DSP�ؼ�ԭ��X����
#ifndef CVTE_DEF_DSP_WIDGET_PX
#define CVTE_DEF_DSP_WIDGET_PX			(CVTE_DEF_HOME_WIDGET_UINT_PX+CVTE_DEF_HOME_WIDGET_UNIT_CX*6+CVTE_DEF_HOME_WIDGET_UNIT_CX+CVTE_DEF_HOME_WIDGET_UNIT_CX/2+2)
#endif

//DSP�ؼ�ԭ��Y����
#ifndef CVTE_DEF_DSP_WIDGET_PY
#define CVTE_DEF_DSP_WIDGET_PY			(CVTE_DEF_HOME_WIDGET_UINT_PY-CVTE_DEF_HOME_WIDGET_UNIT_CY/2-CVTE_DEF_HOME_WIDGET_UNIT_CY)
#endif

//DSP�ؼ��Ƿ�ɼ�
#ifndef CVTE_DEF_DSP_WIDGET_VISIBLE
#define CVTE_DEF_DSP_WIDGET_VISIBLE		1
#endif



/******************************************************************************/
/******************************* CVTE ��ƵID���� ******************************/
/******************************************************************************/

#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_DEFAULT
#define CVTE_DEF_AUDIO_DEVICE_ID_MAIN_DEFAULT			1
#endif

#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_ALERT
#define CVTE_DEF_AUDIO_DEVICE_ID_MAIN_ALERT				1
#endif

#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_MEDIA
#define CVTE_DEF_AUDIO_DEVICE_ID_MAIN_MEDIA				1
#endif

#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_TELEPHONY
#define CVTE_DEF_AUDIO_DEVICE_ID_MAIN_TELEPHONY			1
#endif

#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_SPEECHRECOGNITION
#define CVTE_DEF_AUDIO_DEVICE_ID_MAIN_SPEECHRECOGNITION 1
#endif

#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAIN_COMPATIBILITY
#define CVTE_DEF_AUDIO_DEVICE_ID_MAIN_COMPATIBILITY		1
#endif

#ifndef CVTE_DEF_AUDIO_DEVICE_ID_ALT_DEFAULT
#define CVTE_DEF_AUDIO_DEVICE_ID_ALT_DEFAULT			0
#endif

#ifndef CVTE_DEF_AUDIO_DEVICE_ID_ALT_COMPATIBILITY
#define CVTE_DEF_AUDIO_DEVICE_ID_ALT_COMPATIBILITY		0
#endif

#ifndef CVTE_DEF_AUDIO_DEVICE_ID_MAINHIGH_MEDIA
#define CVTE_DEF_AUDIO_DEVICE_ID_MAINHIGH_MEDIA			1
#endif

#ifndef CVTE_DEF_AUDIO_DEVICE_ID_INPUT
#define CVTE_DEF_AUDIO_DEVICE_ID_INPUT					0
#endif

/******************************************************************************/
/****************************** CVTE LCD ��������  ****************************/
/******************************************************************************/
//LCD������Сֵ
#ifndef CVTE_DEF_LCD_BRIGHTNESS_MIN
#define CVTE_DEF_LCD_BRIGHTNESS_MIN				0
#endif

//LCD�������ֵ
#ifndef CVTE_DEF_LCD_BRIGHTNESS_MAX
#define CVTE_DEF_LCD_BRIGHTNESS_MAX				100
#endif

//LCD������ʼ��Χ
#ifndef CVGE_DEF_LCD_BRIGHTNESS_RANGE_START
#define CVGE_DEF_LCD_BRIGHTNESS_RANGE_START		22
#endif

//LCD���������Χ
#ifndef CVTE_DEF_LCD_BRIGHTNESS_RANGE_END
#define CVTE_DEF_LCD_BRIGHTNESS_RANGE_END		52
#endif

//LCD���������������Сֵ
#ifndef CVTE_DEF_LCD_BRIGHTNESS_SILDER_MIN
#define CVTE_DEF_LCD_BRIGHTNESS_SILDER_MIN		0
#endif

//LCD����������������ֵ
#ifndef CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX
#define CVTE_DEF_LCD_BRIGHTNESS_SILDER_MAX		15
#endif

//LCD����gammaֵ��Сֵ
#ifndef CVTE_DEF_LCD_GAMMA_MIN
#define CVTE_DEF_LCD_GAMMA_MIN					0
#endif

//LCD����gammaֵ���ֵ
#ifndef CVTE_DEF_LCD_GAMMA_MAX
#define CVTE_DEF_LCD_GAMMA_MAX					200
#endif

/******************************************************************************/
/**************************** CVTE LCD �ռ���������  **************************/
/******************************************************************************/
//LCD�ռ䱳����ʼ��Χ
#ifndef CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START
#define CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_START		37
#endif

//LCD�ռ䱳�������Χ
#ifndef CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END
#define CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_RANGE_END		CVTE_DEF_LCD_BRIGHTNESS_RANGE_END	//52
#endif

//LCD�ռ䱳��Ĭ��ֵ
#ifndef CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_DEFAULT
#define CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_DEFAULT			47
#endif

/******************************************************************************/
/**************************** CVTE LCD ҹ����������  **************************/
/******************************************************************************/
//LCDҹ�䱳����ʼ��Χ
#ifndef CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START
#define CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_START	 CVGE_DEF_LCD_BRIGHTNESS_RANGE_START	//22
#endif

//LCDҹ�䱳�������Χ
#ifndef CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END
#define CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_RANGE_END		37
#endif

//LCDҹ�䱳��Ĭ��ֵ
#ifndef CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_DEFAULT
#define CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_DEFAULT		32
#endif

/******************************************************************************/
/***************************** CVTE LED�� ��������  ***************************/
/******************************************************************************/
//LED������Сֵ
#ifndef CVTE_DEF_LED_BRIGHTNESS_MIN
#define CVTE_DEF_LED_BRIGHTNESS_MIN				0
#endif

//LED�������ֵ
#ifndef CVTE_DEF_LED_BRIGHTNESS_MAX
#define CVTE_DEF_LED_BRIGHTNESS_MAX				100
#endif

/******************************************************************************/
/*************************** CVTE LED�� �ռ���������  *************************/
/******************************************************************************/
//LED�ռ䱳����ʼ��Χ
#ifndef CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START
#define CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START		40
#endif

//LED�ռ䱳�������Χ
#ifndef CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END
#define CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END		CVTE_DEF_LED_BRIGHTNESS_MAX	//100
#endif

//LED�ռ䱳��Ĭ��ֵ
#ifndef CVTE_DEF_DAYMODE_LED_BRIGHTNESS_DEFAULT
#define CVTE_DEF_DAYMODE_LED_BRIGHTNESS_DEFAULT			90
#endif

//LED�ռ䱳�������������Сֵ
#ifndef CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MIN
#define CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MIN		0
#endif

//LED�ռ䱳��������������ֵ
#ifndef CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MAX
#define CVTE_DEF_DAYMODE_LED_BRIGHTNESS_SILDER_MAX		(CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_END-CVTE_DEF_DAYMODE_LED_BRIGHTNESS_RANGE_START)	//60
#endif 

/******************************************************************************/
/*************************** CVTE LED�� ҹ����������  *************************/
/******************************************************************************/
//LEDҹ�䱳����ʼ��Χ
#ifndef CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START
#define CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START	10
#endif

//LEDҹ�䱳�������Χ
#ifndef CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END
#define CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END		40
#endif

//LEDҹ�䱳��Ĭ��ֵ
#ifndef CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_DEFAULT
#define CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_DEFAULT		35
#endif

//LEDҹ�䱳�������������Сֵ
#ifndef CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MIN
#define CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MIN	0
#endif

//LEDҹ�䱳��������������ֵ
#ifndef CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MAX
#define CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_SILDER_MAX	(CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_END-CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_RANGE_START)	//30
#endif

/******************************************************************************/
/***************************    CVTE CarPlay ����     *************************/
/******************************************************************************/
//carplay��λʱ��
#ifndef CVTE_DEF_CARPLAY_RESTART_TIME_MS
#define CVTE_DEF_CARPLAY_RESTART_TIME_MS	0
#endif

//carplay CPCA���ܿ���
#ifndef CVTE_EN_CPCA
#define CVTE_EN_CPCA						CVTE_DISABLE
#endif

//carplay CPMAC���ܿ���
#ifndef CVTE_EN_CPMAC
#define CVTE_EN_CPMAC						CVTE_DISABLE
#endif

/******************************************************************************/
/***************************   CVTE TV Widget ����    *************************/
/******************************************************************************/
//TV�ؼ�����
#ifndef CVTE_DEF_TV_WIDGET_TYPE
#define CVTE_DEF_TV_WIDGET_TYPE				CVTE_DEF_TV_WIDGET_TYPE_DTV		
#endif

//TV�ؼ�����
#ifndef CVTE_TV_WIDGET_NAME_STRING
#define CVTE_TV_WIDGET_NAME_STRING			TEXT("DTV")
#endif

/******************************************************************************/
/***************************   CVTE Camera Widget ����    *************************/
/******************************************************************************/
//����ͷ�ؼ�����
#ifndef CVTE_DEF_CAMERA_WIDGET_TYPE
#define CVTE_DEF_CAMERA_WIDGET_TYPE			CVTE_DEF_CAMERA_WIDGET_TYPE_DEFAULT		
#endif

//����ͷ�ؼ�����
#ifndef CVTE_CAMERA_WIDGET_NAME_STRING
#define CVTE_CAMERA_WIDGET_NAME_STRING		TEXT("Camera")
#endif

/******************************************************************************/
/************************** CVTE ע���Ĭ������ *******************************/
/******************************************************************************/

//Font ����
#ifndef CVTE_REG_FONT_DEFAULT
#define CVTE_REG_FONT_DEFAULT				CVTE_DEF_FONT_TYPE_HELVETICA		
#endif 

//TimeZone Ĭ��ʱ��
#ifndef CVTE_REG_TIMEZONE_DEFAULT
#define CVTE_REG_TIMEZONE_DEFAULT			CVTE_DEF_TIME_ZONE_000GMT_GMT_STANDARD_TIME
#endif

//Background Path����ͼƬ·��
#ifndef CVTE_REG_BACKGROUND_PATH_DEFAULT
#define CVTE_REG_BACKGROUND_PATH_DEFAULT	_T("\\ResidentFlash\\LightningUI\\background\\IMAGES\\LIGHTNING.jpg")
#endif

//Navi Path������ͼ·��
#ifndef CVTE_REG_NAVI_PATH_DEFAULT
#define CVTE_REG_NAVI_PATH_DEFAULT			_T("\\GPS CARD\\NaviOne\\NaviOne.exe")
#endif

//Bluetooth Device Name�����豸����
#ifndef CVTE_REG_BT_DEVICE_NAME_DEFAULT
#define CVTE_REG_BT_DEVICE_NAME_DEFAULT	    _T("Automotive BT Device")
#endif

//Radio Region����Ƶ��
//CVTE_DEF_RADIO_REGION_MIN~CVTE_DEF_RADIO_REGION_MAX
#ifndef CVTE_REG_RADIO_REGION_DEFAULT
#define CVTE_REG_RADIO_REGION_DEFAULT	    CVTE_DEF_RADIO_REGION_EUROPE
#endif

//RDS AF̨ͣ������ (Լ��ʵֵdb��2��)
//CVTE_DEF_RDS_AF_LEVEL_MIN~CVTE_DEF_RDS_AF_LEVEL_MAX
#ifndef CVTE_REG_RDS_AF_LEVEL_DEFAULT
#define CVTE_REG_RDS_AF_LEVEL_DEFAULT		70
#endif

//RDS �ܿ���
#ifndef CVTE_REG_RDS_ENABLE_DEFAULT
#define CVTE_REG_RDS_ENABLE_DEFAULT			CVTE_ENABLE
#endif

//RDS AF �Զ�ѡ̨���ܿ���
#ifndef CVTE_REG_RDS_AF_ENABLE_DEFAULT
#define CVTE_REG_RDS_AF_ENABLE_DEFAULT		CVTE_ENABLE
#endif

//RDS TA �Զ�����ͬ���͵�̨���ܿ���
#ifndef CVTE_REG_RDS_TA_ENABLE_DEFAULT
#define CVTE_REG_RDS_TA_ENABLE_DEFAULT		CVTE_ENABLE
#endif

//Language
//CVTE_DEF_LANGUAGE_MIN~CVTE_DEF_LANGUAGE_MAX
//Ĭ��ϵͳ����
#ifndef CVTE_REG_LANGUAGE_DEFAULT
#define CVTE_REG_LANGUAGE_DEFAULT			CVTE_DEF_LANGUAGE_ENGLISH
#endif

//Time Mode
//CVTE_DEF_TIME_MODE_12H/CVTE_DEF_TIME_MODE_24H
//ʱ��ģʽ
#ifndef CVTE_REG_TIME_MODE_DEFAULT
#define CVTE_REG_TIME_MODE_DEFAULT			CVTE_DEF_TIME_MODE_24H
#endif

//Drive Type
//CVTE_DEF_DRIVE_TYPE_LEFT~CVTE_DEF_DRIVE_TYPE_RIGHT
//��ʻģʽ
#ifndef CVTE_REG_DRIVE_TYPE_DEFAULT
#define CVTE_REG_DRIVE_TYPE_DEFAULT			CVTE_DEF_DRIVE_TYPE_LEFT
#endif

//Backlight
//CVTE_DEF_BACKLIGHT_MIN~CVTE_DEF_BACKLIGHT_MAX
//Ĭ�ϱ���
#ifndef CVTE_REG_BACKLIGHT_DEFAULT
#define CVTE_REG_BACKLIGHT_DEFAULT			CVTE_DEF_BACKLIGHT_MAX
#endif

//DTV Video Color
//CVTE_DEF_VIDEO_COLOR_TYPE_MIN~CVTE_DEF_VIDEO_COLOR_TYPE_MAX
//DTVĬ����Ƶ��ʽ
#ifndef CVTE_REG_DTV_COLOR_DEFAULT
#define CVTE_REG_DTV_COLOR_DEFAULT			CVTE_DEF_VIDEO_COLOR_TYPE_NTSM_MJ
#endif

//DVD Video Color
//CVTE_DEF_VIDEO_COLOR_TYPE_MIN~CVTE_DEF_VIDEO_COLOR_TYPE_MAX
//DVDĬ����Ƶ��ʽ
#ifndef CVTE_REG_DVD_COLOR_DEFAULT
#define CVTE_REG_DVD_COLOR_DEFAULT			CVTE_DEF_VIDEO_COLOR_TYPE_NTSM_MJ
#endif

//Reverse Video Color
//CVTE_DEF_VIDEO_COLOR_TYPE_MIN~CVTE_DEF_VIDEO_COLOR_TYPE_MAX
//����Ĭ����Ƶ��ʽ
#ifndef CVTE_REG_REARVIEW_COLOR_DEFAULT
#define CVTE_REG_REARVIEW_COLOR_DEFAULT		CVTE_DEF_VIDEO_COLOR_TYPE_NTSM_MJ
#endif

//LCD GAMMA
//Ĭ��GAMMAֵ
#ifndef CVTE_REG_GAMMA_DEFAULT
#define CVTE_REG_GAMMA_DEFAULT				130
#endif

//LED
//0~255
//Red
//LED��ɫĬ��ֵ
#ifndef CVTE_REG_LED_R_DEFAULT
#define CVTE_REG_LED_R_DEFAULT				255
#endif
//Green
//LED��ɫĬ��ֵ
#ifndef CVTE_REG_LED_G_DEFAULT
#define CVTE_REG_LED_G_DEFAULT				255
#endif
//Blue
//LED��ɫĬ��ֵ
#ifndef CVTE_REG_LED_B_DEFAULT
#define CVTE_REG_LED_B_DEFAULT				255
#endif

//Auto Start Navi
//CVTE_DISABLE/CVTE_ENABLE
//�Զ��򿪵������ܿ���
#ifndef CVTE_REG_AUTO_START_NAVI_DEFAULT
#define CVTE_REG_AUTO_START_NAVI_DEFAULT	CVTE_DISABLE
#endif

//Key Beep
//CVTE_DISABLE/CVTE_ENABLE
//Ĭ�ϴ�beep�����ܿ���
#ifndef CVTE_REG_KEEP_BEEP_DEFAULT
#define CVTE_REG_KEEP_BEEP_DEFAULT			CVTE_ENABLE
#endif

//Reverse Volume Auto Adjustment
//CVTE_DISABLE/CVTE_ENABLE
//�������������ƹ��ܿ���
#ifndef CVTE_REG_REAR_VOL_CONTROL_DEFAULT
#define CVTE_REG_REAR_VOL_CONTROL_DEFAULT	CVTE_ENABLE
#endif

//Display Blur Effect
//CVTE_DISABLE/CVTE_ENABLE
//ģ��Ч�����ܿ���
#ifndef CVTE_REG_BLUR_EFFECT_DEFAULT
#define CVTE_REG_BLUR_EFFECT_DEFAULT		CVTE_DISABLE
#endif

//Led Anim Effect
//CVTE_DISABLE/CVTE_ENABLE
//LED����Ч�����ܿ���
#ifndef CVTE_REG_LED_ANIM_EFFECT_DEFAULT
#define CVTE_REG_LED_ANIM_EFFECT_DEFAULT	CVTE_DISABLE
#endif

//Activate Factory Mode
//CVTE_DISABLE/CVTE_ENABLE
//����ģʽ
#ifndef CVTE_REG_FACTORY_MODE_DEFAULT
#define CVTE_REG_FACTORY_MODE_DEFAULT		CVTE_DISABLE
#endif

//Snapshot Enable
//CVTE_DISABLE/CVTE_ENABLE
//��������
#ifndef CVTE_REG_SNAPSHOT_ENABLE_DEFAULT
#define CVTE_REG_SNAPSHOT_ENABLE_DEFAULT	CVTE_DISABLE
#endif

//DAB Enable
//CVTE_DISABLE/CVTE_ENABLE
//DAB���ܿ���
#ifndef CVTE_REG_DAB_ENABLE_DEFAULT
#define CVTE_REG_DAB_ENABLE_DEFAULT			CVTE_ENABLE
#endif

#ifndef CVTE_REG_DAB_ANT_POWER_ON_DEFAULT
#define CVTE_REG_DAB_ANT_POWER_ON_DEFAULT	CVTE_ENABLE
#endif

#ifndef CVTE_REG_DAB_SERVICE_LINK_DEFAULT
#define CVTE_REG_DAB_SERVICE_LINK_DEFAULT	CVTE_ENABLE
#endif 

#ifndef CVTE_REG_DAB_LBAND_DEFAULT
#define CVTE_REG_DAB_LBAND_DEFAULT			CVTE_ENABLE
#endif

//DAB�Զ���̨
#ifndef CVTE_REG_DAB_TA_DEFAULT
#define CVTE_REG_DAB_TA_DEFAULT				CVTE_ENABLE
#endif

//DayMode & NightMode
//LCD
//LCD�ռ�ģʽĬ�ϱ���
#ifndef CVTE_REG_LCD_DAYMODE_BRIGHTNESS_DEFAULT
#define CVTE_REG_LCD_DAYMODE_BRIGHTNESS_DEFAULT		CVTE_DEF_DAYMODE_LCD_BRIGHTNESS_DEFAULT
#endif

//LCDҹ��ģʽĬ�ϱ���
#ifndef CVTE_REG_LCD_NIGHTMODE_BRIGHTNESS_DEFAULT
#define CVTE_REG_LCD_NIGHTMODE_BRIGHTNESS_DEFAULT	CVTE_DEF_NIGHTMODE_LCD_BRIGHTNESS_DEFAULT
#endif

//LED
//LED�ռ�ģʽĬ�ϱ���
#ifndef CVTE_REG_LED_DAYMODE_BRIGHTNESS_DEFAULT
#define CVTE_REG_LED_DAYMODE_BRIGHTNESS_DEFAULT		CVTE_DEF_DAYMODE_LED_BRIGHTNESS_DEFAULT
#endif

//LEDҹ��ģʽĬ�ϱ���
#ifndef CVTE_REG_LED_NIGHTMODE_BRIGHTNESS_DEFAULT
#define CVTE_REG_LED_NIGHTMODE_BRIGHTNESS_DEFAULT	CVTE_DEF_NIGHTMODE_LED_BRIGHTNESS_DEFAULT
#endif 

//Audio Media
#ifndef CVTE_REG_AUDIO_VOLUME_MEDIA_DEFAULT
#define CVTE_REG_AUDIO_VOLUME_MEDIA_DEFAULT			15
#endif

//Audio Phone
//��Ƶ�绰Ĭ������
#ifndef CVTE_REG_AUDIO_VOLUME_PHONE_DEFAULT
#define CVTE_REG_AUDIO_VOLUME_PHONE_DEFAULT			15
#endif 

//Audio Speech
//��Ƶ����Ĭ������
#ifndef CVTE_REG_AUDIO_VOLUME_SPEECH_DEFAULT
#define CVTE_REG_AUDIO_VOLUME_SPEECH_DEFAULT		15
#endif 
/***************************CVTE_DEF_XXX_VERSION*******************************/
//release�汾�汾��
#ifndef CVTE_DEF_RELEASE_VERSION_NUM
#define CVTE_DEF_RELEASE_VERSION_NUM		1
#endif

//APP�汾��
#ifndef CVTE_DEF_APP_VERSION
#define CVTE_DEF_APP_VERSION				"V1.7.10"
#endif

//APP�汾��
#define CVTE_DEF_APP_VERSION_STRING			"C"##StringifyExpansion(CUSTOMER_ID)##"_"##CVTE_DEF_APP_VERSION##"."##StringifyExpansion(CVTE_DEF_RELEASE_VERSION_NUM)
#define CVTE_DEF_APP_VERSION_WIDE_STRING	_T("C")##_T(StringifyExpansion(CUSTOMER_ID))##_T("_")##_T(CVTE_DEF_APP_VERSION)##_T(".")##_T(StringifyExpansion(CVTE_DEF_RELEASE_VERSION_NUM))

//Ӳ���汾��
#ifndef CVTE_DEF_HARDWARE_VERSION
#define CVTE_DEF_HARDWARE_VERSION			"V1.3.0"
#endif

//Ӳ���汾��
#define CVTE_DEF_HARDWARE_VERSION_STRING		"H"##StringifyExpansion(CUSTOMER_ID)##"_"##CVTE_DEF_HARDWARE_VERSION
#define CVTE_DEF_HARDWARE_VERSION_WIDE_STRING	_T("H")##_T(StringifyExpansion(CVTE_MODEL_TYPE))##_T("_")##_T(CVTE_DEF_HARDWARE_VERSION)

/******************************************************************************/

//carplay GPS���ں�
#ifndef CVTE_DEF_CARPLAY_GPS_UART_PORT
#define CVTE_DEF_CARPLAY_GPS_UART_PORT		6
#endif

//android auto GPS���ں�
#ifndef CVTE_DEF_ANDROIDAUTO_GPS_UART_PORT
#define CVTE_DEF_ANDROIDAUTO_GPS_UART_PORT	6
#endif

//��Ƶ���������
#ifndef CVTE_DEF_AUDIO_CODEC_SAMPLERATE_44100
#define CVTE_DEF_AUDIO_CODEC_SAMPLERATE_44100	0
#endif

#endif


