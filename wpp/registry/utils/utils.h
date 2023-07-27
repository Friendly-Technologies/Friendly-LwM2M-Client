/*
 * utils.h
 *
 *  Created on: 11 Jul 2023
 *      Author: valentin
 */

#ifndef UTILS_H
#define UTILS_H


#include <functional>
#include <string>

#include "Operation.h"
#include "types.h"

namespace wpp {

class Listener {
public:
	virtual void handlEvent(const DataID &id, Operation::TYPE operation) = 0;
	virtual ~Listener();
};

}


#endif //UTILS_H

