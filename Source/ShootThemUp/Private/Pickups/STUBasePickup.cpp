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

    check(SphereComponent);

    //функция вычисления угла вращения
    GenerateRatationYaw();
}

void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //вращаем пикап
    AddActorLocalRotation(FRotator(0.0f, RatationYaw, 0.0f));
}

//переопределяем стандартную функцию AActor перекрытия одного актора другим
void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    //получаем Pawn на котором произошел overlap
    const auto Pawn = Cast<APawn>(OtherActor);

    //если пикап удалось взять
    if (GivePickupTo(Pawn))
    {
        PickupWasTaken();
    }
}

//виртуальная функция для переопределения в унаследованных пикапах
bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    //базовый пикап взять нельзя
    return false;
}

void ASTUBasePickup::PickupWasTaken()
{
    //отключаем коллизии
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    if (GetRootComponent())
    {
        //первый параметр - реднерится ли текущий компонент, второй - его дочерние компоненты
        GetRootComponent()->SetVisibility(false, true);
    }
    //создаем таймер
    FTimerHandle RespawnTimerHandle;
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
}

void ASTUBasePickup::Respawn()
{
    //функция вычисления угла вращения
    GenerateRatationYaw();

    //включаем коллизии
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    if (GetRootComponent())
    {
        //первый параметр - реднерится ли текущий компонент, второй - его дочерние компоненты
        GetRootComponent()->SetVisibility(true, true);
    }
}

//функция вычисления угла вращения
void ASTUBasePickup::GenerateRatationYaw()
{
    //переменная направления вращения
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;

    RatationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}
