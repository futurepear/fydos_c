#include "weapon.h"

Pickaxe<float>* pickaxeFactory(int cooldown, int damage, Vector<float>& center) {

	Pickaxe<float>* pickaxe = new Pickaxe<>{ center, damage };
	pickaxe->setAnimation("x", { {0.6f, 0}, {0.6f, cooldown / 3}, {0.6f, cooldown} });
	pickaxe->setAnimation("y", { {0, 0}, {0, cooldown / 3}, {0, cooldown} });
	pickaxe->setAnimation("angle", { {0, 0}, {1.3f, cooldown / 3}, {0.0f, cooldown} });

	return pickaxe;
}

Usable<float>* weaponFactory(int weaponID, Vector<float>& center) {
	const WeaponData& data = Dictionary::weapons[weaponID];
	int cooldown = data.cooldown;
	int damage = data.damage;

	using enum WeaponType;
	if (data.type == PICKAXE)
		return pickaxeFactory(cooldown, damage, center);

	return new Hand<float>{ center };
}