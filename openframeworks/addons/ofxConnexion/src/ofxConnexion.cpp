#include "ofxConnexion.h"

bool ConnexionData::getButton(int button) {
	return (buttonState & (1 << button)) != 0;
}

ofEvent<ConnexionData> ofxConnexion::connexionEvent;
ConnexionData ofxConnexion::connexionData;
UInt16 ofxConnexion::clientId;

void ofxConnexion::start(string appName) {
	InstallConnexionHandlers(driverHandler, 0L, 0L);
	
	unsigned char* pappName = new unsigned char[appName.size() + 1];
	pappName[0] = (unsigned char) appName.size();
	memcpy(&(pappName[1]), appName.c_str(), appName.size());
	clientId = RegisterConnexionClient(0, (UInt8*) pappName, kConnexionClientModeTakeOver, kConnexionMaskAll);
	delete [] pappName;
}

void ofxConnexion::stop() {
	UnregisterConnexionClient(clientId);
	CleanupConnexionHandlers();
}


// super obscure: http://www.3dconnexion.com/forum/viewtopic.php?t=406
#define OFX_CONNEXION_LED_ON (-15)
#define OFX_CONNEXION_LED_OFF (-1)

void ofxConnexion::setLed(bool state) {
	SInt32 result;
	ConnexionClientControl(clientId, kConnexionCtlSetLEDState, state ? OFX_CONNEXION_LED_ON : OFX_CONNEXION_LED_OFF, &result);
}

void ofxConnexion::driverHandler(io_connect_t connection, natural_t messageType, void *messageArgument) {
	ConnexionDeviceStatePtr msg = (ConnexionDeviceStatePtr)messageArgument;
	switch(messageType) {
		case kConnexionMsgDeviceState:
			switch (msg->command) {
				case kConnexionCmdHandleAxis: {
					memcpy(connexionData.translation, &(msg->axis[0]), 3 * sizeof(short));
					memcpy(connexionData.rotation, &(msg->axis[3]), 3 * sizeof(short));
				}
				case kConnexionCmdHandleButtons: {
					connexionData.deviceId = msg->address;
					connexionData.buttonState = msg->buttons;
					
					ofNotifyEvent(connexionEvent, connexionData);
				}
			}
	}
}
