/*
Blade variables that are defined to know whether to change these later
buff/nerf damage yad yad
*/


#define BLADE_STANDARD_DAMAGE 30
#define BLADE_FIRE_DAMAGE1 40
#define BLADE_FIRE_DAMAGE2 60
#define BLADE_KICK 200
#define BLADE_START_RANGE 40
#define BLADE_RANGE2 60
#define BLADE_RANGE3 80
#define BLADE_SP_RANGE 600
#define BLADE_SP_DAMAGE 200


//Freeze think function to determine how long a person should be frozen and by how much
void blade_freeze_think(edict_t *self)
{
	vec3_t		dir;
	int			slow;
	float		points;
	vec3_t		v;

	if(self->owner->is_frozen == 0)
	{
		G_FreeEdict(self);
		return;
	}

	if (level.time > self->freeze_dura)
	{
		self->owner->is_frozen = 0;
		G_FreeEdict(self);
		return;
	}

	if (!self->owner)
	{
		G_FreeEdict(self);
		return;
	}

	slow = self->freeze_slow;
	
	if (self->speed != (self->original_speed * slow))
	{
		self->speed *= slow;
		self->freeze_dura = level.time + 0.8;
	}
	self->nextthink = level.time + .2;
}

void freeze_person(edict_t *target, edict_t *owner, int slow)
{
	edict_t *freeze;

	if (target->is_frozen > 0)
		return;
	target->is_frozen = 1;
	target->original_speed = target->speed;
	freeze = G_Spawn();
	freeze->movetype = MOVETYPE_NOCLIP;
	freeze->clipmask = MASK_SHOT;
	freeze->solid = SOLID_NOT;
	freeze->s.effects |= EF_ANIM_ALLFAST | EF_BFG | EF_HYPERBLASTER;//|EF_GRENADE|EF_BLASTER;
	freeze->velocity[0] = target->velocity[0];
	freeze->velocity[1] = target->velocity[1];
	freeze->velocity[2] = target->velocity[2];

	VectorClear(freeze->mins);
	VectorClear(freeze->maxs);
	freeze->owner = target;
	freeze->delay = level.time + 5;
	freeze->nextthink = level.time + .8;
	freeze->freeze_dura = level.time + 0.8;
	freeze->think = blade_freeze_think;
	freeze->freeze_slow = slow;
	freeze->classname = "Freeze";
	freeze->s.sound = gi.soundindex("weapons/bfg__l1a.wav");
	gi.linkentity(freeze);

	VectorCopy(target->s.origin, freeze->s.origin);
}

//Elect think function to determine how long a person should be Elect and how long he will take damage
void blade_elect_think(edict_t *self)
{
	vec3_t		dir;
	int			damage;
	float		points;
	vec3_t		v;

	if(self->owner->is_elect == 0)
	{
		G_FreeEdict(self);
		return;
	}

	if (level.time > self->elect_dura)
	{
		self->owner->is_elect = 0;
		G_FreeEdict(self);
		return;
	}

	if (!self->owner)
	{
		G_FreeEdict(self);
		return;
	}

	damage = self->elect_damage;

	T_Damage(self->owner, self, self, dir, self->owner->s.origin, vec3_origin, damage, 0, DAMAGE_NO_KNOCKBACK, 0);
	self->elect_dura = level.time + 0.8;
	VectorCopy(self->owner->s.origin, self->s.origin);
	self->nextthink = level.time + .2;
}

//Elect the opposing person
void elect_person(edict_t *target, edict_t *owner, int damage)
{
	edict_t *elect;

	if (target->is_elect > 0)
		return;
	target->is_elect = 1;
	elect = G_Spawn();
	elect->movetype = MOVETYPE_NOCLIP;
	elect->clipmask = MASK_SHOT;
	elect->solid = SOLID_NOT;
	elect->s.effects |= EF_ANIM_ALLFAST | EF_BFG | EF_HYPERBLASTER;//|EF_GRENADE|EF_BLASTER;
	elect->velocity[0] = target->velocity[0];
	elect->velocity[1] = target->velocity[1];
	elect->velocity[2] = target->velocity[2];

	VectorClear(elect->mins);
	VectorClear(elect->maxs);
	elect->owner = target;
	elect->delay = level.time + 5;
	elect->nextthink = level.time + .8;
	elect->elect_dura = level.time + 0.8;
	elect->think = blade_elect_think;
	elect->elect_damage = damage;
	elect->classname = "electric";
	elect->s.sound = gi.soundindex("weapons/bfg__l1a.wav");
	gi.linkentity(elect);

	VectorCopy(target->s.origin, elect->s.origin);
}

void Fire_Blade(edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick)
{
	trace_t tr;

	vec3_t end;

	if(self->special_blade_active == 1)
	{
		VectorMA(start, BLADE_SP_RANGE, aimdir, end);
	}
	else if (self->current_level == 2)
	{
		VectorMA(start, BLADE_RANGE2, aimdir, end);
	}
	else if (self->current_level == 5)
	{
		VectorMA(start, BLADE_RANGE3, aimdir, end);
	}
	else
	{
		VectorMA(start, BLADE_START_RANGE, aimdir, end);
	}

	tr = gi.trace(self->s.origin, NULL, NULL, end, self, MASK_SHOT);

	if (!((tr.surface) && (tr.surface->flags & SURF_SKY)))
	{
		if (tr.fraction < 1.0)
		{
			if (tr.ent->takedamage)
			{
				//This tells us to damage the thing that in our path
				T_Damage(tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal, damage, kick, 0, MOD_BLADE);
				//This will check if the target will have a status effect on them upon hit
				
				if ((self->current_level == 3 || self->current_level == 4) && self->choosen_element == 1)
				{
					freeze_person(tr.ent,self, 0.5);
				}
				else if ((self->current_level == 3 || self->current_level == 4) && self->choosen_element == 2)
				{
					elect_person(tr.ent, self, 6);
				}
				else if (self->current_level == 5 && self->choosen_element == 1)
				{
					freeze_person(tr.ent, self, 0.3);
				}
				else if (self->current_level == 5 && self->choosen_element == 2)
				{
					elect_person(tr.ent, self, 10);
				}
				
			}
			else
			{
				if (strncmp(tr.surface->name, "sky", 3) != 0)
				{
					gi.WriteByte(svc_temp_entity);
					gi.WriteByte(TE_SPARKS);
					gi.WritePosition(tr.endpos);
					gi.WriteDir(tr.plane.normal);
					gi.multicast(tr.endpos, MULTICAST_PVS);

					if (self->client)
						PlayerNoise(self, tr.endpos, PNOISE_IMPACT);
				}
			}
		}
	}
	return;
}

// Basic non-special effect attack
void blade_attack(edict_t *ent, vec3_t g_offset, int damage)
{
	vec3_t  forward, right;
	vec3_t  start;
	vec3_t  offset;

	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight - 8);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	Fire_Blade(ent, start, forward, damage, BLADE_KICK);
	if(ent->health >= 100)
	{
		Weapon_Blaster_Fire(ent);
	}
}

void Weapon_blade_fire (edict_t *ent)
{
	int damage;

	//determine damage base on level and if it has the fire element
	if(ent->special_blade_active == 1)
	{
		damage = BLADE_SP_DAMAGE;
	}
	else if ((ent->current_level == 3 || ent->current_level == 4) && ent->choosen_element == 0)
	{
		damage = BLADE_FIRE_DAMAGE1;
	}
	else if (ent->current_level == 5 && ent->choosen_element == 0)
	{
		damage = BLADE_FIRE_DAMAGE2;
	}
	else
	{
		damage = BLADE_STANDARD_DAMAGE;
	}
	
	//Is it in quad damage?
	if (is_quad)
	{
		damage *= 4;
	}

	blade_attack(ent, vec3_origin, damage);
	ent->client->ps.gunframe++;
}


//frame mandatory stuff
void Weapon_Blade (edict_t *ent)
{
	static int pause_frames[] = {14,32,0};
	static int fire_frames[] = {2,8,0};

	Weapon_Generic (ent, 1, 15, 48, 50, pause_frames, fire_frames, Weapon_blade_fire);
}