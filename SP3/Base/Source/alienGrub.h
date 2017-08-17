#ifndef ALIEN_GRUB_H
#define ALIEN_GRUB_H

#include <string>
#include "AlienBase.h"
using namespace std;

class alienGrub : public alienBase
{
public:
	alienGrub(string alienName, unsigned int alienHealth, unsigned int alienSpeed, unsigned int alienDamage); 
	virtual ~alienGrub();

protected:

};
#endif