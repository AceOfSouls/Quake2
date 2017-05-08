void special_1(edict_t *ent)//slashy slashy
{
	ent->special_blade_active = 1;
	Weapon_blade_fire(ent);
	ent->special_blade_active = 0;
}

void special_2(edict_t *ent)//tanky tanky
{
	if(ent->health < 500)
		ent->health = 500;
	PlayerNoise(ent, ent->s.origin, PNOISE_SELF);
}

void special_3(edict_t *ent)//shooty shooty
{
	ent->special_blade_active = 1;
	Weapon_Blaster_Fire(ent);
	weapon_bfg_fire(ent);
	ent->special_blade_active = 0;
}

void special_4(edict_t *ent)//Invis
{
	ent->special_inv = 1;
}

void special_6(edict_t *ent)//speed
{
	ent->special_speed = 1;
	ent->special_spdura = level.time + 6;
	VectorCopy(ent->velocity,ent->oldvelo); 
}

void special_5(edict_t *ent)//OP OP
{
	special_1(ent);
	special_2(ent);
	special_3(ent);
	special_4(ent);
	special_6(ent);
}

void fire_special(edict_t *ent)
{
	if(ent->special_charge == 100)
	{
		if(ent->choosen_special == 0)
			special_1(ent);
		else if(ent->choosen_special == 1)
			special_2(ent);
		else if(ent->choosen_special == 2)
			special_3(ent);
		else if(ent->choosen_special == 3)
			special_4(ent);
		else if(ent->choosen_special == 4)
			special_5(ent);
		else if(ent->choosen_special == 5)
			special_6(ent);
		ent->special_charge = 0;
		ent->special_charge_time = 0;
	}
	else
	{
		gi.bprintf(PRINT_MEDIUM, "Your Special is not ready!", ent->current_level);
	}
}
