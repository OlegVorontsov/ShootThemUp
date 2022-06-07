// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->InitSphereRadius(5.0f);

    //�������� ��������
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    SetRootComponent(SphereComponent);

    //������� �������� �������� �������
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
    //������ ��������� ��������
    MovementComponent->InitialSpeed = 2000.0f;
    //��������� ���������� � ����� ����� ������ ������ �� ������
    MovementComponent->ProjectileGravityScale = 0.0f;
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();
    check(MovementComponent);
    check(SphereComponent);

    //������ �������� �������� ���������� ����������� �������� �� ��������� ��������
    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;

    //��������� ������������� ������ ��������� �� ����� �������� ������
    SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);

    //�������� ������� OnProjectileHit ����� ������� OnComponentHit
    SphereComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);

    //������������ ����� ����� ������
    SetLifeSpan(LifeSeconds);
}

//������� ������������ � �������
void ASTUProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!GetWorld())
        return;

    //����� ������������ ������������� �������� ������
    MovementComponent->StopMovementImmediately();

    //������� ����� ����� ��� � ����� ����� ��������� ���� ������� �� �����
    //� �������������� ������� TakeDamage ���� �������
    UGameplayStatics::ApplyRadialDamage(GetWorld(), //
        DamageAmount,                               //
        GetActorLocation(),                         //
        DamageRadius,                               //
        UDamageType::StaticClass(),                 //
        {GetOwner()},                               //������ ������� �������������� ��� ��������� ������
        this,                                       //��������� �� �� ��� ����� �����
        GetController(),                            //��������� �� ����������
        DoFullDamage);

    //������ ��� ������ �����
    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);

    //���������� ������
    Destroy();
}

AController* ASTUProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
