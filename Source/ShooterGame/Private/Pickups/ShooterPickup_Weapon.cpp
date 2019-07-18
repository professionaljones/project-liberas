// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "Weapons/ShooterWeapon.h"
#include "ShooterPickup_Weapon.h"

AShooterPickup_Weapon::AShooterPickup_Weapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//WeaponToAdd = NULL;
}

bool AShooterPickup_Weapon::CanBePickedUp(AShooterCharacter* TestPawn) const
{
	///*AShooterWeapon* TestWeapon = (TestPawn ? TestPawn->FindWeapon(WeaponToAdd) : NULL);
	//if (bIsActive && TestWeapon)
	//{
	//	return TestWeapon->GetCurrentAmmo() < TestWeapon->GetMaxAmmo();
	//}

	return TestPawn && TestPawn->IsAlive();
}

void AShooterPickup_Weapon::GivePickupTo(AShooterCharacter* Pawn)
{
	FActorSpawnParameters SpawnParameters;
	if (WeaponToAdd)
	{
		if (Pawn->FindWeapon(WeaponToAdd) == NULL)
		{
			NewWeapon = GetWorld()->SpawnActor<AShooterWeapon>(WeaponToAdd, GetActorLocation(), GetActorRotation(), SpawnParameters);
			Pawn->AddWeapon(NewWeapon);
		}

		if (Pawn)
		{
			const auto Events = Online::GetEventsInterface();
			const auto Identity = Online::GetIdentityInterface();

			if (Events.IsValid() && Identity.IsValid())
			{
				AShooterPlayerController* PC = Cast<AShooterPlayerController>(Pawn->Controller);
				if (PC)
				{
					ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->Player);

					if (LocalPlayer)
					{

					}
				}

			}
		}
	}
}
