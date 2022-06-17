// ShootThemUp. Game by Vorontsov Oleg. All rigths reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
    GENERATED_BODY()

public:
    ASTUBasePickup();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    USphereComponent* SphereComponent;

    //���������� ������� ��������� �� �����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float RespawnTime = 5.0f;

    //�������������� ����������� ������� AActor ���������� ������ ������ ������
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    //���������� ���� �������� ������
    float RatationYaw = 0.0f;

    //����������� ������� ��� ��������������� � �������������� �������
    virtual bool GivePickupTo(APawn* PlayerPawn);

    void PickupWasTaken();
    void Respawn();

    //������� ���������� ���� ��������
    void GenerateRatationYaw();
};
