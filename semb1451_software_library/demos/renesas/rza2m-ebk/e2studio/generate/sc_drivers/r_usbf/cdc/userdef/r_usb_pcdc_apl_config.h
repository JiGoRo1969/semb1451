/*
 * r_usb_pcdc_apl_config.h
 *
 *  Created on: 2019/10/14
 *      Author: n-okada
 */

#ifndef SC_DRIVERS_R_USBF_CDC_USERDEF_R_USB_PCDC_APL_CONFIG_H_
#define SC_DRIVERS_R_USBF_CDC_USERDEF_R_USB_PCDC_APL_CONFIG_H_

//	10.3.1 IP 選択
//	ポート0 及びポート1 (デフォルト)を選択してください。
// #define 	USE_USBIP USB_IP0
#define 	USE_USBIP USB_IP1
//	10.3.2 サンプルアプリケーション選択
//	Echo モード及びUSB-シリアル変換モードを選択してください。
// #define 	OPERATION_MODE USB_ECHO
#define 	OPERATION_MODE USB_UART
//	10.3.3 低電力消費モード使用設定
//	低電力消費モードを使用する場合は以下のように設定してください。
#define 	USE_LPW
//	低電力消費モードを使用しない場合はUSE_LPW 設定をコメントアウトしてください。
//	10.3.4 対応スピードの選択
//	HS およびFS(デフォルト)の切り替えは以下を変更してください。
#define 	USB_SUPPORT_SPEED	USB_FS
// #define 	USB_SUPPORT_SPEED	USB_HS

#endif /* SC_DRIVERS_R_USBF_CDC_USERDEF_R_USB_PCDC_APL_CONFIG_H_ */
