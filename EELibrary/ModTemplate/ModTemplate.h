#pragma once
#include "ModExports.h"

class MODTEMPLATE_API MOD_NAME : public eelib::mod::Mod
{
public:
    MOD_NAME(void);
    ~MOD_NAME(void);

	bool OnStart();
    bool OnUpdate();
    bool OnStop();
};

