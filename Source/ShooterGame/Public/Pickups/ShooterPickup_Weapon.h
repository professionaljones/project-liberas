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

public:
	/** What weapon are we picking up */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = Pickup)
		class AShooterWeapon* NewWeapon;

protected:

	/** how much ammo does it give if it already exists in inventory */
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		int32 AmmoClips;

	/** What weapon are we picking up */
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		TSubclassOf<AShooterWeapon> WeaponToAdd;

	

	/** give pickup */
	virtual void GivePickupTo(AShooterCharacter* Pawn) override;
};
