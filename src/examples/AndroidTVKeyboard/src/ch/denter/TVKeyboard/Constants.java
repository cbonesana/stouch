package com.itdimension.remotepanel;

import java.util.UUID;

public class Constants {
	public enum mReturnsValue {
		RETURN_TRUE, RETURN_FALSE, RETURN_NO_PAGING
	};

	public static final String DEFAULT_PATH = "sdcard/Android/data/com.itdimension.remotepanel/files";
	public static final String NAME_APP_XML_FILE = "apps.xml";
	public static final String NAME_CONTACTS_XML_FILE = "contacts.xml";
	public static final String NAME_APP_RAR_FILE = "apps.zip";
	public static final String NAME_CONTACTS_RAR_FILE = "contacts.zip";
	public static final String API_KEY = "ba06c2aa";

	// ----------BtService-------------------------//

	public static final int BT_ERROR_CONNECTION_LOST = -2;
	public static final int BT_ERROR_CONNECTION_FAIL = -1;
	public static final int BT_ERROR_IN_WRITE_MSG = 0;
	public static final int BT_REGISTER_CLIENT = 1;
	public static final int BT_UNREGISTER_CLIENT = 2;
	public static final int BT_STATE_CHANGE = 3;
	public static final int BT_READ = 4;
	public static final int BT_WRITE = 5;
	public static final int BT_TOAST = 6;
	public static final int BT_MSG_FOR_SEND = 7;
	public static final int BT_MSG_SENDED = 8;
	public static final String BT_TAG = "BT";

	// Constants that indicate the current connection state
	public static final int STATE_NONE = -1;
	public static final int STATE_SEARCHING = 0; // we're doing search
	// connections
	public static final int STATE_CONNECTING = 2; // now initiating an outgoing
	// connection
	public static final int STATE_CONNECTED = 3; // now connected to a remote
	public static final int STATE_SEND = 4;

	public static final long BIND_TIMEOUT = 100;

	// ---------MainActivity----------------------//

	// Key names received from the BluetoothChatService Handler
	public static final String DEVICE_NAME = "device_name";
	public static final String TOAST = "toast";
	public static final String TYPEFASE = "PT_DIN_Condensed_Cyrillic.ttf";

	public static final int TIMER_PERIOD = 100;
	public static final int TIMER_DELAY = 0;
	public static final long CLICK_PERIOD = 3000;
	public static final long CLICK_TOUCHPAD_PERIOD = 100;
	public static final long CLICK_PERIOD_FOR_POWER_OFF = 5000;

	public static final int ALPHABET_SIZE = 26;

	// Intent request codes
	public static final int REQUEST_SETTINGS_ACTIVITY = 1;
	public static final int REQUEST_ENABLE_BT = 2;
	public static final int REQUEST_SKYPE_ACTIVITY = 3;
	public static final int REQUEST_SKYPE_LOGIN_ACTIVITY = 4;
	public static final int REQUEST_SKYPE_NEW_ACCOUNT_ACTIVITY = 5;
	// Intent result codes
	public static final int RESULT_SKYPE_LOGIN_FALSE = 0;
	public static final int RESULT_SKYPE_LOGIN_TRUE = 1;

	// Type of commands
	public static final char TYPE_K = 'K';
	public static final char TYPE_AV_BOARD_UART = 'C';
	public static final char TYPE_PC_BOARD_MOUSE = 'M';
	public static final char TYPE_AV_BOARD = 'S';
	public static final char TYPE_CHECKSUM = 'Q';
	public static final char TYPE_MOTORIZED_STAND = 'B';
	public static final char TYPE_D = 'D';
	// Modes
	public static final byte CTRL = 0x01;
	public static final byte SHIFT = 0x02;
	public static final byte ALT = 0x04;

	// COMMANDS
	public static final byte[] HOME = { TYPE_K, 0x04, 0x3A, 0x00, 0x00 };
	public static final byte[] BACK = { TYPE_K, 0x00, 0x29, 0x00, 0x00 };
	public static final byte[] STAND_BY = { TYPE_AV_BOARD_UART, 'S', 'T', 'B',
			'Y' };
	public static final byte[] POWER_OFF = { TYPE_AV_BOARD_UART, 'P', 'W', 'O',
			'F' };
	public static final byte[] TV = { TYPE_K, 0x00, 0x3A, 0x00, 0x00 };
	public static final byte[] INTERNET = { TYPE_K, 0x00, 0x3E, 0x00, 0x00 };
	public static final byte[] HELP_ONLINE = { TYPE_K, 0x00, 0x6E, 0x00, 0x00 };
	public static final byte[] RESET = { TYPE_K, 0x00, 0x6C, 0x00, 0x00 };

	// TV LAYOUT
	// player

	public static final byte[] PLAY_PAUSE = { TYPE_K, ALT, 0x13, 0x00, 0x00 };
	public static final byte[] REWIND = { TYPE_K, ALT, 0x15, 0x00, 0x00 };
	public static final byte[] FORWARD = { TYPE_K, ALT, 0x09, 0x00, 0x00 };
	public static final byte[] STOP = { TYPE_K, ALT, 0x16, 0x00, 0x00 };
	// public static final byte[] REC = {TYPE_K, ALT, 0x13,0x00,0x00};

	public static final byte[] GUIDE = { TYPE_K, 0x00, 0x70, 0x00, 0x00 };
	public static final byte[] SOURCES = { TYPE_AV_BOARD_UART, 'S', 'U', 'R',
			'C' };
	public static final byte[] MUTE = { TYPE_D, 'M', 'U', 'T', 'E' };

	public static final byte[] VOL_PLUS = { TYPE_D, 'V', 'O', 'L', '+' };
	public static final byte[] VOL_MINUS = { TYPE_D, 'V', 'O', 'L', '-' };
	public static final byte[] CH_PLUS = { TYPE_K, 0x00, 0x41, 0x00, 0x00 };
	public static final byte[] CH_MINUS = { TYPE_K, 0x00, 0x40, 0x00, 0x00 };
	public static final byte[] CH_LIST = { TYPE_K, 0x00, 0x71, 0x00, 0x00 };

	public static final byte[] FULL_SCREEN_ON = { TYPE_K, 0x00, 0x3C, 0x00,
			0x00 };
	public static final byte[] FULL_SCREEN_OFF = { TYPE_K, ALT, 0x3C, 0x00,
			0x00 };

	public static final byte[] ZOOM_IN = { TYPE_K, 0x00, 0x6D, 0x00, 0x00 };
	public static final byte[] ZOOM_OUT = { TYPE_K, ALT, 0x6D, 0x00, 0x00 };

	public static final byte[] SEARCH_VIDEO = { TYPE_K, 0x00, 0x43, 0x00, 0x00 };

	public static final byte[] SEARCH_INTERNET = { TYPE_K, 0x00, 0x44, 0x00,
			0x00 };
	public static final byte[] CLEAR = { TYPE_K, 0x00, 0x45, 0x00, 0x00 };
	public static final byte[] ENTER = { TYPE_K, 0x00, 0x28, 0x00, 0x00 };
	public static final byte[] BACKSPACE = { TYPE_K, 0x00, 0x2A, 0x00, 0x00 };
	public static final byte[] SPACE = { TYPE_K, 0x00, 0x2C, 0x00, 0x00 };
	public static final byte[] TAB = { TYPE_K, 0x00, 0x2B, 0x00, 0x00 };

	// turn buttons
	public static final byte[] Key_Left_Pressed = { TYPE_MOTORIZED_STAND, 'T',
			'L', 'O', 'N' };
	public static final byte[] Key_Left_Released = { TYPE_MOTORIZED_STAND, 'T',
			'L', 'O', 'F' };
	public static final byte[] Key_Right_Pressed = { TYPE_MOTORIZED_STAND, 'T',
			'R', 'O', 'N' };
	public static final byte[] Key_Right_Released = { TYPE_MOTORIZED_STAND,
			'T', 'R', 'O', 'F' };

	// keyboard
	public final static byte[] ALPHABET_CODS = { 0x04, 0x05, 0x06, 0x07, 0x08,
			0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13,
			0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D // alphabet
	};
	public final static byte[][] SYMBOL_CODES = { { 0x00, 0x2D },
			{ SHIFT, 0x1E }, { SHIFT, 0x33 }, { SHIFT, 0x2D }, { 0x00, 0x20 },
			{ 0x00, 0x31 }, { 0x00, 0x38 }, { SHIFT, 0x26 }, { 0x00, 0x25 },
			{ SHIFT, 0x27 }, { SHIFT, 0x24 }, { SHIFT, 0x20 }, { SHIFT, 0x37 },
			{ SHIFT, 0x36 }, { 0x00, 0x26 }, { 0x00, 0x27 }, { 0x00, 0x1E },
			{ 0x00, 0x21 }, { SHIFT, 0x2E }, { 0x00, 0x22 }, { 0x00, 0x24 },
			{ SHIFT, 0x38 }, { 0x00, 0x1F }, { 0x00, 0x36 }, { 0x00, 0x23 },
			{ 0x00, 0x37 } };

	// Navigator TV
	public static final byte[] NAVIGATE_UP_TV = { TYPE_K, 0x00, 0x52, 0x00,
			0x00 };
	public static final byte[] NAVIGATE_DOWN_TV = { TYPE_K, 0x00, 0x51, 0x00,
			0x00, };
	public static final byte[] NAVIGATE_OK_TV = { TYPE_K, 0x00, 0x58, 0x00,
			0x00 };
	public static final byte[] NAVIGATE_F9 = { TYPE_K, 0x00, 0x42, 0x00, 0x00 };
	public static final byte[] NAVIGATE_LEFT_TV = { TYPE_K, 0x00, 0x50, 0x00,
			0x00 };
	public static final byte[] NAVIGATE_RIGHT_TV = { TYPE_K, 0x00, 0x4F, 0x00,
			0x00 };
	// NAVIGATOR LAYOUT
	public static final byte[] NAVIGATE_UP = { TYPE_K, 0x00, 0x4B, 0x00, 0x00 };
	public static final byte[] NAVIGATE_DOWN = { TYPE_K, 0x00, 0x4E, 0x00,
			0x00, };
	public static final byte[] NAVIGATE_OK = { TYPE_K, ALT, 0x58, 0x00, 0x00 };
	public static final byte[] NAVIGATE_LEFT = { TYPE_K, SHIFT, 0x2B, 0x00,
			0x00 };
	public static final byte[] NAVIGATE_RIGHT = { TYPE_K, 0x00, 0x2B, 0x00,
			0x00 };
	// swiping
	public static final byte[] SWIPE_LEFT = { TYPE_K, 0x00, 0x50, 0x00, 0x00 };
	public static final byte[] SWIPE_RIGHT = { TYPE_K, 0x00, 0x2A, 0x00, 0x00 };
	public static final byte[] SWIPE_UP = { TYPE_K, ALT, 0x4E, 0x00, 0x00 };
	public static final byte[] SWIPE_DOWN = { TYPE_K, ALT, 0x4B, 0x00, 0x00 };

	public static final byte[] PINCH_IN = { TYPE_K, ALT, 0x6D, 0x00, 0x00 };
	public static final byte[] PINCH_OUT = { TYPE_K, 0x00, 0x6D, 0x00, 0x00 };
	public final static byte[] COMERCIAL_AT = { SHIFT, 0x1F };
	public static final byte[] LOW_LINE = { SHIFT, 0x2D };
	public static final byte[] MiNUS = { 0x00, 0x2D };
	public static final byte[] DOT = { 0x00, 0x37 };
	public final static byte[] SLASH = { 0x00, 0x38 };

	// Number buttons
	public static final byte[][] NUM = { { TYPE_K, 0x00, 0x27, 0x00, 0x00 },
			{ TYPE_K, 0x00, 0x1E, 0x00, 0x00 },
			{ TYPE_K, 0x00, 0x1F, 0x00, 0x00 },
			{ TYPE_K, 0x00, 0x20, 0x00, 0x00 },
			{ TYPE_K, 0x00, 0x21, 0x00, 0x00 },
			{ TYPE_K, 0x00, 0x22, 0x00, 0x00 },
			{ TYPE_K, 0x00, 0x23, 0x00, 0x00 },
			{ TYPE_K, 0x00, 0x24, 0x00, 0x00 },
			{ TYPE_K, 0x00, 0x25, 0x00, 0x00 },
			{ TYPE_K, 0x00, 0x26, 0x00, 0x00 } };
	// app managment
	public static final byte[] T = { TYPE_K, 0x00, 0x17, 0x00, 0x00 };
	public static final byte[] R = { TYPE_K, 0x00, 0x15, 0x00, 0x00 };
	public static final byte[] P = { TYPE_K, 0x00, 0x13, 0x00, 0x00 };

	public static final byte[] F10 = { TYPE_K, 0x00, 0x43, 0x00, 0x00 };

	// the release event to the PC keyboard
	public static final byte[] RELEASE_EVENT = { 'K', 0x00, 0x00, 0x00, 0x00 };

	// MAx distance for mouse move smoothly
	public static final int MAX_DIS = 15;

	// Symbols types
	public static final int TO_LOW_CAST_SYMBOLS = 0;
	public static final int TO_HIGHT_CAST_SYMBOLS = 1;
	public static final int TO_SYMBOLS_SET_1 = 2;
	public static final int TO_SYMBOLS_SET_2 = 3;

	public static final String COMM_VALLUE = ".com";

	// =====BTService===================================//
	public static final String PASSWORD = "1234";

	// Unique UUID for this application
	public static final UUID MY_UUID_SECURE = UUID
			.fromString("00001101-0000-1000-8000-00805F9B34FB");

	// The name of the device that connects to
	public static final String NEED_ADDRESS = "BLUROSSO";

	// ------SettingsActivity-------------------------------//

	public static final int REQUEST_SETTINGS_AMPLIFIR = 1;
	public static final int REQUEST_SETTINGS_SOURCES = 2;
	public static final int REQUEST_SETTINGS_APP_CHOICE = 3;

	public static final byte[] ASPECT_KEY = { TYPE_AV_BOARD_UART, 'A', 'S',
			'P', 'C' };
	public static final byte[] PICTURE_MODE = { TYPE_AV_BOARD_UART, 'P', 'I',
			'C', 'M' };
	public static final byte[] MODE_3D = { TYPE_AV_BOARD_UART, '3', 'D', 'M',
			'D' };
	public static final String SERVER_COORDINATES = "ftp.tv-surf.com";
	public static final String SERVER_LOGIN = "fase7";
	public static final String SERVER_PASSWORD = "go3denter";
	public static final String SERVER_XML_FILE = "android.xml";
	public static final String SERVER_APK_FILE = "android.apk";

	// ------SourcesSettingsActivity------------------------//

	public static final byte[] SOURCE_SET_01 = { TYPE_AV_BOARD_UART, 'S', 'H',
			'D', '4' };
	public static final byte[] SOURCE_SET_02 = { TYPE_AV_BOARD_UART, 'S', 'H',
			'D', '1' };
	public static final byte[] SOURCE_SET_03 = { TYPE_AV_BOARD_UART, 'S', 'H',
			'D', '2' };

	// -------AmplifierModeSettingsActivity------------------//
	public static final byte[] AMPLIFIER_SET_01 = { TYPE_D, 'S', 'A', '0', '1' };
	public static final byte[] AMPLIFIER_SET_02 = { TYPE_D, 'S', 'A', '0', '2' };
	public static final byte[] AMPLIFIER_SET_03 = { TYPE_D, 'S', 'A', '0', '3' };
	public static final byte[] AMPLIFIER_SET_04 = { TYPE_D, 'S', 'A', '0', '4' };
	public static final byte[] AMPLIFIER_SET_05 = { TYPE_D, 'S', 'A', '0', '5' };
	public static final byte[] AMPLIFIER_SET_06 = { TYPE_D, 'S', 'A', '0', '6' };
	public static final byte[] AMPLIFIER_SET_07 = { TYPE_D, 'S', 'A', '0', '7' };
	public static final byte[] AMPLIFIER_SET_08 = { TYPE_D, 'S', 'A', '0', '8' };
	public static final byte[] AMPLIFIER_SET_09 = { TYPE_D, 'S', 'A', '0', '9' };
	public static final byte[] AMPLIFIER_SET_10 = { TYPE_D, 'S', 'A', '1', '0' };
	public static final byte[] AMPLIFIER_SET_11 = { TYPE_D, 'S', 'A', '1', '1' };
	public static final byte[] AMPLIFIER_SET_12 = { TYPE_D, 'S', 'A', '1', '2' };
	public static final byte[] AMPLIFIER_SET_13 = { TYPE_D, 'S', 'A', '1', '3' };
	public static final byte[] AMPLIFIER_SET_14 = { TYPE_D, 'S', 'A', '1', '4' };
	public static final byte[] AMPLIFIER_SET_15 = { TYPE_D, 'S', 'A', '1', '5' };
	public static final byte[] AMPLIFIER_SET_16 = { TYPE_D, 'S', 'A', '1', '6' };
	public static final byte[] AMPLIFIER_SET_17 = { TYPE_D, 'S', 'A', '1', '7' };
	public static final byte[] AMPLIFIER_SET_18 = { TYPE_D, 'S', 'A', '1', '8' };
	public static final byte[] AMPLIFIER_SET_19 = { TYPE_D, 'S', 'A', '1', '9' };
	public static final byte[] AMPLIFIER_SET_20 = { TYPE_D, 'S', 'A', '2', '0' };
	public static final byte[] AMPLIFIER_SET_21 = { TYPE_D, 'S', 'A', '2', '1' };
	public static final byte[] AMPLIFIER_SET_22 = { TYPE_D, 'S', 'A', '2', '2' };
	public static final byte[] AMPLIFIER_SET_23 = { TYPE_D, 'S', 'A', '2', '3' };
	public static final byte[] AMPLIFIER_SET_24 = { TYPE_D, 'S', 'A', '2', '4' };
	public static final byte[] AMPLIFIER_SET_25 = { TYPE_D, 'S', 'A', '2', '5' };
	public static final byte[] AMPLIFIER_SET_26 = { TYPE_D, 'S', 'A', '2', '6' };
	public static final byte[] AMPLIFIER_SET_27 = { TYPE_D, 'S', 'A', '2', '7' };
	public static final byte[] AMPLIFIER_SET_28 = { TYPE_D, 'S', 'A', '2', '8' };
	public static final byte[] AMPLIFIER_SET_29 = { TYPE_D, 'S', 'A', '2', '9' };
	public static final byte[] AMPLIFIER_SET_30 = { TYPE_D, 'S', 'A', '3', '0' };
	public static final byte[] AMPLIFIER_SET_31 = { TYPE_D, 'S', 'A', '3', '1' };
	// --------dialogs-----------------------------/

	public static final int DIALOG = 0;
	public static final int PARSE_VERSION = 0;
	public static final int PARSE_APP = 1;

	// ---------------ftp------------------//

	public static final int FTP_UNABLE_CONNECT = 0;
	public static final int FTP_UNABLE_DISCONNECT = 1;
	public static final int FTP_FILE_NOTFOUND = 2;
	public static final int FTP_FILE_READING_ERROR = 3;

	// -----Skype---------------------------------//

	public static final String SKYPE_SHOW_USER_TERMS_URL = "http://www.skype.com/intl/en-us/legal/terms/tou/";
	public static final String SKYPE_SHOW_PRIVACY_POLICY_URL = "http://www.skype.com/intl/en-us/legal/privacy/general/";
	public static final String SKYPE_LOG_IN = "skype_login?";
	public static final String SKYPE_LOG_OUT = "skype_logout";
	public static final String SKYPE_WORD_SKYPE_ACCOUNT = "_account?";
	public static final String SKYPE_WORD_NAME = "name=";
	public static final String SKYPE_WORD_SKYPE = "skype=";
	public static final String SKYPE_WORD_PASSWORD = "pwd=";
	public static final String SKYPE_WORD_OPEN_URL = "browser_open";
	public static final String SKYPE_WORD_AND = "&";
	public static final String SKYPE_WORD_URL = "url= ";

	public static final int SKYPE_NAME_MIN_SIZE = 6;
	public static final int SKYPE_NAME_MAX_SIZE = 150;
	public static final int SKYPE_PWD_MIN_SIZE = 4;
	// create skype account Internal checks
	public static final int SKYPE_CHECK = 0;
	public static final int SKYPE_CHECKS_NAME_MIN_SIZE = 1;
	public static final int SKYPE_CHECKS_NAME_MAX_SIZE = 2;
	public static final int SKYPE_CHECKS_PWD_MIN_SIZE = 3;
	public static final int SKYPE_CHECKS_PWD_EQUALS = 4;

	public static final char[] CAPTIN_SYMBOLS = {
			// Small symbols
			'\u0061', '\u0062', '\u0063', '\u0064', '\u0065', '\u0066',
			'\u0067', '\u0068', '\u0069', '\u006A', '\u006B', '\u006C',
			'\u006D', '\u006E', '\u006F', '\u0070', '\u0071', '\u0072',
			'\u0073',
			'\u0074',
			'\u0075',
			'\u0076',
			'\u0077',
			'\u0078',
			'\u0079',
			'\u007A',
			// <!-- Big symbols -->
			'\u0041', '\u0042', '\u0043', '\u0044', '\u0045', '\u0046',
			'\u0047', '\u0048', '\u0049', '\u004A', '\u004B', '\u004C',
			'\u004D', '\u004E', '\u004F', '\u0050', '\u0051', '\u0052',
			'\u0053', '\u0054', '\u0055', '\u0056', '\u0057',
			'\u0058',
			'\u0059',
			'\u005A',
			// <'!-- Numpad -->
			'\u002D', '\u0021', '\u003A',
			'\u005F',
			'\u0033',
			'/',// 5c
			'\u002F', '\u0028', '\u0038', '\u0029', '\u0026', '\u0023',
			'\u003E', '\u003C', '\u0039', '\u0030', '\u0031', '\u0034',
			'\u002B', '\u0035', '\u0037', '\u003F', '\u0032', '\u002C',
			'\u0036', '\u002E', '\u0023'
	// <!-- Numpad + Shift -->
	// '\u2192',
	// '/',// 5c
	// '\u00A9', '\u00A2', '\u007C', '\u20AC', '\u25CB', '\u005E',
	// '\u00D7', '\u005F', '\u003D', '\u005B', '\u003E', '\u003C',
	// '\u007B', '\u007D', '\u007E', '\u25CF', '\u00A3', '\u221A',
	// '\u00F7', '\u00B6', '\u0060', '\u00AE', '\u03C0', '\u2122',
	// '\u005D'
	};

}