/*
 * Substation.h
 *
 *  Created on: 20 Feb 2020
 *      Author: maro
 */

#ifndef SUBSTATION_H_
#define SUBSTATION_H_


#include "TypeDefs.h"
#include <FastLED.h>

class Substation {
public:
	Substation(CRGB* crgb);
	void setup(void);
	void loop(void);
	void activate(void);
	void deactivate(void);
	bool isActive(void);
	bool statusHasChanged(void);
	virtual ~Substation();
private:
	CRGB* _crgb;
	bool _statusHasChanged;
	bool _isActive;
};

#endif /* SUBSTATION_H_ */
