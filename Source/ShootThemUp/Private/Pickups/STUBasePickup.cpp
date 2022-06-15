// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BasePickupLog, All, All);

ASTUBasePickup::ASTUBasePickup()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->InitSphereRadius(50.0f);

    //включаем коллизии
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    SetRootComponent(SphereComponent);
}

void ASTUBasePickup::BeginPlay()
{
    Super::BeginPlay();
}

void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//переопределяем стандартную функцию AActor перекрытия одного актора другим
void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    UE_LOG(BasePickupLog, Display, TEXT("Pickup was taken!"));

    Destroy();
}
