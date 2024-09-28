// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoxelBodyGenerator.generated.h"

class APlanetVoxel;

UCLASS()
class PLANETS_API AVoxelBodyGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoxelBodyGenerator();

	UPROPERTY(EditAnywhere, Category = "Planet")
	float VoxelSize;

	UPROPERTY(EditAnywhere, Category = "Planet")
	int PlanetRadius;

	UPROPERTY(EditAnywhere, Category = "Planet")
	TObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditAnywhere, Category = "Planet")
	TObjectPtr<UMaterialInterface> Material2;

	UPROPERTY(EditAnywhere, Category = "Planet")
	TObjectPtr<UMaterialInterface> Material3;

	UPROPERTY(EditAnywhere, Category = "Planet")
	FIntVector ChunkSize = FIntVector(16, 16, 16);

	//FVector ChunkLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
