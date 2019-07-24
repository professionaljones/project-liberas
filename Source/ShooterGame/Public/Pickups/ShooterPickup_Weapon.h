// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ShooterPickup.h"
#include "ShooterPickup_Weapon.generated.h"

class AShooterCharacter;
class AShooterWeapon;

// A pickup object that replenishes ammunition for a weapon
UCLASS(Abstract, Blueprintable)
class AShooterPickup_Weapon : public AShooterPickup
{
	GENERATED_UCLASS_BODY()

		/** check if pawn can use this pickup */
		virtual bool CanBePickedUp(AShooterCharacter* TestPawn) const override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = Pickup)
	AShooterWeapon* WeaponToAdd;

	/** how much ammo does it give if it already exists in inventory */
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		int32 AmmoClips;

	/** What weapon are we picking up */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Pickup)
		TSubclassOf<AShooterWeapon> NewWeapon;

	/** does player want to auto equip new weapon */
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		bool bShouldAutoEquip = false;

	

	/** give pickup */
	virtual void GivePickupTo(AShooterCharacter* Pawn) override;
};
