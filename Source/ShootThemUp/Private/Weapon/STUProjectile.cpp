// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->InitSphereRadius(5.0f);
    SetRootComponent(SphereComponent);
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();
}
