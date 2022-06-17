// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BasePickupLog, All, All);

ASTUBasePickup::ASTUBasePickup()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->InitSphereRadius(50.0f);

    //�������� ��������
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    SetRootComponent(SphereComponent);
}

void ASTUBasePickup::BeginPlay()
{
    Super::BeginPlay();

    check(SphereComponent);

    //������� ���������� ���� ��������
    GenerateRatationYaw();
}

void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //������� �����
    AddActorLocalRotation(FRotator(0.0f, RatationYaw, 0.0f));
}

//�������������� ����������� ������� AActor ���������� ������ ������ ������
void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    //�������� Pawn �� ������� ��������� overlap
    const auto Pawn = Cast<APawn>(OtherActor);

    //���� ����� ������� �����
    if (GivePickupTo(Pawn))
    {
        PickupWasTaken();
    }
}

//����������� ������� ��� ��������������� � �������������� �������
bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    //������� ����� ����� ������
    return false;
}

void ASTUBasePickup::PickupWasTaken()
{
    //��������� ��������
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    if (GetRootComponent())
    {
        //������ �������� - ���������� �� ������� ���������, ������ - ��� �������� ����������
        GetRootComponent()->SetVisibility(false, true);
    }
    //������� ������
    FTimerHandle RespawnTimerHandle;
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
}

void ASTUBasePickup::Respawn()
{
    //������� ���������� ���� ��������
    GenerateRatationYaw();

    //�������� ��������
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    if (GetRootComponent())
    {
        //������ �������� - ���������� �� ������� ���������, ������ - ��� �������� ����������
        GetRootComponent()->SetVisibility(true, true);
    }
}

//������� ���������� ���� ��������
void ASTUBasePickup::GenerateRatationYaw()
{
    //���������� ����������� ��������
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;

    RatationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}
