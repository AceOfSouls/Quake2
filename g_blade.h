/*
Blade variables that are defined to know whether to change these later
buff/nerf damage yad yad
*/

#define BLADE_STANDARD_DAMAGE 60
#define BLADE_FIRE_DAMAGE1 80
#define BLADE_FIRE_DAMAGE2 100
#define BLADE_KICK 200
#define BLADE_START_RANGE 40
#define BLADE_RANGE2 60
#define BLADE_RANGE3 80


void Fire_Blade(edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int element)
{
	trace_t tr;

	vec3_t dir;
	vec3_t forward;
	vec3_t up;
	vec3_t right;
	vec3_t end;

	tr = gi.trace(self->s.origin, NULL, NULL, start, self, MASK_SHOT);
}

void Weapon_blade_fire (edict_t *ent)
{
	int		damage = 100;
	int		range = 30;

	if(ent->current_level == 2)
	{
		range = 40;
	}
	else if(ent->current_level == 5)
	{
		range = 60;
	}

	if(ent->current_level > 2 && ent->current_level < 5)
	{
		if(ent->choosen_element == 0)
		{
			damage = 130;
		}
		else if(ent->choosen_element == 1)
		{
			ent->freeze_slow = 40;
			ent->freeze_dura = 5;
		}
		else
		{
			ent->elect_damage = 6;
			ent->elect_dura = 6;
		}
	}
	else if(ent->current_level == 5)
	{
		if(ent->choosen_element == 0)
		{
			damage = 150;
		}
		else if(ent->choosen_element == 1)
		{
			ent->freeze_slow = 60;
			ent->freeze_dura = 5;
		}
		else
		{
			ent->elect_damage = 10;
			ent->elect_dura = 6;
		}
	}

	if (is_quad)
	{
		damage *=4;
	}

}

void Weapon_Blade (edict_t *ent)
{
	static int pause_frames[] = {14,32,50,0};
	static int fire_frames[] = {2,8,0};

	Weapon_Generic (ent, 1, 15, 48, 50, pause_frames, fire_frames, Weapon_blade_fire);
}