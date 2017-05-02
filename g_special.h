void special_1(edict_t *ent)
{
	ent->special_blade_active = 1;
	Weapon_blade_fire(ent);
	ent->special_blade_active = 0;
}

void special_2(edict_t *ent)
{
	if(ent->health < 500)
		ent->health = 500;
}

void special_3(edict_t *ent)
{
	ent->special_blade_active = 1;
	Weapon_Blaster_Fire(ent);
	weapon_bfg_fire(ent);
	ent->special_blade_active = 0;
}

void fire_special(edict_t *ent)
{
	if(ent->special_charge == 100)
	{
		if(ent->choosen_special == 0)
			special_1(ent);
		else if(ent->choosen_special == 1)
			special_2(ent);
		else
			special_3(ent);

		ent->special_charge = 0;
		ent->special_charge_time = 0;
	}
	else
	{
		gi.bprintf(PRINT_MEDIUM, "Your Special is not ready!", ent->current_level);
	}
}
