#pragma once

#include "Status.hpp"

class BaseAI {
public:

	virtual void think(const Status& my, const Status& enemy) = 0;
	virtual void output() = 0;

protected:

};
